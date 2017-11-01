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

////////////////////////////////////////////////////////

static std::string DeclareVariable(AST::VariableDefExpr & expr)
{
  std::stringstream strm;

  cout << "declare global " << expr.m_variable.m_name << endl;
  Variable & var = expr.m_variable;

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

  if (expr.m_dim != 0) {
    initExpr = "";
    type += " *"; 
  }
  strm << type << " " << var.m_normalizedName << initExpr << ";" << endl;
  if (expr.m_dim != 0) {
    strm << "int " << var.m_normalizedName << "_dim[" << expr.m_dim << "];\n";
  }

  return strm.str();
}

////////////////////////////////////////////////////////

static std::string DeclareFunction(const FunctionDef & func)
{
  std::stringstream strm;
  if (func.m_returnType.empty())
    strm << "void";
  else  
    strm << func.m_returnType;
  strm << " " << func.m_name << "(";
  bool first = true;
  for (auto & r : func.m_args) {
    if (!first)
      strm << ", ";
    first = false;
    strm << r;
  }
  strm << ")";
  return strm.str();  
}

//////////////////////////////////////////////////////////////////////////

CodegenCXX::CodegenCXX(const std::string & genType, const std::string & fn, SourceFileExprList & tree)
  : CodeGenerator(genType, fn, tree)
{  
}

bool CodegenCXX::Open(int argc, const char ** argv)
{
  if (!CodeGenerator::Open())
    return false;

  return true;
} 

void CodegenCXX::Output(ostream & strm)
{
  if (m_prefixStream.str().length() > 0)
    strm << m_prefixStream.str() << "\n";

  if (m_constStringStrm.str().length() > 0)
    strm << m_constStringStrm.str() << "\n";

  if (m_externFuncStrm.str().length() > 0)
    strm << m_externFuncStrm.str() << "\n";

  if (m_globalVarsStrm.str().length() > 0)  
    strm << m_globalVarsStrm.str() << "\n";

  strm << m_codeStrm.str();
}

bool CodegenCXX::Close(const std::string & outputFilename)
{
  // declare file prefix header
  m_prefixStream << "#include <inttypes.h>\n"
                 << "#include <stdlib.h>\n"
                 << "#include <stdbool.h>\n"
                 << "#include <string.h>\n"
                 ;

  // declare const strings
  for (auto & r : m_constStringMap)
    m_constStringStrm << "const char * " << r.second << " = \"" << r.first << "\";\n";   

  // declare external functions
  if (m_externFunctionMap.size() > 0) {    
    if (m_genType != "c") 
      m_externFuncStrm << "extern \"C\" {\n";
  
    for (auto & r : m_externFunctionMap)
      m_externFuncStrm << "extern " << DeclareFunction(r.second) << ";\n";

    if (m_genType != "c") 
      m_externFuncStrm << "} // extern \"C\"\n\n";
  }

  // declare global vars
  for (auto & r : m_globalScope->m_vars)
    m_globalVarsStrm << DeclareVariable(*r.second) << "\n";

  // create output filename
  m_srcFilename = Filename(m_inputFilename.GetDir() + m_inputFilename.GetBasename() + "." + m_genType);

  // create output file
  if (g_dump)
    Output(cout);

  m_outputFile.open(m_srcFilename.c_str());

  if (!m_outputFile.good()) {
    cerr << "error: cannot create output file '" << m_srcFilename << "'" << endl;
    return false;
  }
  
  cout << "info: outputting to '" << m_srcFilename << "'" << endl;

  Output(m_outputFile);

  m_outputFile.close();

  return true;
} 

///////////////////////////////////////////////////////////

static std::string CTypeForVarType(Variable::Type type)
{
  switch (type) {
    case Variable::eString:
      return "char *";
    case Variable::eInt16:
      return "uin16_t";
    case Variable::eSingle:
      return "float";
    case Variable::eDouble:
      return "double";
    default:
      InternalError("Cannot create temp for variable type " << (int)type);  
  }
  return "";
}

std::string CreateGotoTarget(const std::string & marker)
{
  std::stringstream strm;
  strm << "line_" << marker;
  return strm.str();
}

bool CodegenCXX::Visit(Expr & expr)
{
  return CXXError(expr);
}

bool CodegenCXX::Visit(SourceFileExprList & expr)
{
  m_codeStrm << "int main(int argc, char *argv[])\n"
             "{\n"
             ;

  if (!Run(expr))
    return false;           

  m_codeStrm << "}\n"
          ; 

  return true;
}

bool CodegenCXX::Visit(ExprList & expr)
{
  return CXXError(typeid(expr).name());
}

