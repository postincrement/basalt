#include "llvm_codegen.h"

#include <cstdint>
#include <cstring>
#include <iostream>

using namespace std;

static std::string Sanitise(const std::string & name)
{
  std::string out = "g_";
  for (char ch : name) {
    if (isalnum(static_cast<unsigned char>(ch)))
      out += ch;
  }
  if (out.size() == 2)
    out += "v";
  return out;
}

LLVM_CodeGenerator::LLVM_CodeGenerator()
  : CodeGenerator({ ".ll", "t", "", "", 2, 2 })
{
}

const char * LLVM_CodeGenerator::Ty(VarType type)
{
  switch (type) {
    case VarType::eInt16: return "i16";
    case VarType::eInt32: return "i32";
    case VarType::eSingle: return "float";
    case VarType::eDouble: return "double";
    case VarType::eString: return "ptr";
    default: return "i16";
  }
}

bool LLVM_CodeGenerator::IsFloat(VarType type)
{
  return type == VarType::eSingle || type == VarType::eDouble;
}

std::string LLVM_CodeGenerator::FLit(float value)
{
  double widened = value;
  uint64_t bits = 0;
  memcpy(&bits, &widened, sizeof bits);
  char buf[32];
  snprintf(buf, sizeof buf, "0x%016llX", static_cast<unsigned long long>(bits));
  return buf;
}

std::string LLVM_CodeGenerator::DLit(double value)
{
  uint64_t bits = 0;
  memcpy(&bits, &value, sizeof bits);
  char buf[32];
  snprintf(buf, sizeof buf, "0x%016llX", static_cast<unsigned long long>(bits));
  return buf;
}

std::string LLVM_CodeGenerator::Zero(VarType type)
{
  if (type == VarType::eSingle || type == VarType::eDouble)
    return "0.0";
  if (type == VarType::eString)
    return "null";
  return "0";
}

std::string LLVM_CodeGenerator::Escape(const std::string & text)
{
  std::string out;
  for (unsigned char ch : text) {
    if (ch == '\\' || ch == '"') {
      out += '\\';
      out += static_cast<char>(ch);
    }
    else if (ch >= 32 && ch < 127)
      out += static_cast<char>(ch);
    else {
      char buf[8];
      snprintf(buf, sizeof buf, "\\%02X", ch);
      out += buf;
    }
  }
  out += "\\00";
  return out;
}

std::string LLVM_CodeGenerator::Tmp()
{
  return "%t" + std::to_string(m_tmp++);
}

std::string LLVM_CodeGenerator::Block(const std::string & hint)
{
  return hint + std::to_string(m_blockId++);
}

void LLVM_CodeGenerator::Emit(const std::string & insn)
{
  if (m_term) {
    std::string dead = Block("dead");
    m_body << dead << ":\n";
    m_term = false;
  }
  m_body << "  " << insn << "\n";
}

void LLVM_CodeGenerator::Term(const std::string & insn)
{
  Emit(insn);
  m_term = true;
}

std::string LLVM_CodeGenerator::Eval(const AST::Expr & expr)
{
  std::string result;
  expr.Evaluate(*this, result);
  return result;
}

std::string LLVM_CodeGenerator::Conv(const std::string & value, VarType from, VarType to)
{
  if (from == to || from == VarType::eNone || to == VarType::eNone)
    return value;
  std::string tmp = Tmp();
  if (IsFloat(from) && IsFloat(to)) {
    if (to == VarType::eSingle)
      Emit(tmp + " = fptrunc double " + value + " to float");
    else
      Emit(tmp + " = fpext float " + value + " to double");
  }
  else if (!IsFloat(from) && IsFloat(to))
    Emit(tmp + " = sitofp " + Ty(from) + " " + value + " to " + Ty(to));
  else if (IsFloat(from) && !IsFloat(to))
    Emit(tmp + " = fptosi " + Ty(from) + " " + value + " to " + Ty(to));
  else if (from == VarType::eInt16 && to == VarType::eInt32)
    Emit(tmp + " = sext i16 " + value + " to i32");
  else if (from == VarType::eInt32 && to == VarType::eInt16)
    Emit(tmp + " = trunc i32 " + value + " to i16");
  else
    return value;
  return tmp;
}

std::string LLVM_CodeGenerator::Bin(const char * iop, const char * fop,
                                    const AST::NumericBinaryOperation & expr)
{
  VarType type = expr.GetType();
  std::string lhs = Conv(Eval(*expr.m_lhs), expr.m_lhs->GetType(), type);
  std::string rhs = Conv(Eval(*expr.m_rhs), expr.m_rhs->GetType(), type);
  std::string tmp = Tmp();
  const char * op = IsFloat(type) ? fop : iop;
  Emit(tmp + " = " + op + " " + Ty(type) + " " + lhs + ", " + rhs);
  return tmp;
}

