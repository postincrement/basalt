#include <iostream>
#include <typeinfo>
#include <cxxabi.h>

using namespace std;

#include "basalt.h"
#include "codegen.h"

CodeGenerator::CodeGenerator()
{
}

bool CodeGenerator::Run(std::ostream * outputStream, const AST::Program & program)
{
  m_program      = &program;
  m_outputStream = outputStream;

  CheckVars();
  Body();
  return true;
}

bool CodeGenerator::CheckVars()
{
  // check global vars
  for (auto & r : AST::g_globalVars) {
    AST::VarInfo & info = r.second;
    if ((info.m_lhsLine != 0) && (info.m_rhsLine == 0)) {
      SourceWarning(eWarning_VarDefinedButNotUsed, info.m_lhsLine, r.first);
    }
  }

  // check gotos
  for (auto & r : AST::g_gotoInfo) {
    const std::string & lineNumber = r.first;
    if (AST::g_lineNumberInfo.count(lineNumber) == 0) {
      unsigned line = *r.second.m_usedLine.begin();
      SourceError(eError_GotoDestinationNotFound, line, lineNumber);
    }
  }

  return true;
}

static std::string DemangleTypeName(const std::type_info & r)
{
  const char * mangledName = r.name();
  int status;
  char * demangledName = abi::__cxa_demangle (mangledName, NULL, NULL, &status);
  std::string ret(demangledName);
  free(demangledName);
  return ret;
}

int CodeGenerator::Generate(const AST::Node & expr)
{
  cerr << "warning: unimplemented Generate for " << DemangleTypeName(typeid(expr)) << "\n";
}

int CodeGenerator::Print(const AST::Node & expr)
{
  const std::type_info & r = typeid(expr);
  cerr << "warning: unimplemented Print for " <<DemangleTypeName(typeid(expr)) << "\n";
}

int CodeGenerator::Evaluate(const AST::Node & expr, std::string & result)
{
  const std::type_info & r = typeid(expr);
  cerr << "warning: unimplemented Evaluate for " <<DemangleTypeName(typeid(expr)) << "\n";
}

///////////////////////////////////////////////////////