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
    Pass1(const AST::Parser & g_parser);
    bool Run();

  protected:
    const AST::Parser & m_parser;
};

#endif // COMPILER_PASS