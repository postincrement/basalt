#ifndef COMPILER_PASS
#define COMPILER_PASS

#include <fstream>
#include <set>
#include <string>
#include <typeinfo>
#include <cxxabi.h>

#include "parser/ast.h"
#include "errorcode.h"

class CompilerPass
{
  public:
    CompilerPass();
};

#endif // COMPILER_PASS