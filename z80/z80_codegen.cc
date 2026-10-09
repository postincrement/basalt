#include "z80_codegen.h"
#include "../irutil.h"

#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

Z80_OutputGenerator::Z80_OutputGenerator(CodeGenerator & codeGenerator)
  : OutputGenerator({ ".asm", 0, 0 }, codeGenerator)
{
}

void Z80_OutputGenerator::A(const std::string & text)
{
  *m_outputStream << "    " << text << "\n";
}

void Z80_OutputGenerator::Lbl(const std::string & name)
{
  *m_outputStream << name << ":\n";
}

std::string Z80_OutputGenerator::Lab()
{
  return "L" + std::to_string(++m_lab);
}

void Z80_OutputGenerator::Prepare()
{
  if (m_ready)
    return;
  for (auto & entry : AST::g_globalVars)
    m_types[entry.first] = entry.second.m_type;
  m_ready = true;
}

VarType Z80_OutputGenerator::TypeOf(const std::string & name) const
{
  auto it = m_types.find(name);
  if (it == m_types.end())
    return VarType::eNone;
  return it->second;
}

int Z80_OutputGenerator::WidthOf(VarType type) const
{
  if (type == VarType::eSingle || type == VarType::eDouble || type == VarType::eInt32)
    return 4;
  return 2;
}

std::string Z80_OutputGenerator::FloatConst(float value)
{
  uint32_t bits = 0;
  memcpy(&bits, &value, sizeof bits);
  auto found = m_floats.find(bits);
  if (found != m_floats.end())
    return found->second;
  std::string name = "fc_" + std::to_string(m_floats.size());
  m_floats[bits] = name;
  m_useFloat = true;
  return name;
}

void Z80_OutputGenerator::LoadFacc(const std::string & name)
{
  m_useFloat = true;
  if (IsNumberLiteral(name)) {
    A("ld    hl," + FloatConst(strtof(name.c_str(), nullptr)));
    A("call  fload");
    return;
  }
  VarType type = TypeOf(name);
  if (!IsFloatType(type) && type != VarType::eNone && type != VarType::eDouble) {
    LoadHL(name);
    A("xor   a");
    A("ld    (sf_i2f_mode),a");
    A("call  i2f");
    return;
  }
  A("ld    hl," + Mangle(name));
  A("call  fload");
}

void Z80_OutputGenerator::StoreFacc(const std::string & name)
{
  m_useFloat = true;
  A("ld    hl," + Mangle(name));
  A("call  fstore");
}

void Z80_OutputGenerator::LoadHL(const std::string & name)
{
  if (IsNumberLiteral(name))
    A("ld    hl," + std::to_string(static_cast<int>(strtol(name.c_str(), nullptr, 10))));
  else
    A("ld    hl,(" + Mangle(name) + ")");
}

void Z80_OutputGenerator::StoreHL(const std::string & name)
{
  A("ld    (" + Mangle(name) + "),hl");
}

void Z80_OutputGenerator::LoadString(const std::string & name)
{
  if (name.rfind("str_", 0) == 0)
    A("ld    hl," + name);
  else
    A("ld    hl,(" + Mangle(name) + ")");
}

void Z80_OutputGenerator::ZeroTest(const std::string & name, VarType type, const std::string & ifZero)
{
  if (IsFloatType(type) || (IsNumberLiteral(name) && name.find('.') != std::string::npos)) {
    LoadFacc(name);
    A("call  f_nz");
    A("jp    z," + ifZero);
    return;
  }
  if (IsNumberLiteral(name) && strtol(name.c_str(), nullptr, 10) == 0) {
    A("jp    " + ifZero);
    return;
  }
  LoadHL(name);
  A("ld    a,h");
  A("or    l");
  A("jp    z," + ifZero);
}

