#include "llvm_codegen.h"
#include "../basalt.h"
#include "../irutil.h"

#include <algorithm>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

LLVM_OutputGenerator::LLVM_OutputGenerator(CodeGenerator & codeGenerator)
  : OutputGenerator({ ".ll", 2, 2 }, codeGenerator)
{
}

void LLVM_OutputGenerator::Prepare()
{
  if (m_ready)
    return;
  for (auto & entry : AST::g_globalVars)
    m_types[entry.first] = entry.second.m_type;
  m_ready = true;
}

void LLVM_OutputGenerator::BeginNode(const CodeGenerator::Node & node)
{
  m_debugLine = node.m_debugLine;
  m_debugText = node.m_debugText;
}

int LLVM_OutputGenerator::DebugLoc(unsigned line)
{
  auto found = m_debugLocs.find(line);
  if (found != m_debugLocs.end())
    return found->second;
  int id = m_nextDebug++;
  m_debugLocs[line] = id;
  return id;
}

void LLVM_OutputGenerator::WriteInstr(std::ostream & strm, const std::string & text, unsigned line)
{
  strm << "  " << text;
  if (g_debugInfo)
    strm << ", !dbg !" << DebugLoc(line);
  strm << "\n";
}

void LLVM_OutputGenerator::NoteStatement()
{
  if (!g_debugInfo || m_debugLine == 0 || m_debugText.empty())
    return;
  if (!m_labeledLines.insert(m_debugLine).second)
    return;
  int id = m_nextDebug++;
  m_debugLabels.push_back({ m_debugLine, m_debugText, id });
  *m_outputStream << "  call void @llvm.dbg.label(metadata !" << id
                  << "), !dbg !" << DebugLoc(m_debugLine) << "\n";
}

void LLVM_OutputGenerator::Emit(const std::string & text)
{
  NoteStatement();
  WriteInstr(*m_outputStream, text, m_debugLine);
}

std::string LLVM_OutputGenerator::MetaQuoted(const std::string & text) const
{
  std::string out = "\"";
  for (unsigned char ch : text) {
    if (ch == '"' || ch == '\\' || ch < 32 || ch >= 127) {
      char buf[8];
      snprintf(buf, sizeof buf, "\\%02X", ch);
      out += buf;
    }
    else
      out += static_cast<char>(ch);
  }
  out += "\"";
  return out;
}

void LLVM_OutputGenerator::WriteDebugMetadata(std::ostream & strm)
{
  std::string path = m_inputFilename.empty() ? "stdin.bas" : m_inputFilename;
  std::string directory = ".";
  std::string filename = path;
  auto slash = path.find_last_of('/');
  if (slash != std::string::npos) {
    directory = path.substr(0, slash);
    filename = path.substr(slash + 1);
    if (directory.empty())
      directory = "/";
  }

  std::string source;
  {
    std::ifstream input(path);
    if (input)
      source.assign(std::istreambuf_iterator<char>(input), std::istreambuf_iterator<char>());
  }
  if (source.empty()) {
    unsigned last = 0;
    for (auto & line : AST::g_program.m_list)
      last = std::max(last, line->GetSourceLineNumber());
    std::vector<std::string> lines(last);
    for (auto & line : AST::g_program.m_list) {
      unsigned number = line->GetSourceLineNumber();
      if (number > 0 && number <= lines.size())
        lines[number - 1] = line->GetLine();
    }
    for (size_t i = 0; i < lines.size(); ++i) {
      if (i != 0)
        source += '\n';
      source += lines[i];
    }
    if (!source.empty())
      source += '\n';
  }

  strm << "\ndeclare void @llvm.dbg.label(metadata)\n";
  strm << "!llvm.dbg.cu = !{!0}\n";
  strm << "!llvm.module.flags = !{!6, !7, !8}\n";
  strm << "!llvm.ident = !{!9}\n";
  strm << "!0 = distinct !DICompileUnit(language: DW_LANG_C, file: !1, producer: \"basalt\", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug)\n";
  strm << "!1 = !DIFile(filename: " << MetaQuoted(filename)
       << ", directory: " << MetaQuoted(directory)
       << ", source: " << MetaQuoted(source) << ")\n";
  strm << "!2 = distinct !DISubprogram(name: \"main\", scope: !1, file: !1, line: 1, type: !3, scopeLine: 1, spFlags: DISPFlagDefinition, unit: !0)\n";
  strm << "!3 = !DISubroutineType(types: !4)\n";
  strm << "!4 = !{!5}\n";
  strm << "!5 = !DIBasicType(name: \"int\", size: 32, encoding: DW_ATE_signed)\n";
  strm << "!6 = !{i32 7, !\"Dwarf Version\", i32 5}\n";
  strm << "!7 = !{i32 2, !\"Debug Info Version\", i32 3}\n";
  strm << "!8 = !{i32 2, !\"PIC Level\", i32 2}\n";
  strm << "!9 = !{!\"basalt\"}\n";
  for (auto & entry : m_debugLocs) {
    strm << "!" << entry.second << " = !DILocation(line: " << entry.first
         << ", column: 1, scope: !2)\n";
  }
  for (auto & label : m_debugLabels) {
    strm << "!" << label.m_id << " = !DILabel(scope: !2, name: " << MetaQuoted(label.m_text)
         << ", file: !1, line: " << label.m_line << ")\n";
  }
}

