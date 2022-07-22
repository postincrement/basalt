#include <iostream>

using namespace std;

#include "basalt.h"
#include "outputgen.h"

#define DEFAULT_TEMP_PREFIX "temp_"

CodeGenerator::CodeGenerator(AST::Parser & parser)
  : m_parser(parser)
  , m_program(parser.m_program)
{
}

///////////////////////////////////////////////////////

void CodeGenerator::CompilerErrorInternal(ErrorCode code, unsigned ln, const std::string & msg)
{
  g_application.CompilerErrorInternal(code, ln, msg);
}

///////////////////////////////////////////////////////

bool CodeGenerator::Run(const std::string &inputFilename)
{
  m_inputFilename = inputFilename;

  CheckVars();
  CheckJumps();

  for (auto & line : m_parser.m_program.m_list) {
    if (line == nullptr)
      continue;
    if (m_parser.m_jumpDestinationInfo.count(line->GetBasicLineNumber()) > 0)
      Add<GotoTarget>(std::string("line_") + line->GetBasicLineNumber());
    for (auto & statement : line->m_statements->m_list) {
      Add<BlockStart>();
      SetLineNumber(statement->m_lineNumber);
      statement->Generate(*this);
      Add<BlockEnd>();
      //m_output << "#  " << statement->m_text << endl;
    }
  }

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

std::string CodeGenerator::GetTempName()
{
  stringstream name;
  name << DEFAULT_TEMP_PREFIX << m_tempIndex++;
  return name.str();
}

bool CodeGenerator::LookupGlobalVar(const std::string & varName, AST::VarInfo & var)
{
  if (m_parser.m_globalVars.count(varName) == 0) {
    cerr << "cannot find global var " << varName << endl;
    return false;
  }

  var = m_parser.m_globalVars[varName];
  return true;
}

///////////////////////////////////////////////////////

int CodeGenerator::Generate(const AST::Node & expr)
{
//  InternalError(
  cout << "unimplemented Generate for " << DemangleTypeName(typeid(expr)) << endl;
  return 1;
}

int CodeGenerator::Evaluate(const AST::Node & expr, std::string &result)
{
  InternalError("unimplemented Evaluate for " << DemangleTypeName(typeid(expr)));
  return 1;
}

int CodeGenerator::Print(const AST::Node & expr)
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

int CodeGenerator::Generate(const AST::Statement & statement)
{
//  m_currentStatementLine = statement.m_lineNumber;
  //CompilerError(eWarning_NotImplemented, statement.m_lineNumber, "unimplemented Generate for " << DemangleTypeName(typeid(statement)));
  return 0;
}

int CodeGenerator::Generate(const AST::GotoStatement & statement)
{
  Add<Goto>(std::string("line_")  + statement.GetRef());
  return 0;
}

int CodeGenerator::Generate(const AST::GosubStatement & statement)
{
  Add<Gosub>(statement.GetRef());
  return 0;
}

int CodeGenerator::Generate(const AST::ReturnStatement & statement)
{
  Add<Return>();
  return 0;
}

int CodeGenerator::Generate(const AST::System & statement)
{
  Add<System>();
  return 0;
}

int CodeGenerator::Generate(const AST::End & statement)
{
  Add<End>();
  return 0;
}

//////////////////////////////////////////////////////////////////////////

int CodeGenerator::Generate(const AST::Print & printExpr)
{
  m_printUsed = true;
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

int CodeGenerator::Print(const AST::StringConstant & expr)
{
  Add<PrintStringConst, std::string>(expr.GetValue());
  return 0;
}

int CodeGenerator::Print(const AST::PrintComma & expr)
{
  Add<PrintTab>();
  return 0;
}

int CodeGenerator::Print(const AST::PrintSemiColon & expr)
{
  return 0;
}

int CodeGenerator::Print(const AST::NumericExpr & expr)
{
  VarType type = expr.GetType();
  std::string str;    
  expr.Evaluate(*this, str);
  if (expr.IsConstant()) {
    switch (type) {
      case VarType::eNone:
      case VarType::eString:
        break;
      case VarType::eInt16:
        Add<PrintInt16Const, VarType, std::string>(type, str);
        break;
      case VarType::eInt32:
        Add<PrintInt32Const, VarType, std::string>(type, str);
        break;
      case VarType::eSingle:
        Add<PrintSingleConst, VarType, std::string>(type, str);
        break;
      case VarType::eDouble:
      cerr << "double constant " << str << endl;
        Add<PrintDoubleConst, VarType, std::string>(type, str);
        break;
    }
  }
  else {
    switch (type) {
      case VarType::eNone:
      case VarType::eString:
        break;
      case VarType::eInt16:
        Add<PrintInt16Var, VarType, std::string>(type, str);
        break;
      case VarType::eInt32:
        Add<PrintInt32Var, VarType, std::string>(type, str);
        break;
      case VarType::eSingle:
        Add<PrintSingleVar, VarType, std::string>(type, str);
        break;
      case VarType::eDouble:
        Add<PrintDoubleVar, VarType, std::string>(type, str);
        break;
    }
  }

  return 0;
}

int CodeGenerator::Print(const AST::StringExpr & expr)
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

int CodeGenerator::Print(const AST::Int16Constant & expr)
{
  VarType type = expr.GetType();
  std::string str = expr.AsString();

  Add<PrintInt16Const, VarType, std::string>(type, str);

  return 0;
}

int CodeGenerator::Print(const AST::Int32Constant & expr)
{
  VarType type = expr.GetType();
  std::string str = expr.AsString();

  Add<PrintInt32Const, VarType, std::string>(type, str);

  return 0;
}

int CodeGenerator::Print(const AST::SingleConstant & expr)
{
  VarType type = expr.GetType();
  std::string str = expr.AsString();

  Add<PrintSingleConst, VarType, std::string>(type, str);

  return 0;
}

int CodeGenerator::Print(const AST::DoubleConstant & expr)
{
  VarType type = expr.GetType();
  std::string str = expr.AsString();

  Add<PrintDoubleConst, VarType, std::string>(type, str);

  return 0;
}

///////////////////////////////////////////////////////////////////////
//
//  Expressions
//

int CodeGenerator::Generate(const AST::AssignStatement & expr)
{
  m_currentStatementLine = expr.m_lineNumber;
  return expr.m_expr->Generate(*this);
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
    Add<UnaryOperator>("=", VarType::eString, var.m_originalName, rhs);
  }

  return 0;
}

///////////////////////////////////////////////////////////////////////
//
//  Numeric expressions  
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

int CodeGenerator::Generate(const AST::NumericAssign & expr)
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
    Add<UnaryOperator>("=", expr.GetType(), var.m_originalName, rhs);
  }

  return 0;
}

