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

bool CodeGenerator::LookupGlobalVar(const std::string & varName, AST::VarInfo & var)
{
  if (m_parser.m_globalVars.count(varName) == 0)
    return false;

  var = m_parser.m_globalVars[varName];
  return true;
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
  CompilerError(eWarning_NotImplemented, m_lineNumber, "unimplemented Print for " << DemangleTypeName(typeid(expr)));
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
  CompilerError(eWarning_NotImplemented, statement.m_lineNumber, "unimplemented Generate for " << DemangleTypeName(typeid(statement)));
  return 0;
}

//////////////////////////////////////////////////////////////////////////

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
  if (trailingNewLine) {
    m_funcsUsed.insert("print_newline");
    PrintNewLine();
  }
  return 0;
}

int CodeGenerator::Print(const AST::StringConstant & expr)
{
  m_funcsUsed.insert("print_string");
  PrintStringConst(expr.GetValue());
  return 0;
}

int CodeGenerator::Print(const AST::PrintComma & expr)
{
  m_funcsUsed.insert("print_tab");
  PrintTab();
  return 0;
}

int CodeGenerator::Print(const AST::PrintSemiColon & expr)
{
  return 0;
}

int CodeGenerator::Print(const AST::NumericExpr & expr)
{
  VarType type = expr.GetType();
  std::string typeStr = std::string(AST::GetVarTypeInfo(type).m_name);
  std::string str;    
  expr.Evaluate(*this, str);
  if (expr.IsVarRef()) {
    m_funcsUsed.insert("print_var_" + typeStr);
    PrintNumericVar(str, type);
  }
  else if (expr.IsConstant()) {
    m_funcsUsed.insert("print_" + typeStr);
    PrintNumericConst(str, type);
  }
  else {
    std::string tempName = CreateTempVar(type);
    NumericAssign(tempName, str, type);
    m_funcsUsed.insert("print_var_" + typeStr);
    PrintNumericVar(tempName, type);
    DestroyTempVar(tempName);
  }

  return 0;
}

int CodeGenerator::Print(const AST::StringExpr & expr)
{
  VarType type = expr.GetType();
  std::string str;    
  expr.Evaluate(*this, str);
  if (expr.IsVarRef()) {
    m_funcsUsed.insert("print_var_string");
    PrintStringVar(str);
  }
  else if (expr.IsConstant()) {
    m_funcsUsed.insert("print_string");
    PrintStringConst(str);
  }
  else {
    std::string tempName = CreateTempVar(type);
    StringAssign(tempName, str);
    m_funcsUsed.insert("print_var_string");
    PrintStringVar(tempName);
    DestroyTempVar(tempName);
  }

  return 0;
}

///////////////////////////////////////////////////////////////////////
//
//  String functions
//

int CodeGenerator::Evaluate(const AST::StringConstant & expr, std::string & result)
{
  result = expr.GetValue();
  return 0;
}

int CodeGenerator::Evaluate(const AST::StringVarRef & expr, std::string & result) 
{ 
  result = expr.GetName();
  return 0; 
}

int CodeGenerator::Generate(const AST::StringAssign & expr)
{
  AST::VarInfo var;
  if (!LookupGlobalVar(expr.m_lhs->GetName(), var))
    return -1;

  if (expr.m_rhs == nullptr) {
    InternalError("expression missing rhs");
    return -1;
  }

  std::string rhs;

  expr.m_rhs->Evaluate(*this, rhs);

  if (expr.m_lhs->IsVarRef() &&
      expr.m_rhs->IsVarRef() &&
      (var.m_originalName == rhs)) {
    CompilerError(eWarning_RemovedUnecessaryAssignment, m_lineNumber, ""); // TopOutput() << "/* optimised out */\n";
  }
  else {
    if (expr.m_rhs->IsConstant() || expr.m_rhs->IsVarRef()) {
      TopOutput() << cvar.m_cname << " = strdup(\"" << rhs << "\");\n";
    }
    else {
      TopOutput() << "if (!" << rhs << ")\n";
      TopOutput() << "  " << cvar.m_cname << " = 0;\n";
      TopOutput() << "else\n";
      TopOutput() << "  " << cvar.m_cname << " = " << rhs << ";\n";
    }

    Pop();
  }

  return eOp_NextStatement;

}


///////////////////////////////////////////////////////////////////////
//
//  Integer expressions  
//

int CodeGenerator::Evaluate(const AST::NumericConstant & expr, std::string & result)
{
  result = expr.AsString();
  return 0;
}

int CodeGenerator::Evaluate(const AST::NumericVarRef & expr, std::string & result) 
{ 
  result = expr.GetName();
  return 0; 
}

///////////////////////////////////////////////////////////////////////
//
//  Functions  
//

int CodeGenerator::EvaluateNumericFunction(const std::string & op, const AST::NumericExpr * arg, std::string & result)
{
  if (arg == nullptr)
    return -1;

  std::string argStr;
  arg->Evaluate(*this, argStr);

  result = NumericFunction(op, argStr, arg->GetType());

  return 0;
}

int CodeGenerator::EvaluateStringFunction(const std::string & op, const AST::NumericExpr * arg, std::string & result)
{
  if (arg == nullptr)
    return -1;

  std::string argStr;
  arg->Evaluate(*this, argStr);

  result = StringFunction(op, argStr);

  return 0;
}

int CodeGenerator::Evaluate(const AST::IntFunction & expr, std::string & result)
{
  return EvaluateNumericFunction("(int)", expr.GetArg1(), result);
}

int CodeGenerator::Evaluate(const AST::SqrFunction & expr, std::string & result)
{
  return EvaluateNumericFunction("sqrt", expr.GetArg1(), result);
}

int CodeGenerator::Evaluate(const AST::TabFunction & expr, std::string & result)
{
  return EvaluateStringFunction("tab", expr.GetArg1(), result);
}

int CodeGenerator::Evaluate(const AST::ChrFunction & expr, std::string & result)
{
  return EvaluateStringFunction("chr", expr.GetArg1(), result);
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