void Z80_OutputGenerator::StoreRelation(const std::string & op, const std::string & dest)
{
  std::string no = Lab();
  std::string yes = Lab();
  std::string done = Lab();
  if (op == "==") {
    A("ld    a,h");
    A("or    l");
    A("jp    nz," + no);
  }
  else if (op == "!=") {
    A("ld    a,h");
    A("or    l");
    A("jp    z," + no);
  }
  else if (op == "<") {
    A("bit   7,h");
    A("jp    z," + no);
  }
  else if (op == ">") {
    A("bit   7,h");
    A("jp    nz," + no);
    A("ld    a,l");
    A("or    a");
    A("jp    z," + no);
  }
  else if (op == "<=") {
    A("bit   7,h");
    A("jp    nz," + yes);
    A("ld    a,h");
    A("or    l");
    A("jp    z," + yes);
    A("jp    " + no);
  }
  else {
    A("bit   7,h");
    A("jp    nz," + no);
  }
  Lbl(yes);
  A("ld    hl,-1");
  A("jp    " + done);
  Lbl(no);
  A("ld    hl,0");
  Lbl(done);
  StoreHL(dest);
}

void Z80_OutputGenerator::IncludeFile(ostream & strm, const std::string & filename)
{
  std::string path = __FILE__;
  auto slash = path.find_last_of('/');
  path = path.substr(0, slash + 1) + filename;
  ifstream input(path);
  if (!input) {
    cerr << "error: cannot read " << path << "\n";
    return;
  }
  strm << input.rdbuf() << "\n";
}

void Z80_OutputGenerator::OutputFilePrologue(ostream & strm)
{
  int tab = g_languageProfile ? g_languageProfile->GetTabWidth() : 14;
  strm << "; Z80 generator by basalt\n"
       << "wboot: equ 0\n"
       << "bdos:  equ 5\n"
       << "    org  0x100\n"
       << "    jp    start\n\n"
       << "temp:\n"
       << "conout: jp    0\n"
       << "tabwid: db    " << tab << "\n"
       << "column: db    0\n"
       << "pow_base:\tdb\t0,0,0,0\n\n";

  for (auto & entry : AST::g_globalVars) {
    std::string name = Mangle(entry.first);
    if (IsArrayName(entry.first)) {
      strm << name << ":\tds\t" << (ArrayLength(entry.first) * WidthOf(entry.second.m_type)) << "\n";
    }
    else if (entry.second.m_type == VarType::eString) {
      strm << name << ":\tdw\t0\n";
      strm << name << "_b:\tds\t64\n";
    }
    else if (IsFloatType(entry.second.m_type) || entry.second.m_type == VarType::eInt32)
      strm << name << ":\tdb\t0,0,0,0\n";
    else
      strm << name << ":\tdw\t0\n";
  }
  for (auto & code : m_codeGenerator.m_code) {
    auto * temp = dynamic_cast<CodeGenerator::CreateTempVar *>(code.get());
    if (temp == nullptr)
      continue;
    if (temp->m_type == VarType::eString) {
      strm << temp->m_value << ":\tdw\t0\n";
      strm << temp->m_value << "_b:\tds\t128\n";
    }
    else if (IsFloatType(temp->m_type) || temp->m_type == VarType::eInt32)
      strm << temp->m_value << ":\tdb\t0,0,0,0\n";
    else
      strm << temp->m_value << ":\tdw\t0\n";
  }
  for (auto & fc : m_floats) {
    uint32_t bits = fc.first;
    strm << fc.second << ":\tdb\t"
         << (bits & 0xff) << ","
         << ((bits >> 8) & 0xff) << ","
         << ((bits >> 16) & 0xff) << ","
         << ((bits >> 24) & 0xff) << "\n";
  }
  strm << "\n";
  for (auto & entry : AST::g_stringConstants) {
    strm << "str_" << entry.second << ":";
    const std::string & text = entry.first;
    bool plain = !text.empty();
    for (unsigned char ch : text) {
      if (ch < 32 || ch > 126 || ch == '"')
        plain = false;
    }
    if (plain)
      strm << "\tdm\t\"" << text << "\"\n\tdb\t0\n";
    else {
      strm << "\tdb\t";
      if (text.empty())
        strm << "0\n";
      else {
        for (size_t i = 0; i < text.size(); ++i)
          strm << (i ? "," : "") << static_cast<int>(static_cast<unsigned char>(text[i]));
        strm << ",0\n";
      }
    }
  }
  strm << "\n";
  IncludeFile(strm, "rt.asm");
  if (m_useFloat || m_codeGenerator.GetFuncsUsed().count("input") || m_codeGenerator.IsPrintUsed()) {
    m_useFloat = true;
    IncludeFile(strm, "softf.asm");
  }
  strm << "start:\n"
       << "    ld    hl,(bdos+1)\n"
       << "    ld    sp,hl\n"
       << "    ld    hl,(wboot+1)\n"
       << "    ld    de,9\n"
       << "    add   hl,de\n"
       << "    ld    (conout+1),hl\n"
       << "    ld    ix,temp\n";
}