int CodeGenerator::EvaluateUnaryNumericOperator(
    const std::string & op, 
    const AST::UnaryNumericOperation & expr, 
    std::string & result)
{
  if (expr.m_arg == nullptr)
    return -1;

  std::string val;
  expr.m_arg->Evaluate(*this, val);

  result = GetTempName();
  Add<CreateTempVar, VarType, std::string>(expr.GetType(), result);
  Add<UnaryOperator>(op, expr.m_arg->GetType(), result, val);

  return 0;
}

template <typename T>
T FoldValue(const T x, const T y, const std::string & op, unsigned lineNumber)
{      
  if (op == "+")
    return x + y;
  else if (op == "-")
    return x - y;
  else if (op == "*") 
    return x * y;
  else if (op == "/") 
    return x / y;
  else if (op == "==") 
    return (x == y) ? -1 : 0;
  else if (op == "!=") 
    return (x != y) ? -1 : 0;
  else if (op == ">") 
    return (x > y) ? -1 : 0;
  else if (op == ">=") 
    return (x >= y) ? -1 : 0;
  else if (op == "<") 
    return (x < y) ? -1 : 0;
  else if (op == "<=") 
    return (x <= y) ? -1 : 0;
  else   
    CompilerError(eError_UnknownInternalType, lineNumber, "Unknown internal type"); // TopOutput() << "/* optimised out */\n";
  return 0;  
}

