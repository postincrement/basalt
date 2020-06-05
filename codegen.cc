#include <iostream>
#include <typeinfo>

using namespace std;

#include "basalt.h"
#include "codegen.h"
#include "c_codegen.h"


struct CTypeInfoRec {
  const char * m_ctype; 
  const char * m_initializer;
  const char * m_printFn;
  const char * m_suffix;
};

// must be indexed by VarType
static CTypeInfoRec g_varTypeInfo[] = {
  { 0 },                        // none
  { "int16_t",      "0", "basalt_print_int16",  "int16"  },     // eInt16
  { "int32_t",      "0", "basalt_print_int32",  "int32"  },     // eInt32
  { "float",        "0", "basalt_print_single", "single" },    // eSingle
  { "double",       "0", "basalt_print_double", "double" },    // eDouble
  { "const char *", "0", "basalt_print_string", "string" }     // eString
};

CodeGenerator::CodeGenerator(const std::string & inputFilename, 
                             const std::string & outputFilename,
                                  const AST::Program & program)
  : m_outputFilename(outputFilename)
  , m_inputFilename(inputFilename)
  , m_program(program)
{
}

bool CodeGenerator::Run()
{
  // check stuff
  CheckVars();

  // generate output
  if (m_outputFilename == "-") {
    m_outputStream = &std::cout;
  }
  else {
    m_outputFile.open(m_outputFilename, std::ofstream::out | std::ofstream::trunc);
    if (!m_outputFile.is_open()) {
      cerr << "error: cannot create output file '" << m_outputFilename << "'" << endl;
      return false;
    }
    m_outputStream = &m_outputFile;
  }
  Body();
  return true;
}

bool CodeGenerator::CheckVars()
{
  for (auto & r : AST::g_globalVars) {
    AST::VarInfo & info = r.second;
    if ((info.m_lhsLine != 0) && (info.m_rhsLine == 0)) {
      SourceWarning(eWarning_VarDefinedButNotUsed, info.m_lhsLine, r.first);
    }
  }

  return true;
}


////////////////////////////////////////////////////////////

C_CodeGenerator::C_CodeGenerator (const std::string & inputFilename, 
                                  const std::string & outputFilename,
                                      const AST::Program & program)
  : CodeGenerator(inputFilename, outputFilename, program)
{
}

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

  PushClosure();
  for (auto & r : m_program.m_list) {
    r->Generate(*this);
  }

  cout << "program has " << m_program.m_list.size() << " elements" << endl;

  *m_outputStream
           << "/* Generator by basalt */\n"
           << "#include <stdlib.h>\n"
           << "#include <stdint.h>\n"
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

  *m_outputStream 
           << "int main(int argc, char * argv[])\n"
           << "{\n"
           << "  basalt_init();\n" 
           << Top().Output().str()
           << "  exit(0);\n"
           << "}\n"
           ;
}

int C_CodeGenerator::Generate(const AST::Node & expr)
{
  const std::type_info & r = typeid(expr);
  cerr << "warning: unimplemented Generate for " << r.name() << "\n";
}

int C_CodeGenerator::Print(const AST::Node & expr)
{
  const std::type_info & r = typeid(expr);
  cerr << "warning: unimplemented Print for " << r.name() << "\n";
}

int C_CodeGenerator::Generate(const AST::SourceLine & line)
{
  TopOutput()  
        << "\n"
        << "#line " << line.GetSourceLineNumber() 
        << " \"" << m_inputFilename << "\"\n"
        << "  /* " << line.GetLine() << " */\n";
  if (line.m_statements)       
    line.m_statements->Generate(*this);      
  return 0;
}


int C_CodeGenerator::Generate(const AST::Statement & statement)
{
  Closure & us = Top();
  for (auto & r : statement.m_list) {
    PushClosure();
    r->Generate(*this);
    us.Output() << TopOutput().str();
    PopClosure();
  }
  return 0;
}

////////////////////////////////////////////////////////////////

int C_CodeGenerator::Generate(const AST::Print & expr)
{
  Closure & us = Top();
  for (auto & r : expr.m_list) {
    PushClosure();
    r->Print(*this);
    us.Output() << TopOutput().str();
    PopClosure();
  }
  us.Output() << "  basalt_print_newline();\n";
  return 0;
}

////////////////////////////////////////////////////////////////

int C_CodeGenerator::Generate(const AST::StringConstant & expr)
{
  TopOutput() << "strdup(\"" << expr.GetValue() << "\")";
  return 0;
}

int C_CodeGenerator::Print(const AST::StringConstant & expr)
{
  TopOutput() << "  basalt_print_string(\"" << expr.GetValue() << "\");\n";
  return 0;
}

////////////////////////////////////////////////////////////////

int C_CodeGenerator::Generate(const AST::Int16Constant & expr)
{
  TopOutput() << expr.GetValue();
  return 0;
}

int C_CodeGenerator::Print(const AST::Int16Constant & expr)
{
  TopOutput() << "  basalt_print_int16(" << expr.GetValue() << ");\n";
  return 0;
}

////////////////////////////////////////////////////////////////

int C_CodeGenerator::Generate(const AST::Int32Constant & expr)
{
  TopOutput() << expr.GetValue();
  return 0;
}

int C_CodeGenerator::Print(const AST::Int32Constant & expr)
{
  TopOutput() << "  basalt_print_int32(" << expr.GetValue() << ");\n";
  return 0;
}

////////////////////////////////////////////////////////////////

int C_CodeGenerator::Generate(const AST::SingleConstant & expr)
{
  TopOutput() << expr.GetValue();
  return 0;
}

