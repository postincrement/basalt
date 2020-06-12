#include "../basalt.h"

#include "codegen.h"
#include <iostream>
#include <typeinfo>
#include <cxxabi.h>

using namespace std;

#define STRING_CTYPE    "char *"
#define TEMP_PREFIX     "temp_"
#define LINEFN_PREFIX   "Line_"
#define ENDLINE_FN      "Line_end"

struct CTypeInfoRec {
  const char * m_ctype; 
  const char * m_initializer;
  const char * m_printFn;
  const char * m_strFn;
  const char * m_suffix;
};

// must be indexed by VarType
static CTypeInfoRec g_varTypeInfo[] = {
  { 0 },                        // none
  { "int16_t",    "0", "basalt_print_int16",  "basalt_str_int16",  "int16"  },     // eInt16
  { "int32_t",    "0", "basalt_print_int32",  "basalt_str_int32",  "int32"  },     // eInt32
  { "float",      "0", "basalt_print_single", "basalt_str_single", "single" },    // eSingle
  { "double",     "0", "basalt_print_double", "basalt_str_double", "double" },    // eDouble
  { STRING_CTYPE, "0", "basalt_print_string", 0,                   "string" }     // eString
};


////////////////////////////////////////////////////////////

C_CodeGenerator::C_CodeGenerator ()
  : CodeGenerator(TEMP_PREFIX)
{
}

////////////////////////////////////////////////////////////////

std::string C_CodeGenerator::GetOutputFileExtension() const
{
  return ".c";
}

////////////////////////////////////////////////////////////////

static std::string QuoteLiteral(const std::string & str_)
{
  std::string str("\"");
  str += str_;
  str += "\"";
  return str;
}

////////////////////////////////////////////////////////////////

static bool CreateCVar(const std::string & name,
                       const AST::VarInfo & var, 
                       C_CodeGenerator::CVarDef & cvar,
                       int tag)
{
  stringstream strm;

  strm << "USER_";
  for (auto r : name) {
    if (isalnum(r))
      strm << r;
  }
  if (tag != 0)
    strm << "_" << tag;
  strm << "_";  
  strm << g_varTypeInfo[(int)var.m_type].m_suffix;
  cvar.m_cname = strm.str();
  cvar.m_type  = var.m_type;

  return true;
} 

