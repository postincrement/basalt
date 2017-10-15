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

///////////////////////////////////////////////////////////

CodeGenerator::CodeGenerator(SourceFileExprList & tree)
  : Visitor(tree)
{ }

std::string CodeGenerator::GetTempName(const std::string & prefix)
{
  std::stringstream strm;
  strm << prefix << "_" << m_tempCounter++;
  return strm.str();
}

CodeGenerator::FindConstStrings::FindConstStrings(CodeGenerator & gen)
  : m_gen(gen)
{ }

bool CodeGenerator::FindConstStrings::operator()(AST::Expr & expr)
{
  ConstantStringExpr * constDef = dynamic_cast<ConstantStringExpr *>(&expr);
  if (constDef != nullptr) {
    if (m_gen.m_constStrings.count(constDef->m_value) == 0) {
      std::string tempName(m_gen.GetTempName("const_string"));
      m_gen.m_constStrings[constDef->m_value] = tempName;
    }
  }
  return true;
}


