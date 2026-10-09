#include "c_codegen.h"
#include "../irutil.h"

#include <iostream>

using namespace std;

C_OutputGenerator::C_OutputGenerator(CodeGenerator & codeGenerator)
  : OutputGenerator({ ".c", 2, 2 }, codeGenerator)
{
}

std::string C_OutputGenerator::Operand(const std::string & name) const
{
  if (IsNumberLiteral(name))
    return name;
  return Mangle(name);
}

std::string C_OutputGenerator::StringPtr(const std::string & name) const
{
  if (name.rfind("str_", 0) == 0)
    return name;
  std::string mangled = Mangle(name);
  return "(" + mangled + " ? " + mangled + " : \"\")";
}

void C_OutputGenerator::Line(const std::string & text)
{
  *m_outputStream << std::string(m_indent, ' ') << text << "\n";
}

std::string C_OutputGenerator::IndexText(const CodeGenerator::IndexOp & node) const
{
  const auto & bounds = AST::g_arrayBounds[node.m_array];
  std::string offset = "(int)(" + Operand(node.m_indexes[0]) + ")";
  for (size_t i = 1; i < node.m_indexes.size(); ++i) {
    int dim = (i < bounds.size()) ? bounds[i] + 1 : 11;
    offset = "((" + offset + ") * " + std::to_string(dim) + " + (int)(" + Operand(node.m_indexes[i]) + "))";
  }
  return Mangle(node.m_array) + "[" + offset + "]";
}

void C_OutputGenerator::OutputFilePrologue(ostream & strm)
{
  strm << "/* Generated from " << m_inputFilename << " */\n"
       << "#include <stdint.h>\n"
       << "#include <stdio.h>\n"
       << "#include <stdlib.h>\n"
       << "#include <string.h>\n"
       << "#include <unistd.h>\n"
       << "#include <math.h>\n\n";

  m_needStrings = m_codeGenerator.GetFuncsUsed().count("sets") != 0
               || m_codeGenerator.GetFuncsUsed().count("cat") != 0;
  for (auto & entry : AST::g_globalVars) {
    if (entry.second.m_type == VarType::eString)
      m_needStrings = true;
  }
  if (m_needStrings) {
    strm << "static char * basalt_dup(const char * src);\n";
    strm << "static char * basalt_concat(const char * a, const char * b);\n";
  }

  for (auto & entry : AST::g_globalVars) {
    if (IsArrayName(entry.first)) {
      strm << "static " << CTypeName(entry.second.m_type) << " " << Mangle(entry.first)
           << "[" << ArrayLength(entry.first) << "] = {0};\n";
    }
    else if (entry.second.m_type == VarType::eString)
      strm << "static char * " << Mangle(entry.first) << " = 0;\n";
    else
      strm << "static " << CTypeName(entry.second.m_type) << " " << Mangle(entry.first) << " = 0;\n";
  }
  strm << "\n";

  for (auto & entry : AST::g_stringConstants) {
    strm << "static const char str_" << entry.second << "[] = \""
         << EscapeC(entry.first) << "\";\n";
  }
  if (!AST::g_stringConstants.empty())
    strm << "\n";

  if (m_codeGenerator.IsPrintUsed() || m_codeGenerator.GetFuncsUsed().count("width")) {
    int tab = g_languageProfile ? g_languageProfile->GetTabWidth() : 14;
    strm << "int g_outputColumn = 0;\n";
    strm << "int g_tabLen = " << tab << ";\n\n";
  }

  BasaltWriteCRuntimeDecls(strm, m_codeGenerator.GetFuncsUsed());

  strm << "int main(void)\n{\n";
  for (auto & code : m_codeGenerator.m_code) {
    auto * temp = dynamic_cast<CodeGenerator::CreateTempVar *>(code.get());
    if (temp == nullptr)
      continue;
    if (temp->m_type == VarType::eString)
      strm << "  char * " << temp->m_value << " = 0;\n";
    else
      strm << "  " << CTypeName(temp->m_type) << " " << temp->m_value << " = 0;\n";
  }
  if (m_codeGenerator.GetFuncsUsed().count("gosub")) {
    strm << "  int gs_sp = 0;\n";
    strm << "  int gs_stk[64];\n";
  }
}

