#include "codegen.h"
#include "outputgen.h"
#include "irutil.h"
#include "basalt.h"

#include <cmath>
#include <iomanip>
#include <iostream>
#include <typeinfo>
#include <cxxabi.h>

using namespace std;

#define GEN_NODE(T) \
  int CodeGenerator::T::Generate(OutputGenerator & gen) { return gen.Generate(*this); }

GEN_NODE(BlockStart)
GEN_NODE(BlockEnd)
GEN_NODE(GotoTarget)
GEN_NODE(Goto)
GEN_NODE(Gosub)
GEN_NODE(Return)
GEN_NODE(End)
GEN_NODE(System)
GEN_NODE(If)
GEN_NODE(Else)
GEN_NODE(EndIf)
GEN_NODE(CreateTempVar)
GEN_NODE(PrintNewLine)
GEN_NODE(PrintTab)
GEN_NODE(PrintStringConst)
GEN_NODE(PrintStringVar)
GEN_NODE(PrintNumber)
GEN_NODE(UnaryOperator)
GEN_NODE(BinaryOperator)
GEN_NODE(IndexOp)
GEN_NODE(Input)
GEN_NODE(OnGoto)
GEN_NODE(Clear)
GEN_NODE(Width)

void CodeGenerator::CompilerErrorInternal(ErrorCode code, unsigned ln, const std::string & msg)
{
  g_application.CompilerErrorInternal(code, ln, msg);
}

std::string CodeGenerator::GetTempName()
{
  return "temp_" + std::to_string(m_tempIndex++);
}

std::string CodeGenerator::NumberText(const AST::NumericConstant & expr) const
{
  std::ostringstream strm;
  switch (expr.GetType()) {
    case VarType::eInt16:
      strm << expr.AsInt16();
      break;
    case VarType::eInt32:
      strm << expr.AsInt32();
      break;
    case VarType::eSingle:
      strm << std::setprecision(9) << expr.AsSingle();
      break;
    case VarType::eDouble:
      strm << std::setprecision(17) << expr.AsDouble();
      break;
    default:
      strm << 0;
      break;
  }
  return strm.str();
}

bool CodeGenerator::Build(const AST::Program & program)
{
  m_program = &program;
  ApplyDefaultArrayBounds();
  CheckVars();
  CheckJumps();
  for (auto & line : program.m_list) {
    if (line != nullptr)
      line->Generate(*this);
  }
  return true;
}

bool CodeGenerator::CheckVars()
{
  for (auto & entry : AST::g_globalVars) {
    const AST::VarInfo & info = entry.second;
    if (info.m_lhsLine != 0 && info.m_rhsLine == 0)
      CompilerError(eWarning_VarDefinedButNotUsed, info.m_lhsLine,
                    "variable '" << entry.first << "' defined but not used");
  }
  return true;
}

bool CodeGenerator::CheckJumps()
{
  for (auto & entry : AST::g_jumpDestinationInfo) {
    if (AST::g_lineNumberInfo.count(entry.first) == 0) {
      unsigned line = entry.second.m_usedLine.empty() ? 0 : *entry.second.m_usedLine.begin();
      CompilerError(eError_GotoDestinationNotFound, line, entry.first);
    }
  }
  return true;
}

template <typename T>
static T FoldValue(T x, T y, const std::string & op)
{
  if (op == "+") return x + y;
  if (op == "-") return x - y;
  if (op == "*") return x * y;
  if (op == "/") return x / y;
  if (op == "==") return (x == y) ? T(-1) : T(0);
  if (op == "!=") return (x != y) ? T(-1) : T(0);
  if (op == ">") return (x > y) ? T(-1) : T(0);
  if (op == ">=") return (x >= y) ? T(-1) : T(0);
  if (op == "<") return (x < y) ? T(-1) : T(0);
  if (op == "<=") return (x <= y) ? T(-1) : T(0);
  return T(0);
}