bool C_CodeGenerator::Body()
{
  for (auto & r : AST::g_globalVars) {
    AST::VarInfo & info = r.second;
    int tag = 0;
    CVarDef cvar;
    for (;;) {
      if (!CreateCVar(r.first, info, cvar, tag)) {
        cerr << "internal error: cannot map var to C type" << endl;
        return false;
      }
      if (m_cnames.count(cvar.m_cname) == 0)
        break;
      ++tag;  
    }
    m_globalVars[r.first] = cvar;
    m_cnames.insert(cvar.m_cname);
  }

  *m_outputStream
           << "/* C generator by basalt */\n"
           << "#include <stdlib.h>\n"
           << "#include <stdint.h>\n"
           << "#include <math.h>\n"
           << "#include <string.h>\n"
           << "\n"
           << "extern int basalt_init();\n"
           << "extern int basalt_print_tab();\n"
           << "extern int basalt_print_newline();\n"
           << "extern int basalt_print_string(const char *);\n"
           << "extern int basalt_print_int16(int);\n"
           << "extern int basalt_print_int32(int);\n"
           << "extern int basalt_print_single(float);\n"
           << "extern int basalt_print_double(double);\n"
           << "extern int basalt_strlen(const char *);\n"
           << "extern char * basalt_strdup(const char *);\n"
           << "extern char * basalt_str_single(float);\n"
           << "extern char * basalt_str_double(double);\n"
           << "extern char * basalt_str_int16(int16_t);\n"
           << "extern char * basalt_str_int32(int32_t);\n"
           << "\n"
           ;

  *m_outputStream << "/* Vars */\n";  
  for (auto & r : m_globalVars) {
    CTypeInfoRec & info = g_varTypeInfo[(int)r.second.m_type];
    *m_outputStream << info.m_ctype
                    << " "
                    << r.second.m_cname
                    << " = "
                    << info.m_initializer
                    << "; /* " << r.first << " */\n"
                    ;
  }
  *m_outputStream << "\n";

  *m_outputStream << "struct LineFunction {\n"
                  << "  struct LineFunction (* m_func)();\n"
                  << "};\n"
                  ;

  for (size_t i = 0; i < m_program->m_list.size(); ++i) {
    auto & r = m_program->m_list[i];
    *m_outputStream << "struct LineFunction " LINEFN_PREFIX << i << "(); /* " << r->GetBasicLineNumber() << " */\n";
  }

  for (size_t i = 0; i < m_program->m_list.size(); ++i) {
    auto & r = m_program->m_list[i];

    *m_outputStream << "struct LineFunction " LINEFN_PREFIX << i << "() /* " << r->GetBasicLineNumber() << " */\n"
                    << "{\n"
                    << "  struct LineFunction nextLine;\n"
                    << "  nextLine.m_func = ";
    if (i == m_program->m_list.size()-1) {
      *m_outputStream << "0";
    }
    else {
      *m_outputStream << "&" << LINEFN_PREFIX << (i+1);
    } 
    *m_outputStream << ";\n"
                    ;
                    
    Push();
    r->Generate(*this);
    *m_outputStream << Pop()
                    << "  return nextLine;\n"
                    << "}\n"
                    << "\n"
                   ;
  }

  *m_outputStream 
           << "int main(int argc, char * argv[])\n"
           << "{\n"
           << "  basalt_init();\n"
           << "  struct LineFunction line;\n"
           << "  line.m_func = &" LINEFN_PREFIX << 0 << ";\n"
           << "  while (line.m_func != 0) {\n" 
           << "    line = (*line.m_func)();\n"
           << "  }\n"
           << "  exit(0);\n"
           << "}\n"
           ;
}

int C_CodeGenerator::Generate(const AST::SourceLine & line)
{
  if (g_enableLineNumbers) {
    TopOutput(false)  
          << "\n"
          << "#line " << line.GetSourceLineNumber() 
          << " \"" << m_inputFilename << "\"\n";
  }
  TopOutput(false) << "  /* " << line.GetLine() << " */\n";
  //if (AST::g_lineNumberInfo.count(line.GetBasicLineNumber()) > 0) {
  //  TopOutput() << "  line_" << line.GetBasicLineNumber() << ":\n";
  //}

  return CodeGenerator::Generate(line);
}

int C_CodeGenerator::Generate(const AST::End & expr)
{
  TopOutput() << "nextLine.m_func = 0;\n";
  return 0;
}

////////////////////////////////////////////////////////////////

int C_CodeGenerator::Generate(const AST::Print & expr)
{
  for (auto & r : expr.m_list) {
    r->Print(*this);
  }
  TopOutput() << "basalt_print_newline();\n";
  return 0;
}

////////////////////////////////////////////////////////////////

int C_CodeGenerator::Generate(const AST::Goto & expr)
{
  //TopOutput() << "  goto line_" << expr.GetRef() << ";\n";  
  return 0;
}

////////////////////////////////////////////////////////////////

void C_CodeGenerator::CatStrings(const std::string & tempName,
                                 const std::string & lhs, 
                                 const std::string & rhs, 
                                 const std::string & pre,
                                 bool indent)
{
  TopOutput(indent) << pre << tempName << " = (char *)malloc(strlen(" << lhs << ") + strlen(" << rhs << ") + 1);\n";
  TopOutput(indent) << "strcpy(" << tempName << ", " << lhs << ");\n";
  TopOutput(indent) << "strcat(" << tempName << ", " << rhs << ");\n";  
}

