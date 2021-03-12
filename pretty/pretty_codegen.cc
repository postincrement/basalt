#include "../src/basalt.h"

#include "pretty_codegen.h"
#include <iostream>
#include <iomanip>
#include <typeinfo>

using namespace std;


////////////////////////////////////////////////////////////

Pretty_CodeGenerator::Pretty_CodeGenerator (const AST::Parser & parser)
  : CodeGenerator(
    {
      ".txt", "temp", "", "", 4, 0
    }, parser)
{
}

bool Pretty_CodeGenerator::Body()
{
  for (auto & line : m_parser.m_program.m_list) {
    if (line == nullptr)
      continue;
    if (m_parser.m_jumpDestinationInfo.count(line->GetBasicLineNumber()) > 0)  
      m_output << line->GetBasicLineNumber() << endl;
    for (auto & statement : line->m_statements->m_list) {
      SetCurrentLine(statement->m_lineNumber);
      m_output << "#  " << statement->m_text << endl;
      statement->Generate(*this);
    }
  }

  *m_outputStream << m_output.str();

  return true;           
}

///////////////////////////////////////////////////////////////////

std::string Pretty_CodeGenerator::CreateTempVar(VarType type)
{
  return "temp";
}

///////////////////////////////////////////////////////////////////

int Pretty_CodeGenerator::PrintTab()
{
  m_output << "print_tab" << endl;
  return 0;
}

int Pretty_CodeGenerator::PrintNewLine()
{
  m_output << "print_newline" << endl;
  return 0;
}

///////////////////////////////////////////////////////////////

int Pretty_CodeGenerator::PrintNumericConst(const std::string & value, VarType type)
{
  m_output << "print \"" << value << "\"" << endl;
  return 0;
}

int Pretty_CodeGenerator::PrintNumericVar(const std::string & name, VarType type)
{
  m_output << "print_var " <<  AST::GetVarTypeInfo(type).m_name << " " << name << endl;
  return 0;
}

std::string Pretty_CodeGenerator::NumericFunction(
    const std::string & op, 
    const std::string & arg,
    VarType type)
{ 
  std::stringstream strm;
  strm << "numeric_function " << op << "(" << arg << ")";
  return strm.str(); 
}

int Pretty_CodeGenerator::NumericAssign(
       const std::string & lhs, 
       const std::string & rhs, 
       VarType type)
{
  m_output << lhs << " = " << rhs << endl;
  return 0;
}

///////////////////////////////////////////////////////////////

int Pretty_CodeGenerator::PrintStringConst(const std::string & str)
{
  m_output << "print \"" << str << "\"" << endl;
  return 0;
}

int Pretty_CodeGenerator::PrintStringVar(const std::string & name)
{
  m_output << "print_var string " << name << endl;
  return 0;
}

int Pretty_CodeGenerator::StringAssign(
        const std::string & lhs, 
        const std::string & rhs)
{
  m_output << lhs << " = " << rhs << endl;
  return 0;
}

std::string Pretty_CodeGenerator::StringFunction(
        const std::string & op, 
        const std::string & arg) 
{
  std::stringstream strm;
  strm << "string_function " << op << "(" << arg << ")";
  return strm.str(); 
}


#if 0
int Z80_CodeGenerator::Generate(const AST::SourceLine & line)
{
  TopOutput(false) << "; " << line.GetLine() << "\n";
  return CodeGenerator::Generate(line);
}

int Z80_CodeGenerator::Generate(const AST::End & expr)
{
  TopOutput() << "    jp    end\n";
  return 0;
}

int Z80_CodeGenerator::Generate(const AST::Rem & expr)
{
  return 0;
}

///////////////////////////////////////////////////////////////////

void Z80_CodeGenerator::LoadRegPair(const std::string & regPair, const std::string & val)
{
  if (val.empty()) {
    m_regs.erase(regPair);
    m_regs.erase(std::string(regPair[0], 1));
    m_regs.erase(std::string(regPair[1], 1));
  }
  //if (m_regs[regPair] != val) {
    m_regs[regPair] = val;
    m_regs.erase(std::string(regPair[0], 1));
    m_regs.erase(std::string(regPair[1], 1));
    TopOutput() << "ld    " << regPair << "," << val << "\n";
  //}
}

