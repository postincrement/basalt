#include <iostream>

using namespace std;

#include "basalt.h"
#include "codegen.h"

#define DEFAULT_TEMP_PREFIX "temp_"

PseudoCodeGenerator::PseudoCodeGenerator(AST::Parser & parser)
  : m_parser(parser)
  , m_program(parser.m_program)
{
}

///////////////////////////////////////////////////////

void PseudoCodeGenerator::CompilerErrorInternal(ErrorCode code, unsigned ln, const std::string & msg)
{
  g_application.CompilerErrorInternal(code, ln, msg);
}

///////////////////////////////////////////////////////

bool PseudoCodeGenerator::Run(const std::string &inputFilename)
{
  m_inputFilename = inputFilename;

  CheckVars();
  CheckJumps();

  for (auto & line : m_parser.m_program.m_list) {
    if (line == nullptr)
      continue;
    //if (m_parser.m_jumpDestinationInfo.count(line->GetBasicLineNumber()) > 0)  
    //  m_output << line->GetBasicLineNumber() << endl;
    for (auto & statement : line->m_statements->m_list) {
      SetLineNumber(statement->m_lineNumber);
      //m_output << "#  " << statement->m_text << endl;
      statement->Generate(*this);
    }
  }

  return true;
}

///////////////////////////////////////////////////////

bool PseudoCodeGenerator::CheckVars()
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

bool PseudoCodeGenerator::CheckJumps()
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

std::string PseudoCodeGenerator::GetTempName()
{
  stringstream name;
  name << DEFAULT_TEMP_PREFIX << m_tempIndex++;
  return name.str();
}

bool PseudoCodeGenerator::LookupGlobalVar(const std::string & varName, AST::VarInfo & var)
{
  if (m_parser.m_globalVars.count(varName) == 0)
    return false;

  var = m_parser.m_globalVars[varName];
  return true;
}

///////////////////////////////////////////////////////

int PseudoCodeGenerator::Generate(const AST::Node &expr)
{
//  InternalError("unimplemented Generate for " << DemangleTypeName(typeid(expr)));
  return 1;
}

int PseudoCodeGenerator::Evaluate(const AST::Node &expr, std::string &result)
{
  InternalError("unimplemented Evaluate for " << DemangleTypeName(typeid(expr)));
  return 1;
}

int PseudoCodeGenerator::Print(const AST::Node &expr)
{
  CompilerError(eWarning_NotImplemented, m_lineNumber, "unimplemented Print for " << DemangleTypeName(typeid(expr)));
  return 0;
}

int PseudoCodeGenerator::Generate(const AST::SourceLine &line)
{
  if (line.m_statements)
    return line.m_statements->Generate(*this);

  cerr << "no statements on source line" << endl;
  return 0;
}

int PseudoCodeGenerator::Generate(const AST::Statement &statement)
{
//  m_currentStatementLine = statement.m_lineNumber;
  CompilerError(eWarning_NotImplemented, statement.m_lineNumber, "unimplemented Generate for " << DemangleTypeName(typeid(statement)));
  return 0;
}

//////////////////////////////////////////////////////////////////////////

int PseudoCodeGenerator::Generate(const AST::Print & printExpr)
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
    Add<PrintNewLine>();
  }
  return 0;
}

int PseudoCodeGenerator::Print(const AST::StringConstant & expr)
{
  Add<PrintStringConst, std::string>(expr.GetValue());
  return 0;
}

int PseudoCodeGenerator::Print(const AST::PrintComma & expr)
{
  Add<PrintTab>();
  return 0;
}

int PseudoCodeGenerator::Print(const AST::PrintSemiColon & expr)
{
  return 0;
}