int C_CodeGenerator::Generate(const AST::StringAssign & expr)
{
  C_CodeGenerator::CVarDef cvar;
  if (!LookupGlobalVar(expr.m_lhs->GetName(), cvar))
    return -1;

  if (expr.m_rhs == nullptr) {
    if (!g_disableWarnings)
      TopOutput() << "#warning \"missing rhs\"\n";
    return -1;
  }

  std::string rhs;

  expr.m_rhs->Evaluate(*this, rhs);

  if (expr.m_lhs->IsVarRef() && 
      expr.m_rhs->IsVarRef() && 
      (cvar.m_cname == rhs)) {
    TopOutput() << "/* optimised out */\n";
  }
  else {
    Push();

    TopOutput() << "if (" << cvar.m_cname << ")\n";
    TopOutput() << "  free(" << cvar.m_cname << ");\n";

    if (expr.m_rhs->IsConstant() || expr.m_rhs->IsVarRef()) {
      TopOutput() << cvar.m_cname << " = strdup(\"" << rhs << "\");\n";
    }
    else {
      TopOutput() << "if (!" << rhs << ")\n";
      TopOutput() << "  " << cvar.m_cname << " = 0;\n";
      TopOutput() << "else\n";
      TopOutput() << "  " << cvar.m_cname << " = " << rhs << ";\n";
    }

    Pop();
  }

  return 0;
}

int C_CodeGenerator::Evaluate(const AST::StringAddition & expr, std::string & result)
{
  std::string lhs, rhs;
  expr.m_lhs->Evaluate(*this, lhs);
  expr.m_rhs->Evaluate(*this, rhs);

  // if either operand is a variable, deal with possibility
  // that one (or both) may be null
  int code = 0;
  if (expr.m_lhs->IsConstant()) {
    code += 0x10;
    lhs = QuoteLiteral(lhs);
  }
  else if (expr.m_lhs->IsVarRef()) {
    code += 0x20;
  }
  else {
    code += 0x30;
  }
  if (expr.m_rhs->IsConstant()) {
    code += 0x01;
    rhs = QuoteLiteral(rhs);
  }
  else if (expr.m_rhs->IsVarRef()) {
    code += 0x02;
  }
  else {
    code += 0x03;
  }

  stringstream strm;
  std::string temp1 = Top().GetTempName();
  result = temp1;

  switch (code) {
    case 0x11:  // both constants
      CatStrings(temp1, lhs, rhs, STRING_CTYPE " ");
      break;
    case 0x12:  // left constant, right var
      TopOutput() << STRING_CTYPE " " << temp1 << ";\n";
      TopOutput() << "if (!" << rhs << ")\n";
      TopOutput() << "  " << temp1 << " = strdup(" << lhs << ");\n";
      TopOutput() << "else\n";
      Push();
      CatStrings(temp1, lhs, rhs, "");
      Pop();
      break;
    case 0x13:  // left constant, right expr
      CatStrings(temp1, lhs, rhs, STRING_CTYPE " ");
      TopOutput() << "free(" << rhs << ");\n";
      break;

    case 0x21:  // left var, right constant
      TopOutput() << STRING_CTYPE " " << temp1 << ";\n";
      TopOutput() << "if (!" << lhs << ")\n";
      TopOutput() << "  " << temp1 << " = strdup(" << rhs << ");\n";
      TopOutput() << "else\n";
      Push();
      CatStrings(temp1, lhs, rhs, "");
      Pop();
      break;
    case 0x22:  // left var, right var
      TopOutput() << STRING_CTYPE " " << temp1 << " = 0;\n";
      TopOutput() << "if (("<< lhs << " | " << rhs << ") != 0)\n";
      Push();
      TopOutput() << "if (!" << lhs << ")\n";
      TopOutput() << "  " << temp1 << " = strdup(" << rhs << ");\n";
      TopOutput() << "else if (!" << rhs << ")";
      TopOutput() << "  " << temp1 << " = strdup(" << lhs << ");\n";
      TopOutput() << "else {\n";
      Push();
      CatStrings(temp1, lhs, rhs, "");
      Pop();
      Pop();
    case 0x23:  // left var, right expr
      TopOutput() << STRING_CTYPE " " << temp1 << ";\n";
      TopOutput() << "if (!" << lhs << ")\n";
      TopOutput() << "  " << temp1 << " = " << rhs << ";\n";
      Push();
      CatStrings(temp1, lhs, rhs, "");
      TopOutput() << "free(" << rhs << ");\n";
      Pop();
      break;

    case 0x31:  // left expression, right constant
      CatStrings(temp1, lhs, rhs, STRING_CTYPE " ");
      TopOutput() << "free(" << lhs << ");\n";
      break;
    case 0x32:  // left expression, right var
      TopOutput() << STRING_CTYPE " " << temp1 << ";\n";
      TopOutput() << "if (!" << rhs << ")\n";
      TopOutput() << "  " << temp1 << " = " << lhs << ";\n";
      Push();
      CatStrings(temp1, lhs, rhs, "");
      TopOutput() << "free(" << lhs << ");\n";
      Pop();
      break;
    case 0x33:  // left expression, right expr
      CatStrings(temp1, lhs, rhs, STRING_CTYPE " ");
      TopOutput() << "free(" << rhs << ");\n";
      TopOutput() << "free(" << lhs << ");\n";
      break;
  }
}