std::string LLVM_CodeGenerator::Cmp(const char * iop, const char * fop,
                                    const AST::NumericBinaryOperation & expr)
{
  VarType type = expr.m_lhs->GetType();
  std::string lhs = Eval(*expr.m_lhs);
  std::string rhs = Conv(Eval(*expr.m_rhs), expr.m_rhs->GetType(), type);
  std::string cmp = Tmp();
  if (IsFloat(type))
    Emit(cmp + " = fcmp " + fop + " " + Ty(type) + " " + lhs + ", " + rhs);
  else
    Emit(cmp + " = icmp " + iop + " " + Ty(type) + " " + lhs + ", " + rhs);
  std::string wide = Tmp();
  std::string flag = Tmp();
  Emit(wide + " = zext i1 " + cmp + " to i16");
  Emit(flag + " = sub i16 0, " + wide);
  return flag;
}

std::string LLVM_CodeGenerator::Truth(const AST::NumericExpr & expr)
{
  std::string value = Eval(expr);
  std::string cmp = Tmp();
  if (IsFloat(expr.GetType()))
    Emit(cmp + " = fcmp une " + std::string(Ty(expr.GetType())) + " " + value + ", 0.0");
  else
    Emit(cmp + " = icmp ne " + std::string(Ty(expr.GetType())) + " " + value + ", 0");
  return cmp;
}

std::string LLVM_CodeGenerator::ElementPtr(const AST::NumericSubscript & expr)
{
  Var & var = m_vars[expr.GetName()];
  const auto & bounds = AST::g_arrayBounds[expr.GetName()];
  std::string acc;
  for (size_t dim = 0; dim < expr.m_indexes.size(); ++dim) {
    std::string idx = Eval(*expr.m_indexes[dim]);
    std::string as64 = Tmp();
    if (expr.m_indexes[dim]->GetType() == VarType::eInt16)
      Emit(as64 + " = sext i16 " + idx + " to i64");
    else if (expr.m_indexes[dim]->GetType() == VarType::eInt32)
      Emit(as64 + " = sext i32 " + idx + " to i64");
    else {
      std::string as32 = Tmp();
      Emit(as32 + " = fptosi " + std::string(Ty(expr.m_indexes[dim]->GetType())) + " " + idx + " to i32");
      Emit(as64 + " = sext i32 " + as32 + " to i64");
    }
    if (acc.empty()) {
      acc = as64;
    }
    else {
      int span = dim < bounds.size() ? bounds[dim - 1] + 1 : 1;
      std::string scaled = Tmp();
      std::string added = Tmp();
      Emit(scaled + " = mul i64 " + acc + ", " + std::to_string(span));
      Emit(added + " = add i64 " + scaled + ", " + as64);
      acc = added;
    }
  }
  if (acc.empty())
    acc = "0";
  std::string ptr = Tmp();
  Emit(ptr + " = getelementptr " + var.agg + ", ptr " + var.global + ", i64 0, i64 " + acc);
  return ptr;
}

void LLVM_CodeGenerator::StoreNumeric(const AST::NumericVarRef & lhs, const AST::NumericExpr & rhs)
{
  std::string value = Conv(Eval(rhs), rhs.GetType(), lhs.GetType());
  if (auto * subscript = dynamic_cast<const AST::NumericSubscript *>(&lhs)) {
    if (AST::g_userFunctions.count(subscript->GetName()) == 0) {
      std::string ptr = ElementPtr(*subscript);
      Emit("store " + std::string(Ty(lhs.GetType())) + " " + value + ", ptr " + ptr);
      return;
    }
  }
  Var & var = m_vars[lhs.GetName()];
  Emit("store " + std::string(Ty(lhs.GetType())) + " " + value + ", ptr " + var.global);
}

void LLVM_CodeGenerator::PrintValue(VarType type, const std::string & value)
{
  if (type == VarType::eInt16)
    Emit("call i32 @basalt_print_int16(i16 " + value + ")");
  else if (type == VarType::eInt32)
    Emit("call i32 @basalt_print_int32(i32 " + value + ")");
  else if (type == VarType::eSingle)
    Emit("call i32 @basalt_print_single(float " + value + ")");
  else if (type == VarType::eDouble)
    Emit("call i32 @basalt_print_double(double " + value + ")");
  else if (type == VarType::eString)
    Emit("call i32 @basalt_print_string(ptr " + value + ")");
}