bool CodeGenerator::FoldBinary(const std::string & op, VarType type,
                               const std::string & lhs, const std::string & rhs,
                               std::string & result)
{
  if (!IsNumberLiteral(lhs) || !IsNumberLiteral(rhs))
    return false;
  if (op == "/" && std::strtod(rhs.c_str(), nullptr) == 0.0)
    return false;
  if (op == "&" || op == "|") {
    int32_t left = std::stoi(lhs);
    int32_t right = std::stoi(rhs);
    result = std::to_string(op == "&" ? (left & right) : (left | right));
    return true;
  }
  std::ostringstream strm;
  switch (type) {
    case VarType::eInt16:
      strm << static_cast<int16_t>(FoldValue<int32_t>(std::stoi(lhs), std::stoi(rhs), op));
      break;
    case VarType::eInt32:
      strm << FoldValue<int32_t>(std::stoi(lhs), std::stoi(rhs), op);
      break;
    case VarType::eSingle:
      strm << std::setprecision(9) << FoldValue<float>(std::stof(lhs), std::stof(rhs), op);
      break;
    case VarType::eDouble:
      strm << std::setprecision(17) << FoldValue<double>(std::stod(lhs), std::stod(rhs), op);
      break;
    default:
      return false;
  }
  result = strm.str();
  return true;
}

void CodeGenerator::EmitPrint(VarType type, const std::string & value, bool literal)
{
  m_printUsed = true;
  if (type == VarType::eString) {
    if (literal)
      Add<PrintStringConst>(value);
    else
      Add<PrintStringVar>(value);
    return;
  }
  Add<PrintNumber>(type, value, literal || IsNumberLiteral(value));
}

int CodeGenerator::Generate(const AST::Node & expr)
{
  cerr << "unimplemented Generate for " << typeid(expr).name() << endl;
  return 1;
}

int CodeGenerator::Evaluate(const AST::Node & expr, std::string & result)
{
  cerr << "unimplemented Evaluate for " << typeid(expr).name() << endl;
  result = "0";
  return 1;
}

int CodeGenerator::Print(const AST::Node & node)
{
  if (auto * text = dynamic_cast<const AST::StringExpr *>(&node))
    return Print(*text);
  if (auto * number = dynamic_cast<const AST::NumericExpr *>(&node))
    return Print(*number);
  cerr << "unimplemented Print for " << typeid(node).name() << endl;
  return 0;
}

int CodeGenerator::Generate(const AST::SourceLine & line)
{
  if (!line.GetBasicLineNumber().empty() &&
      AST::g_jumpDestinationInfo.count(line.GetBasicLineNumber()) != 0)
    Add<GotoTarget>("line_" + line.GetBasicLineNumber());
  if (line.m_statements == nullptr)
    return 0;
  for (auto & statement : line.m_statements->m_list) {
    if (statement == nullptr)
      continue;
    Add<BlockStart>();
    m_lineNumber = statement->m_lineNumber;
    statement->Generate(*this);
    Add<BlockEnd>();
  }
  return 0;
}

int CodeGenerator::Generate(const AST::Statement &)
{
  return 0;
}

int CodeGenerator::Generate(const AST::Rem &)
{
  return 0;
}

int CodeGenerator::Generate(const AST::GotoStatement & statement)
{
  Add<Goto>("line_" + statement.GetRef());
  return 0;
}

int CodeGenerator::Generate(const AST::GosubStatement & statement)
{
  m_funcsUsed.insert("gosub");
  Add<Gosub>(statement.GetRef());
  return 0;
}

int CodeGenerator::Generate(const AST::ReturnStatement &)
{
  m_funcsUsed.insert("gosub");
  Add<Return>();
  return 0;
}

int CodeGenerator::Generate(const AST::System &)
{
  Add<System>();
  return 0;
}

int CodeGenerator::Generate(const AST::End &)
{
  Add<End>();
  return 0;
}