void Z80_CodeGenerator::LoadReg(char reg, const std::string & val)
{
  std::string regPair;
  switch (reg) {
    case 'b':
    case 'c':
      regPair = "bc";
      break;
    case 'd':
    case 'e':
      regPair = "de";
      break;
    case 'h':
    case 'l':
      regPair = "hl";
      break;
  }
  std::string regStr(reg, 1);
  if (val.empty()) {
    m_regs.erase(regPair);
    m_regs.erase(regStr);
  }
  //if (m_regs[regStr] != val) {
    m_regs.erase(regPair);
    m_regs[regStr] = val;
    TopOutput() << "ld    " << reg << "," << val << "\n";
  //}
}

int Z80_CodeGenerator::Print(const AST::StringConstant & expr)
{
  int index = AST::g_stringConstants[expr.GetValue()];

  STRM_STR_DECL(hl, "str_" << index);
  LoadHL(hl);

  int len = expr.GetValue().length();
  STRM_STR_DECL(lenStr, len);

  if (len < 0x100) {
    LoadC(lenStr);
    TopOutput() << "call  " PrintStrc_FUNC "\n"
                     ;
  }
  else {
    LoadBC(lenStr);
    TopOutput() << "call  " PrintStr_FUNC "\n"
                     ;
  }

  m_funcsUsed.insert(PrintStr_FUNC);                

  return 0;
}

///////////////////////////////////////////////////////////////////

int Z80_CodeGenerator::Print(const AST::Int16Constant & expr)
{
  STRM_STR_DECL(val, expr.GetValue());
  LoadHL(val);
  TopOutput() << "call  " PrintI16s_FUNC "\n"
                   ;

  m_funcsUsed.insert(PrintI16s_FUNC);

  return 0;
}

///////////////////////////////////////////////////////////////////

int Z80_CodeGenerator::Generate(const AST::Print & printExpr)
{
  AST::ExprList & expr = *printExpr.m_list;
  for (auto & r : expr.m_list) {
    r->Print(*this);
  }

  if ((expr.m_list.size() > 0) && !expr.m_list[expr.m_list.size()-1]->IsPrintSemiColon()) {
    TopOutput() << "call  " << PrintNewline_FUNC << "\n";
    m_funcsUsed.insert(PrintNewline_FUNC);
  }
  
  return 0;
}

int Z80_CodeGenerator::Print(const AST::NumericExpr & expr) 
{
  std::string str;    
  expr.Evaluate(*this, str);

  std::string funcName = g_varTypeInfo[(int)expr.GetType()].m_printFn;
  m_funcsUsed.insert(funcName);
  TopOutput() << "call  " << funcName << "\n"
                   ;  
  return 0;  
}

int Z80_CodeGenerator::Print(const AST::NumericVarRef & expr)
{
  if (m_globalVars.count(expr.GetName()) == 0) {
    cerr << "error: unknown variable \"" << expr.GetName() << "\"" << endl;
    return -1;
  }
  AsmVarDef & avar = m_globalVars[expr.GetName()];

  std::string funcName = g_varTypeInfo[(int)expr.GetType()].m_printFn;
  m_funcsUsed.insert(funcName);
  STRM_STR_DECL(val, "(" << avar.m_aname << ")");
  LoadHL(val);
  TopOutput() << "call  " << funcName << "\n"
                   ;  
  return 0;  
}

int Z80_CodeGenerator::Print(const AST::PrintComma & expr)
{
  TopOutput() << "call  print_tab\n";
  m_funcsUsed.insert(PrintTab_FUNC);
  return 0;
}

///////////////////////////////////////////////////////////////////

int Z80_CodeGenerator::Generate(const AST::GotoStatement & expr)
{
  int line = ResolveGotoDestination(expr.GetRef());
  if (line < 0) {
    cerr << "internal error: cannot resolve goto destination '" << expr.GetRef() << "'" << endl;
    return 1;
  }

  TopOutput() << "jp    line_" << line << "\n"; 
  return 0;
}

///////////////////////////////////////////////////////////////////

void Z80_CodeGenerator::AssignExprToRegPair(const std::string & regPair, const AST::Expr & expr)
{  
  std::string exprVal;
  expr.Evaluate(*this, exprVal);
  switch (expr.GetType()) {
    case VarType::eInt16:
      if (expr.IsConstant()) {
        LoadRegPair(regPair, exprVal);
      }
      else if (expr.IsVarRef()) {
        STRM_STR_DECL(val, "(" << exprVal << ")");
        LoadHL(val);
        if (regPair != "hl") {
          TopOutput() << "ex  hl," << regPair << "\n"; 
        }
      }
      break;
    default:
      cerr << "error: numeric type not supported for assign" << endl;
      exit(-1);  
  }
}