void Z80_OutputGenerator::OutputFileEpilogue(ostream & strm)
{
  strm << "end:\n    rst   0\n";
}

int Z80_OutputGenerator::Generate(CodeGenerator::CreateTempVar & node)
{
  Prepare();
  m_types[node.m_value] = node.m_type;
  return 0;
}

int Z80_OutputGenerator::Generate(CodeGenerator::GotoTarget & node)
{
  Prepare();
  Lbl(node.m_value);
  return 0;
}

int Z80_OutputGenerator::Generate(CodeGenerator::Goto & node)
{
  Prepare();
  A("jp    " + node.m_value);
  return 0;
}

int Z80_OutputGenerator::Generate(CodeGenerator::Gosub & node)
{
  Prepare();
  A("call  line_" + node.m_value);
  return 0;
}

int Z80_OutputGenerator::Generate(CodeGenerator::Return &)
{
  Prepare();
  A("ret");
  return 0;
}

int Z80_OutputGenerator::Generate(CodeGenerator::End &)
{
  Prepare();
  A("jp    end");
  return 0;
}

int Z80_OutputGenerator::Generate(CodeGenerator::System &)
{
  Prepare();
  A("jp    end");
  return 0;
}

int Z80_OutputGenerator::Generate(CodeGenerator::If & node)
{
  Prepare();
  IfFrame frame;
  frame.m_elseLabel = Lab();
  frame.m_endLabel = Lab();
  ZeroTest(node.m_cond, node.m_type, frame.m_elseLabel);
  m_ifStack.push_back(frame);
  return 0;
}

int Z80_OutputGenerator::Generate(CodeGenerator::Else &)
{
  Prepare();
  IfFrame & frame = m_ifStack.back();
  frame.m_sawElse = true;
  A("jp    " + frame.m_endLabel);
  Lbl(frame.m_elseLabel);
  return 0;
}

int Z80_OutputGenerator::Generate(CodeGenerator::EndIf &)
{
  Prepare();
  IfFrame frame = m_ifStack.back();
  m_ifStack.pop_back();
  if (!frame.m_sawElse)
    Lbl(frame.m_elseLabel);
  Lbl(frame.m_endLabel);
  return 0;
}

int Z80_OutputGenerator::Generate(CodeGenerator::PrintNewLine &)
{
  Prepare();
  A("call  print_newline");
  return 0;
}

int Z80_OutputGenerator::Generate(CodeGenerator::PrintTab &)
{
  Prepare();
  A("call  print_tab");
  return 0;
}

int Z80_OutputGenerator::Generate(CodeGenerator::PrintStringConst & node)
{
  Prepare();
  auto found = AST::g_stringConstants.find(node.m_value);
  if (found == AST::g_stringConstants.end())
    return 0;
  A("ld    hl,str_" + std::to_string(found->second));
  A("call  print_zstr");
  return 0;
}

int Z80_OutputGenerator::Generate(CodeGenerator::PrintStringVar & node)
{
  Prepare();
  std::string skip = Lab();
  LoadString(node.m_value);
  A("ld    a,h");
  A("or    l");
  A("jr    nz," + skip);
  A("ld    hl,empty");
  Lbl(skip);
  A("call  print_zstr");
  return 0;
}

int Z80_OutputGenerator::Generate(CodeGenerator::PrintNumber & node)
{
  Prepare();
  if (node.m_type == VarType::eDouble && node.m_literal) {
    char buf[80];
    snprintf(buf, sizeof buf, "% .13lg ", strtod(node.m_value.c_str(), nullptr));
    std::string text(buf);
    if (AST::g_stringConstants.count(text) == 0)
      AST::g_stringConstants[text] = AST::g_stringConstantIndex++;
    A("ld    hl,str_" + std::to_string(AST::g_stringConstants[text]));
    A("call  print_zstr");
    return 0;
  }
  if (IsFloatType(node.m_type)) {
    LoadFacc(node.m_value);
    A("call  print_f");
    return 0;
  }
  LoadHL(node.m_value);
  A("call  print_i16s");
  return 0;
}

