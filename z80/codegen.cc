#include "../basalt.h"

#include "codegen.h"
#include <iostream>
#include <typeinfo>

using namespace std;

////////////////////////////////////////////////////////////

Z80_CodeGenerator::Z80_CodeGenerator ()
{
}

std::string Z80_CodeGenerator::GetOutputFileExtension() const
{
  return ".asm";
}

bool Z80_CodeGenerator::Body()
{
  cout << "generating Z80" << endl;

#if 0
  for (auto & r : AST::g_globalVars) {
    AST::VarInfo & info = r.second;
    int tag = 0;
  }

  *m_outputStream
           << "/* Z80 generator by basalt */\n"
           << "\n"
           ;

  for (size_t i = 0; i < m_program->m_list.size(); ++i) {
    auto & r = m_program->m_list[i];
    //*m_outputStream << "struct LineFunction " LINEFN_PREFIX << i << "(); /* " << r->GetBasicLineNumber() << " */\n";
  }

  for (size_t i = 0; i < m_program->m_list.size(); ++i) {
    //auto & r = m_program->m_list[i];
  }
#endif

  *m_outputStream 
           << "int main(int argc, char * argv[])\n"
           << "{\n"
           << "  exit(0);\n"
           << "}\n"
           ;

cerr << "finished Z80" << endl;

  return true;           
}

int Z80_CodeGenerator::Generate(const AST::SourceLine & line)
{
  return 0;
}

int Z80_CodeGenerator::Generate(const AST::Statement & statement)
{
  return 0;
}

int Z80_CodeGenerator::Generate(const AST::End & expr)
{
  return 0;
}

