#include <typeinfo>

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

AST::Visitor::Visitor(AST::SourceFileExprList & tree)
  : m_tree(tree)
{
}

bool AST::Visitor::Visit(Expr & expr)
{
  return VisitError(expr);
}

bool AST::Visitor::Visit(SourceFileExprList & expr)
{
  for (auto & r : expr) {
    if (r != nullptr)
      r->Generate(*this);
  }
  return true;
}


bool AST::Visitor::Visit(ExprList & expr)
{
  for (auto & r : expr) {
    if (r != nullptr)
      r->Generate(*this);
  }
  return true;
}

bool AST::Visitor::Visit(LineMarkerExpr & expr)
{
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

bool AST::Visitor::Visit(CallExpr & expr)
{
  return VisitError(expr);
}

bool AST::Visitor::Visit(ConstantIntExpr<short int> & expr)
{
  return VisitError(expr);
}