int C_CodeGenerator::Print(const AST::SingleConstant & expr)
{
  TopOutput() << "  basalt_print_single(" << expr.GetValue() << ");\n";
  return 0;
}

////////////////////////////////////////////////////////////////

int C_CodeGenerator::Generate(const AST::DoubleConstant & expr)
{
  TopOutput() << expr.GetValue();
  return 0;
}

int C_CodeGenerator::Print(const AST::DoubleConstant & expr)
{
  TopOutput() << "  basalt_print_double(" << expr.GetValue() << ");\n";
  return 0;
}

////////////////////////////////////////////////////////////////

int C_CodeGenerator::Print(const AST::PrintComma & expr)
{
  TopOutput() << "  basalt_print_tab();\n";
  return 0;
}

int C_CodeGenerator::Print(const AST::PrintSemiColon & expr)
{
  return 0;
}

////////////////////////////////////////////////////////////////

bool C_CodeGenerator::LookupGlobalVar(const AST::VarRef & var, 
                                C_CodeGenerator::CVarDef & cvar)
{
  std::string name = var.GetName();
  if (m_globalVars.count(name) == 0)
    return false;

  cvar = m_globalVars[name];
  return true;
}

////////////////////////////////////////////////////////////////

int C_CodeGenerator::Generate(const AST::Assign & expr)
{
  Closure & us = Top();

  C_CodeGenerator::CVarDef cvar;
  if (!LookupGlobalVar(*expr.m_lhs, cvar))
    return -1;

  if (expr.m_rhs == nullptr) {
    us.Output() << "#warning \"missing rhs\"\n";
  }
  else {
    PushClosure();
    expr.m_rhs->Generate(*this);
    us.Output() << "  " << cvar.m_cname << " = " << TopOutput().str() << ";" << endl;
    PopClosure();
  }

  return 0;
}

////////////////////////////////////////////////////////////////

int C_CodeGenerator::BinaryOperator(const std::string & op, const AST::BinaryOperation & expr)
{
  Closure & us = Top();

  if (expr.m_lhs == nullptr) {
    us.Output() << "#warning \"missing lhs\"\n";
  }
  else if (expr.m_rhs == nullptr) {
    us.Output() << "#warning \"missing rhs\"\n";
  }
  else {
    VarType ltype = expr.m_lhs->GetType() ;
    VarType rtype = expr.m_rhs->GetType() ;
    VarType etype = VarType::eNone;

    if (ltype != rtype) {
      if (ltype == VarType::eString) {
        cerr << "error: rhs must be string type" << endl;
      }
      else if (rtype == VarType::eString) {
        cerr << "error: lhs must be string type" << endl;
      }
      else {
        if (rtype == VarType::eDouble || ltype == VarType::eDouble)
          etype = VarType::eDouble;
        else if (rtype == VarType::eSingle || ltype == VarType::eSingle)
          etype = VarType::eSingle;
        else 
          etype = g_languageProfile->GetIntegerType();
      }
    }

    PushClosure();
    expr.m_lhs->Generate(*this);
    std::string lhs = TopOutput().str();
    PopClosure();
    PushClosure();
    expr.m_rhs->Generate(*this);
    std::string rhs = TopOutput().str();
    PopClosure();
    us.Output() << "(" << lhs << " " << op << " " << rhs << ")";
  }

  return 0;
}

int C_CodeGenerator::Generate(const AST::Addition & expr)
{
  return BinaryOperator("+", expr);
}

int C_CodeGenerator::Generate(const AST::Subtraction & expr)
{
  return BinaryOperator("-", expr);
}

int C_CodeGenerator::Generate(const AST::Multiplication & expr)
{
  return BinaryOperator("*", expr);
}

int C_CodeGenerator::Generate(const AST::Division & expr)
{
  return BinaryOperator("/", expr);
}

////////////////////////////////////////////////////////////////

int C_CodeGenerator::UnaryOperator(const std::string & op, const AST::UnaryOperation & expr)
{
  Closure & us = Top();

  if (expr.m_expr == nullptr) {
    us.Output() << "#warning \"missing lhs\"\n";
  }
  else {
    PushClosure();
    expr.m_expr->Generate(*this);
    std::string code = TopOutput().str();
    PopClosure();
    us.Output() << "(" << op << " " << code << ")";
  }

  return 0;
}

int C_CodeGenerator::Generate(const AST::Negation & expr)
{
  return UnaryOperator("-", expr);
}

int C_CodeGenerator::Generate(const AST::Power & expr)
{
  return UnaryOperator("^", expr);
}

////////////////////////////////////////////////////////////////

int C_CodeGenerator::Generate(const AST::VarRef & expr)
{
  if (m_globalVars.count(expr.GetName()) == 0) {
    cerr << "error: unknown variable \"" << expr.GetName() << "\"" << endl;
    return -1;
  }
  CVarDef & cvar = m_globalVars[expr.GetName()];
  TopOutput() << cvar.m_cname;  
  return 0;
}

int C_CodeGenerator::Print(const AST::VarRef & expr)
{
  if (m_globalVars.count(expr.GetName()) == 0) {
    cerr << "error: unknown variable \"" << expr.GetName() << "\"" << endl;
    return -1;
  }
  CVarDef & cvar = m_globalVars[expr.GetName()];
  std::string funcName = g_varTypeInfo[(int)expr.GetType()].m_printFn;
  TopOutput() << "  " << funcName << "(" << cvar.m_cname << ");\n";  
  return 0;
}
