using namespace std;

#include "ast.h"

using namespace AST;

////////////////////////////////////////////////////////
//
// dumper visit functions
//

static bool DumpError(const std::string & str)
{
  cout << "error: unimplemented dump function for type " << str << endl;
  return true;
}

static bool DumpError(Expr & expr)
{
  return DumpError(typeid(expr).name());
}

static unsigned tempNumber = 1;

static std::string CreateTemp()
{
  stringstream strm;
  strm << "temp_" << tempNumber++;
  return strm.str();
}

//////////////////////////////////////////////////////////////////////////

CodegenDumper::CodegenDumper(SourceFileExprList & tree)
  : Visitor(tree)
  , m_strm(cout)
{  
}

bool CodegenDumper::Open(const std::string & inputFilename, int argc, const char ** argv)
{
  return true;
} 

bool CodegenDumper::Close()
{
  return true;
} 

bool CodegenDumper::Visit(Expr & expr)
{
  return DumpError(expr);
}

bool CodegenDumper::Visit(SourceFileExprList & expr)
{
  for (auto & r : expr) {
    if (r != nullptr)
      r->Generate(*this);
  }
  return true;
}

bool CodegenDumper::Visit(ExprList & expr)
{
  return DumpError(typeid(expr).name());
}

bool CodegenDumper::Visit(LineMarkerExpr & expr)
{
  m_strm << "; " << expr.m_lineNumber << ": " << expr.m_line << endl;
  return true;
}

bool CodegenDumper::Visit(VariableDefExpr & expr)
{
  m_strm << "var '" << expr.GetName() << "'" << endl; 
  return true;
}

bool CodegenDumper::Visit(VariableRefExpr & expr)
{
  return DumpError(expr);
}

bool CodegenDumper::Visit(UnaryExpr & expr)
{
  return DumpError(expr);
}

bool CodegenDumper::Visit(BinaryExpr & expr)
{
  return DumpError(expr);
}

///////////////////////////////////////////////////////////////////////

bool CodegenDumper::Visit(StringConstantExpr & expr)
{
  std::string temp = CreateTemp();  
  m_strm << temp << " = '" << expr.m_value << "'" << endl;
  m_vars.push_back(temp);
  return true;
}

///////////////////////////////////////////////////////////////////////

bool CodegenDumper::Visit(ConstantIntExpr<short int> & expr)
{
  std::string temp = CreateTemp();  
  m_strm << temp << " = '" << expr.m_intValue << "'" << endl;
  m_vars.push_back(temp);
  return true;
}

///////////////////////////////////////////////////////////////////////

bool CodegenDumper::Visit(BIFExpr & expr)
{
  std::vector<std::string> args;
  if (expr.m_args != nullptr) {
    for (auto & r : *expr.m_args) {
      r->Generate(*this);
      if (m_vars.size() > 0) {
        std::string result = m_vars.back();
        args.push_back(result);
      }
    }
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

bool CodegenDumper::Visit(CallExpr & expr)
{
  return DumpError(expr);
}

///////////////////////////////////////////////////////////////////////