void LLVM_CodeGenerator::CallUser(const AST::NumericSubscript & expr, std::string & result, bool asFloat)
{
  auto found = AST::g_userFunctions.find(expr.GetName());
  if (found == AST::g_userFunctions.end() || found->second.m_body == nullptr) {
    result = "0";
    return;
  }
  const AST::UserFunction & fn = found->second;
  std::vector<std::string> args;
  for (auto * index : expr.m_indexes)
    args.push_back(Eval(*index));
  std::vector<std::pair<std::string, std::string>> saved;
  for (size_t i = 0; i < fn.m_params.size() && i < args.size(); ++i) {
    Var & var = m_vars[fn.m_params[i]];
    std::string old = Tmp();
    Emit(old + " = load " + Ty(var.type) + ", ptr " + var.global);
    std::string incoming = Conv(args[i], expr.m_indexes[i]->GetType(), var.type);
    Emit("store " + std::string(Ty(var.type)) + " " + incoming + ", ptr " + var.global);
    saved.push_back(std::make_pair(var.global, old));
  }
  bool bodyInt = fn.m_body->GetType() == VarType::eInt16 || fn.m_body->GetType() == VarType::eInt32;
  std::string value;
  if (bodyInt && !asFloat)
    value = Eval(*fn.m_body);
  else
    value = Conv(Eval(*fn.m_body), fn.m_body->GetType(), VarType::eSingle);
  for (auto it = saved.rbegin(); it != saved.rend(); ++it) {
    VarType type = VarType::eSingle;
    for (auto & entry : m_vars) {
      if (entry.second.global == it->first)
        type = entry.second.type;
    }
    Emit("store " + std::string(Ty(type)) + " " + it->second + ", ptr " + it->first);
  }
  result = value;
}

bool LLVM_CodeGenerator::Body()
{
  for (auto & entry : AST::g_subscriptArity) {
    if (AST::g_userFunctions.count(entry.first) != 0)
      continue;
    if (AST::g_arrayBounds.count(entry.first) == 0)
      AST::g_arrayBounds[entry.first] = std::vector<int>(entry.second, 10);
  }

  int tag = 0;
  for (auto & entry : AST::g_globalVars) {
    Var var;
    var.type = entry.second.m_type;
    var.global = "@" + Sanitise(entry.first);
    if (m_vars.count(entry.first) == 0) {
      bool clash = false;
      for (auto & have : m_vars) {
        if (have.second.global == var.global)
          clash = true;
      }
      if (clash)
        var.global += std::to_string(++tag);
    }
    auto bounds = AST::g_arrayBounds.find(entry.first);
    if (bounds != AST::g_arrayBounds.end() && AST::g_userFunctions.count(entry.first) == 0) {
      var.array = true;
      var.count = 1;
      var.agg = "";
      for (int upper : bounds->second) {
        int size = upper + 1;
        var.count *= size;
        var.agg = "[" + std::to_string(size) + " x " + (var.agg.empty() ? Ty(var.type) : var.agg) + "]";
      }
      m_decl << var.global << " = global " << var.agg << " zeroinitializer\n";
    }
    else if (var.type == VarType::eString) {
      m_decl << var.global << " = global ptr null\n";
    }
    else {
      m_decl << var.global << " = global " << Ty(var.type) << " " << Zero(var.type) << "\n";
    }
    m_vars[entry.first] = var;
  }

  m_decl << "@gs_sp = global i32 0\n";
  m_decl << "@gs_id = global i32 0\n";
  m_decl << "@gs_stk = global [64 x i32] zeroinitializer\n";

  m_body << "define i32 @main() {\nentry:\n";
  m_term = false;
  for (auto & line : m_program->m_list)
    line->Generate(*this);
  if (!m_term)
    m_body << "  br label %exit\n";
  if (!m_conts.empty()) {
    m_body << "gs_dispatch:\n";
    m_body << "  %gs_back = load i32, ptr @gs_id\n";
    m_body << "  switch i32 %gs_back, label %exit [\n";
    for (auto & cont : m_conts)
      m_body << "    i32 " << cont.id << ", label %" << cont.block << "\n";
    m_body << "  ]\n";
  }
  m_body << "exit:\n  ret i32 0\n}\n";

  *m_outputStream << "; basalt llvm backend\n";
  *m_outputStream << "declare i32 @basalt_print_string(ptr)\n";
  *m_outputStream << "declare i32 @basalt_print_int16(i16)\n";
  *m_outputStream << "declare i32 @basalt_print_int32(i32)\n";
  *m_outputStream << "declare i32 @basalt_print_single(float)\n";
  *m_outputStream << "declare i32 @basalt_print_double(double)\n";
  *m_outputStream << "declare i32 @basalt_print_tab()\n";
  *m_outputStream << "declare i32 @basalt_print_newline()\n";
  *m_outputStream << "declare void @basalt_set_width(i32)\n";
  *m_outputStream << "declare void @basalt_set_string(ptr, ptr)\n";
  *m_outputStream << "declare void @basalt_clear_string(ptr)\n";
  *m_outputStream << "declare double @basalt_read_number()\n";
  *m_outputStream << "declare void @basalt_input_string(ptr)\n";
  *m_outputStream << "declare void @basalt_line_input(ptr)\n";
  *m_outputStream << "declare float @basalt_rnd(float)\n";
  *m_outputStream << "declare double @pow(double, double)\n";
  *m_outputStream << "declare i32 @strcmp(ptr, ptr)\n";
  *m_outputStream << "declare void @llvm.memset.p0.i64(ptr, i8, i64, i1)\n";
  *m_outputStream << "@empty = private constant [1 x i8] c\"\\00\"\n";
  for (auto & entry : AST::g_stringConstants) {
    std::string escaped = Escape(entry.first);
    *m_outputStream << "@s" << entry.second << " = private constant ["
                    << (entry.first.size() + 1) << " x i8] c\"" << escaped << "\"\n";
  }
  *m_outputStream << m_decl.str();
  *m_outputStream << m_body.str();
  return true;
}

