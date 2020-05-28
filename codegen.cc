#include <iostream>

using namespace std;

#include "codegen.h"

CodeGenerator::CodeGenerator(const std::string & inputFilename, 
                             const std::string & outputFilename,
                                  AST::NodeList & program)
  : m_outputFilename(outputFilename)
  , m_inputFilename(inputFilename)
  , m_program(program)
{
}

bool CodeGenerator::Run()
{
  if (m_outputFilename == "-") {
    m_outputStream = &std::cout;
  }
  else {
    m_outputFile.open(m_outputFilename, std::ofstream::out | std::ofstream::trunc);
    if (!m_outputFile.is_open()) {
      cerr << "error: cannot create output file '" << m_outputFilename << "'" << endl;
      return false;
    }
    m_outputStream = &m_outputFile;
  }
  Prologue();
  for (auto & r : m_program.m_list) {
    Dispatch(r.get());
  }
  Epilogue();
  return true;
}

void CodeGenerator::Prologue()
{}

void CodeGenerator::Epilogue()
{}

////////////////////////////////////////////////////////////

C_CodeGenerator::C_CodeGenerator (const std::string & inputFilename, 
                                  const std::string & outputFilename,
                                      AST::NodeList & program)
  : CodeGenerator(inputFilename, outputFilename, program)
{
}

void C_CodeGenerator::Prologue()
{
  *m_outputStream
           << "/* Generator by basalt */\n"
           << "#include <stdlib.h>\n"
           << "extern void basalt_init();\n"
           << "extern void basalt_print_string(const char *);\n"
           << "int main(int argc, char * argv[])\n"
           << "{\n"
           << "  basalt_init();\n" 
           ;
}

void C_CodeGenerator::Epilogue()
{
  *m_outputStream
           << "  exit(0);\n"
           << "}\n"
           ;
}
void C_CodeGenerator::Dispatch(AST::Node * node)
{
  node->C_Generate(*this);
}

void AST::Node::C_Generate(C_CodeGenerator & gen)
{
  //cerr << "unknown node" << endl;
}

void AST::Node::C_Print(C_CodeGenerator & gen)
{
  //cerr << "unknown node" << endl;
}

void AST::SourceLine::C_Generate(C_CodeGenerator & gen)
{
  *gen.m_outputStream 
        << "#line " << m_lineNumber << " \"" << gen.m_inputFilename << "\"\n"
        << "  /* " << m_line << " */\n";
}

void AST::StringValue::C_Print(C_CodeGenerator & gen)
{
  *gen.m_outputStream << "  basalt_print_string();\n" << endl;
}

void AST::PrintComma::C_Print(C_CodeGenerator & gen)
{
  *gen.m_outputStream << "  basalt_print_comma();\n" << endl;
}

void AST::Print::C_Generate(C_CodeGenerator & gen)
{
  *gen.m_outputStream << "// print\n";
  for (auto & r : m_list) {
    *gen.m_outputStream << "// arg1\n";
    r->C_Print(gen);
  }
}