int CodeGenerator::Generate(const AST::Print & printExpr)
{
  m_printUsed = true;
  bool trailingNewLine = true;
  if (printExpr.m_list != nullptr && !printExpr.m_list->m_list.empty()) {
    for (auto & item : printExpr.m_list->m_list) {
      if (item != nullptr)
        item->Print(*this);
    }
    if (printExpr.m_list->m_list.back()->IsPrintSemiColon())
      trailingNewLine = false;
  }
  if (trailingNewLine)
    Add<PrintNewLine>();
  return 0;
}

int CodeGenerator::Print(const AST::PrintComma &)
{
  Add<PrintTab>();
  return 0;
}

int CodeGenerator::Print(const AST::PrintSemiColon &)
{
  return 0;
}

int CodeGenerator::Print(const AST::StringConstant & expr)
{
  EmitPrint(VarType::eString, expr.GetValue(), true);
  return 0;
}

int CodeGenerator::Print(const AST::StringVarRef & expr)
{
  EmitPrint(VarType::eString, expr.GetName(), false);
  return 0;
}

int CodeGenerator::Print(const AST::StringExpr & expr)
{
  std::string value;
  expr.Evaluate(*this, value);
  EmitPrint(VarType::eString, value, false);
  return 0;
}

int CodeGenerator::Print(const AST::NumericExpr & expr)
{
  std::string value;
  expr.Evaluate(*this, value);
  EmitPrint(expr.GetType(), value, expr.IsConstant());
  return 0;
}

int CodeGenerator::Print(const AST::Int16Constant & expr) { return Print(static_cast<const AST::NumericExpr &>(expr)); }
int CodeGenerator::Print(const AST::Int32Constant & expr) { return Print(static_cast<const AST::NumericExpr &>(expr)); }
int CodeGenerator::Print(const AST::SingleConstant & expr) { return Print(static_cast<const AST::NumericExpr &>(expr)); }
int CodeGenerator::Print(const AST::DoubleConstant & expr) { return Print(static_cast<const AST::NumericExpr &>(expr)); }
int CodeGenerator::Print(const AST::NumericVarRef & expr) { return Print(static_cast<const AST::NumericExpr &>(expr)); }

int CodeGenerator::Print(const AST::NumericSubscript & expr)
{
  std::string value;
  if (Evaluate(expr, value) != 0)
    return -1;
  VarType type = expr.GetType();
  auto function = AST::g_userFunctions.find(expr.GetName());
  if (function != AST::g_userFunctions.end() && function->second.m_body != nullptr)
    type = function->second.m_body->GetType();
  EmitPrint(type, value, IsNumberLiteral(value));
  return 0;
}

int CodeGenerator::Generate(const AST::AssignStatement & expr)
{
  m_lineNumber = expr.m_lineNumber;
  if (expr.m_expr != nullptr)
    return expr.m_expr->Generate(*this);
  return 0;
}

int CodeGenerator::Evaluate(const AST::StringConstant & expr, std::string & result)
{
  auto found = AST::g_stringConstants.find(expr.GetValue());
  unsigned index = (found == AST::g_stringConstants.end()) ? 0 : found->second;
  result = "str_" + std::to_string(index);
  return 0;
}

int CodeGenerator::Evaluate(const AST::StringVarRef & expr, std::string & result)
{
  if (AST::g_globalVars.count(expr.GetName()) == 0) {
    CompilerError(Error_UndeclaredVariable, m_lineNumber, "variable \"" << expr.GetName() << "\" not declared");
    result = "0";
    return -1;
  }
  result = expr.GetName();
  return 0;
}

int CodeGenerator::Generate(const AST::StringAssign & expr)
{
  if (expr.m_rhs == nullptr)
    return -1;
  std::string rhs;
  expr.m_rhs->Evaluate(*this, rhs);
  Add<UnaryOperator>("sets", VarType::eString, expr.m_lhs->GetName(), rhs);
  return 0;
}

int CodeGenerator::Evaluate(const AST::StringAddition & expr, std::string & result)
{
  std::string lhs, rhs;
  if (expr.m_lhs != nullptr)
    expr.m_lhs->Evaluate(*this, lhs);
  if (expr.m_rhs != nullptr)
    expr.m_rhs->Evaluate(*this, rhs);
  result = GetTempName();
  Add<CreateTempVar>(VarType::eString, result);
  Add<BinaryOperator>("cat", VarType::eString, result, lhs, rhs);
  return 0;
}

