#include <typeinfo>
#include <assert.h>

using namespace std;

#include "ast.h"

using namespace AST;

static bool VisitError(Expr & expr)
{
  cout << "error: unimplemented visit function for type " << typeid(expr).name() << endl;
  return true;
}

////////////////////////////////////////////////////////
//
// default visit functions
//

bool AST::Visitor::Visit(Expr & expr)
{
  return VisitError(expr);
}

bool AST::Visitor::Visit(ExprList & expr)
{
  for (auto & r : expr) {
    r->Accept(*this);
  }
  return true;
}

bool AST::Visitor::Visit(StringVariableDefExpr & expr)
{ 
  return VisitError(expr);
}

bool AST::Visitor::Visit(IntVariableDefExpr<int16_t> & expr)
{ 
  //return VisitError(expr);
  return true;
}

bool AST::Visitor::Visit(StringVariableRefExpr & expr)
{ 
  return VisitError(expr);
}

bool AST::Visitor::Visit(StringConstantExpr & expr)
{
  return VisitError(expr);
}

bool AST::Visitor::Visit(VariableDefExpr & expr)
{
  return VisitError(expr);
}

bool AST::Visitor::Visit(VariableRefExpr & expr)
{
  return VisitError(expr);
}

bool AST::Visitor::Visit(Int16VariableRefExpr & expr)
{ 
  return VisitError(expr);
}

bool AST::Visitor::Visit(UnaryExpr & expr)
{
  return VisitError(expr);
}

bool AST::Visitor::Visit(BinaryExpr & expr)
{
  return VisitError(expr);
}

bool AST::Visitor::Visit(StringBinaryExpr & expr)
{
  return VisitError(expr);
}

bool AST::Visitor::Visit(Int16BinaryExpr & expr)
{
  return VisitError(expr);
}

bool AST::Visitor::Visit(BIFExpr & expr)
{
  return VisitError(expr);
}

bool AST::Visitor::Visit(PrintCommaExpr & expr)
{
  return VisitError(expr);
}

bool AST::Visitor::Visit(PrintSemiColonExpr & expr)
{
  return VisitError(expr);
}

bool AST::Visitor::Visit(CallExpr & expr)
{
  return VisitError(expr);
}

bool AST::Visitor::Visit(ConstantIntExpr<short int> & expr)
{
  return VisitError(expr);
}

////////////////////////////////////////////////////////
//
// dumper visit functions
//

static bool DumpError(Expr & expr)
{
  cout << "error: unimplemented dump function for type " << typeid(expr).name() << endl;
  return true;
}

static unsigned tempNumber = 1;

static std::string CreateTemp()
{
  stringstream strm;
  strm << "temp_" << tempNumber++;
  return strm.str();
}


AST::Dumper::Dumper(ExprList & tree, std::ostream & strm)
  : m_tree(tree)
  , m_strm(strm)
{  
}

bool AST::Dumper::Visit(Expr & expr)
{
  return DumpError(expr);
}

bool AST::Dumper::Visit(ExprList & expr)
{
  for (auto & r : expr) {
    r->Accept(*this);
  }
  return true;
}

bool AST::Dumper::Visit(VariableDefExpr & expr)
{
  return DumpError(expr);
}

bool AST::Dumper::Visit(VariableRefExpr & expr)
{
  return DumpError(expr);
}

bool AST::Dumper::Visit(UnaryExpr & expr)
{
  return DumpError(expr);
}

bool AST::Dumper::Visit(BinaryExpr & expr)
{
  return DumpError(expr);
}

///////////////////////////////////////////////////////////////////////

bool AST::Dumper::Visit(StringConstantExpr & expr)
{
  std::string temp = CreateTemp();  
  m_strm << temp << " = '" << expr.m_value << "'" << endl;
  m_vars.push_back(temp);
  return true;
}

bool AST::Dumper::Visit(StringVariableDefExpr & expr)
{
  m_strm << "string var '" << expr.m_name << "'" << endl; 
  return true;
}

