#include "../basalt.h"

#include "codegen.h"
#include <iostream>
#include <typeinfo>

using namespace std;

struct AsmTypeInfoRec {
  const char * m_asmtype; 
  const char * m_initializer;
  const char * m_printFn;
  const char * m_strFn;
  const char * m_suffix;
};

// must be indexed by VarType
static AsmTypeInfoRec g_varTypeInfo[] = {
  { 0 },                        // none
  { "word",   "0", "basalt_print_int16",  "basalt_str_int16",  "int16"  },     // eInt16
  { "dword",  "0", "basalt_print_int32",  "basalt_str_int32",  "int32"  },     // eInt32
  { "word",   "0", "basalt_print_single", "basalt_str_single", "single" },    // eSingle
  { "dword",  "0", "basalt_print_double", "basalt_str_double", "double" },    // eDouble
  { "word",   "0", "basalt_print_string", 0,                   "string" }     // eString
};

////////////////////////////////////////////////////////////

Z80_CodeGenerator::Z80_CodeGenerator ()
{
}

std::string Z80_CodeGenerator::GetOutputFileExtension() const
{
  return ".asm";
}

static bool CreateAVar(const std::string & name,
                       const AST::VarInfo & var, 
                       Z80_CodeGenerator::AsmVarDef & avar,
                       int tag)
{
  stringstream strm;

  strm << "var_";
  for (auto r : name) {
    if (isalnum(r))
      strm << r;
  }
  if (tag != 0)
    strm << "_" << tag;
  strm << "_";  
  strm << g_varTypeInfo[(int)var.m_type].m_suffix;
  avar.m_aname = strm.str();
  avar.m_type  = var.m_type;

  return true;
} 


bool Z80_CodeGenerator::Body()
{
  for (auto & r : AST::g_globalVars) {
    AST::VarInfo & info = r.second;
    int tag = 0;
    AsmVarDef avar;
    for (;;) {
      if (!CreateAVar(r.first, info, avar, tag)) {
        cerr << "internal error: cannot map var to C type" << endl;
        return false;
      }
      if (m_anames.count(avar.m_aname) == 0)
        break;
      ++tag;  
    }
    m_globalVars[r.first] = avar;
    m_anames.insert(avar.m_aname);
  }

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

  *m_outputStream << "\torg\t0x100\n"
                  << "\tjp\tstart\n"
                  ;

  if (AST::g_stringConstants.size() > 0) {
    *m_outputStream << "\n";
    *m_outputStream << "; String constants\n";
    for (auto & r : AST::g_stringConstants) {
      *m_outputStream << "str_" << r.second << ":\t" 
                      << "ds\t"
                      << "\"" << r.first << "\""
                      << endl;
    }
    *m_outputStream << "\n";
  }
  *m_outputStream << "start:\n";

  Push();

  for (size_t i = 0; i < m_program->m_list.size(); ++i) {
    auto & r = m_program->m_list[i];
    r->Generate(*this);
  }
  *m_outputStream << Pop()
                  << "\n"
                  << "end:\tjp\t0x0000\n"
           ;

  *m_outputStream << "\n"
                  << "; Vars\n"
                  ;         

  for (auto & r : m_globalVars) {
    AsmTypeInfoRec & info = g_varTypeInfo[(int)r.second.m_type];
    *m_outputStream << r.second.m_aname << ":\t"
                    << info.m_asmtype
                    << "\t"
                    << info.m_initializer
                    << "\t; " << r.first << "\n"
                    ;
  }
  *m_outputStream << "\n";

  return true;           
}

int Z80_CodeGenerator::Generate(const AST::Statement & statement)
{
  return 0;
}

int Z80_CodeGenerator::Generate(const AST::End & expr)
{
  TopOutput() << "\tjp\tend\n";
  return 0;
}