////////////////////////////////////////////////////////////////

int C_CodeGenerator::Generate(const AST::NumericAssign & expr)
{
  Closure & us = Top();

  C_CodeGenerator::CVarDef cvar;
  if (!LookupGlobalVar(expr.m_lhs->GetName(), cvar))
    return -1;

  if (expr.m_rhs == nullptr) {
    if (!g_disableWarnings)
      us.Output() << "#warning \"missing rhs\"\n";
    return -1;
  }

  std::string rhs;
  expr.m_rhs->Evaluate(*this, rhs);
  us.Output() << "  " << cvar.m_cname << " = " << rhs << ";" << endl;

  return 0;
}

////////////////////////////////////////////////////////////////

int C_CodeGenerator::Evaluate(const AST::StringVarRef & expr, std::string & result)
{
  if (m_globalVars.count(expr.GetName()) == 0) {
    cerr << "error: unknown variable \"" << expr.GetName() << "\"" << endl;
    return -1;
  }
  CVarDef & cvar = m_globalVars[expr.GetName()];
  result = cvar.m_cname;  
  return 0;  
}

int C_CodeGenerator::Print(const AST::StringVarRef & expr)
{
  if (m_globalVars.count(expr.GetName()) == 0) {
    cerr << "error: unknown variable \"" << expr.GetName() << "\"" << endl;
    return -1;
  }
  CVarDef & cvar = m_globalVars[expr.GetName()];
  TopOutput(true) << "basalt_print_string(" <<  cvar.m_cname << ");\n";  
  return 0;
}

////////////////////////////////////////////////////////////////

int C_CodeGenerator::Evaluate(const AST::StringConstant & expr, std::string & result)
{
  result = expr.GetValue();
  return 0;
}

int C_CodeGenerator::Print(const AST::StringConstant & expr)
{
  TopOutput(true) << "basalt_print_string(" << QuoteLiteral(expr.GetValue()) << ");\n";
  return 0;
}

////////////////////////////////////////////////////////////////

int C_CodeGenerator::Evaluate(const AST::Int16Constant & expr, std::string & result)
{
  stringstream strm;
  strm << expr.GetValue();
  result = strm.str();
  return 0;
}

int C_CodeGenerator::Print(const AST::Int16Constant & expr)
{
  TopOutput(true) << "basalt_print_int16(" << expr.GetValue() << ");\n";
  return 0;
}

////////////////////////////////////////////////////////////////

int C_CodeGenerator::Evaluate(const AST::Int32Constant & expr, std::string & result)
{
  stringstream strm;
  strm << expr.GetValue();
  result = strm.str();
}

int C_CodeGenerator::Print(const AST::Int32Constant & expr)
{
  TopOutput(true) << "basalt_print_int32(" << expr.GetValue() << ");\n";
  return 0;
}

