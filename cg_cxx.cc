#include <typeinfo>

using namespace std;

#include "ast.h"

using namespace AST;

////////////////////////////////////////////////////////
//
// CXX visit functions
//

static bool CXXError(const std::string & str)
{
  cout << "error: unimplemented C++ function for type " << str << endl;
  return true;
}

static bool CXXError(Expr & expr)
{
  return CXXError(typeid(expr).name());
}

//////////////////////////////////////////////////////////////////////////

CodegenCXX::CodegenCXX(SourceFileExprList & tree)
  : CodeGenerator(tree)
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

  m_ostrm << "#include <stdio.h>\n\n";

  return true;
} 

bool CodegenCXX::Close()
{
  m_ostrm.close();
  return true;
} 

///////////////////////////////////////////////////////////

bool CodegenCXX::Visit(LineMarkerExpr & expr)
{
  m_ostrm << "  // " << expr.m_lineNumber << ": " << expr.m_line << endl;
  return true;
}

bool CodegenCXX::Visit(Expr & expr)
{
  return CXXError(expr);
}

struct FindGlobalVars
{
  FindGlobalVars(CodegenCXX & gen)
    : m_gen(gen)
  { }

  bool operator()(Expr & expr)
  {
    VariableDefExpr * varDef = dynamic_cast<VariableDefExpr *>(&expr);
    if ((varDef != nullptr) && varDef->m_global && (m_vars.count(varDef->m_variable.m_name) == 0))
      m_vars[varDef->m_variable.m_name] = varDef;
    return true;
  }

  std::map<std::string, VariableDefExpr *> m_vars;
  
  CodegenCXX & m_gen;
};

bool CodegenCXX::Visit(SourceFileExprList & expr)
{
  // look for global variable definitions
  {
    FindGlobalVars fn(*this);
    Traverse<FindGlobalVars>(expr, fn);

    bool first = true;
    for (auto & r : fn.m_vars) {
      Variable & var = r.second->m_variable;

      std::string type;
      std::string initExpr = " = ";

      switch (var.m_type) {
        case Variable::eString:
          type = "char *";
          initExpr += "nullptr";
          break;
        case Variable::eInt16:
          type = "int16_t";
          initExpr += "0";
          break;
        case Variable::eSingle:
          type = "float";
          initExpr += "0";
          break;
        case Variable::eDouble:
          type = "double";
          initExpr += "0";
          break;
        case Variable::eUntyped:
          return false;
      }

      if (r.second->m_dim != 0) {
        initExpr = "";
        type += " *"; 
      }
      if (first) {
        m_ostrm << "// global variables" << endl;
        first = false;
      }  
      m_ostrm << type << " " << var.m_normalizedName << initExpr << ";" << endl;
      if (r.second->m_dim != 0) {
        m_ostrm << "int " << var.m_normalizedName << "_dim[" << r.second->m_dim << "];\n";
      }
    }
    if (!first)
      m_ostrm << "\n";
  }

  // look for global string definitions
  {
    FindConstStrings();
    bool first = true;
    for (auto & r : m_constStrings) {
      if (first) {
        m_ostrm << "// constant strings" << endl;
        first = false;
      }  
      m_ostrm << "const char * " << r.second << " = \"" << r.first << "\";" << endl;
    }
    if (!first)
      m_ostrm << "\n";
  }
  
  m_ostrm << "int main(int argc, char *argv[])\n"
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
  return CXXError(typeid(expr).name());
}

bool CodegenCXX::Visit(VariableDefExpr & expr)
{
  // global variables handled elsewhere
  if (expr.m_global)
    return true;

  return CXXError(expr);
}

bool CodegenCXX::Visit(VariableRefExpr & expr)
{
  m_ostrm << "  // reference to " << expr.m_variable.m_name << endl;
  return true;
}

bool CodegenCXX::Visit(BinaryExpr & expr)
{
  if (expr.m_op == '=') {
    if (expr.m_lhs != nullptr) {
      expr.m_lhs->Generate(*this);
    }

    return true;
  }

  return CXXError(expr);
}

bool CodegenCXX::Visit(UnaryExpr & expr)
{
  return CXXError(expr);
}

///////////////////////////////////////////////////////////////////////

bool CodegenCXX::Visit(ConstantStringExpr & expr)
{
  // string constants handled elsewhere
  return true;
}

///////////////////////////////////////////////////////////////////////

bool CodegenCXX::Visit(ConstantExpr<short int> & expr)
{
  return CXXError(expr);
}

bool CodegenCXX::Visit(ConstantExpr<float> & expr)
{
  return CXXError(expr);
}

///////////////////////////////////////////////////////////////////////

bool CodegenCXX::Visit(BIFExpr & expr)
{
  if (expr.m_name == "print_newline")
    m_ostrm << "  printf(\"\\n\");\n";

  else if (expr.m_name == "print_comma")  
    m_ostrm << "  printf(\",\");\n";

  else if (expr.m_name == "print_expr") {
    if ((expr.m_args == nullptr) || (expr.m_args->size() != 1))
      cerr << "internal error: BIF print has invalid args" << endl;
    else {      
      AST::Expr * printExpr = (*expr.m_args)[0];

      // string constant
      AST::ConstantStringExpr * constantString = dynamic_cast<AST::ConstantStringExpr *>(printExpr);
      if (constantString != nullptr) {
        auto r = m_constStrings.find(constantString->m_value);
        if (r == m_constStrings.end()) {
          cerr << "internal error: cannot find constant string def for '" << constantString->m_value << "'" << endl;
        }
        else { 
          m_ostrm << "  printf(\"%s\", " << r->second << ");\n";
        }
        return true;
      }

      // variable reference
      AST::VariableRefExpr * varRef = dynamic_cast<AST::VariableRefExpr *>(printExpr);
      if (varRef != nullptr) {
        std::string type;
        switch (varRef->m_variable.m_type) {
          case Variable::eString:
            type = "s";
            break;          
          case Variable::eInt16:
            type = "i";
            break;          
          case Variable::eSingle:
            type = "f";
            break;          
          case Variable::eDouble:
            type = "lf";
            break;          
          case Variable::eUntyped:
            cerr << "internal error: unknown print variable type " << varRef->m_variable.m_type << endl;
            exit(1);
        }
        m_ostrm << "  printf(\"%" << type << "\", " << varRef->m_variable.m_normalizedName << ");\n";
        return true;
      }

      // unknown
      else {
        cerr << "internal error: unknown print expression type " << typeid(printExpr).name() << endl;
        return false;
      }
    }  
  } 

  else {

  }

  return true;
}

bool CodegenCXX::Visit(CallExpr & expr)
{
  return CXXError(expr);
}

///////////////////////////////////////////////////////////////////////