int CodeGenerator::Evaluate(const AST::NumericVarRef & expr, std::string & result)
{
  if (AST::g_globalVars.count(expr.GetName()) == 0) {
    CompilerError(Error_UndeclaredVariable, m_lineNumber, "variable \"" << expr.GetName() << "\" not declared");
    result = "0";
    return -1;
  }
  result = expr.GetName();
  return 0;
}

int CodeGenerator::Evaluate(const AST::Int16Constant & expr, std::string & result)
{
  result = NumberText(expr);
  return 0;
}

int CodeGenerator::Evaluate(const AST::Int32Constant & expr, std::string & result)
{
  result = NumberText(expr);
  return 0;
}

int CodeGenerator::Evaluate(const AST::SingleConstant & expr, std::string & result)
{
  result = NumberText(expr);
  return 0;
}

int CodeGenerator::Evaluate(const AST::DoubleConstant & expr, std::string & result)
{
  result = NumberText(expr);
  return 0;
}

int CodeGenerator::Generate(const AST::NumericAssign & expr)
{
  if (expr.m_rhs == nullptr)
    return -1;
  std::string rhs;
  expr.m_rhs->Evaluate(*this, rhs);
  auto * subscript = dynamic_cast<const AST::NumericSubscript *>(expr.m_lhs);
  if (subscript != nullptr && AST::g_userFunctions.count(subscript->GetName()) == 0) {
    std::vector<std::string> indexes;
    for (auto * index : subscript->m_indexes) {
      std::string name;
      index->Evaluate(*this, name);
      indexes.push_back(name);
    }
    Add<IndexOp>(true, subscript->GetType(), subscript->GetName(), indexes, rhs);
    return 0;
  }
  Add<UnaryOperator>("=", expr.m_lhs->GetType(), expr.m_lhs->GetName(), rhs);
  return 0;
}

int CodeGenerator::BinaryNumeric(const std::string & op, const AST::NumericBinaryOperation & expr, std::string & result)
{
  if (expr.m_lhs == nullptr || expr.m_rhs == nullptr)
    return -1;
  std::string lhs, rhs;
  expr.m_lhs->Evaluate(*this, lhs);
  expr.m_rhs->Evaluate(*this, rhs);
  if (FoldBinary(op, expr.GetType(), lhs, rhs, result))
    return 0;
  result = GetTempName();
  Add<CreateTempVar>(expr.GetType(), result);
  Add<BinaryOperator>(op, expr.GetType(), result, lhs, rhs);
  return 0;
}

int CodeGenerator::CompareNumeric(const std::string & op, const AST::NumericBinaryOperation & expr, std::string & result)
{
  if (expr.m_lhs == nullptr || expr.m_rhs == nullptr)
    return -1;
  std::string lhs, rhs;
  expr.m_lhs->Evaluate(*this, lhs);
  expr.m_rhs->Evaluate(*this, rhs);
  VarType operand = expr.m_lhs->GetType();
  if (static_cast<int>(expr.m_rhs->GetType()) > static_cast<int>(operand))
    operand = expr.m_rhs->GetType();
  if (FoldBinary(op, operand, lhs, rhs, result))
    return 0;
  result = GetTempName();
  Add<CreateTempVar>(VarType::eInt16, result);
  Add<BinaryOperator>(op, operand, result, lhs, rhs);
  return 0;
}

