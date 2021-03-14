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

int Pretty_CodeGenerator::Generate(PseudoCodeGenerator::LineNumber & node)
{
  *m_outputStream << INDENT() << node.m_value << ":" << endl;
  return 0;
}

int Pretty_CodeGenerator::Generate(PseudoCodeGenerator::BlockStart & node)
{
  m_blockStack.push(m_outputStream);
  m_outputStream = new std::stringstream;
  m_indent+=2;
  return 0;
}

int Pretty_CodeGenerator::Generate(PseudoCodeGenerator::BlockEnd & node)
{
  m_indent-=2;
  std::stringstream * lastBlock = m_outputStream;
  m_outputStream = m_blockStack.back();
  if (lastBlock->str().length() > 0) {
    *m_outputStream << INDENT() << "{" << endl 
                    << lastBlock->str()
                    << INDENT() << "}" << endl;
  }
  m_blockStack.pop();
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

PRINT_SIMPLE_NODE(PrintNewLine);
PRINT_SIMPLE_NODE(PrintTab);

PRINT_STRING_NODE(PrintStringVar);
PRINT_STRING_NODE(PrintStringConst);

PRINT_TYPED_STRING_NODE(PrintNumericVar);
PRINT_TYPED_STRING_NODE(PrintNumericConst);

int Pretty_CodeGenerator::Generate(PseudoCodeGenerator::StringAssign & node)
{
  *m_outputStream << INDENT() << "string_assign " << node.m_lhs << " " << node.m_rhs << endl;
  return 0;
}

int Pretty_CodeGenerator::Generate(PseudoCodeGenerator::NumericAssign & node)
{
  *m_outputStream << INDENT() << "numeric_assign " << node.m_lhs << " " << node.m_rhs << endl;
  return 0;
}

int Pretty_CodeGenerator::Generate(PseudoCodeGenerator::FunctionCHR & node)
{
  *m_outputStream << INDENT() << node.m_ret << " = chr(" << node.m_arg << ")" << endl;
  return 0;
}

int Pretty_CodeGenerator::Generate(PseudoCodeGenerator::FunctionTAB & node)
{
  *m_outputStream << INDENT() << node.m_ret << " = tab(" << node.m_arg << ")" << endl;
  return 0;
}

