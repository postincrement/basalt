#include <iostream>

using namespace std;

#include "basalt.h"
#include "codegen.h"

#define DEFAULT_TEMP_PREFIX "temp_"

CodeGenerator::CodeGenerator(const Config & config, const AST::Parser & parser)
  : m_config(config)
  , m_parser(parser)
  , m_program(parser.m_program)
{
}

///////////////////////////////////////////////////////

const CodeGenerator::Config &CodeGenerator::GetConfig() const
{
  return m_config;
}

///////////////////////////////////////////////////////

void CodeGenerator::CompilerErrorInternal(ErrorCode code, unsigned ln, const std::string & msg)
{
  g_application.CompilerErrorInternal(code, ln, msg);
}

///////////////////////////////////////////////////////

bool CodeGenerator::Run(const std::string &inputFilename, std::ostream *outputStream)
{
  m_inputFilename = inputFilename;
  m_outputStream = outputStream;

  CheckVars();
  CheckJumps();

  Body();
  return true;
}

///////////////////////////////////////////////////////

bool CodeGenerator::CheckVars()
{
  // check global vars
  for (auto &r : m_parser.m_globalVars) {
    const AST::VarInfo &info = r.second;
    if ((info.m_lhsLine != 0) && (info.m_rhsLine == 0)) {
      CompilerError(eWarning_VarDefinedButNotUsed, info.m_lhsLine, "variable '" << r.first << "' defined but not used");
    }
  }

  return true;
}

///////////////////////////////////////////////////////

bool CodeGenerator::CheckJumps()
{
  // goto list is indexed by goto destination
  for (auto & r : m_parser.m_jumpDestinationInfo) {

    // get the destination of the goto
    const std::string & lineNumber = r.first;

    // print error if destination not found 
    if (m_parser.m_lineNumberInfo.count(lineNumber) == 0) {
      unsigned line = *r.second.m_usedLine.begin();
      CompilerError(eError_GotoDestinationNotFound, line, lineNumber);
    }
  }

  return true;
}

///////////////////////////////////////////////////////

std::string DemangleTypeName(const std::type_info &r)
{
  const char *mangledName = r.name();
  int status;
  char *demangledName = abi::__cxa_demangle(mangledName, NULL, NULL, &status);
  std::string ret(demangledName);
  free(demangledName);
  return ret;
}

///////////////////////////////////////////////////////

int CodeGenerator::Generate(const AST::Node &expr)
{
//  InternalError("unimplemented Generate for " << DemangleTypeName(typeid(expr)));
  return 1;
}

int CodeGenerator::Evaluate(const AST::Node &expr, std::string &result)
{
  InternalError("unimplemented Evaluate for " << DemangleTypeName(typeid(expr)));
  return 1;
}

int CodeGenerator::Print(const AST::Node &expr)
{
  CompilerError(eWarning_NotImplemented, 0, "unimplemented Print for " << DemangleTypeName(typeid(expr)));
  return 0;
}

int CodeGenerator::Generate(const AST::SourceLine &line)
{
  if (line.m_statements)
    return line.m_statements->Generate(*this);

  cerr << "no statements on source line" << endl;
  return 0;
}

int CodeGenerator::Generate(const AST::Statement &statement)
{
//  m_currentStatementLine = statement.m_lineNumber;
//  CompilerError(eWarning_NotImplemented, statement.m_lineNumber, "unimplemented Generate for " << DemangleTypeName(typeid(statement)));
  return 0;
}

int CodeGenerator::Generate(const AST::Print & printExpr)
{
  bool trailingNewLine = true;
  if (printExpr.m_list != nullptr) {
    AST::ExprList & expr = *printExpr.m_list;
    if (expr.m_list.size() > 0) {
      for (auto & r : expr.m_list) {
        if (r != nullptr)
          r->Print(*this);
      }
      if (expr.m_list[expr.m_list.size()-1]->IsPrintSemiColon())
        trailingNewLine = false;
    }
  }
  if (trailingNewLine)
    PrintNewLine();
  return 0;
}

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

int CodeGenerator::Print(const AST::PrintSemiColon & expr)
{
  return 0;
}



#if 0

///////////////////////////////////////////////////////

CodeGenerator::Closure &CodeGenerator::Top()
{
  if (m_stack.size() < 1) {
    InternalError("closure stack smashed");
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
  int indent = (m_stack.size() < 1) ? m_config.m_indent : (Top().GetIndent() + m_config.m_indentInc);
  m_stack.push_back(CreateClosure(indent));
  if (m_stack.size() > 1)
  {
    std::string str = GetConfig().m_open;
    if (!str.empty())
      TopOutput(false) << Top().Indent(-m_config.m_indentInc) << "{\n";
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

std::string CodeGenerator::GetGlobalTempName()
{
  std::stringstream strm;
  strm << "g" << m_config.m_tempPrefix << m_globalTempIndex++;
  return strm.str();
}


///////////////////////////////////////////////////////////////////////////

int CodeGenerator::ResolveGotoDestination(const std::string & ref)
{
  auto r = m_parser.m_lineNumberInfo.find(ref);
  if (r == m_parser.m_lineNumberInfo.end())
    return -1;
  return r->second;  
}

///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////

CodeGenerator::Closure *CodeGenerator::CreateClosure(int indent) const
{
  return new Closure(m_config.m_tempPrefix, indent);
}

CodeGenerator::Closure::Closure(const std::string &tempPrefix, int indent)
    : m_tempPrefix(tempPrefix)
    , m_indent(indent)
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

///////////////////////////////////////////////////////

///////////////////////////////////////////////////////

///////////////////////////////////////////////////////

///////////////////////////////////////////////////////

#endif


