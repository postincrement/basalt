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
#if 0
  for (auto & r : AST::g_globalVars) {
    AST::VarInfo & info = r.second;
    int tag = 0;
  }
#endif

  *m_outputStream
           << "; Z80 generator by basalt\n"
           << "\n"
           ;

  for (size_t i = 0; i < m_program->m_list.size(); ++i) {
    auto & r = m_program->m_list[i];
    //*m_outputStream << "struct LineFunction " LINEFN_PREFIX << i << "(); /* " << r->GetBasicLineNumber() << " */\n";
  }

#if 0
  for (size_t i = 0; i < m_program->m_list.size(); ++i) {
    //auto & r = m_program->m_list[i];
  }
#endif

  *m_outputStream << "\torg 0x100\n"
                  << "\tjp start\n"
                  ;

  for (size_t i = 0; i < m_program->m_list.size(); ++i) {
    auto & r = m_program->m_list[i];
    r->Generate(*this);
  }

  *m_outputStream << "start:\n"
                  << "\n"
                  << "end:\tjp 0x0000\n"
           ;

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