int LLVM_CodeGenerator::Generate(const AST::SourceLine & line)
{
  std::string label = "L" + std::to_string(line.GetSourceLineNumber());
  if (!m_term)
    m_body << "  br label %" << label << "\n";
  m_body << label << ":\n";
  m_term = false;
  if (line.m_statements)
    line.m_statements->Generate(*this);
  return 0;
}

int LLVM_CodeGenerator::Generate(const AST::End & expr)
{
  Term("br label %exit");
  return 0;
}

int LLVM_CodeGenerator::Generate(const AST::System & expr)
{
  Term("br label %exit");
  return 0;
}

int LLVM_CodeGenerator::Generate(const AST::Rem & expr)
{
  return 0;
}

int LLVM_CodeGenerator::Generate(const AST::AssignStatement & expr)
{
  if (expr.m_expr)
    expr.m_expr->Generate(*this);
  return 0;
}

int LLVM_CodeGenerator::Generate(const AST::NumericAssign & expr)
{
  if (expr.m_lhs == nullptr || expr.m_rhs == nullptr)
    return -1;
  StoreNumeric(*expr.m_lhs, *expr.m_rhs);
  return 0;
}

int LLVM_CodeGenerator::Generate(const AST::StringAssign & expr)
{
  if (expr.m_lhs == nullptr || expr.m_rhs == nullptr)
    return -1;
  Var & var = m_vars[expr.m_lhs->GetName()];
  std::string rhs = Eval(*expr.m_rhs);
  Emit("call void @basalt_set_string(ptr " + var.global + ", ptr " + rhs + ")");
  return 0;
}

int LLVM_CodeGenerator::Generate(const AST::IfStatement & expr)
{
  if (expr.m_cond == nullptr || expr.m_trueStatements == nullptr)
    return 0;
  std::string cond = Truth(*expr.m_cond);
  std::string yes = Block("then");
  std::string no = Block("else");
  std::string done = Block("endif");
  Term("br i1 " + cond + ", label %" + yes + ", label %" + no);
  m_body << yes << ":\n";
  m_term = false;
  if (!expr.m_trueStatements->m_lineNumber.empty()) {
    int line = ResolveGotoDestination(expr.m_trueStatements->m_lineNumber);
    Term("br label %L" + std::to_string(line));
  }
  else if (expr.m_trueStatements->m_statements)
    expr.m_trueStatements->m_statements->Generate(*this);
  if (!m_term)
    Term("br label %" + done);
  m_body << no << ":\n";
  m_term = false;
  if (expr.m_falseStatements)
    expr.m_falseStatements->Generate(*this);
  if (!m_term)
    Term("br label %" + done);
  m_body << done << ":\n";
  m_term = false;
  return 0;
}