int CodeGenerator::EvaluateBinaryNumericOperator(
   const std::string & op,
   const AST::NumericBinaryOperation & expr, 
   std::string & result)
{
  if ((expr.m_lhs == nullptr) || (expr.m_rhs == nullptr))
    return -1;

  std::string lhs;
  expr.m_lhs->Evaluate(*this, lhs);

  std::string rhs;
  expr.m_rhs->Evaluate(*this, rhs);

  // constant folding
  if (expr.m_lhs->IsConstant() && expr.m_rhs->IsConstant()) {
    VarType toType = (VarType)std::max((int)expr.m_lhs->GetType(), (int)expr.m_rhs->GetType());
    std::stringstream strm;
    switch (toType) {
      case VarType::eNone:
      case VarType::eString:
        break;
      case VarType::eInt16:
        strm << FoldValue<int16_t>(std::stoi(lhs), std::stoi(rhs), op, m_lineNumber);
        break;
      case VarType::eInt32:
        strm << FoldValue<int32_t>(std::stoi(lhs), std::stoi(rhs), op, m_lineNumber);
        break;
      case VarType::eSingle:
        strm << FoldValue<float>(std::stof(lhs), std::stof(rhs), op, m_lineNumber);
        break;
      case VarType::eDouble:
        strm << FoldValue<double>(std::stod(lhs), std::stod(rhs), op, m_lineNumber);
        break;
    }
    result = strm.str();
  }
  else {
    result = GetTempName();
    Add<CreateTempVar, VarType, std::string>(expr.m_lhs->GetType(), result);
    Add<BinaryOperator>(op, expr.m_lhs->GetType(), result, lhs, rhs);
  }

  return 0;
}

int CodeGenerator::EvaluateNumericComparisonOperator(
   const std::string & op,
   const AST::NumericComparisonOperation & expr, 
   std::string & result)
{
  if ((expr.m_lhs == nullptr) || (expr.m_rhs == nullptr)) {
    cout << "lhs or rhs is null" << endl;
    return -1;
  }
 
  std::string lhs;
  expr.m_lhs->Evaluate(*this, lhs);

  std::string rhs;
  expr.m_rhs->Evaluate(*this, rhs);

  // constant folding
  if (expr.m_lhs->IsConstant() && expr.m_rhs->IsConstant()) {
    VarType toType = (VarType)std::max((int)expr.m_lhs->GetType(), (int)expr.m_rhs->GetType());
    std::stringstream strm;
    switch (toType) {
      case VarType::eNone:
      case VarType::eString:
        break;
      case VarType::eInt16:
        strm << FoldValue<int16_t>(std::stoi(lhs), std::stoi(rhs), op, m_lineNumber);
        break;
      case VarType::eInt32:
        strm << FoldValue<int32_t>(std::stoi(lhs), std::stoi(rhs), op, m_lineNumber);
        break;
      case VarType::eSingle:
      try { 
        strm << FoldValue<float>(std::stof(lhs), std::stof(rhs), op, m_lineNumber);
      }
      catch (...) {
        cerr << "codegen failed to convert " << lhs << " " << rhs << endl;
      }
        break;
      case VarType::eDouble:
        strm << FoldValue<double>(std::stod(lhs), std::stod(rhs), op, m_lineNumber);
        break;
    }
    result = strm.str();
  }
  else {
    result = GetTempName();
    Add<CreateTempVar, VarType, std::string>(VarType::eInt16, result);
    Add<BinaryOperator>(op, VarType::eInt16, result, lhs, rhs);
  }

  return 0;
}


int CodeGenerator::Evaluate(const AST::NumericEquality & expr, std::string & result)
{
  return EvaluateNumericComparisonOperator("==", expr, result);
}

int CodeGenerator::Evaluate(const AST::NumericNotEquality & expr, std::string & result)
{
  return EvaluateNumericComparisonOperator("!=", expr, result);
}

int CodeGenerator::Evaluate(const AST::NumericGreaterThan & expr, std::string & result)
{
  return EvaluateNumericComparisonOperator(">", expr, result);
}

int CodeGenerator::Evaluate(const AST::NumericGreaterThanEqual & expr, std::string & result)
{
  return EvaluateNumericComparisonOperator(">=", expr, result);
}

int CodeGenerator::Evaluate(const AST::NumericLessThan & expr, std::string & result)
{
  return EvaluateNumericComparisonOperator("<", expr, result);
}

int CodeGenerator::Evaluate(const AST::NumericLessThanEqual & expr, std::string & result)
{
  return EvaluateNumericComparisonOperator("<=", expr, result);
}

int CodeGenerator::Evaluate(const AST::NumericAddition & expr, std::string & result)
{
  return EvaluateBinaryNumericOperator("+", expr, result);
}

int CodeGenerator::Evaluate(const AST::Subtraction & expr, std::string & result)
{
  return EvaluateBinaryNumericOperator("-", expr, result);
}

int CodeGenerator::Evaluate(const AST::Multiplication & expr, std::string & result)
{
  return EvaluateBinaryNumericOperator("*", expr, result);
}