void LLVM_OutputGenerator::EnsureOpen()
{
  if (!m_terminated)
    return;
  std::string label = Lab("dead");
  *m_outputStream << label << ":\n";
  m_terminated = false;
}

void LLVM_OutputGenerator::Close(const std::string & next)
{
  if (!m_terminated)
    Emit("br label %" + next);
  m_terminated = true;
}

std::string LLVM_OutputGenerator::Tmp()
{
  return "%t" + std::to_string(++m_ssa);
}

std::string LLVM_OutputGenerator::Lab(const std::string & prefix)
{
  return prefix + "_" + std::to_string(++m_lab);
}

const char * LLVM_OutputGenerator::Ty(VarType type) const
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

std::string LLVM_OutputGenerator::Ptr(const std::string & name) const
{
  if (name.rfind("temp_", 0) == 0)
    return "%" + name;
  if (name.rfind("str_", 0) == 0)
    return "@" + name;
  return "@" + Mangle(name);
}

std::string LLVM_OutputGenerator::FLit(float value) const
{
  double widened = value;
  uint64_t bits = 0;
  memcpy(&bits, &widened, sizeof bits);
  char buf[32];
  snprintf(buf, sizeof buf, "0x%016llX", static_cast<unsigned long long>(bits));
  return buf;
}

std::string LLVM_OutputGenerator::DLit(double value) const
{
  uint64_t bits = 0;
  memcpy(&bits, &value, sizeof bits);
  char buf[32];
  snprintf(buf, sizeof buf, "0x%016llX", static_cast<unsigned long long>(bits));
  return buf;
}

std::string LLVM_OutputGenerator::Literal(VarType type, const std::string & text) const
{
  if (type == VarType::eSingle)
    return FLit(strtof(text.c_str(), nullptr));
  if (type == VarType::eDouble)
    return DLit(strtod(text.c_str(), nullptr));
  if (type == VarType::eInt32)
    return std::to_string(static_cast<int32_t>(strtol(text.c_str(), nullptr, 10)));
  return std::to_string(static_cast<int16_t>(strtol(text.c_str(), nullptr, 10)));
}

VarType LLVM_OutputGenerator::TypeOf(const std::string & name) const
{
  auto it = m_types.find(name);
  if (it == m_types.end())
    return VarType::eNone;
  return it->second;
}

std::string LLVM_OutputGenerator::Cast(const std::string & value, VarType from, VarType to)
{
  if (from == to)
    return value;
  std::string id = Tmp();
  std::string src = Ty(from);
  std::string dst = Ty(to);
  if (IsFloatType(from) && IsFloatType(to))
    Emit(id + " = " + (static_cast<int>(to) > static_cast<int>(from) ? "fpext" : "fptrunc") + " " + src + " " + value + " to " + dst);
  else if (IsFloatType(from) && !IsFloatType(to))
    Emit(id + " = fptosi " + src + " " + value + " to " + dst);
  else if (!IsFloatType(from) && IsFloatType(to))
    Emit(id + " = sitofp " + src + " " + value + " to " + dst);
  else if (static_cast<int>(to) > static_cast<int>(from))
    Emit(id + " = sext " + src + " " + value + " to " + dst);
  else
    Emit(id + " = trunc " + src + " " + value + " to " + dst);
  return id;
}