int Z80_CodeGenerator::Generate(const AST::AssignStatement & expr)
{
  m_currentStatementLine = expr.m_lineNumber;
  return expr.m_expr->Generate(*this);
}

int Z80_CodeGenerator::Generate(const AST::NumericAssign & expr)
{
  Closure & us = Top();

  AsmVarDef avar;
  if (!LookupGlobalVar(expr.m_lhs->GetName(), avar))
    return -1;

  if (expr.m_rhs == nullptr) {
    InternalError("expression missing rhs");
    return -1;
  }

  AssignExprToHL(*expr.m_rhs);
  us.Output() << "    ld    (" << avar.m_aname << "),hl\n";

  return 0;
}

///////////////////////////////////////////////////////////////////

bool Z80_CodeGenerator::LookupGlobalVar(const std::string & varName, 
                                       AsmVarDef & avar)
{
  if (m_globalVars.count(varName) == 0)
    return false;

  avar = m_globalVars[varName];
  return true;
}

int Z80_CodeGenerator::Evaluate(const AST::NumericVarRef & expr, std::string & result)
{
  if (m_globalVars.count(expr.GetName()) == 0) {
    CompilerError(Error_UndeclaredVariable, 0, "unknown variable \"" << expr.GetName() << "\"");
    return -1;
  }
  AsmVarDef & avar = m_globalVars[expr.GetName()];
  result = avar.m_aname;  
  return 0;
}

////////////////////////////////////////////////////////////////

int Z80_CodeGenerator::EvaluateBinaryOperands(const AST::NumericBinaryOperation & expr, bool commutative)
{
  if ((expr.m_lhs == nullptr) || (expr.m_rhs == nullptr))
    return -1;

  if (expr.m_rhs->IsConstant()) {
    AssignExprToHL(*expr.m_lhs);
    AssignExprToRegPair("de", *expr.m_rhs);
  }
  else if (expr.m_lhs->IsConstant()) {
    AssignExprToHL(*expr.m_rhs);
    AssignExprToRegPair("de", *expr.m_lhs);
    TopOutput() << "ex    de,hl\n";
  }
  else {
    AssignExprToHL(*expr.m_rhs);
    TopOutput() << "push  hl\n";
    AssignExprToHL(*expr.m_lhs);
    TopOutput() << "pop   de\n";
  }

  return 0;
}

int Z80_CodeGenerator::Evaluate(const AST::NumericAddition & expr, std::string & result)
{
  EvaluateBinaryOperands(expr, true);

  switch (expr.GetType()) {
    case VarType::eInt16:
      TopOutput() << "add   hl,de\n";
      break;
    default:
      cerr << "error: numeric type not supported for binary op +" << endl;
      exit(-1);  
  }

  return 0;
}

int Z80_CodeGenerator::Evaluate(const AST::Subtraction & expr, std::string & result)
{
  EvaluateBinaryOperands(expr, false);

  switch (expr.GetType()) {
    case VarType::eInt16:
      TopOutput() << "or    a\n";
      TopOutput() << "sbc   hl,de\n";
      break;
    default:
      cerr << "error: numeric type not supported for binary op -" << endl;
      exit(-1);  
  }

  return 0;
}


int Z80_CodeGenerator::NumericComparisonOperator(const AST::NumericBinaryOperation & expr, const std::string & funcBase)
{
  EvaluateBinaryOperands(expr, false);

  switch (expr.GetType()) {
    case VarType::eInt16:
      {
        std::string typeFuncName = funcBase + "_i16";
        m_funcsUsed.insert(typeFuncName);
        TopOutput() << "call  " << typeFuncName << "\n";
      }
      break;
    default:
      cerr << "error: numeric type not supported for binary op " << funcBase << endl;
      exit(-1);  
  }

  return 0;
}

int Z80_CodeGenerator::Evaluate(const AST::NumericEquality & expr, std::string & result)
{
  return NumericComparisonOperator(expr, "equal");
}

int Z80_CodeGenerator::Evaluate(const AST::NumericNotEquality & expr, std::string & result)
{
  return NumericComparisonOperator(expr, "nequal");
}

int Z80_CodeGenerator::Evaluate(const AST::NumericGreaterThan & expr, std::string & result)
{
  return NumericComparisonOperator(expr, "gt");
}

int Z80_CodeGenerator::Evaluate(const AST::NumericGreaterThanEqual & expr, std::string & result)
{
  return NumericComparisonOperator(expr, "gte");
}