int Z80_OutputGenerator::Generate(CodeGenerator::UnaryOperator & node)
{
  Prepare();
  bool asFloat = IsFloatType(node.m_type) || node.m_type == VarType::eDouble;
  if (node.m_func == "=" || node.m_func == "cast") {
    if (node.m_type == VarType::eString) {
      LoadString(node.m_arg);
      A("ld    de," + Mangle(node.m_ret) + "_b");
      A("call  scopy");
      A("ld    hl," + Mangle(node.m_ret) + "_b");
      A("ld    (" + Mangle(node.m_ret) + "),hl");
      return 0;
    }
    if (asFloat || IsFloatType(TypeOf(node.m_arg)) || (IsNumberLiteral(node.m_arg) && !asFloat && node.m_func == "cast")) {
      if (asFloat) {
        LoadFacc(node.m_arg);
        StoreFacc(node.m_ret);
        return 0;
      }
    }
    if (asFloat) {
      LoadFacc(node.m_arg);
      StoreFacc(node.m_ret);
      return 0;
    }
    if (IsFloatType(TypeOf(node.m_arg))) {
      LoadFacc(node.m_arg);
      A("call  f2i");
      StoreHL(node.m_ret);
      return 0;
    }
    LoadHL(node.m_arg);
    StoreHL(node.m_ret);
    return 0;
  }
  if (node.m_func == "sets") {
    LoadString(node.m_arg);
    A("ld    de," + Mangle(node.m_ret) + "_b");
    A("call  scopy");
    A("ld    hl," + Mangle(node.m_ret) + "_b");
    A("ld    (" + Mangle(node.m_ret) + "),hl");
    return 0;
  }
  if (node.m_func == "neg") {
    if (asFloat) {
      LoadFacc(node.m_arg);
      A("call  fneg");
      StoreFacc(node.m_ret);
    }
    else {
      LoadHL(node.m_arg);
      A("xor   a");
      A("sub   l");
      A("ld    l,a");
      A("ld    a,0");
      A("sbc   a,h");
      A("ld    h,a");
      StoreHL(node.m_ret);
    }
    return 0;
  }
  if (node.m_func == "abs") {
    if (asFloat) {
      LoadFacc(node.m_arg);
      A("ld    a,(facc+3)");
      A("and   0x7f");
      A("ld    (facc+3),a");
      StoreFacc(node.m_ret);
    }
    else {
      std::string pos = Lab();
      LoadHL(node.m_arg);
      A("bit   7,h");
      A("jp    z," + pos);
      A("xor   a");
      A("sub   l");
      A("ld    l,a");
      A("ld    a,0");
      A("sbc   a,h");
      A("ld    h,a");
      Lbl(pos);
      StoreHL(node.m_ret);
    }
    return 0;
  }
  if (node.m_func == "int") {
    LoadFacc(node.m_arg);
    A("call  f2i");
    StoreHL(node.m_ret);
    return 0;
  }
  if (node.m_func == "rnd") {
    LoadFacc(node.m_arg);
    A("call  rnd");
    StoreFacc(node.m_ret);
    return 0;
  }
  if (node.m_func == "sqrt") {
    LoadFacc(node.m_arg);
    StoreFacc(node.m_ret);
    return 0;
  }
  if (node.m_func == "len") {
    LoadString(node.m_arg);
    std::string loop = Lab();
    std::string done = Lab();
    A("ld    de,0");
    Lbl(loop);
    A("ld    a,h");
    A("or    l");
    A("jp    z," + done);
    A("ld    a,(hl)");
    A("or    a");
    A("jp    z," + done);
    A("inc   hl");
    A("inc   de");
    A("jp    " + loop);
    Lbl(done);
    A("ex    de,hl");
    StoreHL(node.m_ret);
    return 0;
  }
  return 0;
}