std::string LLVM_OutputGenerator::LoadAs(VarType type, const std::string & name)
{
  EnsureOpen();
  if (IsNumberLiteral(name))
    return Literal(type, name);
  VarType have = TypeOf(name);
  if (have == VarType::eNone || have == VarType::eString)
    have = type;
  std::string id = Tmp();
  Emit(id + " = load " + Ty(have) + ", ptr " + Ptr(name));
  return Cast(id, have, type);
}

void LLVM_OutputGenerator::Store(VarType type, const std::string & value, const std::string & name)
{
  EnsureOpen();
  Emit("store " + std::string(Ty(type)) + " " + value + ", ptr " + Ptr(name));
}

std::string LLVM_OutputGenerator::StringPtr(const std::string & name, bool nullable)
{
  EnsureOpen();
  if (name.rfind("str_", 0) == 0)
    return "@" + name;
  std::string id = Tmp();
  Emit(id + " = load ptr, ptr " + Ptr(name));
  if (nullable)
    return id;
  std::string ok = Tmp();
  std::string sel = Tmp();
  Emit(ok + " = icmp ne ptr " + id + ", null");
  Emit(sel + " = select i1 " + ok + ", ptr " + id + ", ptr @empty");
  return sel;
}

std::string LLVM_OutputGenerator::LlvmString(const std::string & text) const
{
  std::string out;
  for (unsigned char ch : text) {
    if (ch == '\\') {
      out += "\\5C";
    }
    else if (ch == '"') {
      out += "\\22";
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

void LLVM_OutputGenerator::OutputFilePrologue(ostream & strm)
{
  strm << "; Generated from " << m_inputFilename << "\n";
  if (g_debugInfo)
    strm << "source_filename = " << MetaQuoted(m_inputFilename) << "\n";
  strm << "declare i32 @basalt_print_string(ptr)\n";
  strm << "declare i32 @basalt_print_int16(i16 signext)\n";
  strm << "declare i32 @basalt_print_int32(i32)\n";
  strm << "declare i32 @basalt_print_single(float)\n";
  strm << "declare i32 @basalt_print_double(double)\n";
  strm << "declare i32 @basalt_print_tab()\n";
  strm << "declare i32 @basalt_print_newline()\n";
  strm << "declare void @basalt_set_width(i32)\n";
  strm << "declare void @basalt_set_string(ptr, ptr)\n";
  strm << "declare void @basalt_clear_string(ptr)\n";
  strm << "declare double @basalt_read_number()\n";
  strm << "declare void @basalt_input_string(ptr)\n";
  strm << "declare void @basalt_line_input(ptr)\n";
  strm << "declare float @basalt_rnd(float)\n";
  strm << "declare double @pow(double, double)\n";
  strm << "declare i32 @strcmp(ptr, ptr)\n";
  strm << "declare i64 @strlen(ptr)\n";
  strm << "declare ptr @basalt_concat(ptr, ptr)\n";
  strm << "declare double @llvm.sqrt.f64(double)\n";
  strm << "declare void @llvm.memset.p0.i64(ptr, i8, i64, i1)\n\n";
  strm << "@empty = private constant [1 x i8] c\"\\00\"\n";
  strm << "@gs_sp = global i32 0\n";
  strm << "@gs_stk = global [64 x i32] zeroinitializer\n";

  for (auto & entry : AST::g_stringConstants) {
    strm << "@str_" << entry.second << " = private constant ["
         << (entry.first.size() + 1) << " x i8] c\"" << LlvmString(entry.first) << "\"\n";
  }
  for (auto & entry : AST::g_globalVars) {
    std::string name = "@" + Mangle(entry.first);
    if (IsArrayName(entry.first)) {
      strm << name << " = internal global [" << ArrayLength(entry.first) << " x " << Ty(entry.second.m_type)
           << "] zeroinitializer\n";
    }
    else if (entry.second.m_type == VarType::eString)
      strm << name << " = internal global ptr null\n";
    else if (IsFloatType(entry.second.m_type))
      strm << name << " = internal global " << Ty(entry.second.m_type) << " 0.0\n";
    else
      strm << name << " = internal global " << Ty(entry.second.m_type) << " 0\n";
  }
  strm << "\ndefine i32 @main()";
  if (g_debugInfo)
    strm << " !dbg !2";
  strm << " {\nentry:\n";
  for (auto & code : m_codeGenerator.m_code) {
    auto * temp = dynamic_cast<CodeGenerator::CreateTempVar *>(code.get());
    if (temp == nullptr)
      continue;
    const char * init = "0";
    if (temp->m_type == VarType::eString)
      init = "null";
    else if (IsFloatType(temp->m_type))
      init = "0.0";
    WriteInstr(strm, std::string("%") + temp->m_value + " = alloca " + Ty(temp->m_type), 0);
    WriteInstr(strm, std::string("store ") + Ty(temp->m_type) + " " + init + ", ptr %" + temp->m_value, 0);
  }
  WriteInstr(strm, "br label %body", 0);
  strm << "body:\n";
  m_terminated = false;
}

void LLVM_OutputGenerator::OutputFileEpilogue(ostream & strm)
{
  if (!m_terminated)
    WriteInstr(strm, "br label %exit_ok", 0);
  if (!m_gosubs.empty()) {
    strm << "gs_return:\n";
    WriteInstr(strm, "%gsp = load i32, ptr @gs_sp", 0);
    WriteInstr(strm, "%gsp1 = add i32 %gsp, -1", 0);
    WriteInstr(strm, "store i32 %gsp1, ptr @gs_sp", 0);
    WriteInstr(strm, "%gslot = getelementptr [64 x i32], ptr @gs_stk, i32 0, i32 %gsp1", 0);
    WriteInstr(strm, "%gid = load i32, ptr %gslot", 0);
    strm << "  switch i32 %gid, label %exit_ok [\n";
    for (int id : m_gosubs)
      strm << "    i32 " << id << ", label %gs_" << id << "\n";
    strm << "  ]";
    if (g_debugInfo)
      strm << ", !dbg !" << DebugLoc(0);
    strm << "\n";
  }
  strm << "exit_ok:\n";
  WriteInstr(strm, "ret i32 0", 0);
  strm << "}\n";
  if (g_debugInfo)
    WriteDebugMetadata(strm);
}

int LLVM_OutputGenerator::Generate(CodeGenerator::CreateTempVar & node)
{
  Prepare();
  m_types[node.m_value] = node.m_type;
  return 0;
}

int LLVM_OutputGenerator::Generate(CodeGenerator::GotoTarget & node)
{
  Prepare();
  Close(node.m_value);
  *m_outputStream << node.m_value << ":\n";
  m_terminated = false;
  return 0;
}

int LLVM_OutputGenerator::Generate(CodeGenerator::Goto & node)
{
  Prepare();
  EnsureOpen();
  Emit("br label %" + node.m_value);
  m_terminated = true;
  return 0;
}

int LLVM_OutputGenerator::Generate(CodeGenerator::Gosub & node)
{
  Prepare();
  EnsureOpen();
  int id = ++m_gosubId;
  m_gosubs.push_back(id);
  std::string sp = Tmp();
  std::string sp1 = Tmp();
  std::string slot = Tmp();
  Emit(sp + " = load i32, ptr @gs_sp");
  Emit(slot + " = getelementptr [64 x i32], ptr @gs_stk, i32 0, i32 " + sp);
  Emit("store i32 " + std::to_string(id) + ", ptr " + slot);
  Emit(sp1 + " = add i32 " + sp + ", 1");
  Emit("store i32 " + sp1 + ", ptr @gs_sp");
  Emit("br label %line_" + node.m_value);
  *m_outputStream << "gs_" << id << ":\n";
  m_terminated = false;
  return 0;
}

int LLVM_OutputGenerator::Generate(CodeGenerator::Return &)
{
  Prepare();
  EnsureOpen();
  Emit("br label %gs_return");
  m_terminated = true;
  return 0;
}

int LLVM_OutputGenerator::Generate(CodeGenerator::End &)
{
  Prepare();
  EnsureOpen();
  Emit("ret i32 0");
  m_terminated = true;
  return 0;
}

int LLVM_OutputGenerator::Generate(CodeGenerator::System &)
{
  Prepare();
  EnsureOpen();
  Emit("ret i32 0");
  m_terminated = true;
  return 0;
}

int LLVM_OutputGenerator::Generate(CodeGenerator::If & node)
{
  Prepare();
  EnsureOpen();
  IfFrame frame;
  frame.m_elseLabel = Lab("else");
  frame.m_endLabel = Lab("endif");
  std::string cond = LoadAs(node.m_type == VarType::eNone ? VarType::eInt16 : node.m_type, node.m_cond);
  std::string bit = Tmp();
  std::string thenLabel = Lab("then");
  if (IsFloatType(node.m_type))
    Emit(bit + " = fcmp une " + std::string(Ty(node.m_type)) + " " + cond + ", 0.0");
  else
    Emit(bit + " = icmp ne " + std::string(Ty(node.m_type == VarType::eNone ? VarType::eInt16 : node.m_type)) + " " + cond + ", 0");
  Emit("br i1 " + bit + ", label %" + thenLabel + ", label %" + frame.m_elseLabel);
  *m_outputStream << thenLabel << ":\n";
  m_terminated = false;
  m_ifStack.push_back(frame);
  return 0;
}

int LLVM_OutputGenerator::Generate(CodeGenerator::Else &)
{
  Prepare();
  IfFrame & frame = m_ifStack.back();
  frame.m_sawElse = true;
  Close(frame.m_endLabel);
  *m_outputStream << frame.m_elseLabel << ":\n";
  m_terminated = false;
  return 0;
}

int LLVM_OutputGenerator::Generate(CodeGenerator::EndIf &)
{
  Prepare();
  IfFrame frame = m_ifStack.back();
  m_ifStack.pop_back();
  Close(frame.m_endLabel);
  if (!frame.m_sawElse) {
    *m_outputStream << frame.m_elseLabel << ":\n";
    Emit("br label %" + frame.m_endLabel);
  }
  *m_outputStream << frame.m_endLabel << ":\n";
  m_terminated = false;
  return 0;
}

int LLVM_OutputGenerator::Generate(CodeGenerator::PrintNewLine &)
{
  Prepare();
  EnsureOpen();
  Emit("call i32 @basalt_print_newline()");
  return 0;
}

int LLVM_OutputGenerator::Generate(CodeGenerator::PrintTab &)
{
  Prepare();
  EnsureOpen();
  Emit("call i32 @basalt_print_tab()");
  return 0;
}

int LLVM_OutputGenerator::Generate(CodeGenerator::PrintStringConst & node)
{
  Prepare();
  EnsureOpen();
  auto found = AST::g_stringConstants.find(node.m_value);
  if (found == AST::g_stringConstants.end())
    return 0;
  Emit("call i32 @basalt_print_string(ptr @str_" + std::to_string(found->second) + ")");
  return 0;
}

int LLVM_OutputGenerator::Generate(CodeGenerator::PrintStringVar & node)
{
  Prepare();
  std::string ptr = StringPtr(node.m_value, false);
  Emit("call i32 @basalt_print_string(ptr " + ptr + ")");
  return 0;
}

int LLVM_OutputGenerator::Generate(CodeGenerator::PrintNumber & node)
{
  Prepare();
  std::string value = LoadAs(node.m_type, node.m_value);
  std::string func = "basalt_print_int16";
  if (node.m_type == VarType::eInt32) func = "basalt_print_int32";
  else if (node.m_type == VarType::eSingle) func = "basalt_print_single";
  else if (node.m_type == VarType::eDouble) func = "basalt_print_double";
  std::string extend = node.m_type == VarType::eInt16 ? " signext" : "";
  Emit("call i32 @" + func + "(" + Ty(node.m_type) + extend + " " + value + ")");
  return 0;
}

int LLVM_OutputGenerator::Generate(CodeGenerator::UnaryOperator & node)
{
  Prepare();
  if (node.m_func == "sets") {
    EnsureOpen();
    std::string src = StringPtr(node.m_arg, true);
    Emit("call void @basalt_set_string(ptr " + Ptr(node.m_ret) + ", ptr " + src + ")");
    return 0;
  }
  if (node.m_func == "=" || node.m_func == "cast") {
    VarType from = TypeOf(node.m_arg);
    if (from == VarType::eNone || IsNumberLiteral(node.m_arg))
      from = node.m_type;
    std::string value = LoadAs(from, node.m_arg);
    value = Cast(value, from, node.m_type);
    Store(node.m_type, value, node.m_ret);
    return 0;
  }
  if (node.m_func == "neg") {
    std::string value = LoadAs(node.m_type, node.m_arg);
    std::string id = Tmp();
    if (IsFloatType(node.m_type))
      Emit(id + " = fneg " + std::string(Ty(node.m_type)) + " " + value);
    else
      Emit(id + " = sub " + std::string(Ty(node.m_type)) + " 0, " + value);
    Store(node.m_type, id, node.m_ret);
    return 0;
  }
  if (node.m_func == "abs") {
    std::string value = LoadAs(node.m_type, node.m_arg);
    std::string cmp = Tmp();
    std::string neg = Tmp();
    std::string sel = Tmp();
    if (IsFloatType(node.m_type)) {
      Emit(cmp + " = fcmp olt " + std::string(Ty(node.m_type)) + " " + value + ", 0.0");
      Emit(neg + " = fneg " + std::string(Ty(node.m_type)) + " " + value);
    }
    else {
      Emit(cmp + " = icmp slt " + std::string(Ty(node.m_type)) + " " + value + ", 0");
      Emit(neg + " = sub " + std::string(Ty(node.m_type)) + " 0, " + value);
    }
    Emit(sel + " = select i1 " + cmp + ", " + std::string(Ty(node.m_type)) + " " + neg + ", " + std::string(Ty(node.m_type)) + " " + value);
    Store(node.m_type, sel, node.m_ret);
    return 0;
  }
  if (node.m_func == "int") {
    VarType from = TypeOf(node.m_arg);
    if (from == VarType::eNone || IsNumberLiteral(node.m_arg))
      from = VarType::eSingle;
    std::string value = LoadAs(from, node.m_arg);
    std::string id = Tmp();
    if (IsFloatType(from))
      Emit(id + " = fptosi " + std::string(Ty(from)) + " " + value + " to i16");
    else
      id = Cast(value, from, VarType::eInt16);
    Store(VarType::eInt16, id, node.m_ret);
    return 0;
  }
  if (node.m_func == "rnd") {
    std::string value = LoadAs(VarType::eSingle, node.m_arg);
    std::string id = Tmp();
    Emit(id + " = call float @basalt_rnd(float " + value + ")");
    Store(VarType::eSingle, id, node.m_ret);
    return 0;
  }
  if (node.m_func == "sqrt") {
    std::string value = LoadAs(VarType::eDouble, node.m_arg);
    std::string id = Tmp();
    Emit(id + " = call double @llvm.sqrt.f64(double " + value + ")");
    std::string out = Cast(id, VarType::eDouble, node.m_type);
    Store(node.m_type, out, node.m_ret);
    return 0;
  }
  if (node.m_func == "len") {
    std::string ptr = StringPtr(node.m_arg, false);
    std::string n = Tmp();
    std::string id = Tmp();
    Emit(n + " = call i64 @strlen(ptr " + ptr + ")");
    Emit(id + " = trunc i64 " + n + " to i16");
    Store(VarType::eInt16, id, node.m_ret);
    return 0;
  }
  return 0;
}

int LLVM_OutputGenerator::Generate(CodeGenerator::BinaryOperator & node)
{
  Prepare();
  const std::string & op = node.m_func;
  if (op == "cat") {
    std::string a = StringPtr(node.m_arg1, false);
    std::string b = StringPtr(node.m_arg2, false);
    std::string id = Tmp();
    Emit(id + " = call ptr @basalt_concat(ptr " + a + ", ptr " + b + ")");
    Store(VarType::eString, id, node.m_ret);
    return 0;
  }
  if (op == "seq" || op == "sne") {
    std::string a = StringPtr(node.m_arg1, false);
    std::string b = StringPtr(node.m_arg2, false);
    std::string cmp = Tmp();
    std::string bit = Tmp();
    std::string ext = Tmp();
    std::string neg = Tmp();
    Emit(cmp + " = call i32 @strcmp(ptr " + a + ", ptr " + b + ")");
    Emit(bit + " = icmp " + std::string(op == "seq" ? "eq" : "ne") + " i32 " + cmp + ", 0");
    Emit(ext + " = zext i1 " + bit + " to i16");
    Emit(neg + " = mul i16 " + ext + ", -1");
    Store(VarType::eInt16, neg, node.m_ret);
    return 0;
  }
  if (op == "&" || op == "|") {
    std::string lhs = LoadAs(VarType::eInt16, node.m_arg1);
    std::string rhs = LoadAs(VarType::eInt16, node.m_arg2);
    std::string id = Tmp();
    Emit(id + " = " + std::string(op == "&" ? "and" : "or") + " i16 " + lhs + ", " + rhs);
    Store(VarType::eInt16, id, node.m_ret);
    return 0;
  }
  if (op == "pow") {
    std::string lhs = LoadAs(VarType::eDouble, node.m_arg1);
    std::string rhs = LoadAs(VarType::eDouble, node.m_arg2);
    std::string id = Tmp();
    Emit(id + " = call double @pow(double " + lhs + ", double " + rhs + ")");
    std::string out = Cast(id, VarType::eDouble, node.m_type);
    Store(node.m_type, out, node.m_ret);
    return 0;
  }
  bool compare = op == "==" || op == "!=" || op == "<" || op == "<=" || op == ">" || op == ">=";
  VarType operand = node.m_type;
  if (operand == VarType::eNone)
    operand = VarType::eInt16;
  std::string lhs = LoadAs(operand, node.m_arg1);
  std::string rhs = LoadAs(operand, node.m_arg2);
  std::string id = Tmp();
  if (compare) {
    const char * pred = "eq";
    if (op == "!=") pred = IsFloatType(operand) ? "une" : "ne";
    else if (op == "==") pred = IsFloatType(operand) ? "oeq" : "eq";
    else if (op == "<") pred = IsFloatType(operand) ? "olt" : "slt";
    else if (op == "<=") pred = IsFloatType(operand) ? "ole" : "sle";
    else if (op == ">") pred = IsFloatType(operand) ? "ogt" : "sgt";
    else pred = IsFloatType(operand) ? "oge" : "sge";
    std::string bit = Tmp();
    std::string ext = Tmp();
    Emit(bit + " = " + std::string(IsFloatType(operand) ? "fcmp " : "icmp ") + pred + " " + Ty(operand) + " " + lhs + ", " + rhs);
    Emit(ext + " = zext i1 " + bit + " to i16");
    Emit(id + " = mul i16 " + ext + ", -1");
    Store(VarType::eInt16, id, node.m_ret);
    return 0;
  }
  const char * inst = "add";
  if (op == "-") inst = IsFloatType(operand) ? "fsub" : "sub";
  else if (op == "+") inst = IsFloatType(operand) ? "fadd" : "add";
  else if (op == "*") inst = IsFloatType(operand) ? "fmul" : "mul";
  else if (op == "/") inst = IsFloatType(operand) ? "fdiv" : "sdiv";
  Emit(id + " = " + inst + " " + Ty(operand) + " " + lhs + ", " + rhs);
  Store(operand, id, node.m_ret);
  return 0;
}

int LLVM_OutputGenerator::Generate(CodeGenerator::IndexOp & node)
{
  Prepare();
  if (node.m_indexes.empty())
    return 0;
  std::string offset = LoadAs(VarType::eInt32, node.m_indexes[0]);
  const auto & bounds = AST::g_arrayBounds[node.m_array];
  for (size_t i = 1; i < node.m_indexes.size(); ++i) {
    int dim = (i < bounds.size()) ? bounds[i] + 1 : 11;
    std::string next = LoadAs(VarType::eInt32, node.m_indexes[i]);
    std::string scaled = Tmp();
    std::string sum = Tmp();
    Emit(scaled + " = mul i32 " + offset + ", " + std::to_string(dim));
    Emit(sum + " = add i32 " + scaled + ", " + next);
    offset = sum;
  }
  int length = ArrayLength(node.m_array);
  std::string ptr = Tmp();
  Emit(ptr + " = getelementptr [" + std::to_string(length) + " x " + Ty(node.m_type) + "], ptr " + Ptr(node.m_array) + ", i32 0, i32 " + offset);
  if (node.m_store) {
    std::string value = LoadAs(node.m_type, node.m_value);
    Emit("store " + std::string(Ty(node.m_type)) + " " + value + ", ptr " + ptr);
  }
  else {
    std::string value = Tmp();
    Emit(value + " = load " + Ty(node.m_type) + ", ptr " + ptr);
    Store(node.m_type, value, node.m_value);
  }
  return 0;
}

int LLVM_OutputGenerator::Generate(CodeGenerator::Input & node)
{
  Prepare();
  EnsureOpen();
  if (node.m_string && node.m_line)
    Emit("call void @basalt_line_input(ptr " + Ptr(node.m_name) + ")");
  else if (node.m_string)
    Emit("call void @basalt_input_string(ptr " + Ptr(node.m_name) + ")");
  else {
    std::string raw = Tmp();
    Emit(raw + " = call double @basalt_read_number()");
    std::string value = Cast(raw, VarType::eDouble, node.m_type);
    Store(node.m_type, value, node.m_name);
  }
  return 0;
}

int LLVM_OutputGenerator::Generate(CodeGenerator::OnGoto & node)
{
  Prepare();
  std::string index = LoadAs(IsFloatType(node.m_type) ? node.m_type : VarType::eInt32, node.m_index);
  std::string asInt = index;
  if (IsFloatType(node.m_type))
    asInt = Cast(index, node.m_type, VarType::eInt32);
  else if (node.m_type == VarType::eInt16)
    asInt = Cast(index, VarType::eInt16, VarType::eInt32);
  std::string after = Lab("on");
  Emit("switch i32 " + asInt + ", label %" + after + " [");
  for (size_t i = 0; i < node.m_lines.size(); ++i)
    Emit("  i32 " + std::to_string(i + 1) + ", label %line_" + node.m_lines[i]);
  Emit("]");
  *m_outputStream << after << ":\n";
  m_terminated = false;
  return 0;
}

int LLVM_OutputGenerator::Generate(CodeGenerator::Clear &)
{
  Prepare();
  EnsureOpen();
  for (auto & entry : AST::g_globalVars) {
    if (AST::g_userFunctions.count(entry.first) != 0 && entry.second.m_type != VarType::eString && !IsArrayName(entry.first))
      continue;
    if (entry.second.m_type == VarType::eString)
      Emit("call void @basalt_clear_string(ptr " + Ptr(entry.first) + ")");
    else if (IsArrayName(entry.first)) {
      int bytes = ArrayLength(entry.first) * TypeWidth(entry.second.m_type);
      Emit("call void @llvm.memset.p0.i64(ptr " + Ptr(entry.first) + ", i8 0, i64 " + std::to_string(bytes) + ", i1 false)");
    }
    else if (IsFloatType(entry.second.m_type))
      Emit("store " + std::string(Ty(entry.second.m_type)) + " 0.0, ptr " + Ptr(entry.first));
    else
      Emit("store " + std::string(Ty(entry.second.m_type)) + " 0, ptr " + Ptr(entry.first));
  }
  return 0;
}

int LLVM_OutputGenerator::Generate(CodeGenerator::Width & node)
{
  Prepare();
  EnsureOpen();
  Emit("call void @basalt_set_width(i32 " + std::to_string(node.m_width) + ")");
  return 0;
}
