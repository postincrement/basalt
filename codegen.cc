#include <iostream>
#include <typeinfo>
#include <cxxabi.h>

using namespace std;

#include "basalt.h"
#include "codegen.h"


CodeGenerator::CodeGenerator(const std::string & inputFilename, 
                             const std::string & outputFilename,
                                  const AST::Program & program)
  : m_outputFilename(outputFilename)
  , m_inputFilename(inputFilename)
  , m_program(program)
{
}

bool CodeGenerator::Run()
{
  // check stuff
  CheckVars();

  // generate output
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



///////////////////////////////////////////////////////
