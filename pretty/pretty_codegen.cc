#include "../src/basalt.h"

#include "pretty_codegen.h"
#include <iostream>
#include <iomanip>
#include <typeinfo>

using namespace std;


////////////////////////////////////////////////////////////

Pretty_OutputGenerator::Pretty_OutputGenerator(CodeGenerator & pseudo)
  : OutputGenerator(
    {
      ".txt", "temp", "", "", 4, 0
    }, pseudo)
{
}

#define INDENT() std::string(m_indent, ' ')

int Pretty_OutputGenerator::Generate(CodeGenerator::Node & node)
{
  *m_outputStream << INDENT() << "A node!" << endl;
  return 0;
}

int Pretty_OutputGenerator::Generate(CodeGenerator::GotoTarget & node)
{
  *m_outputStream << INDENT() << node.m_value << ":" << endl;
  return 0;
}

int Pretty_OutputGenerator::Generate(CodeGenerator::If & node)
{
  *m_outputStream << INDENT() << "if (" << node.m_condVar << ") goto " << node.m_trueTarget << "; else goto " << node.m_falseTarget << ";\n";
  return 0;
}

int Pretty_OutputGenerator::Generate(CodeGenerator::BlockStart & node)
{
  m_blockStack.push(m_outputStream);
  m_outputStream = new std::stringstream;
  m_indent += m_config.m_indentInc;
  return 0;
}

int Pretty_OutputGenerator::Generate(CodeGenerator::BlockEnd & node)
{
  m_indent -= m_config.m_indentInc;
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
int Pretty_OutputGenerator::Generate(CodeGenerator::type & node) \
{ \
  *m_outputStream << INDENT() << node.GetFunc() << endl; \
  return 0; \
} \

#define PRINT_STRING_NODE(type) \
int Pretty_OutputGenerator::Generate(CodeGenerator::type & node) \
{ \
  *m_outputStream << INDENT() << node.GetFunc() << " \"" << node.m_value << "\"" << endl; \
  return 0; \
} \

#define PRINT_TYPED_STRING_NODE(type) \
int Pretty_OutputGenerator::Generate(CodeGenerator::type & node) \
{ \
  *m_outputStream << INDENT() << node.GetFunc() << " " << AST::GetVarTypeInfo(node.m_type).m_name << " " << std::fixed << node.m_value << endl; \
  return 0; \
} \

PRINT_STRING_NODE(Goto);
PRINT_STRING_NODE(Gosub);

PRINT_SIMPLE_NODE(PrintNewLine);
PRINT_SIMPLE_NODE(PrintTab);
PRINT_SIMPLE_NODE(End);
PRINT_SIMPLE_NODE(System);

PRINT_STRING_NODE(PrintStringConst);
PRINT_TYPED_STRING_NODE(PrintInt16Const);
PRINT_TYPED_STRING_NODE(PrintInt32Const);
PRINT_TYPED_STRING_NODE(PrintSingleConst);
PRINT_TYPED_STRING_NODE(PrintDoubleConst);

PRINT_STRING_NODE(PrintStringVar);
PRINT_TYPED_STRING_NODE(PrintInt16Var);
PRINT_TYPED_STRING_NODE(PrintInt32Var);
PRINT_TYPED_STRING_NODE(PrintSingleVar);
PRINT_TYPED_STRING_NODE(PrintDoubleVar);

int Pretty_OutputGenerator::Generate(CodeGenerator::Return & node)
{
  *m_outputStream << INDENT() << "Return\n"; 
  return 0;
} 

int Pretty_OutputGenerator::Generate(CodeGenerator::CreateTempVar & node)
{
  *m_outputStream << INDENT() << "CreateTempVar: " << AST::GetVarTypeInfo(node.m_type).m_name << " " << node.m_value << endl; \
  return 0;
} 

int Pretty_OutputGenerator::Generate(CodeGenerator::UnaryOperator & node)
{
  if (node.m_func == "=")  
    *m_outputStream << INDENT() << "UnaryOperator(assign): " << node.m_ret << " = " << node.m_arg << endl;
  else 
    *m_outputStream << INDENT() << "UnaryOperator: " << node.m_ret << " = " << node.m_func << "(" << node.m_arg << ")" << endl;
  return 0;
}

int Pretty_OutputGenerator::Generate(CodeGenerator::BinaryOperator & node)
{
  if (isalpha(node.m_func[0]))
    *m_outputStream << INDENT() << "BinaryOperator: " << node.m_ret << " = " << node.m_func << "(" << node.m_arg1 << ", " << node.m_arg2 << ")" << endl;
  else  
    *m_outputStream << INDENT() << "BinaryOperator: " << node.m_ret << " = " << node.m_arg1 << " " << node.m_func << " " << node.m_arg2 << endl;
  return 0;
}