int PseudoCodeGenerator::Print(const AST::NumericExpr & expr)
{
  VarType type = expr.GetType();
  std::string str;    
  expr.Evaluate(*this, str);
  if (expr.IsVarRef()) {
    Add<PrintNumericVar, VarType, std::string>(type, str);
  }
  else if (expr.IsConstant()) {
    Add<PrintNumericConst, VarType, std::string>(type, str);
  }
  else {
    Block & block = Add<Block>();
    std::string tempName = GetTempName();
    Add<CreateTempVar, VarType, std::string>(block, type, str);
    Add<NumericAssign>(tempName, str, type);
    Add<PrintNumericVar, VarType, std::string>(block, type, tempName);
    Add<DestroyTempVar,  VarType, std::string>(block, type, tempName);
  }

  return 0;
}

int PseudoCodeGenerator::Print(const AST::StringExpr & expr)
{
  VarType type = expr.GetType();
  std::string str;    
  expr.Evaluate(*this, str);
  if (expr.IsConstant()) {
    Add<PrintStringConst>(str);
  }
  else {
    Add<PrintStringVar>(str);
  }

  return 0;
}


///////////////////////////////////////////////////////////////////////
//
//  Expressions
//

int PseudoCodeGenerator::Generate(const AST::AssignStatement & expr)
{
  m_currentStatementLine = expr.m_lineNumber;
  return expr.m_expr->Generate(*this);
}

///////////////////////////////////////////////////////////////////////
//
//  String functions
//

int PseudoCodeGenerator::Evaluate(const AST::StringConstant & expr, std::string & result)
{
  result = expr.GetValue();
  return 0;
}

int PseudoCodeGenerator::Evaluate(const AST::StringVarRef & expr, std::string & result) 
{ 
  result = expr.GetName();
  return 0; 
}

int PseudoCodeGenerator::Generate(const AST::StringAssign & expr)
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
    Add<StringAssign>(var.m_originalName, rhs);
  }
  return 0;
}


///////////////////////////////////////////////////////////////////////
//
//  Numeric expressions  
//

int PseudoCodeGenerator::Evaluate(const AST::NumericConstant & expr, std::string & result)
{
  result = expr.AsString();
  return 0;
}

int PseudoCodeGenerator::Evaluate(const AST::NumericVarRef & expr, std::string & result) 
{ 
  result = expr.GetName();
  return 0; 
}

int PseudoCodeGenerator::Generate(const AST::NumericAssign & expr)
{
  // TBD
  return 0;
}

///////////////////////////////////////////////////////////////////////
//
//  Functions  
//

int PseudoCodeGenerator::Evaluate(const AST::IntFunction & expr, std::string & result)
{
  return 0; //EvaluateNumericFunction("(int)", expr.GetArg1(), result);
}

int PseudoCodeGenerator::Evaluate(const AST::SqrFunction & expr, std::string & result)
{
  return 0; //EvaluateNumericFunction("sqrt", expr.GetArg1(), result);
}

int PseudoCodeGenerator::Evaluate(const AST::TabFunction & expr, std::string & result)
{
  return EvaluateStringFunction<FunctionTAB>(expr.GetArg1(), result);
}

int PseudoCodeGenerator::Evaluate(const AST::ChrFunction & expr, std::string & result)
{
  return EvaluateStringFunction<FunctionCHR>(expr.GetArg1(), result);
}

///////////////////////////////////////////////////////

int PseudoCodeGenerator::Node::Generate(CodeGenerator & gen)
{
  return gen.Generate(*this);
}

int PseudoCodeGenerator::Block::Generate(CodeGenerator & gen)
{
  return gen.Generate(*this);
}

int PseudoCodeGenerator::CreateTempVar::Generate(CodeGenerator & gen)
{
  return gen.Generate(*this);
}

int PseudoCodeGenerator::DestroyTempVar::Generate(CodeGenerator & gen)
{
  return gen.Generate(*this);
}

int PseudoCodeGenerator::PrintNewLine::Generate(CodeGenerator & gen)
{
  return gen.Generate(*this);
}