////////////////////////////////////////////////////////////////

int C_CodeGenerator::Evaluate(const AST::SingleConstant & expr, std::string & result)
{
  stringstream strm;
  strm << expr.GetValue();
  result = strm.str();
}

int C_CodeGenerator::Print(const AST::SingleConstant & expr)
{
  TopOutput(true) << "basalt_print_single(" << expr.GetValue() << ");\n";
  return 0;
}

////////////////////////////////////////////////////////////////

int C_CodeGenerator::Evaluate(const AST::DoubleConstant & expr, std::string & result)
{
  stringstream strm;
  strm << expr.GetValue();
  result = strm.str();
}

int C_CodeGenerator::Print(const AST::DoubleConstant & expr)
{
  TopOutput(true) << "basalt_print_double(" << expr.GetValue() << ");\n";
  return 0;
}

////////////////////////////////////////////////////////////////

int C_CodeGenerator::Print(const AST::PrintComma & expr)
{
  TopOutput(true) << "basalt_print_tab();\n";
  return 0;
}

int C_CodeGenerator::Print(const AST::PrintSemiColon & expr)
{
  return 0;
}

////////////////////////////////////////////////////////////////

bool C_CodeGenerator::LookupGlobalVar(const std::string & varName, 
                                      C_CodeGenerator::CVarDef & cvar)
{
  if (m_globalVars.count(varName) == 0)
    return false;

  cvar = m_globalVars[varName];
  return true;
}

int C_CodeGenerator::Evaluate(const AST::NumericVarRef & expr, std::string & result)
{
  if (m_globalVars.count(expr.GetName()) == 0) {
    cerr << "error: unknown variable \"" << expr.GetName() << "\"" << endl;
    return -1;
  }
  CVarDef & cvar = m_globalVars[expr.GetName()];
  result = cvar.m_cname;  
  return 0;
}

int C_CodeGenerator::Print(const AST::NumericVarRef & expr)
{
  if (m_globalVars.count(expr.GetName()) == 0) {
    cerr << "error: unknown variable \"" << expr.GetName() << "\"" << endl;
    return -1;
  }
  CVarDef & cvar = m_globalVars[expr.GetName()];
  std::string funcName = g_varTypeInfo[(int)expr.GetType()].m_printFn;
  TopOutput(true) << funcName << "(" << cvar.m_cname << ");\n";  
  return 0;
}

////////////////////////////////////////////////////////////////

int C_CodeGenerator::NumericExpr(const std::string & op, const AST::NumericExpr * expr, std::string & result)
{
  if (expr == nullptr)
    return -1;

  std::string str;    
  expr->Evaluate(*this, str);
  stringstream strm;
  strm << op << "(" << str << ")";
  result = strm.str();
  return 0;
}

int C_CodeGenerator::StringExpr(const std::string & op, const AST::StringExpr * expr, std::string & result)
{
  if (expr == nullptr)
    return -1;

  std::string str;    
  expr->Evaluate(*this, str);
  stringstream strm;
  strm << op << "(" << str << ")";
  result = strm.str();
  return 0;
}

int C_CodeGenerator::NumericBinaryOperator(const std::string & op, const AST::NumericBinaryOperation * expr, std::string & result)
{
  if ((expr == nullptr) || (expr->m_lhs == nullptr) || (expr->m_rhs == nullptr))
    return -1;

  std::string temp1 = Top().GetTempName();
  CTypeInfoRec & info = g_varTypeInfo[(int)expr->GetType()];

  std::string lhs, rhs;
  expr->m_lhs->Evaluate(*this, lhs);
  expr->m_rhs->Evaluate(*this, rhs);
  TopOutput() << info.m_ctype << " " << temp1 << " = " << lhs << " " << op << " " << rhs << ";\n";

  result = temp1;

  return 0;
}