int CodeGenerator::Evaluate(const AST::Division & expr, std::string & result)
{
  return EvaluateBinaryNumericOperator("-", expr, result);
}

///////////////////////////////////////////////////////////////////////
//
//  Functions  
//

int CodeGenerator::EvaluateUnaryStringOperator(const std::string & func, const AST::NumericExpr & arg, std::string & result)
{
  result = GetTempName();
  Add<CreateTempVar, VarType, std::string>(VarType::eString, result);

  if (arg.IsVarRef()) {
    std::string argStr;
    arg.Evaluate(*this, argStr);
    Add<UnaryOperator>(func, VarType::eString, result, argStr);
  }
  else if (arg.IsConstant()) {
    std::string argStr;
    arg.Evaluate(*this, argStr);
    Add<UnaryOperator>(func, VarType::eString, result, argStr);
  }
  else {
    std::string argStr;
    arg.Evaluate(*this, argStr);
    Add<UnaryOperator>(func, VarType::eString, result, argStr);
  }

  return 0;
}

int CodeGenerator::Evaluate(const AST::IntFunction & expr, std::string & result)
{
  return EvaluateUnaryNumericOperator("INT", expr, result);
}

int CodeGenerator::Evaluate(const AST::SqrFunction & expr, std::string & result)
{
  return EvaluateUnaryNumericOperator("SQRT", expr, result);
}

int CodeGenerator::Evaluate(const AST::RndFunction & expr, std::string & result)
{
  return EvaluateUnaryNumericOperator("RND", expr, result);
}

int CodeGenerator::Evaluate(const AST::AbsFunction & expr, std::string & result)
{
  return EvaluateUnaryNumericOperator("ABS", expr, result);
}

int CodeGenerator::Evaluate(const AST::NumericCast & expr, std::string & result)
{
  std::string val;
  expr.m_from->Evaluate(*this, val);

  VarType toType   = expr.GetType();
  result = GetTempName();
  Add<CreateTempVar, VarType, std::string>(toType, result);
  Add<BinaryOperator>("CAST", toType, result, AST::GetVarTypeInfo(toType).m_name, val);
  return 0;
}

int CodeGenerator::Evaluate(const AST::LenFunction & expr, std::string & result)
{
  return EvaluateUnaryNumericOperator("LEN", expr, result);
}

int CodeGenerator::Evaluate(const AST::TabFunction & expr, std::string & result)
{
  return EvaluateUnaryStringOperator("TAB", *expr.GetArg1(), result);
}

int CodeGenerator::Evaluate(const AST::ChrFunction & expr, std::string & result)
{
  return EvaluateUnaryStringOperator("CHR", *expr.GetArg1(), result);
}

int CodeGenerator::Generate(const AST::IfStatement & expr)
{
  if (expr.m_cond == nullptr)
    return 0;
    
  std::string cond;
  expr.m_cond->Evaluate(*this, cond);
  std::string condVar = GetTempName();
  Add<CreateTempVar, VarType, std::string>(VarType::eInt16, condVar);
  Add<UnaryOperator>("=", VarType::eInt16, condVar, cond);  

  std::string trueTarget;
  std::string falseTarget;
  stringstream strm; strm << expr.m_lineNumber;
  std::string endifTarget = std::string("endif_") + strm.str();

  std::string trueGoto = expr.m_trueStatements->m_lineNumber;
  Block trueBlock;
  bool haveTrueBody = false;
  if (!trueGoto.empty())
    trueTarget = std::string("line_") + trueGoto;
  else if (expr.m_trueStatements->m_statements != nullptr) {
    trueTarget = std::string("if_") + strm.str();
    Block saveBlock = PushBlock();
    expr.m_trueStatements->m_statements->Generate(*this);
    trueBlock = PopBlock(saveBlock);
    haveTrueBody = true;
  }

  std::string falseGoto = expr.m_falseStatements->m_lineNumber;
  bool havefalseBody = false;
  if (!falseGoto.empty())
    falseTarget = std::string("line_") + falseGoto;
  else if (expr.m_falseStatements->m_statements != nullptr) {
    falseTarget = std::string("else_") + strm.str();
    havefalseBody = true;
  }

  Add<If>(condVar, trueTarget, falseTarget);

  if (haveTrueBody) {
    Add<GotoTarget>(trueTarget);
    Add(trueBlock);
  }

  bool addEndif = false;
  if (havefalseBody) {
    if (haveTrueBody) {
      Add<Goto>(endifTarget);
      addEndif = true;
    }
    Add<GotoTarget>(falseTarget);
    expr.m_falseStatements->m_statements->Generate(*this);
  }

  if (addEndif)
    Add<GotoTarget>(endifTarget);

  return 0;
}