int Z80_CodeGenerator::Evaluate(const AST::NumericLessThan & expr, std::string & result)
{
  return NumericComparisonOperator(expr, "lt");
}

int Z80_CodeGenerator::Evaluate(const AST::NumericLessThanEqual & expr, std::string & result)
{
  return NumericComparisonOperator(expr, "lte");
}

////////////////////////////////////////////////////////////////

int Z80_CodeGenerator::Generate(const AST::IfStatement & expr)
{
  if (expr.m_cond == nullptr)
    return -1;
  if (expr.m_trueStatements == nullptr)
    return -1;

  AssignExprToHL(*expr.m_cond);    
  TopOutput(false) << "    ld    a,h\n"
                   << "    or    l\n";

  std::string temp2;
  std::string gotoLine = expr.m_trueStatements->m_lineNumber;
  if (!gotoLine.empty()) {
    int line = ResolveGotoDestination(gotoLine);
    if (line < 0) {
      cerr << "internal error: cannot resolve goto destination '" << gotoLine << "'" << endl;
      return 1;
    }
    TopOutput() << "jp    nz,line_" << line << "\n";
  }
  else {
    std::string temp1 = GetGlobalTempName();  
    temp2 = GetGlobalTempName();  
    TopOutput() << "jp    z," << temp1 << "  ; branch if false\n";
    Push();
    expr.m_trueStatements->m_statements->Generate(*this);
    Pop();
    TopOutput() << "jp    " << temp2 << "\n";
    TopOutput(false) << temp1 << ":\n";
  }
  if (expr.m_falseStatements) {
    Push();
    expr.m_falseStatements->Generate(*this);
    Pop();
  }
  if (gotoLine.empty()) {
    TopOutput(false) << temp2 << ":\n";
  }
  return 0;
}

////////////////////////////////////////////////////////////////

int Z80_CodeGenerator::Generate(const AST::ForStatement & expr)
{
  // get index variable
  AsmVarDef avar;
  if (!LookupGlobalVar(expr.m_var->GetName(), avar))
    return -1;

  // set initial value of index
  AssignExprToHL(*expr.m_fromVal);
  TopOutput(true) << "ld    (" << avar.m_aname << "),hl\n";

  std::string forRef = GetGlobalTempName();

  // create FOR queue entry
  ForBlock forBlock;
  forBlock.m_ref   = forRef; 
  forBlock.m_type  = expr.m_var->GetType();
  forBlock.m_index = avar.m_aname; 

  // get STEP value
  forBlock.m_step = expr.m_stepVal;

  // get TO value
  forBlock.m_to = expr.m_toVal;

  m_forQueue.push_back(forBlock);

  // output symbol for start of FOR block
  TopOutput(false) << forRef << ":\n";

  return 0;
}

int Z80_CodeGenerator::Generate(const AST::NextStatement & expr)
{
  // ensure we are in a FOR loop
  if (m_forQueue.size() == 0) {
    cerr << "Mismatched next\n";
    exit(-1);
  }

  // get information for topmost FOR
  ForBlock forBlock = m_forQueue.back();
  m_forQueue.pop_back();

  // increment index
  LoadHL("(" + forBlock.m_index + ")");
  TopOutput() << "push  hl\n";
  if (forBlock.m_step == nullptr) {
    TopOutput() << "inc   hl\n";
  }
  else {
    AssignExprToRegPair("de", *forBlock.m_step);
    TopOutput() << "add   hl,de\n";
  }
  TopOutput() << "ld    (" + forBlock.m_index + "),hl\n";

  // compare index to TO, and increment and jump if not yet reached
  TopOutput() << "pop   hl\n";
  AssignExprToRegPair("de", *forBlock.m_to);
  TopOutput() << "or    a\n";
  TopOutput() << "sbc   hl,de\n";
  TopOutput() << "jp    c," << forBlock.m_ref << "\n";
  TopOutput(false) << "\n";

  /*
  AssignETopOutput() << "    nc," << forBlock.m_ref << "\n";


  TopOutput() << "if (" << forBlock.m_index << " <= " << forBlock.m_to << ")\n";
  Push();
  TopOutput() << forBlock.m_index << " += " << forBlock.m_step << ";\n";
  TopOutput() << "nextBlock.m_func = &" BLOCKFN_PREFIX << forBlock.m_ref << ";\n";
  TopOutput() << "return nextBlock;\n";
  Pop();

  // get the index variable and compare to the 
*/

  return 0;
}

#endif