int CodeGenerator::Evaluate(const AST::NumericAddition & expr, std::string & result) { return BinaryNumeric("+", expr, result); }
int CodeGenerator::Evaluate(const AST::Subtraction & expr, std::string & result) { return BinaryNumeric("-", expr, result); }
int CodeGenerator::Evaluate(const AST::Multiplication & expr, std::string & result) { return BinaryNumeric("*", expr, result); }
int CodeGenerator::Evaluate(const AST::Division & expr, std::string & result) { return BinaryNumeric("/", expr, result); }
int CodeGenerator::Evaluate(const AST::NumericEquality & expr, std::string & result) { return CompareNumeric("==", expr, result); }
int CodeGenerator::Evaluate(const AST::NumericNotEquality & expr, std::string & result) { return CompareNumeric("!=", expr, result); }
int CodeGenerator::Evaluate(const AST::NumericGreaterThan & expr, std::string & result) { return CompareNumeric(">", expr, result); }
int CodeGenerator::Evaluate(const AST::NumericGreaterThanEqual & expr, std::string & result) { return CompareNumeric(">=", expr, result); }
int CodeGenerator::Evaluate(const AST::NumericLessThan & expr, std::string & result) { return CompareNumeric("<", expr, result); }
int CodeGenerator::Evaluate(const AST::NumericLessThanEqual & expr, std::string & result) { return CompareNumeric("<=", expr, result); }

int CodeGenerator::Evaluate(const AST::LogicalAnd & expr, std::string & result)
{
  return BinaryNumeric("&", expr, result);
}

int CodeGenerator::Evaluate(const AST::LogicalOr & expr, std::string & result)
{
  return BinaryNumeric("|", expr, result);
}

int CodeGenerator::UnaryNumeric(const std::string & op, VarType type, const AST::NumericExpr * arg, std::string & result)
{
  if (arg == nullptr) {
    result = "0";
    return -1;
  }
  std::string value;
  arg->Evaluate(*this, value);
  if (IsNumberLiteral(value) && (op == "neg" || op == "abs" || op == "int")) {
    double number = std::strtod(value.c_str(), nullptr);
    std::ostringstream strm;
    if (op == "neg")
      number = -number;
    else if (op == "abs")
      number = std::fabs(number);
    else
      number = static_cast<int16_t>(number);
    if (type == VarType::eInt16)
      strm << static_cast<int16_t>(number);
    else if (type == VarType::eInt32)
      strm << static_cast<int32_t>(number);
    else if (type == VarType::eSingle)
      strm << std::setprecision(9) << static_cast<float>(number);
    else
      strm << std::setprecision(17) << number;
    result = strm.str();
    return 0;
  }
  result = GetTempName();
  Add<CreateTempVar>(type, result);
  Add<UnaryOperator>(op, type, result, value);
  return 0;
}

int CodeGenerator::Evaluate(const AST::Negation & expr, std::string & result)
{
  return UnaryNumeric("neg", expr.GetType(), dynamic_cast<const AST::NumericExpr *>(expr.m_expr), result);
}

int CodeGenerator::Evaluate(const AST::Power & expr, std::string & result)
{
  if (expr.m_lhs == nullptr || expr.m_rhs == nullptr)
    return -1;
  std::string lhs, rhs;
  expr.m_lhs->Evaluate(*this, lhs);
  expr.m_rhs->Evaluate(*this, rhs);
  result = GetTempName();
  Add<CreateTempVar>(expr.GetType(), result);
  Add<BinaryOperator>("pow", expr.GetType(), result, lhs, rhs);
  m_funcsUsed.insert("pow");
  return 0;
}

int CodeGenerator::Evaluate(const AST::NumericCast & expr, std::string & result)
{
  if (expr.m_from == nullptr) {
    result = "0";
    return -1;
  }
  std::string value;
  expr.m_from->Evaluate(*this, value);
  if (IsNumberLiteral(value)) {
    AST::NumericConstant * held = nullptr;
    (void)held;
    double number = std::strtod(value.c_str(), nullptr);
    std::ostringstream strm;
    switch (expr.GetType()) {
      case VarType::eInt16: strm << static_cast<int16_t>(number); break;
      case VarType::eInt32: strm << static_cast<int32_t>(number); break;
      case VarType::eSingle: strm << std::setprecision(9) << static_cast<float>(number); break;
      case VarType::eDouble: strm << std::setprecision(17) << static_cast<double>(number); break;
      default: strm << value; break;
    }
    result = strm.str();
    return 0;
  }
  if (expr.m_from->GetType() == expr.GetType()) {
    result = value;
    return 0;
  }
  result = GetTempName();
  Add<CreateTempVar>(expr.GetType(), result);
  Add<UnaryOperator>("cast", expr.GetType(), result, value);
  return 0;
}