int LLVM_CodeGenerator::Generate(const AST::ForStatement & expr)
{
  if (expr.m_var == nullptr || expr.m_fromVal == nullptr || expr.m_toVal == nullptr)
    return -1;
  Var & index = m_vars[expr.m_var->GetName()];
  VarType type = index.type;
  std::string from = Conv(Eval(*expr.m_fromVal), expr.m_fromVal->GetType(), type);
  std::string to = Conv(Eval(*expr.m_toVal), expr.m_toVal->GetType(), type);
  std::string step;
  if (expr.m_stepVal)
    step = Conv(Eval(*expr.m_stepVal), expr.m_stepVal->GetType(), type);
  else {
    std::string cmp = Tmp();
    if (IsFloat(type))
      Emit(cmp + " = fcmp ole " + std::string(Ty(type)) + " " + from + ", " + to);
    else
      Emit(cmp + " = icmp sle " + std::string(Ty(type)) + " " + from + ", " + to);
    step = Tmp();
    std::string one = IsFloat(type) ? std::string("1.0") : std::string("1");
    std::string neg = IsFloat(type) ? std::string("-1.0") : std::string("-1");
    Emit(step + " = select i1 " + cmp + ", " + Ty(type) + " " + one + ", " + Ty(type) + " " + neg);
  }
  int id = m_forId++;
  ForFrame frame;
  frame.type = type;
  frame.index = index.global;
  frame.toSlot = "@for_to_" + std::to_string(id);
  frame.stepSlot = "@for_step_" + std::to_string(id);
  frame.check = Block("forchk");
  frame.after = Block("foraft");
  m_decl << frame.toSlot << " = global " << Ty(type) << " " << Zero(type) << "\n";
  m_decl << frame.stepSlot << " = global " << Ty(type) << " " << Zero(type) << "\n";
  Emit("store " + std::string(Ty(type)) + " " + from + ", ptr " + index.global);
  Emit("store " + std::string(Ty(type)) + " " + to + ", ptr " + frame.toSlot);
  Emit("store " + std::string(Ty(type)) + " " + step + ", ptr " + frame.stepSlot);
  Term("br label %" + frame.check);
  m_body << frame.check << ":\n";
  m_term = false;
  std::string cur = Tmp();
  std::string lim = Tmp();
  std::string inc = Tmp();
  Emit(cur + " = load " + Ty(type) + ", ptr " + index.global);
  Emit(lim + " = load " + Ty(type) + ", ptr " + frame.toSlot);
  Emit(inc + " = load " + Ty(type) + ", ptr " + frame.stepSlot);
  std::string pos = Tmp();
  std::string le = Tmp();
  std::string ge = Tmp();
  std::string cond = Tmp();
  if (IsFloat(type)) {
    Emit(pos + " = fcmp oge " + std::string(Ty(type)) + " " + inc + ", 0.0");
    Emit(le + " = fcmp ole " + std::string(Ty(type)) + " " + cur + ", " + lim);
    Emit(ge + " = fcmp oge " + std::string(Ty(type)) + " " + cur + ", " + lim);
  }
  else {
    Emit(pos + " = icmp sge " + std::string(Ty(type)) + " " + inc + ", 0");
    Emit(le + " = icmp sle " + std::string(Ty(type)) + " " + cur + ", " + lim);
    Emit(ge + " = icmp sge " + std::string(Ty(type)) + " " + cur + ", " + lim);
  }
  Emit(cond + " = select i1 " + pos + ", i1 " + le + ", i1 " + ge);
  std::string body = Block("forb");
  Term("br i1 " + cond + ", label %" + body + ", label %" + frame.after);
  m_body << body << ":\n";
  m_term = false;
  m_fors.push_back(frame);
  return 0;
}

int LLVM_CodeGenerator::Generate(const AST::NextStatement & expr)
{
  if (m_fors.empty())
    return -1;
  ForFrame frame = m_fors.back();
  m_fors.pop_back();
  std::string cur = Tmp();
  std::string inc = Tmp();
  std::string sum = Tmp();
  Emit(cur + " = load " + Ty(frame.type) + ", ptr " + frame.index);
  Emit(inc + " = load " + Ty(frame.type) + ", ptr " + frame.stepSlot);
  const char * op = IsFloat(frame.type) ? "fadd" : "add";
  Emit(sum + " = " + op + " " + std::string(Ty(frame.type)) + " " + cur + ", " + inc);
  Emit("store " + std::string(Ty(frame.type)) + " " + sum + ", ptr " + frame.index);
  Term("br label %" + frame.check);
  m_body << frame.after << ":\n";
  m_term = false;
  return 0;
}

int LLVM_CodeGenerator::Generate(const AST::GotoStatement & expr)
{
  int line = ResolveGotoDestination(expr.GetRef());
  if (line < 0)
    return 1;
  Term("br label %L" + std::to_string(line));
  return 0;
}

int LLVM_CodeGenerator::Generate(const AST::GosubStatement & expr)
{
  int line = ResolveGotoDestination(expr.GetRef());
  if (line < 0)
    return 1;
  int id = ++m_contId;
  std::string cont = Block("cont");
  std::string sp = Tmp();
  std::string slot = Tmp();
  Emit(sp + " = load i32, ptr @gs_sp");
  Emit(slot + " = getelementptr [64 x i32], ptr @gs_stk, i64 0, i32 " + sp);
  Emit("store i32 " + std::to_string(id) + ", ptr " + slot);
  std::string next = Tmp();
  Emit(next + " = add i32 " + sp + ", 1");
  Emit("store i32 " + next + ", ptr @gs_sp");
  Term("br label %L" + std::to_string(line));
  m_body << cont << ":\n";
  m_term = false;
  m_conts.push_back(Cont{ id, cont });
  return 0;
}

int LLVM_CodeGenerator::Generate(const AST::ReturnStatement & expr)
{
  std::string sp = Tmp();
  std::string prev = Tmp();
  std::string slot = Tmp();
  std::string id = Tmp();
  Emit(sp + " = load i32, ptr @gs_sp");
  Emit(prev + " = sub i32 " + sp + ", 1");
  Emit("store i32 " + prev + ", ptr @gs_sp");
  Emit(slot + " = getelementptr [64 x i32], ptr @gs_stk, i64 0, i32 " + prev);
  Emit(id + " = load i32, ptr " + slot);
  Emit("store i32 " + id + ", ptr @gs_id");
  Term("br label %gs_dispatch");
  return 0;
}