int PseudoCodeGenerator::PrintTab::Generate(CodeGenerator & gen)
{
  return gen.Generate(*this);
}

int PseudoCodeGenerator::PrintStringConst::Generate(CodeGenerator & gen)
{
  return gen.Generate(*this);
}

int PseudoCodeGenerator::PrintStringVar::Generate(CodeGenerator & gen)
{
  return gen.Generate(*this);
}

int PseudoCodeGenerator::PrintNumericConst::Generate(CodeGenerator & gen)
{
  return gen.Generate(*this);
}

int PseudoCodeGenerator::PrintNumericVar::Generate(CodeGenerator & gen)
{
  return gen.Generate(*this);
}

int PseudoCodeGenerator::FunctionCHR::Generate(CodeGenerator & gen)
{
  return gen.Generate(*this);
}

int PseudoCodeGenerator::FunctionTAB::Generate(CodeGenerator & gen)
{
  return gen.Generate(*this);
}

int PseudoCodeGenerator::StringAssign::Generate(CodeGenerator & gen)
{
  return gen.Generate(*this);
}

int PseudoCodeGenerator::NumericAssign::Generate(CodeGenerator & gen)
{
  return gen.Generate(*this);
}

///////////////////////////////////////////////////////

CodeGenerator::CodeGenerator(const Config & config, PseudoCodeGenerator & pseudo)
  : m_config(config)
  , m_pseudo(pseudo)
{
}

const CodeGenerator::Config & CodeGenerator::GetConfig() const
{
  return m_config;
}

bool CodeGenerator::Run(const std::string &inputFilename, std::ostream *outputStream)
{
  m_inputFilename = inputFilename;
  m_outputStream = outputStream;

  cerr << m_pseudo.m_pseudoCode.m_code.size() << " nodes found" << endl;

  // traverse here
  for (auto & code : m_pseudo.m_pseudoCode.m_code) {
    code->Generate(*this);
  }

  return true;
}


#if 0

///////////////////////////////////////////////////////

PseudoCodeGenerator::Closure &PseudoCodeGenerator::Top()
{
  if (m_stack.size() < 1) {
    InternalError("closure stack smashed");
    exit(-1);
  }
  return *m_stack[m_stack.size() - 1];
}

std::stringstream &PseudoCodeGenerator::TopOutput(bool indent)
{
  if (indent)
    Top().Output() << Top().Indent();
  return Top().Output();
}

void PseudoCodeGenerator::Push()
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

std::string PseudoCodeGenerator::Pop()
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

std::string PseudoCodeGenerator::GetGlobalTempName()
{
  std::stringstream strm;
  strm << "g" << m_config.m_tempPrefix << m_globalTempIndex++;
  return strm.str();
}


///////////////////////////////////////////////////////////////////////////

int PseudoCodeGenerator::ResolveGotoDestination(const std::string & ref)
{
  auto r = m_parser.m_lineNumberInfo.find(ref);
  if (r == m_parser.m_lineNumberInfo.end())
    return -1;
  return r->second;  
}

///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////

PseudoCodeGenerator::Closure *PseudoCodeGenerator::CreateClosure(int indent) const
{
  return new Closure(m_config.m_tempPrefix, indent);
}

PseudoCodeGenerator::Closure::Closure(const std::string &tempPrefix, int indent)
    : m_tempPrefix(tempPrefix)
    , m_indent(indent)
{
}

PseudoCodeGenerator::Closure::~Closure()
{
}

std::string PseudoCodeGenerator::Closure::Indent(int n) const
{
  return std::string(m_indent + n, ' ');
}

int PseudoCodeGenerator::Closure::GetIndent() const
{
  return m_indent;
}

std::stringstream &PseudoCodeGenerator::Closure::Output()
{
  return m_output;
}

///////////////////////////////////////////////////////

///////////////////////////////////////////////////////

///////////////////////////////////////////////////////

///////////////////////////////////////////////////////

#endif