int CodeGenerator::Evaluate(const AST::IntFunction & expr, std::string & result)
{
  return UnaryNumeric("int", expr.GetType(), expr.GetArg1(), result);
}

int CodeGenerator::Evaluate(const AST::SqrFunction & expr, std::string & result)
{
  return UnaryNumeric("sqrt", expr.GetType(), expr.GetArg1(), result);
}

int CodeGenerator::Evaluate(const AST::RndFunction & expr, std::string & result)
{
  m_funcsUsed.insert("rnd");
  if (expr.m_arg == nullptr) {
    result = GetTempName();
    Add<CreateTempVar>(VarType::eSingle, result);
    Add<UnaryOperator>("rnd", VarType::eSingle, result, "1");
    return 0;
  }
  return UnaryNumeric("rnd", VarType::eSingle, expr.m_arg, result);
}

int CodeGenerator::Evaluate(const AST::AbsFunction & expr, std::string & result)
{
  return UnaryNumeric("abs", expr.GetType(), expr.m_arg, result);
}

int CodeGenerator::Evaluate(const AST::LenFunction & expr, std::string & result)
{
  if (expr.GetArg1() == nullptr) {
    result = "0";
    return -1;
  }
  std::string value;
  expr.GetArg1()->Evaluate(*this, value);
  result = GetTempName();
  Add<CreateTempVar>(VarType::eInt16, result);
  Add<UnaryOperator>("len", VarType::eInt16, result, value);
  return 0;
}

int CodeGenerator::Evaluate(const AST::NumericSubscript & expr, std::string & result)
{
  auto function = AST::g_userFunctions.find(expr.GetName());
  if (function != AST::g_userFunctions.end()) {
    const AST::UserFunction & fn = function->second;
    if (fn.m_params.size() != expr.m_indexes.size() || fn.m_body == nullptr) {
      CompilerError(eError_Parser, m_lineNumber, "wrong number of arguments for FN " << expr.GetName());
      result = "0";
      return -1;
    }
    std::vector<std::string> args;
    for (auto * index : expr.m_indexes) {
      std::string arg;
      index->Evaluate(*this, arg);
      args.push_back(arg);
    }
    std::vector<std::pair<std::string, std::string>> saved;
    for (size_t i = 0; i < fn.m_params.size(); ++i) {
      auto info = AST::g_globalVars.find(fn.m_params[i]);
      if (info == AST::g_globalVars.end()) {
        result = "0";
        return -1;
      }
      std::string slot = GetTempName();
      Add<CreateTempVar>(info->second.m_type, slot);
      Add<UnaryOperator>("=", info->second.m_type, slot, fn.m_params[i]);
      Add<UnaryOperator>("=", info->second.m_type, fn.m_params[i], args[i]);
      saved.push_back(std::make_pair(fn.m_params[i], slot));
    }
    std::string value;
    fn.m_body->Evaluate(*this, value);
    VarType bodyType = fn.m_body->GetType();
    result = GetTempName();
    Add<CreateTempVar>(bodyType, result);
    Add<UnaryOperator>("=", bodyType, result, value);
    for (auto it = saved.rbegin(); it != saved.rend(); ++it) {
      auto info = AST::g_globalVars.find(it->first);
      Add<UnaryOperator>("=", info->second.m_type, it->first, it->second);
    }
    return 0;
  }

  std::vector<std::string> indexes;
  for (auto * index : expr.m_indexes) {
    std::string name;
    index->Evaluate(*this, name);
    indexes.push_back(name);
  }
  result = GetTempName();
  Add<CreateTempVar>(expr.GetType(), result);
  Add<IndexOp>(false, expr.GetType(), expr.GetName(), indexes, result);
  return 0;
}