int LLVM_CodeGenerator::Generate(const AST::Print & printExpr)
{
  if (printExpr.m_list == nullptr)
    return 0;
  for (auto & item : printExpr.m_list->m_list)
    item->Print(*this);
  if (!printExpr.m_list->m_list.empty() &&
      !printExpr.m_list->m_list.back()->IsPrintSemiColon())
    Emit("call i32 @basalt_print_newline()");
  return 0;
}

int LLVM_CodeGenerator::Generate(const AST::OnGotoStatement & expr)
{
  if (expr.m_index == nullptr)
    return -1;
  std::string value = Eval(*expr.m_index);
  std::string as32 = Conv(value, expr.m_index->GetType(), VarType::eInt32);
  std::string fall = Block("onfall");
  std::string insn = "switch i32 " + as32 + ", label %" + fall + " [";
  for (size_t i = 0; i < expr.m_lines.size(); ++i) {
    int line = ResolveGotoDestination(expr.m_lines[i]);
    if (line < 0)
      return 1;
    insn += " i32 " + std::to_string(i + 1) + ", label %L" + std::to_string(line);
  }
  insn += " ]";
  Term(insn);
  m_body << fall << ":\n";
  m_term = false;
  return 0;
}

int LLVM_CodeGenerator::Generate(const AST::ClearStatement & expr)
{
  for (auto & entry : m_vars) {
    if (AST::g_userFunctions.count(entry.first) != 0)
      continue;
    if (entry.second.type == VarType::eString) {
      Emit("call void @basalt_clear_string(ptr " + entry.second.global + ")");
    }
    else if (entry.second.array) {
      int width = 4;
      if (entry.second.type == VarType::eInt16)
        width = 2;
      else if (entry.second.type == VarType::eDouble)
        width = 8;
      Emit("call void @llvm.memset.p0.i64(ptr " + entry.second.global + ", i8 0, i64 " +
           std::to_string(entry.second.count * width) + ", i1 false)");
    }
    else {
      Emit("store " + std::string(Ty(entry.second.type)) + " " + Zero(entry.second.type) +
           ", ptr " + entry.second.global);
    }
  }
  return 0;
}

int LLVM_CodeGenerator::Generate(const AST::WidthStatement & expr)
{
  Emit("call void @basalt_set_width(i32 " + std::to_string(expr.m_width) + ")");
  return 0;
}

int LLVM_CodeGenerator::Generate(const AST::InputStatement & expr)
{
  if (!expr.m_prompt.empty()) {
    if (AST::g_stringConstants.count(expr.m_prompt) == 0)
      AST::g_stringConstants[expr.m_prompt] = AST::g_stringConstantIndex++;
    Emit("call i32 @basalt_print_string(ptr @s" +
         std::to_string(AST::g_stringConstants[expr.m_prompt]) + ")");
  }
  if (expr.m_vars == nullptr)
    return 0;
  for (auto & item : expr.m_vars->m_list) {
    auto * str = dynamic_cast<AST::StringVarRef *>(item.get());
    auto * numeric = dynamic_cast<AST::NumericVarRef *>(item.get());
    if (str != nullptr) {
      Var & var = m_vars[str->GetName()];
      if (expr.m_lineInput)
        Emit("call void @basalt_line_input(ptr " + var.global + ")");
      else
        Emit("call void @basalt_input_string(ptr " + var.global + ")");
    }
    else if (numeric != nullptr) {
      Var & var = m_vars[numeric->GetName()];
      std::string raw = Tmp();
      Emit(raw + " = call double @basalt_read_number()");
      std::string converted = Conv(raw, VarType::eDouble, numeric->GetType());
      Emit("store " + std::string(Ty(numeric->GetType())) + " " + converted + ", ptr " + var.global);
    }
  }
  return 0;
}

int LLVM_CodeGenerator::Generate(const AST::DimStatement & expr)
{
  return 0;
}

int LLVM_CodeGenerator::Generate(const AST::DefStatement & expr)
{
  return 0;
}

int LLVM_CodeGenerator::Evaluate(const AST::Int16Constant & expr, std::string & result)
{
  result = std::to_string(expr.GetValue());
  return 0;
}

int LLVM_CodeGenerator::Evaluate(const AST::Int32Constant & expr, std::string & result)
{
  result = std::to_string(expr.GetValue());
  return 0;
}

int LLVM_CodeGenerator::Evaluate(const AST::SingleConstant & expr, std::string & result)
{
  result = FLit(expr.AsSingle());
  return 0;
}

int LLVM_CodeGenerator::Evaluate(const AST::DoubleConstant & expr, std::string & result)
{
  result = DLit(expr.AsDouble());
  return 0;
}

int LLVM_CodeGenerator::Evaluate(const AST::StringConstant & expr, std::string & result)
{
  result = "@s" + std::to_string(AST::g_stringConstants[expr.GetValue()]);
  return 0;
}

