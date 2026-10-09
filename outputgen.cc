#include "outputgen.h"

#include <iostream>

bool OutputGenerator::Run(const std::string & inputFilename, std::ostream * outputStream)
{
  m_inputFilename = inputFilename;
  m_outputStream = new std::ostringstream(std::ios_base::ate);
  m_indent = m_config.m_indent;

  for (auto & code : m_codeGenerator.m_code)
    code->Generate(*this);

  OutputFilePrologue(*outputStream);
  *outputStream << m_outputStream->str();
  OutputFileEpilogue(*outputStream);

  delete m_outputStream;
  m_outputStream = nullptr;
  return true;
}

void OutputGenerator::OutputFilePrologue(std::ostream &)
{
}

void OutputGenerator::OutputFileEpilogue(std::ostream &)
{
}

int OutputGenerator::Unimplemented(const char * kind)
{
  std::cerr << "unimplemented output node " << kind << "\n";
  return 0;
}

int OutputGenerator::Generate(CodeGenerator::Node &) { return Unimplemented("node"); }
int OutputGenerator::Generate(CodeGenerator::BlockStart &) { return 0; }
int OutputGenerator::Generate(CodeGenerator::BlockEnd &) { return 0; }
int OutputGenerator::Generate(CodeGenerator::GotoTarget & node) { return Unimplemented("label"); }
int OutputGenerator::Generate(CodeGenerator::Goto &) { return Unimplemented("goto"); }
int OutputGenerator::Generate(CodeGenerator::Gosub &) { return Unimplemented("gosub"); }
int OutputGenerator::Generate(CodeGenerator::Return &) { return Unimplemented("return"); }
int OutputGenerator::Generate(CodeGenerator::End &) { return Unimplemented("end"); }
int OutputGenerator::Generate(CodeGenerator::System &) { return Unimplemented("system"); }
int OutputGenerator::Generate(CodeGenerator::If &) { return Unimplemented("if"); }
int OutputGenerator::Generate(CodeGenerator::Else &) { return Unimplemented("else"); }
int OutputGenerator::Generate(CodeGenerator::EndIf &) { return Unimplemented("endif"); }
int OutputGenerator::Generate(CodeGenerator::CreateTempVar &) { return 0; }
int OutputGenerator::Generate(CodeGenerator::PrintNewLine &) { return Unimplemented("print-nl"); }
int OutputGenerator::Generate(CodeGenerator::PrintTab &) { return Unimplemented("print-tab"); }
int OutputGenerator::Generate(CodeGenerator::PrintStringConst &) { return Unimplemented("print-str"); }
int OutputGenerator::Generate(CodeGenerator::PrintStringVar &) { return Unimplemented("print-svar"); }
int OutputGenerator::Generate(CodeGenerator::PrintNumber &) { return Unimplemented("print-num"); }
int OutputGenerator::Generate(CodeGenerator::UnaryOperator &) { return Unimplemented("unary"); }
int OutputGenerator::Generate(CodeGenerator::BinaryOperator &) { return Unimplemented("binary"); }
int OutputGenerator::Generate(CodeGenerator::IndexOp &) { return Unimplemented("index"); }
int OutputGenerator::Generate(CodeGenerator::Input &) { return Unimplemented("input"); }
int OutputGenerator::Generate(CodeGenerator::OnGoto &) { return Unimplemented("ongoto"); }
int OutputGenerator::Generate(CodeGenerator::Clear &) { return Unimplemented("clear"); }
int OutputGenerator::Generate(CodeGenerator::Width &) { return Unimplemented("width"); }
