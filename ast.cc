#include <typeinfo>

using namespace std;

#include "ast.h"

using namespace AST;

AST::LineMarkerExpr * g_currentSourceFileMarker = nullptr; 

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

bool AST::Visitor::Visit(ConstantStringExpr & expr)
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

bool AST::Visitor::Visit(UnaryExpr & expr)
{
  return VisitError(expr);
}

bool AST::Visitor::Visit(BinaryExpr & expr)
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

bool AST::Visitor::Visit(ConstantExpr<short int> & expr)
{
  return VisitError(expr);
}

bool AST::Visitor::Visit(ConstantExpr<float> & expr)
{
  return VisitError(expr);
}

bool AST::Visitor::Visit(ConstantExpr<double> & expr)
{
  return VisitError(expr);
}

bool AST::Visitor::Visit(GotoExpr & expr)
{
  return VisitError(expr);
}

///////////////////////////////////////////////////////////