int LLVM_CodeGenerator::Evaluate(const AST::NumericVarRef & expr, std::string & result)
{
  Var & var = m_vars[expr.GetName()];
  result = Tmp();
  Emit(result + " = load " + Ty(var.type) + ", ptr " + var.global);
  return 0;
}

int LLVM_CodeGenerator::Evaluate(const AST::StringVarRef & expr, std::string & result)
{
  Var & var = m_vars[expr.GetName()];
  result = Tmp();
  Emit(result + " = load ptr, ptr " + var.global);
  return 0;
}

int LLVM_CodeGenerator::Evaluate(const AST::NumericSubscript & expr, std::string & result)
{
  if (AST::g_userFunctions.count(expr.GetName()) != 0) {
    auto & fn = AST::g_userFunctions[expr.GetName()];
    bool bodyInt = fn.m_body != nullptr &&
      (fn.m_body->GetType() == VarType::eInt16 || fn.m_body->GetType() == VarType::eInt32);
    CallUser(expr, result, !bodyInt);
    return 0;
  }
  std::string ptr = ElementPtr(expr);
  result = Tmp();
  Emit(result + " = load " + Ty(expr.GetType()) + ", ptr " + ptr);
  return 0;
}

int LLVM_CodeGenerator::Evaluate(const AST::NumericAddition & expr, std::string & result)
{
  result = Bin("add", "fadd", expr);
  return 0;
}

int LLVM_CodeGenerator::Evaluate(const AST::Subtraction & expr, std::string & result)
{
  result = Bin("sub", "fsub", expr);
  return 0;
}

int LLVM_CodeGenerator::Evaluate(const AST::Multiplication & expr, std::string & result)
{
  result = Bin("mul", "fmul", expr);
  return 0;
}

int LLVM_CodeGenerator::Evaluate(const AST::Division & expr, std::string & result)
{
  result = Bin("sdiv", "fdiv", expr);
  return 0;
}

int LLVM_CodeGenerator::Evaluate(const AST::Power & expr, std::string & result)
{
  std::string lhs = Conv(Eval(*expr.m_lhs), expr.m_lhs->GetType(), VarType::eDouble);
  std::string rhs = Conv(Eval(*expr.m_rhs), expr.m_rhs->GetType(), VarType::eDouble);
  std::string raw = Tmp();
  Emit(raw + " = call double @pow(double " + lhs + ", double " + rhs + ")");
  result = Conv(raw, VarType::eDouble, expr.GetType());
  return 0;
}

int LLVM_CodeGenerator::Evaluate(const AST::Negation & expr, std::string & result)
{
  std::string value = Eval(*expr.m_expr);
  result = Tmp();
  if (IsFloat(expr.GetType()))
    Emit(result + " = fneg " + std::string(Ty(expr.GetType())) + " " + value);
  else
    Emit(result + " = sub " + std::string(Ty(expr.GetType())) + " 0, " + value);
  return 0;
}

int LLVM_CodeGenerator::Evaluate(const AST::NumericEquality & expr, std::string & result)
{
  result = Cmp("eq", "oeq", expr);
  return 0;
}

int LLVM_CodeGenerator::Evaluate(const AST::NumericNotEquality & expr, std::string & result)
{
  result = Cmp("ne", "one", expr);
  return 0;
}

int LLVM_CodeGenerator::Evaluate(const AST::NumericGreaterThan & expr, std::string & result)
{
  result = Cmp("sgt", "ogt", expr);
  return 0;
}

int LLVM_CodeGenerator::Evaluate(const AST::NumericGreaterThanEqual & expr, std::string & result)
{
  result = Cmp("sge", "oge", expr);
  return 0;
}

int LLVM_CodeGenerator::Evaluate(const AST::NumericLessThan & expr, std::string & result)
{
  result = Cmp("slt", "olt", expr);
  return 0;
}

int LLVM_CodeGenerator::Evaluate(const AST::NumericLessThanEqual & expr, std::string & result)
{
  result = Cmp("sle", "ole", expr);
  return 0;
}

int LLVM_CodeGenerator::Evaluate(const AST::LogicalAnd & expr, std::string & result)
{
  std::string lhs = Conv(Eval(*expr.m_lhs), expr.m_lhs->GetType(), VarType::eInt16);
  std::string rhs = Conv(Eval(*expr.m_rhs), expr.m_rhs->GetType(), VarType::eInt16);
  result = Tmp();
  Emit(result + " = and i16 " + lhs + ", " + rhs);
  return 0;
}

int LLVM_CodeGenerator::Evaluate(const AST::LogicalOr & expr, std::string & result)
{
  std::string lhs = Conv(Eval(*expr.m_lhs), expr.m_lhs->GetType(), VarType::eInt16);
  std::string rhs = Conv(Eval(*expr.m_rhs), expr.m_rhs->GetType(), VarType::eInt16);
  result = Tmp();
  Emit(result + " = or i16 " + lhs + ", " + rhs);
  return 0;
}

