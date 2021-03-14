#include "../src/basalt.h"

#include "pretty_codegen.h"
#include <iostream>
#include <iomanip>
#include <typeinfo>

using namespace std;


////////////////////////////////////////////////////////////

Pretty_CodeGenerator::Pretty_CodeGenerator(PseudoCodeGenerator & pseudo)
  : CodeGenerator(
    {
      ".txt", "temp", "", "", 4, 0
    }, pseudo)
{
}

#define INDENT() std::string(m_indent, ' ')

int Pretty_CodeGenerator::Generate(PseudoCodeGenerator::Node & node)
{
  *m_outputStream << INDENT() << "A node!" << endl;
  return 0;
}

int Pretty_CodeGenerator::Generate(PseudoCodeGenerator::Block & node)
{
  *m_outputStream << INDENT() << "{" << endl;
  m_indent+=2;
  for (auto & code : node.m_code) {
    code->Generate(*this);
  }
  m_indent-=2;
  *m_outputStream << INDENT() << "}" << endl;
  return 0;
}

#define PRINT_SIMPLE_NODE(type) \
int Pretty_CodeGenerator::Generate(PseudoCodeGenerator::type & node) \
{ \
  *m_outputStream << INDENT() << node.GetFunc() << endl; \
  return 0; \
} \

#define PRINT_STRING_NODE(type) \
int Pretty_CodeGenerator::Generate(PseudoCodeGenerator::type & node) \
{ \
  *m_outputStream << INDENT() << node.GetFunc() << " \"" << node.m_value << "\"" << endl; \
  return 0; \
} \

#define PRINT_TYPED_STRING_NODE(type) \
int Pretty_CodeGenerator::Generate(PseudoCodeGenerator::type & node) \
{ \
  *m_outputStream << INDENT() << node.GetFunc() << " " << AST::GetVarTypeInfo(node.m_type).m_name << " \"" << node.m_value << "\"" << endl; \
  return 0; \
} \

PRINT_TYPED_STRING_NODE(CreateTempVar);
PRINT_TYPED_STRING_NODE(DestroyTempVar);

PRINT_SIMPLE_NODE(PrintNewLine);
PRINT_SIMPLE_NODE(PrintTab);

PRINT_STRING_NODE(PrintStringVar);
PRINT_STRING_NODE(PrintStringConst);

PRINT_TYPED_STRING_NODE(PrintNumericVar);
PRINT_TYPED_STRING_NODE(PrintNumericConst);

int Pretty_CodeGenerator::Generate(PseudoCodeGenerator::StringAssign & node)
{
  *m_outputStream << "string_assign " << node.m_lhs << " " << node.m_rhs << endl;
  return 0;
}

int Pretty_CodeGenerator::Generate(PseudoCodeGenerator::NumericAssign & node)
{
  *m_outputStream << "numeric_assign " << node.m_lhs << " " << node.m_rhs << endl;
  return 0;
}

int Pretty_CodeGenerator::Generate(PseudoCodeGenerator::FunctionCHR & node)
{
  *m_outputStream << node.m_ret << " = chr(" << node.m_arg << ")" << endl;
  return 0;
}

int Pretty_CodeGenerator::Generate(PseudoCodeGenerator::FunctionTAB & node)
{
  *m_outputStream << node.m_ret << " = tab(" << node.m_arg << ")" << endl;
  return 0;
}