int Z80_OutputGenerator::Generate(CodeGenerator::BinaryOperator & node)
{
  Prepare();
  const std::string & op = node.m_func;
  bool asFloat = IsFloatType(node.m_type);
  if (op == "seq" || op == "sne") {
    LoadString(node.m_arg1);
    A("push  hl");
    LoadString(node.m_arg2);
    A("pop   de");
    A("ex    de,hl");
    A("call  streq");
    if (op == "sne") {
      std::string nz = Lab();
      A("ld    a,h");
      A("or    l");
      A("ld    hl,-1");
      A("jp    z," + nz);
      A("ld    hl,0");
      Lbl(nz);
    }
    StoreHL(node.m_ret);
    return 0;
  }
  if (op == "&" || op == "|") {
    LoadHL(node.m_arg1);
    A("push  hl");
    LoadHL(node.m_arg2);
    A("pop   de");
    A("ld    a,l");
    A(std::string(op == "&" ? "and   e" : "or    e"));
    A("ld    l,a");
    A("ld    a,h");
    A(std::string(op == "&" ? "and   d" : "or    d"));
    A("ld    h,a");
    StoreHL(node.m_ret);
    return 0;
  }
  if (op == "pow") {
    std::string loop = Lab();
    std::string done = Lab();
    LoadFacc(node.m_arg1);
    A("ld    hl,pow_base");
    A("call  fstore");
    LoadFacc(node.m_arg2);
    A("call  f2i");
    A("push  hl");
    A("ld    hl," + FloatConst(1.0f));
    A("call  fload");
    A("pop   hl");
    A("ld    b,l");
    A("ld    a,b");
    A("or    a");
    A("jp    z," + done);
    Lbl(loop);
    A("push  bc");
    A("ld    hl,pow_base");
    A("ld    de,farg");
    A("ld    bc,4");
    A("ldir");
    A("call  fmul");
    A("pop   bc");
    A("djnz  " + loop);
    Lbl(done);
    StoreFacc(node.m_ret);
    return 0;
  }
  if (op == "cat") {
    LoadString(node.m_arg1);
    A("ld    de," + Mangle(node.m_ret) + "_b");
    A("call  scopy");
    A("dec   de");
    LoadString(node.m_arg2);
    A("call  scopy");
    A("ld    hl," + Mangle(node.m_ret) + "_b");
    A("ld    (" + Mangle(node.m_ret) + "),hl");
    return 0;
  }
  if (asFloat || op == "+" || op == "-" || op == "*" || op == "/" ||
      op == "==" || op == "!=" || op == "<" || op == "<=" || op == ">" || op == ">=") {
    if (asFloat) {
      LoadFacc(node.m_arg1);
      A("call  fpush");
      LoadFacc(node.m_arg2);
      A("call  fpop");
      if (op == "+") A("call  fadd");
      else if (op == "-") A("call  fsub");
      else if (op == "*") A("call  fmul");
      else if (op == "/") A("call  fdiv");
      else {
        A("call  fcmp");
        StoreRelation(op, node.m_ret);
        return 0;
      }
      StoreFacc(node.m_ret);
      return 0;
    }
  }
  if (op == "+" || op == "-") {
    LoadHL(node.m_arg2);
    A("push  hl");
    LoadHL(node.m_arg1);
    A("pop   de");
    if (op == "+")
      A("add   hl,de");
    else {
      A("or    a");
      A("sbc   hl,de");
    }
    StoreHL(node.m_ret);
    return 0;
  }
  if (op == "*" || op == "/") {
    LoadFacc(node.m_arg1);
    A("call  fpush");
    LoadFacc(node.m_arg2);
    A("call  fpop");
    A(op == "*" ? "call  fmul" : "call  fdiv");
    A("call  f2i");
    StoreHL(node.m_ret);
    return 0;
  }
  LoadHL(node.m_arg2);
  A("push  hl");
  LoadHL(node.m_arg1);
  A("pop   de");
  A("call  icmp");
  StoreRelation(op, node.m_ret);
  return 0;
}