bool CodegenCXX::Visit(VariableRefExpr & expr)
{
  ValueDef def(expr.m_variable.m_normalizedName, expr.m_variable.m_type);
  m_currentScope->m_valueStack.push_back(def);
  return true;
}

bool CodegenCXX::Visit(UnaryExpr & expr)
{
  return CXXError(expr);
}

///////////////////////////////////////////////////////////////////////

bool CodegenCXX::Visit(ConstantExpr<short int> & expr)
{
  std::stringstream strm;
  strm << expr.m_value;
  ValueDef val(strm.str(), Variable::Type::eInt16);
  m_currentScope->m_valueStack.push_back(val);
  return true;
}

bool CodegenCXX::Visit(ConstantExpr<float> & expr)
{
  std::stringstream strm;
  strm << expr.m_value;
  if (strm.str().find('.') == std::string::npos)
    strm << ".";
  strm << "f";

  ValueDef val(strm.str(), Variable::Type::eSingle);
  m_currentScope->m_valueStack.push_back(val);
  return true;
}

bool CodegenCXX::Visit(ConstantExpr<double> & expr)
{
  std::stringstream strm;
  strm << expr.m_value;
  if (strm.str().find('.') == std::string::npos)
    strm << ".";
  strm << "f";
    
  ValueDef val(strm.str(), Variable::Type::eDouble);
  m_currentScope->m_valueStack.push_back(val);
  return true;
}

///////////////////////////////////////////////////////////////////////

bool CodegenCXX::Visit(CallExpr & expr)
{
  return CXXError(expr);
}

bool CodegenCXX::Visit(GotoExpr & expr)
{
  m_codeStrm << "goto " << CreateGotoTarget(expr.m_marker) << ";\n";
  return true;
}

///////////////////////////////////////////////////////////////////////

void CodegenCXX::AssignString(const std::string & lhs, const std::string & rhs)
{
  std::string indent = "    ";
  m_codeStrm << indent << "if (" << lhs << " != 0L)\n"
           << indent << "  free(" << lhs << ");\n"
           << indent << "if (" << rhs << " != 0L)\n"
           ;
           
  if (m_currentScope->m_cleanupList.count(rhs) != 0) {
    m_codeStrm << indent << "  " << lhs << " = " << rhs << ";"
             << "\n";
             m_currentScope->m_cleanupList.erase(rhs);         
  }
  else {
    m_codeStrm << indent << "  " << lhs << " = strdup(" << rhs << ");"
          << "\n";
  }
  m_codeStrm << indent << "else\n"
           << indent << "  " << lhs << " = 0L;\n"
           ;
}

void CodegenCXX::AssignVar(const std::string & lhs, const std::string & rhs)
{
  m_codeStrm << lhs << " = " << rhs << ";\n";
}

void CodegenCXX::JoinStrings(const std::string & lhs, const std::string & rhs)
{
  std::string tempName(GetTempName("temp"));
  std::string indent;
  m_codeStrm << indent << "char * " << tempName << " = 0L;\n"
           << indent << "{\n"
           << indent << "  bool l = " << lhs << " != 0L;\n"
           << indent << "  bool r = " << rhs << " != 0L;\n"
           << indent << "  if (l & r) {\n"
           << indent << "    " << tempName << " = malloc(1 + strlen(" << lhs << ") + strlen(" << rhs << "));\n"
           << indent << "    strcpy(" << tempName << ", " << lhs << ");\n"   
           << indent << "    strcat(" << tempName << ", " << rhs << ");\n"
           << indent << "  }\n"
           << indent << "  else if (l) \n"
           << indent << "    " << tempName << " = strdup(" << lhs << ");\n"
           << indent << "  else if (r)\n"
           << indent << "    " << tempName << " = strdup(" << rhs << ");\n"
           << indent << "};\n"
           ;
  ValueDef def(tempName, Variable::eString);
  m_currentScope->m_valueStack.push_back(def);
  m_currentScope->m_cleanupList.insert(tempName);
}

void CodegenCXX::BinaryOp(const ValueDef & result, const ValueDef & lhs, char op, const ValueDef & rhs)
{
  m_codeStrm << CTypeForVarType(lhs.m_type) 
             << " " << result.m_name
             << " = " << lhs.m_name 
             << " " << op 
             << " " << rhs.m_name 
             << ";\n"; 
}

bool CodegenCXX::CallFunction(const std::string & returnTypeStr, const std::string & name, const std::vector<std::string> & args)
{
  std::stringstream strm;
  strm << name << "(";
  bool first = true;
  for (auto & r : args) {
    if (!first) {
      strm << ", ";
    }     
    first = false;
    strm << r;
  }
  strm << ");\n";
  m_codeStrm << strm.str();
  return true;
}
