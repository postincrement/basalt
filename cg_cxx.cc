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

CodegenCXX::CodegenCXX(const std::string & genType, SourceFileExprList & tree)
  : CodeGenerator(genType, tree)
{  
}

bool CodegenCXX::Open(const std::string & inputFilename, int argc, const char ** argv)
{
  if (g_dump)
    m_ostrm = &cout;
  else {  
    m_ostrm = &m_outputFile;
    Filename ifn(inputFilename);

    // create output filename
    m_srcFilename = Filename(ifn.GetDir() + ifn.GetBasename() + "." + m_genType);

    // create output file
    m_outputFile.open(m_srcFilename.c_str());
    if (!m_outputFile.good()) {
      cerr << "error: cannot create output file '" << m_srcFilename << "'" << endl;
      return false;
    }
    
    cout << "info: outputting to '" << m_srcFilename << "'" << endl;
  }

  *m_ostrm << "#include <inttypes.h>\n\n";

  RuntimeFunctionDef * rtDef = g_runtimeDefs;
  std::stringstream strm;
  if (m_genType != "c") 
    strm << "extern \"C\" {\n";
  while (rtDef->m_name != nullptr) {
    strm << "extern ";
    if (rtDef->m_returnType == nullptr)
      strm << "void";
    else
      strm << rtDef->m_returnType;
    strm << " basalt_" << rtDef->m_name << "(";
    if (rtDef->m_args != nullptr)
      strm << rtDef->m_args;
    else if (m_genType == "c")
      strm << "void";  
    strm << ");\n";
    rtDef++;
  }
  if (m_genType != "c") 
    strm << "} // extern \"C\"\n\n";

  *m_ostrm << strm.str() << endl;

  return true;
} 

bool CodegenCXX::Close()
{
  m_outputFile.close();
  return true;
} 

///////////////////////////////////////////////////////////

bool CodegenCXX::Visit(LineMarkerExpr & expr)
{
  *m_ostrm << "  // " << expr.m_lineNumber << ": " << expr.m_line << endl;
  return true;
}

bool CodegenCXX::Visit(Expr & expr)
{
  return CXXError(expr);
}

bool CodegenCXX::Visit(SourceFileExprList & expr)
{
  // define global variables
  {
    bool first = true;
    for (auto & r : m_globalVars) {
      Variable & var = r.second->m_variable;

      std::string type;
      std::string initExpr = " = ";

      switch (var.m_type) {
        case Variable::eString:
          type = "char *";
          initExpr += "0L";
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
        default:  
          InternalError("unsupported global variable type " << var.m_type); 
      }

      if (r.second->m_dim != 0) {
        initExpr = "";
        type += " *"; 
      }
      if (first) {
        *m_ostrm << "// global variables" << endl;
        first = false;
      }  
      *m_ostrm << type << " " << var.m_normalizedName << initExpr << ";" << endl;
      if (r.second->m_dim != 0) {
        *m_ostrm << "int " << var.m_normalizedName << "_dim[" << r.second->m_dim << "];\n";
      }
    }
    if (!first)
      *m_ostrm << "\n";
  }

  // define global strings
  {
    bool first = true;
    for (auto & r : m_constStrings) {
      if (first) {
        *m_ostrm << "// constant strings" << endl;
        first = false;
      }  
      *m_ostrm << "const char * " << r.second << " = \"" << r.first << "\";" << endl;
    }
    if (!first)
      *m_ostrm << "\n";
  }
  
  *m_ostrm << "int main(int argc, char *argv[])\n"
             "{\n"
             "  basalt_init();\n"
             ;

  // output code
  for (auto & r : expr) {
    if (r != nullptr)
      r->Generate(*this);
  }

  *m_ostrm << "}\n"
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
  *m_ostrm << "  // reference to " << expr.m_variable.m_name << endl;
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

void CodegenCXX::VCreateFunctionCall(                                      
  const char * returnTypeStr,
  const char * name,
  const char * argsStr_,
  va_list varg)
{
  *m_ostrm << "  " << name << "(";
  if (argsStr_ != nullptr) {    
    std::string argStr(argsStr_);
    std::vector<std::string> tokens;
    Tokenize(tokens, argStr, ',');
    bool first = true;
    for (size_t i = 0; i < tokens.size(); ++i) {
      const char * strVal = va_arg(varg, const char *);
      if (!first) {
        *m_ostrm << ", ";
      }     
      first = false;
      *m_ostrm << strVal;
    }
  }
      
  *m_ostrm << ");\n";
}
  
void CodegenCXX::CreateBIFCall(const std::string & name ...)
{
  va_list argValues;
  va_start(argValues, name);

  std::string bifName(g_runtimeDefPrefix);
  bifName += name;
  
  int i = 0;
  while (g_runtimeDefs[i].m_name != nullptr) {
    RuntimeFunctionDef & funcDef = g_runtimeDefs[i];
    if (name == funcDef.m_name) {
      VCreateFunctionCall(funcDef.m_returnType, bifName.c_str(), funcDef.m_args, argValues);
      return;
    }
    ++i;
  }
  InternalError("unknown BIF '" << name << "'");
}

bool CodegenCXX::Visit(BIFExpr & expr)
{
  if (
      (expr.m_name == "print_eol") || 
      (expr.m_name == "print_tab")
    )
    CreateBIFCall(expr.m_name);

  else if (expr.m_name == "print_expr") {
    if ((expr.m_args == nullptr) || (expr.m_args->size() != 1))
      InternalError("BIF print has invalid args");
    else {      
      AST::Expr * printExpr = (*expr.m_args)[0];

      // string constant
      AST::ConstantStringExpr * constantString = dynamic_cast<AST::ConstantStringExpr *>(printExpr);
      if (constantString != nullptr) {
        auto r = m_constStrings.find(constantString->m_value);
        if (r == m_constStrings.end()) {
          InternalError("cannot find constant string def for '" << constantString->m_value << "'");
        }
        else { 
          CreateBIFCall("print_string", r->second.c_str());
        }
        return true;
      }
      // variable reference
      AST::VariableRefExpr * varRef = dynamic_cast<AST::VariableRefExpr *>(printExpr);
      if (varRef != nullptr) {
        std::string func;
        switch (varRef->m_variable.m_type) {
          case Variable::eString:
            func = "print_string";
            break;          
          case Variable::eInt16:
            func = "print_int16";
            break;          
          case Variable::eSingle:
            func = "print_single";
            break;          
          case Variable::eDouble:
            func = "print_double";
            break;          
          case Variable::eUntyped:
            InternalError("unknown print variable type " << varRef->m_variable.m_type);
            exit(1);
        }
        CreateBIFCall(func, varRef->m_variable.m_normalizedName.c_str());
        return true;
      }

      // unknown
      else {
        InternalError("unknown print expression type " << typeid(printExpr).name());
        return false;
      }
    }  
  } 
  
  else {
    Warning(eWarning_UnknownCXXBIF, "unknown BIF '" << expr.m_name << "'");
  }

  return true;
}

bool CodegenCXX::Visit(CallExpr & expr)
{
  return CXXError(expr);
}

///////////////////////////////////////////////////////////////////////