int Z80_OutputGenerator::Generate(CodeGenerator::IndexOp & node)
{
  Prepare();
  if (node.m_indexes.empty())
    return 0;
  int width = WidthOf(node.m_type);
  const std::string & index = node.m_indexes[0];
  if (IsNumberLiteral(index)) {
    int offset = static_cast<int>(strtol(index.c_str(), nullptr, 10)) * width;
    A("ld    hl," + Mangle(node.m_array) + "+" + std::to_string(offset));
  }
  else {
    if (IsFloatType(TypeOf(index))) {
      LoadFacc(index);
      A("call  f2i");
    }
    else
      LoadHL(index);
    if (width == 4) {
      A("add   hl,hl");
      A("add   hl,hl");
    }
    else if (width == 2)
      A("add   hl,hl");
    A("ld    de," + Mangle(node.m_array));
    A("add   hl,de");
  }
  if (node.m_store) {
    A("push  hl");
    if (IsFloatType(node.m_type)) {
      LoadFacc(node.m_value);
      A("pop   hl");
      A("call  fstore");
    }
    else {
      LoadHL(node.m_value);
      A("ex    de,hl");
      A("pop   hl");
      A("ld    (hl),e");
      A("inc   hl");
      A("ld    (hl),d");
    }
  }
  else if (IsFloatType(node.m_type)) {
    A("call  fload");
    StoreFacc(node.m_value);
  }
  else {
    A("ld    e,(hl)");
    A("inc   hl");
    A("ld    d,(hl)");
    A("ex    de,hl");
    StoreHL(node.m_value);
  }
  return 0;
}

int Z80_OutputGenerator::Generate(CodeGenerator::Input & node)
{
  Prepare();
  m_useFloat = true;
  if (node.m_string) {
    A("ld    de," + Mangle(node.m_name) + "_b");
    A(node.m_line ? "call  line_in" : "call  input_str");
    A("ld    hl," + Mangle(node.m_name) + "_b");
    A("ld    (" + Mangle(node.m_name) + "),hl");
    return 0;
  }
  A("call  input_num");
  if (IsFloatType(node.m_type))
    StoreFacc(node.m_name);
  else {
    A("call  f2i");
    StoreHL(node.m_name);
  }
  return 0;
}

int Z80_OutputGenerator::Generate(CodeGenerator::OnGoto & node)
{
  Prepare();
  if (IsFloatType(node.m_type)) {
    LoadFacc(node.m_index);
    A("call  f2i");
  }
  else
    LoadHL(node.m_index);
  for (size_t i = 0; i < node.m_lines.size(); ++i) {
    A("push  hl");
    A("ld    de," + std::to_string(i + 1));
    A("or    a");
    A("sbc   hl,de");
    A("pop   hl");
    A("jp    z,line_" + node.m_lines[i]);
  }
  return 0;
}

int Z80_OutputGenerator::Generate(CodeGenerator::Clear &)
{
  Prepare();
  for (auto & entry : AST::g_globalVars) {
    if (AST::g_userFunctions.count(entry.first) != 0 && !IsArrayName(entry.first)
        && entry.second.m_type != VarType::eString)
      continue;
    std::string name = Mangle(entry.first);
    if (entry.second.m_type == VarType::eString) {
      A("ld    hl,0");
      A("ld    (" + name + "),hl");
      A("xor   a");
      A("ld    (" + name + "_b),a");
    }
    else if (IsArrayName(entry.first)) {
      int bytes = ArrayLength(entry.first) * WidthOf(entry.second.m_type);
      if (bytes <= 1) {
        A("xor   a");
        A("ld    (" + name + "),a");
      }
      else {
        A("ld    hl," + name);
        A("ld    de," + name + "+1");
        A("ld    bc," + std::to_string(bytes - 1));
        A("ld    (hl),0");
        A("ldir");
      }
    }
    else if (IsFloatType(entry.second.m_type) || entry.second.m_type == VarType::eInt32) {
      A("ld    hl," + name);
      A("ld    de," + name + "+1");
      A("ld    bc,3");
      A("ld    (hl),0");
      A("ldir");
    }
    else {
      A("ld    hl,0");
      A("ld    (" + name + "),hl");
    }
  }
  return 0;
}

int Z80_OutputGenerator::Generate(CodeGenerator::Width & node)
{
  Prepare();
  A("ld    a," + std::to_string(node.m_width));
  A("ld    (tabwid),a");
  return 0;
}