int C_CodeGenerator::UnaryOperator(const std::string & op, const AST::UnaryOperation * expr, std::string & result)
{
  if (expr->m_expr == nullptr)
    return -1;

  std::string str;
  expr->m_expr->Evaluate(*this, str);
  stringstream strm;
  strm << "(" << op << " " << str << ")";
  result = strm.str();

  return 0;
}

////////////////////////////////////////////////////////////////

int C_CodeGenerator::Evaluate(const AST::NumericAddition & expr, std::string & result)
{
  return NumericBinaryOperator("+", &expr, result);
}

int C_CodeGenerator::Evaluate(const AST::Subtraction & expr, std::string & result)
{
  return NumericBinaryOperator("-", &expr, result);
}

int C_CodeGenerator::Evaluate(const AST::Multiplication & expr, std::string & result)
{
  return NumericBinaryOperator("*", &expr, result);
}

int C_CodeGenerator::Evaluate(const AST::Division & expr, std::string & result)
{
  return NumericBinaryOperator("/", &expr, result);
}

int C_CodeGenerator::Evaluate(const AST::Negation & expr, std::string & result)
{
  return UnaryOperator("-", &expr, result);
}

int C_CodeGenerator::Evaluate(const AST::Power & expr, std::string & result)
{
  return UnaryOperator("^", &expr, result);
}

////////////////////////////////////////////////////////////////

int C_CodeGenerator::Evaluate(const AST::IntFunction & expr, std::string & result)
{
  return NumericExpr("(int)", expr.GetArg1(), result);
}

int C_CodeGenerator::Evaluate(const AST::SqrFunction & expr, std::string & result)
{
  return NumericExpr("sqrt", expr.GetArg1(), result);
}

int C_CodeGenerator::Evaluate(const AST::TabFunction & expr, std::string & result)
{
  return NumericExpr("basalt_tab", expr.GetArg1(), result);
}

int C_CodeGenerator::Evaluate(const AST::ChrFunction & expr, std::string & result)
{
  return NumericExpr("basalt_chr", expr.GetArg1(), result);
}

int C_CodeGenerator::Evaluate(const AST::StrFunction & expr, std::string & result)
{
  const char * funcName = g_varTypeInfo[(int)expr.GetArg1()->GetType()].m_strFn;
  if (funcName == 0) {
    cerr << "error: cannot find str function for type " << (int)expr.GetArg1()->GetType() << endl;
    return -1;
  }

  std::string str;
  if (NumericExpr(funcName, expr.GetArg1(), str) != 0)
    return -1;
  std::string temp1 = Top().GetTempName();
  TopOutput() << STRING_CTYPE " " << temp1 << " = " << str << ";\n";
  result = temp1;
  return 0;
}

////////////////////////////////////////////////////////////////

int C_CodeGenerator::Evaluate(const AST::LenFunction & expr, std::string & result)
{
  return StringExpr("basalt_strlen", expr.GetArg1(), result);
}


int C_CodeGenerator::Evaluate(const AST::LeftFunction & expr, std::string & result)
{
  if (expr.GetArg1() == nullptr)
    return -1;
    
  result = "basalt_left()";
  return 0;
}

int C_CodeGenerator::Evaluate(const AST::MidFunction & expr, std::string & result)
{
  if (expr.GetArg1() == nullptr)
    return -1;
    
  result = "basalt_mid()";
  return 0;
}

int C_CodeGenerator::Evaluate(const AST::RightFunction & expr, std::string & result)
{
  if (expr.GetArg1() == nullptr)
    return -1;
    
  result = "basalt_right()";
  return 0;
}

////////////////////////////////////////////////////////////////

int C_CodeGenerator::Evaluate(const AST::NumericCast & expr, std::string & result)
{
  std::string str;
  expr.m_from->Evaluate(*this, str);
  stringstream strm;
  strm << "/* cast from " 
       << (int)expr.m_from->GetType()
       << " to "
       << (int)expr.GetType() 
       << " */ ("
       << str 
       << ")";
  result = strm.str();
  return 0;     
}

