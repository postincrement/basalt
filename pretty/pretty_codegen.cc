#include "pretty_codegen.h"
#include "../irutil.h"

#include <algorithm>

using namespace std;

Pretty_OutputGenerator::Pretty_OutputGenerator(CodeGenerator & codeGenerator)
  : OutputGenerator({ ".pretty", 0, 2 }, codeGenerator)
{
}

void Pretty_OutputGenerator::Line(const std::string & text)
{
  *m_outputStream << std::string(m_indent, ' ') << text << "\n";
}

void Pretty_OutputGenerator::OutputFilePrologue(ostream & strm)
{
  strm << "; " << m_inputFilename << "\n";
}

void Pretty_OutputGenerator::OutputFileEpilogue(ostream &)
{
}

int Pretty_OutputGenerator::Generate(CodeGenerator::Node &)
{
  Line("node");
  return 0;
}

int Pretty_OutputGenerator::Generate(CodeGenerator::GotoTarget & node)
{
  *m_outputStream << node.m_value << ":\n";
  return 0;
}

int Pretty_OutputGenerator::Generate(CodeGenerator::Goto & node)
{
  Line("goto " + node.m_value);
  return 0;
}

int Pretty_OutputGenerator::Generate(CodeGenerator::Gosub & node)
{
  Line("gosub " + node.m_value);
  return 0;
}

int Pretty_OutputGenerator::Generate(CodeGenerator::Return &)
{
  Line("return");
  return 0;
}

int Pretty_OutputGenerator::Generate(CodeGenerator::End &)
{
  Line("end");
  return 0;
}

int Pretty_OutputGenerator::Generate(CodeGenerator::System &)
{
  Line("system");
  return 0;
}

int Pretty_OutputGenerator::Generate(CodeGenerator::If & node)
{
  Line("if " + node.m_cond);
  m_indent += 2;
  return 0;
}

int Pretty_OutputGenerator::Generate(CodeGenerator::Else &)
{
  m_indent = std::max(0, m_indent - 2);
  Line("else");
  m_indent += 2;
  return 0;
}

int Pretty_OutputGenerator::Generate(CodeGenerator::EndIf &)
{
  m_indent = std::max(0, m_indent - 2);
  Line("endif");
  return 0;
}

int Pretty_OutputGenerator::Generate(CodeGenerator::CreateTempVar & node)
{
  Line(std::string(CTypeName(node.m_type)) + " " + node.m_value);
  return 0;
}

int Pretty_OutputGenerator::Generate(CodeGenerator::PrintNewLine &)
{
  Line("print newline");
  return 0;
}

int Pretty_OutputGenerator::Generate(CodeGenerator::PrintTab &)
{
  Line("print tab");
  return 0;
}

int Pretty_OutputGenerator::Generate(CodeGenerator::PrintStringConst & node)
{
  Line("print \"" + node.m_value + "\"");
  return 0;
}

int Pretty_OutputGenerator::Generate(CodeGenerator::PrintStringVar & node)
{
  Line("print " + node.m_value);
  return 0;
}

int Pretty_OutputGenerator::Generate(CodeGenerator::PrintNumber & node)
{
  Line("print " + node.m_value);
  return 0;
}

int Pretty_OutputGenerator::Generate(CodeGenerator::UnaryOperator & node)
{
  Line(node.m_ret + " = " + node.m_func + " " + node.m_arg);
  return 0;
}

int Pretty_OutputGenerator::Generate(CodeGenerator::BinaryOperator & node)
{
  Line(node.m_ret + " = " + node.m_arg1 + " " + node.m_func + " " + node.m_arg2);
  return 0;
}

int Pretty_OutputGenerator::Generate(CodeGenerator::IndexOp & node)
{
  Line(std::string(node.m_store ? "store " : "load ") + node.m_array);
  return 0;
}

int Pretty_OutputGenerator::Generate(CodeGenerator::Input & node)
{
  Line(std::string(node.m_line ? "line input " : "input ") + node.m_name);
  return 0;
}

int Pretty_OutputGenerator::Generate(CodeGenerator::OnGoto & node)
{
  Line("on " + node.m_index + " goto");
  return 0;
}

int Pretty_OutputGenerator::Generate(CodeGenerator::Clear &)
{
  Line("clear");
  return 0;
}

int Pretty_OutputGenerator::Generate(CodeGenerator::Width & node)
{
  Line("width " + std::to_string(node.m_width));
  return 0;
}