bool AST::Dumper::Visit(StringVariableRefExpr & expr)
{
  std::string temp = CreateTemp();
  m_strm << temp << " = '" << expr.m_name << "'" << endl; 
  m_vars.push_back(temp);
  return true;
}

bool AST::Dumper::Visit(StringBinaryExpr & expr)
{
  assert(expr.m_lhs != nullptr);
  assert(expr.m_rhs != nullptr);

  if (expr.m_op == '=') {
    StringVariableRefExpr * var = dynamic_cast<StringVariableRefExpr *>(expr.m_lhs);
    assert(var != nullptr);

    expr.m_rhs->Accept(*this);
    assert(m_vars.size() > 0);  
    std::string rhs = m_vars.back();
    m_vars.pop_back();

    m_strm << var->m_name << " = " << rhs << endl;
    m_vars.push_back(var->m_name);
  }
  else {  
    std::string result = CreateTemp();

    expr.m_lhs->Accept(*this);
    assert(m_vars.size() > 0);  
    std::string lhs = m_vars.back();
    m_vars.pop_back();

    expr.m_rhs->Accept(*this);
    assert(m_vars.size() > 0);  
    std::string rhs = m_vars.back();
    m_vars.pop_back();

    m_strm << result << " = " << lhs << " " << expr.m_op << " " << rhs << endl;
    m_vars.push_back(result);
  }

  return true;
}

///////////////////////////////////////////////////////////////////////

bool AST::Dumper::Visit(ConstantIntExpr<short int> & expr)
{
  std::string temp = CreateTemp();  
  m_strm << temp << " = '" << expr.m_intValue << "'" << endl;
  m_vars.push_back(temp);
  return true;
}

bool AST::Dumper::Visit(IntVariableDefExpr<int16_t> & expr)
{ 
  m_strm << "int16 var '" << expr.m_name << "'" << endl; 
  return true;
}

bool AST::Dumper::Visit(Int16VariableRefExpr & expr)
{ 
  std::string temp = CreateTemp();
  m_strm << temp << " = '" << expr.m_name << "'" << endl; 
  m_vars.push_back(temp);
  return true;  
}

bool AST::Dumper::Visit(Int16BinaryExpr & expr)
{
  assert(expr.m_lhs != nullptr);
  assert(expr.m_rhs != nullptr);

  if (expr.m_op == '=') {
    Int16VariableRefExpr * var = dynamic_cast<Int16VariableRefExpr *>(expr.m_lhs);
    assert(var != nullptr);

    expr.m_rhs->Accept(*this);
    assert(m_vars.size() > 0);  
    std::string rhs = m_vars.back();
    m_vars.pop_back();

    m_strm << var->m_name << " = " << rhs << endl;
    m_vars.push_back(var->m_name);
  }
  else {  
    std::string result = CreateTemp();

    expr.m_lhs->Accept(*this);
    assert(m_vars.size() > 0);  
    std::string lhs = m_vars.back();
    m_vars.pop_back();

    expr.m_rhs->Accept(*this);
    assert(m_vars.size() > 0);  
    std::string rhs = m_vars.back();
    m_vars.pop_back();

    m_strm << result << " = " << lhs << " " << expr.m_op << " " << rhs << endl;
    m_vars.push_back(result);
  }

  return true;
}

///////////////////////////////////////////////////////////////////////

bool AST::Dumper::Visit(BIFExpr & expr)
{
  std::vector<std::string> args;
  for (auto & r : *expr.m_args) {
    r->Accept(*this);
    assert(m_vars.size() > 0);  
    std::string result = m_vars.back();
    args.push_back(result);
  }
  m_strm << expr.m_name << "(";
  bool first = true;
  for (auto & r : args) {
    if (!first)
      m_strm << ", ";
    m_strm << r;
    first = false;
  }
  m_strm << ")" << endl;
  return true;
}

bool AST::Dumper::Visit(PrintCommaExpr & expr)
{
  return DumpError(expr);
}

bool AST::Dumper::Visit(PrintSemiColonExpr & expr)
{
  return DumpError(expr);
}

bool AST::Dumper::Visit(CallExpr & expr)
{
  return DumpError(expr);
}

///////////////////////////////////////////////////////////////////////