int CodeGenerator::Evaluate(const AST::StringCompare & expr, std::string & result)
{
  std::string lhs, rhs;
  if (expr.m_lhs != nullptr)
    expr.m_lhs->Evaluate(*this, lhs);
  if (expr.m_rhs != nullptr)
    expr.m_rhs->Evaluate(*this, rhs);
  result = GetTempName();
  Add<CreateTempVar>(VarType::eInt16, result);
  Add<BinaryOperator>(expr.m_equal ? "seq" : "sne", VarType::eInt16, result, lhs, rhs);
  return 0;
}

int CodeGenerator::Evaluate(const AST::StrFunction & expr, std::string & result)
{
  (void)expr;
  result = GetTempName();
  Add<CreateTempVar>(VarType::eString, result);
  return 0;
}

int CodeGenerator::Evaluate(const AST::TabFunction & expr, std::string & result)
{
  (void)expr;
  result = GetTempName();
  Add<CreateTempVar>(VarType::eString, result);
  return 0;
}

int CodeGenerator::Evaluate(const AST::ChrFunction & expr, std::string & result)
{
  (void)expr;
  result = GetTempName();
  Add<CreateTempVar>(VarType::eString, result);
  return 0;
}

int CodeGenerator::Evaluate(const AST::LeftFunction & expr, std::string & result)
{
  (void)expr;
  result = GetTempName();
  Add<CreateTempVar>(VarType::eString, result);
  return 0;
}

int CodeGenerator::Evaluate(const AST::MidFunction & expr, std::string & result)
{
  (void)expr;
  result = GetTempName();
  Add<CreateTempVar>(VarType::eString, result);
  return 0;
}

int CodeGenerator::Evaluate(const AST::RightFunction & expr, std::string & result)
{
  (void)expr;
  result = GetTempName();
  Add<CreateTempVar>(VarType::eString, result);
  return 0;
}

int CodeGenerator::Generate(const AST::IfStatement & expr)
{
  if (expr.m_cond == nullptr)
    return 0;
  std::string cond;
  expr.m_cond->Evaluate(*this, cond);
  Add<If>(cond, expr.m_cond->GetType());
  if (expr.m_trueStatements != nullptr) {
    if (!expr.m_trueStatements->m_lineNumber.empty())
      Add<Goto>("line_" + expr.m_trueStatements->m_lineNumber);
    else if (expr.m_trueStatements->m_statements != nullptr)
      expr.m_trueStatements->m_statements->Generate(*this);
  }
  if (expr.m_falseStatements != nullptr) {
    Add<Else>();
    expr.m_falseStatements->Generate(*this);
  }
  Add<EndIf>();
  return 0;
}

int CodeGenerator::Generate(const AST::ForStatement & expr)
{
  if (expr.m_var == nullptr || expr.m_fromVal == nullptr || expr.m_toVal == nullptr)
    return -1;
  ForInfo info;
  info.m_index = expr.m_var->GetName();
  info.m_type = expr.m_var->GetType();
  std::string from;
  expr.m_fromVal->Evaluate(*this, from);
  std::string toValue;
  expr.m_toVal->Evaluate(*this, toValue);
  info.m_to = GetTempName();
  Add<CreateTempVar>(info.m_type, info.m_to);
  Add<UnaryOperator>("=", info.m_type, info.m_to, toValue);
  info.m_step = GetTempName();
  Add<CreateTempVar>(info.m_type, info.m_step);
  if (expr.m_stepVal != nullptr) {
    std::string step;
    expr.m_stepVal->Evaluate(*this, step);
    Add<UnaryOperator>("=", info.m_type, info.m_step, step);
  }
  else {
    std::string cond = GetTempName();
    Add<CreateTempVar>(VarType::eInt16, cond);
    Add<BinaryOperator>("<=", info.m_type, cond, from, info.m_to);
    Add<If>(cond, VarType::eInt16);
    Add<UnaryOperator>("=", info.m_type, info.m_step, "1");
    Add<Else>();
    Add<UnaryOperator>("=", info.m_type, info.m_step, "-1");
    Add<EndIf>();
  }
  Add<UnaryOperator>("=", info.m_type, info.m_index, from);
  unsigned id = m_forIndex++;
  info.m_body = "for_body_" + std::to_string(id);
  info.m_check = "for_check_" + std::to_string(id);
  Add<Goto>(info.m_check);
  Add<GotoTarget>(info.m_body);
  m_forStack.push_back(info);
  return 0;
}

