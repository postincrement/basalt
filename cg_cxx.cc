#include <typeinfo>

using namespace std;

#include "ast.h"

using namespace AST;

////////////////////////////////////////////////////////
//
// CPP visit functions
//

static bool CXXError(Expr & expr)
{
  cout << "error: unimplemented C++ function for type " << typeid(expr).name() << endl;
  return true;
}

//////////////////////////////////////////////////////////////////////////

CodegenCXX::CodegenCXX(SourceFileExprList & tree)
  : Visitor(tree)
{  
}

bool CodegenCXX::Open(const std::string & inputFilename, int argc, const char ** argv)
{
  Filename ifn(inputFilename);

  // create output filename
  m_srcFilename = Filename(ifn.GetDir() + ifn.GetBasename() + ".cc");

  // create output file
  m_ostrm.open(m_srcFilename.c_str());
  if (!m_ostrm.good()) {
    cerr << "error: cannot create output file '" << m_srcFilename << "'" << endl;
    return false;
  }
  
  cout << "info: outputting to '" << m_srcFilename << "'" << endl;

  m_ostrm << "#include <iostream>\n\n";

  return true;
} 

void CodegenCXX::Close()
{
  m_ostrm.close();
} 

bool CodegenCXX::Visit(LineMarkerExpr & expr)
{
  m_ostrm << "  // " << expr.m_line << endl;
  return true;
}

///////////////////////////////////////////////////////////

bool CodegenCXX::Visit(Expr & expr)
{
  return CXXError(expr);
}

template <class Fn>
bool Traverse(ExprList & list, Fn & fn)
{
  for (auto & r : list) {
    if (r != nullptr) {
      ExprList * subList = dynamic_cast<ExprList *>(r);
      if (subList != nullptr) { 
        if (!Traverse<Fn>(*subList, fn))
          return false;
      }
      else if (!fn(*r))
        return false;
    }
  }

  return true;
}

struct FindVar
{
  FindVar(CodegenCXX & gen)
    : m_gen(gen)
  { }

  bool operator()(Expr & expr)
  {
    VariableDefExpr * varDef = dynamic_cast<VariableDefExpr *>(&expr);
    if (varDef != nullptr) {
      m_vars[varDef->m_variableName.m_name] = varDef;
    }
    return true;
  }

  std::map<std::string, VariableDefExpr *> m_vars;

  CodegenCXX & m_gen;
};


bool CodegenCXX::Visit(SourceFileExprList & expr)
{
  // look for variable definitions
  FindVar fn(*this);
  Traverse<FindVar>(expr, fn);

  for (auto & r : fn.m_vars) {
    AST::Variable & var = r.second->m_variableName;
    std::string type;
    switch (var.m_type) {
      case AST::Variable::eString:
        type = "std::string";
        break;
      case AST::Variable::eInt16:
        type = "int16_t";
        break;
      case AST::Variable::eSingle:
        type = "float";
        break;
      case AST::Variable::eDouble:
        type = "double";
        break;
    }
    m_ostrm << type << " " << r.first << ";" << endl;
  }

  m_ostrm << "\n"
             "int main(int argc, char *argv[])\n"
             "{\n"
             ;

  // output code
  for (auto & r : expr) {
    if (r != nullptr)
      r->Generate(*this);
  }

  m_ostrm << "}\n"
          ; 

  return true;
}

bool CodegenCXX::Visit(ExprList & expr)
{
  for (auto & r : expr) {
    r->Generate(*this);
  }
  return true;
}

bool CodegenCXX::Visit(VariableDefExpr & expr)
{
  return CXXError(expr);
}

bool CodegenCXX::Visit(VariableRefExpr & expr)
{
  return CXXError(expr);
}

bool CodegenCXX::Visit(UnaryExpr & expr)
{
  return CXXError(expr);
}

bool CodegenCXX::Visit(BinaryExpr & expr)
{
  return CXXError(expr);
}

///////////////////////////////////////////////////////////////////////

bool CodegenCXX::Visit(StringConstantExpr & expr)
{
  return CXXError(expr);
}

bool CodegenCXX::Visit(StringVariableRefExpr & expr)
{
  return CXXError(expr);
}

bool CodegenCXX::Visit(StringBinaryExpr & expr)
{
  return CXXError(expr);
}

///////////////////////////////////////////////////////////////////////

bool CodegenCXX::Visit(ConstantIntExpr<short int> & expr)
{
  return CXXError(expr);
}

bool CodegenCXX::Visit(Int16VariableRefExpr & expr)
{ 
  return CXXError(expr);
}

bool CodegenCXX::Visit(Int16BinaryExpr & expr)
{
  return CXXError(expr);
}

///////////////////////////////////////////////////////////////////////

bool CodegenCXX::Visit(BIFExpr & expr)
{
  return CXXError(expr);
}

bool CodegenCXX::Visit(CallExpr & expr)
{
  return CXXError(expr);
}

///////////////////////////////////////////////////////////////////////