///////////////////////////////////////////////////////

int CodeGenerator::Node::Generate(OutputGenerator & gen)
{
  return gen.Generate(*this);
}

int CodeGenerator::GotoTarget::Generate(OutputGenerator & gen)
{
  return gen.Generate(*this);
}

int CodeGenerator::BlockStart::Generate(OutputGenerator & gen)
{
  return gen.Generate(*this);
}

int CodeGenerator::BlockEnd::Generate(OutputGenerator & gen)
{
  return gen.Generate(*this);
}

int CodeGenerator::CreateTempVar::Generate(OutputGenerator & gen)
{
  return gen.Generate(*this);
}

int CodeGenerator::PrintNewLine::Generate(OutputGenerator & gen)
{
  return gen.Generate(*this);
}

int CodeGenerator::PrintTab::Generate(OutputGenerator & gen)
{
  return gen.Generate(*this);
}

int CodeGenerator::PrintStringConst::Generate(OutputGenerator & gen)
{
  return gen.Generate(*this);
}

int CodeGenerator::PrintStringVar::Generate(OutputGenerator & gen)
{
  return gen.Generate(*this);
}

int CodeGenerator::PrintInt16Var::Generate(OutputGenerator & gen)
{
  return gen.Generate(*this);
}

int CodeGenerator::PrintInt32Var::Generate(OutputGenerator & gen)
{
  return gen.Generate(*this);
}

int CodeGenerator::PrintSingleVar::Generate(OutputGenerator & gen)
{
  return gen.Generate(*this);
}

int CodeGenerator::PrintDoubleVar::Generate(OutputGenerator & gen)
{
  return gen.Generate(*this);
}

int CodeGenerator::PrintInt16Const::Generate(OutputGenerator & gen)
{
  return gen.Generate(*this);
}

int CodeGenerator::PrintInt32Const::Generate(OutputGenerator & gen)
{
  return gen.Generate(*this);
}

int CodeGenerator::PrintSingleConst::Generate(OutputGenerator & gen)
{
  return gen.Generate(*this);
}

int CodeGenerator::PrintDoubleConst::Generate(OutputGenerator & gen)
{
  return gen.Generate(*this);
}

int CodeGenerator::UnaryOperator::Generate(OutputGenerator & gen)
{
  return gen.Generate(*this);
}

int CodeGenerator::BinaryOperator::Generate(OutputGenerator & gen)
{
  return gen.Generate(*this);
}

int CodeGenerator::Goto::Generate(OutputGenerator & gen)
{
  return gen.Generate(*this);
}

int CodeGenerator::Gosub::Generate(OutputGenerator & gen)
{
  return gen.Generate(*this);
}

int CodeGenerator::Return::Generate(OutputGenerator & gen)
{
  return gen.Generate(*this);
}

int CodeGenerator::End::Generate(OutputGenerator & gen)
{
  return gen.Generate(*this);
}

int CodeGenerator::System::Generate(OutputGenerator & gen)
{
  return gen.Generate(*this);
}

int CodeGenerator::If::Generate(OutputGenerator & gen)
{
  return gen.Generate(*this);
}

///////////////////////////////////////////////////////

OutputGenerator::OutputGenerator(const Config & config, CodeGenerator & codeGenerator)
  : m_config(config)
  , m_codeGenerator(codeGenerator)
{
}

const OutputGenerator::Config & OutputGenerator::GetConfig() const
{
  return m_config;
}

bool OutputGenerator::Run(const std::string &inputFilename, std::ostream *outputStream)
{
  m_inputFilename = inputFilename;
  m_outputStream  = new std::stringstream();

  m_indent = m_config.m_indent;

  // traverse here
  for (auto & code : m_codeGenerator.m_code) {
    code->Generate(*this);
  }

  m_indent = m_config.m_indent;
  OutputFilePrologue(*outputStream);

  *outputStream << m_outputStream->str();

  OutputFileEpilogue(*outputStream );

  return true;
}

void OutputGenerator::OutputFilePrologue(ostream & strm)
{}

void OutputGenerator::OutputFileEpilogue(ostream & strm)
{}

bool OutputGenerator::IsPrintUsed() const
{
  return m_codeGenerator.m_printUsed;
}