void C_OutputGenerator::OutputFileEpilogue(ostream & strm)
{
  if (!m_gosubs.empty()) {
    strm << "  gs_return:\n";
    strm << "  if (gs_sp <= 0) return 0;\n";
    strm << "  switch (gs_stk[--gs_sp]) {\n";
    for (int id : m_gosubs)
      strm << "    case " << id << ": goto gs_" << id << ";\n";
    strm << "    default: return 0;\n";
    strm << "  }\n";
  }
  strm << "  return 0;\n}\n\n";

  if (m_needStrings) {
    strm << "static char * basalt_dup(const char * src)\n"
         << "{\n"
         << "  size_t n;\n"
         << "  char * out;\n"
         << "  if (src == 0) src = \"\";\n"
         << "  n = strlen(src);\n"
         << "  out = (char *)malloc(n + 1);\n"
         << "  memcpy(out, src, n + 1);\n"
         << "  return out;\n"
         << "}\n"
         << "static char * basalt_concat(const char * a, const char * b)\n"
         << "{\n"
         << "  size_t na, nb;\n"
         << "  char * out;\n"
         << "  if (a == 0) a = \"\";\n"
         << "  if (b == 0) b = \"\";\n"
         << "  na = strlen(a);\n"
         << "  nb = strlen(b);\n"
         << "  out = (char *)malloc(na + nb + 1);\n"
         << "  memcpy(out, a, na);\n"
         << "  memcpy(out + na, b, nb + 1);\n"
         << "  return out;\n"
         << "}\n";
  }
  BasaltWriteCRuntime(strm, m_codeGenerator.GetFuncsUsed());
}

int C_OutputGenerator::Generate(CodeGenerator::GotoTarget & node)
{
  *m_outputStream << node.m_value << ": ;\n";
  return 0;
}

int C_OutputGenerator::Generate(CodeGenerator::Goto & node)
{
  Line("goto " + node.m_value + ";");
  return 0;
}

int C_OutputGenerator::Generate(CodeGenerator::Gosub & node)
{
  int id = ++m_gosubId;
  m_gosubs.push_back(id);
  Line("gs_stk[gs_sp++] = " + std::to_string(id) + ";");
  Line("goto line_" + node.m_value + ";");
  *m_outputStream << "gs_" << id << ": ;\n";
  return 0;
}

int C_OutputGenerator::Generate(CodeGenerator::Return &)
{
  Line("goto gs_return;");
  return 0;
}

int C_OutputGenerator::Generate(CodeGenerator::End &)
{
  Line("return 0;");
  return 0;
}

int C_OutputGenerator::Generate(CodeGenerator::System &)
{
  Line("return 0;");
  return 0;
}

int C_OutputGenerator::Generate(CodeGenerator::If & node)
{
  Line("if (" + Operand(node.m_cond) + ") {");
  m_indent += 2;
  return 0;
}

int C_OutputGenerator::Generate(CodeGenerator::Else &)
{
  m_indent -= 2;
  Line("} else {");
  m_indent += 2;
  return 0;
}

int C_OutputGenerator::Generate(CodeGenerator::EndIf &)
{
  m_indent -= 2;
  Line("}");
  return 0;
}

int C_OutputGenerator::Generate(CodeGenerator::PrintNewLine &)
{
  Line("print_newline();");
  return 0;
}

int C_OutputGenerator::Generate(CodeGenerator::PrintTab &)
{
  Line("print_tab();");
  return 0;
}

int C_OutputGenerator::Generate(CodeGenerator::PrintStringConst & node)
{
  Line("print_string(\"" + EscapeC(node.m_value) + "\");");
  return 0;
}

int C_OutputGenerator::Generate(CodeGenerator::PrintStringVar & node)
{
  Line("print_string(" + StringPtr(node.m_value) + ");");
  return 0;
}

int C_OutputGenerator::Generate(CodeGenerator::PrintNumber & node)
{
  std::string func = node.m_func;
  std::string value = node.m_literal ? node.m_value : Operand(node.m_value);
  Line(func + "(" + value + ");");
  return 0;
}

