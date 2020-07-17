#include <iostream>
#include <typeinfo>
#include <cxxabi.h>

using namespace std;

#include "basalt.h"
#include "codegen.h"

#define DEFAULT_TEMP_PREFIX "temp_"

CodeGenerator::CodeGenerator(const Config &config)
    : m_config(config)
{
}

const CodeGenerator::Config &CodeGenerator::GetConfig() const
{
  return m_config;
}

bool CodeGenerator::Run(const std::string &inputFilename, std::ostream *outputStream, const AST::Program &program)
{
  m_inputFilename = inputFilename;
  m_outputStream = outputStream;
  m_program = &program;

  CheckVars();
  Body();
  return true;
}

CodeGenerator::Closure &CodeGenerator::Top()
{
  if (m_stack.size() < 1) {
    cerr << "error: closure stack smashed" << endl;
    exit(-1);
  }
  return *m_stack[m_stack.size() - 1];
}

std::stringstream &CodeGenerator::TopOutput(bool indent)
{
  if (indent)
    Top().Output() << Top().Indent();
  return Top().Output();
}

void CodeGenerator::Push()
{
  int indent = (m_stack.size() < 1) ? 2 : (Top().GetIndent() + 2);
  m_stack.push_back(CreateClosure(indent));
  if (m_stack.size() > 1)
  {
    std::string str = GetConfig().m_open;
    if (!str.empty())
      TopOutput(false) << Top().Indent(-2) << "{\n";
  }
}

std::string CodeGenerator::Pop()
{
  std::string str;
  if (m_stack.size() > 1)
  {
    std::string str = GetConfig().m_close;
    if (!str.empty())
      TopOutput(false) << Top().Indent(-2) << "}\n";
  }
  str = TopOutput(false).str();
  if (m_stack.size() > 0)
  {
    delete m_stack.back();
    m_stack.pop_back();
  }
  if (m_stack.size() > 0)
  {
    TopOutput(false) << str;
  }
  return str;
}

///////////////////////////////////////////////////////

bool CodeGenerator::CheckVars()
{
  // check global vars
  for (auto &r : AST::g_globalVars)
  {
    AST::VarInfo &info = r.second;
    if ((info.m_lhsLine != 0) && (info.m_rhsLine == 0)) {
      SourceWarning(eWarning_VarDefinedButNotUsed, info.m_lhsLine, r.first);
    }
  }

  // goto list is indexed by goto destination
  for (auto & r : AST::g_gotoDestinationInfo) {

    // get the destination of the goto
    const std::string & lineNumber = r.first;

    // print error if destination not found 
    if (AST::g_lineNumberInfo.count(lineNumber) == 0) {
      unsigned line = *r.second.m_usedLine.begin();
      SourceError(eError_GotoDestinationNotFound, line, lineNumber);
    }
  }

  return true;
}

int CodeGenerator::ResolveGotoDestination(const std::string & ref)
{
  auto r = AST::g_lineNumberInfo.find(ref);
  if (r == AST::g_lineNumberInfo.end())
    return -1;
  return r->second;  
}

///////////////////////////////////////////////////////

static std::string DemangleTypeName(const std::type_info &r)
{
  const char *mangledName = r.name();
  int status;
  char *demangledName = abi::__cxa_demangle(mangledName, NULL, NULL, &status);
  std::string ret(demangledName);
  free(demangledName);
  return ret;
}

int CodeGenerator::Generate(const AST::Node &expr)
{
  cerr << "warning: unimplemented Generate for " << DemangleTypeName(typeid(expr)) << "\n";
  return 1;
}

int CodeGenerator::Evaluate(const AST::Node &expr, std::string &result)
{
  const std::type_info &r = typeid(expr);
  cerr << "warning: unimplemented Evaluate for " << DemangleTypeName(typeid(expr)) << "\n";
  return 1;
}

///////////////////////////////////////////////////////

CodeGenerator::Closure *CodeGenerator::CreateClosure(int indent) const
{
  return new Closure(m_tempPrefix, indent);
}

CodeGenerator::Closure::Closure(const std::string &tempPrefix, int indent)
    : m_tempPrefix(tempPrefix), m_indent(indent)
{
}

CodeGenerator::Closure::~Closure()
{
}

std::string CodeGenerator::Closure::Indent(int n) const
{
  return std::string(m_indent + n, ' ');
}

int CodeGenerator::Closure::GetIndent() const
{
  return m_indent;
}

std::stringstream &CodeGenerator::Closure::Output()
{
  return m_output;
}

std::string CodeGenerator::Closure::GetTempName()
{
  stringstream name;
  name << DEFAULT_TEMP_PREFIX << m_tempIndex++;
  return name.str();
}

bool CodeGenerator::Closure::HasGoto() const
{
  return m_hasGoto;
}

void CodeGenerator::Closure::SetGoto(bool v)
{
  m_hasGoto = v;
}

///////////////////////////////////////////////////////

int CodeGenerator::Generate(const AST::SourceLine &line)
{
  if (line.m_statements)
    line.m_statements->Generate(*this);

  return 0;
}

int CodeGenerator::Generate(const AST::Statement &statement)
{
  for (auto &r : statement.m_list) {
    r->Generate(*this);
    TopOutput() << TopOutput().str();
  }
  return 0;
}

///////////////////////////////////////////////////////

int CodeGenerator::Print(const AST::Node &expr)
{
  const std::type_info &r = typeid(expr);
  cerr << "warning: unimplemented Print for " << DemangleTypeName(typeid(expr)) << "\n";
  return 1;
}

int CodeGenerator::Print(const AST::PrintSemiColon & expr)
{
  return 0;
}

///////////////////////////////////////////////////////

int CodeGenerator::Evaluate(const AST::StringConstant & expr, std::string & result)
{
  result = expr.GetValue();
  return 0;
}

int CodeGenerator::Evaluate(const AST::Int16Constant & expr, std::string & result)
{
  stringstream strm;
  strm << expr.GetValue();
  result = strm.str();
  return 0;
}

///////////////////////////////////////////////////////




