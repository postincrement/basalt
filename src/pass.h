#ifndef COMPILER_PASS
#define COMPILER_PASS

#include <fstream>
#include <set>
#include <string>
#include <typeinfo>
#include <cxxabi.h>

#include "parser/ast.h"
#include "errorcode.h"

class Pass1 
{
  public:
    Pass1(const AST::Program & g_program);
    bool Run();

  protected:
    const AST::Program & m_program;
};

#endif // COMPILER_PASS