int C_OutputGenerator::Generate(CodeGenerator::UnaryOperator & node)
{
  std::string dest = Operand(node.m_ret);
  std::string src = Operand(node.m_arg);
  if (node.m_func == "=")
    Line(dest + " = " + src + ";");
  else if (node.m_func == "sets")
    Line("{ char * copy = basalt_dup(" + StringPtr(node.m_arg) + "); if (" + dest + ") free(" + dest + "); " + dest + " = copy; }");
  else if (node.m_func == "neg")
    Line(dest + " = -(" + src + ");");
  else if (node.m_func == "int")
    Line(dest + " = (int16_t)(" + src + ");");
  else if (node.m_func == "abs")
    Line(dest + " = (" + src + " < 0) ? -(" + src + ") : (" + src + ");");
  else if (node.m_func == "rnd")
    Line(dest + " = basalt_rnd((float)(" + src + "));");
  else if (node.m_func == "sqrt")
    Line(dest + " = sqrt(" + src + ");");
  else if (node.m_func == "cast")
    Line(dest + " = (" + std::string(CTypeName(node.m_type)) + ")(" + src + ");");
  else if (node.m_func == "len")
    Line(dest + " = (int16_t)strlen(" + StringPtr(node.m_arg) + ");");
  else
    Line("/* " + node.m_func + " */");
  return 0;
}

int C_OutputGenerator::Generate(CodeGenerator::BinaryOperator & node)
{
  std::string dest = Operand(node.m_ret);
  std::string lhs = Operand(node.m_arg1);
  std::string rhs = Operand(node.m_arg2);
  const std::string & op = node.m_func;
  if (op == "==" || op == "!=" || op == "<" || op == "<=" || op == ">" || op == ">=")
    Line(dest + " = (" + lhs + " " + op + " " + rhs + ") ? -1 : 0;");
  else if (op == "&" || op == "|")
    Line(dest + " = (int16_t)(" + lhs + ") " + op + " (int16_t)(" + rhs + ");");
  else if (op == "pow")
    Line(dest + " = (" + std::string(CTypeName(node.m_type)) + ")pow((double)(" + lhs + "), (double)(" + rhs + "));");
  else if (op == "cat")
    Line(dest + " = basalt_concat(" + StringPtr(node.m_arg1) + ", " + StringPtr(node.m_arg2) + ");");
  else if (op == "seq" || op == "sne") {
    const char * cmp = (op == "seq") ? "==" : "!=";
    Line(dest + " = (strcmp(" + StringPtr(node.m_arg1) + ", " + StringPtr(node.m_arg2) + ") " + cmp + " 0) ? -1 : 0;");
  }
  else
    Line(dest + " = " + lhs + " " + op + " " + rhs + ";");
  return 0;
}

int C_OutputGenerator::Generate(CodeGenerator::IndexOp & node)
{
  if (node.m_indexes.empty())
    return 0;
  if (node.m_store)
    Line(IndexText(node) + " = " + Operand(node.m_value) + ";");
  else
    Line(Operand(node.m_value) + " = " + IndexText(node) + ";");
  return 0;
}

int C_OutputGenerator::Generate(CodeGenerator::Input & node)
{
  std::string name = Mangle(node.m_name);
  if (node.m_string && node.m_line)
    Line("basalt_line_input(&" + name + ");");
  else if (node.m_string)
    Line("basalt_input_string(&" + name + ");");
  else
    Line(name + " = (" + std::string(CTypeName(node.m_type)) + ")basalt_read_number();");
  return 0;
}

int C_OutputGenerator::Generate(CodeGenerator::OnGoto & node)
{
  Line("switch ((int)(" + Operand(node.m_index) + ")) {");
  for (size_t i = 0; i < node.m_lines.size(); ++i)
    Line("case " + std::to_string(i + 1) + ": goto line_" + node.m_lines[i] + ";");
  Line("default: break;");
  Line("}");
  return 0;
}

int C_OutputGenerator::Generate(CodeGenerator::Clear &)
{
  for (auto & entry : AST::g_globalVars) {
    if (AST::g_userFunctions.count(entry.first) != 0 && !IsArrayName(entry.first)
        && entry.second.m_type != VarType::eString && entry.first.size() > 2)
      ;
    std::string name = Mangle(entry.first);
    if (entry.second.m_type == VarType::eString) {
      Line("if (" + name + ") free(" + name + ");");
      Line(name + " = 0;");
    }
    else if (IsArrayName(entry.first))
      Line("memset(" + name + ", 0, sizeof " + name + ");");
    else
      Line(name + " = 0;");
  }
  return 0;
}

int C_OutputGenerator::Generate(CodeGenerator::Width & node)
{
  Line("g_tabLen = " + std::to_string(node.m_width) + ";");
  return 0;
}