int LLVM_CodeGenerator::Evaluate(const AST::StringCompare & expr, std::string & result)
{
  auto pointer = [this](const AST::StringExpr * side) {
    std::string value = Eval(*side);
    if (dynamic_cast<const AST::StringConstant *>(side) != nullptr)
      return value;
    std::string nil = Tmp();
    std::string picked = Tmp();
    Emit(nil + " = icmp eq ptr " + value + ", null");
    Emit(picked + " = select i1 " + nil + ", ptr @empty, ptr " + value);
    return picked;
  };
  std::string lhs = pointer(expr.m_lhs);
  std::string rhs = pointer(expr.m_rhs);
  std::string cmp = Tmp();
  Emit(cmp + " = call i32 @strcmp(ptr " + lhs + ", ptr " + rhs + ")");
  std::string flag = Tmp();
  if (expr.m_equal)
    Emit(flag + " = icmp eq i32 " + cmp + ", 0");
  else
    Emit(flag + " = icmp ne i32 " + cmp + ", 0");
  std::string wide = Tmp();
  result = Tmp();
  Emit(wide + " = zext i1 " + flag + " to i16");
  Emit(result + " = sub i16 0, " + wide);
  return 0;
}

int LLVM_CodeGenerator::Evaluate(const AST::IntFunction & expr, std::string & result)
{
  if (expr.GetArg1() == nullptr)
    return -1;
  std::string value = Eval(*expr.GetArg1());
  std::string bits = Tmp();
  if (IsFloat(expr.GetArg1()->GetType()))
    Emit(bits + " = fptosi " + std::string(Ty(expr.GetArg1()->GetType())) + " " + value + " to i32");
  else
    bits = Conv(value, expr.GetArg1()->GetType(), VarType::eInt32);
  result = Conv(bits, VarType::eInt32, expr.GetType());
  return 0;
}

int LLVM_CodeGenerator::Evaluate(const AST::RndFunction & expr, std::string & result)
{
  std::string arg = "1.0";
  if (expr.m_arg != nullptr)
    arg = Conv(Eval(*expr.m_arg), expr.m_arg->GetType(), VarType::eSingle);
  result = Tmp();
  Emit(result + " = call float @basalt_rnd(float " + arg + ")");
  return 0;
}

int LLVM_CodeGenerator::Evaluate(const AST::NumericCast & expr, std::string & result)
{
  if (expr.m_from == nullptr)
    return -1;
  result = Conv(Eval(*expr.m_from), expr.m_from->GetType(), expr.GetType());
  return 0;
}

int LLVM_CodeGenerator::Print(const AST::NumericExpr & expr)
{
  PrintValue(expr.GetType(), Eval(expr));
  return 0;
}

int LLVM_CodeGenerator::Print(const AST::NumericVarRef & expr)
{
  std::string value;
  Evaluate(expr, value);
  PrintValue(expr.GetType(), value);
  return 0;
}

int LLVM_CodeGenerator::Print(const AST::NumericSubscript & expr)
{
  if (AST::g_userFunctions.count(expr.GetName()) != 0) {
    auto & fn = AST::g_userFunctions[expr.GetName()];
    VarType type = fn.m_body != nullptr ? fn.m_body->GetType() : expr.GetType();
    std::string value;
    Evaluate(expr, value);
    PrintValue(type, value);
    return 0;
  }
  std::string value;
  Evaluate(expr, value);
  PrintValue(expr.GetType(), value);
  return 0;
}

int LLVM_CodeGenerator::Print(const AST::Int16Constant & expr)
{
  PrintValue(VarType::eInt16, std::to_string(expr.GetValue()));
  return 0;
}

int LLVM_CodeGenerator::Print(const AST::Int32Constant & expr)
{
  PrintValue(VarType::eInt32, std::to_string(expr.GetValue()));
  return 0;
}

int LLVM_CodeGenerator::Print(const AST::SingleConstant & expr)
{
  PrintValue(VarType::eSingle, FLit(expr.AsSingle()));
  return 0;
}

int LLVM_CodeGenerator::Print(const AST::DoubleConstant & expr)
{
  PrintValue(VarType::eDouble, DLit(expr.AsDouble()));
  return 0;
}

int LLVM_CodeGenerator::Print(const AST::StringConstant & expr)
{
  PrintValue(VarType::eString, "@s" + std::to_string(AST::g_stringConstants[expr.GetValue()]));
  return 0;
}

int LLVM_CodeGenerator::Print(const AST::StringVarRef & expr)
{
  std::string value;
  Evaluate(expr, value);
  PrintValue(VarType::eString, value);
  return 0;
}

int LLVM_CodeGenerator::Print(const AST::PrintComma & expr)
{
  Emit("call i32 @basalt_print_tab()");
  return 0;
}