int CodeGenerator::Generate(const AST::NextStatement & expr)
{
  if (m_forStack.empty()) {
    CompilerError(Error_MismatchedNext, expr.m_lineNumber, "NEXT without FOR");
    return -1;
  }
  ForInfo info = m_forStack.back();
  m_forStack.pop_back();
  std::string inc = GetTempName();
  Add<CreateTempVar>(info.m_type, inc);
  Add<BinaryOperator>("+", info.m_type, inc, info.m_index, info.m_step);
  Add<UnaryOperator>("=", info.m_type, info.m_index, inc);
  Add<GotoTarget>(info.m_check);
  std::string pos = GetTempName();
  std::string below = GetTempName();
  std::string above = GetTempName();
  std::string cond = GetTempName();
  Add<CreateTempVar>(VarType::eInt16, pos);
  Add<CreateTempVar>(VarType::eInt16, below);
  Add<CreateTempVar>(VarType::eInt16, above);
  Add<CreateTempVar>(VarType::eInt16, cond);
  Add<BinaryOperator>(">=", info.m_type, pos, info.m_step, "0");
  Add<BinaryOperator>("<=", info.m_type, below, info.m_index, info.m_to);
  Add<BinaryOperator>(">=", info.m_type, above, info.m_index, info.m_to);
  Add<If>(pos, VarType::eInt16);
  Add<UnaryOperator>("=", VarType::eInt16, cond, below);
  Add<Else>();
  Add<UnaryOperator>("=", VarType::eInt16, cond, above);
  Add<EndIf>();
  Add<If>(cond, VarType::eInt16);
  Add<Goto>(info.m_body);
  Add<EndIf>();
  return 0;
}

int CodeGenerator::Generate(const AST::DimStatement &)
{
  return 0;
}

int CodeGenerator::Generate(const AST::DefStatement &)
{
  return 0;
}

int CodeGenerator::Generate(const AST::InputStatement & expr)
{
  m_funcsUsed.insert("input");
  m_funcsUsed.insert("print_string");
  if (!expr.m_prompt.empty()) {
    if (AST::g_stringConstants.count(expr.m_prompt) == 0)
      AST::g_stringConstants[expr.m_prompt] = AST::g_stringConstantIndex++;
    Add<PrintStringConst>(expr.m_prompt);
  }
  if (expr.m_vars == nullptr)
    return 0;
  for (auto & item : expr.m_vars->m_list) {
    auto * numeric = dynamic_cast<AST::NumericVarRef *>(item.get());
    auto * text = dynamic_cast<AST::StringVarRef *>(item.get());
    if (text != nullptr)
      Add<Input>(text->GetName(), VarType::eString, true, expr.m_lineInput);
    else if (numeric != nullptr)
      Add<Input>(numeric->GetName(), numeric->GetType(), false, false);
  }
  return 0;
}

int CodeGenerator::Generate(const AST::OnGotoStatement & expr)
{
  std::string index = "0";
  VarType type = VarType::eInt16;
  if (expr.m_index != nullptr) {
    expr.m_index->Evaluate(*this, index);
    type = expr.m_index->GetType();
  }
  Add<OnGoto>(index, type, expr.m_lines);
  return 0;
}

int CodeGenerator::Generate(const AST::ClearStatement &)
{
  Add<Clear>();
  return 0;
}

int CodeGenerator::Generate(const AST::WidthStatement & expr)
{
  m_printUsed = true;
  m_funcsUsed.insert("width");
  Add<Width>(expr.m_width);
  return 0;
}
