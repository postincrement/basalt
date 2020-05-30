#include <iostream>
#include <typeinfo>

using namespace std;

#include "codegen.h"
#include "c_codegen.h"

CodeGenerator::CodeGenerator(const std::string & inputFilename, 
                             const std::string & outputFilename,
                                  AST::NodeList & program)
  : m_outputFilename(outputFilename)
  , m_inputFilename(inputFilename)
  , m_program(program)
{
}

bool CodeGenerator::Run()
{
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

////////////////////////////////////////////////////////////

C_CodeGenerator::C_CodeGenerator (const std::string & inputFilename, 
                                  const std::string & outputFilename,
                                      AST::NodeList & program)
  : CodeGenerator(inputFilename, outputFilename, program)
{
}

bool C_CodeGenerator::Body()
{
  PushClosure();
  for (auto & r : m_program.m_list) {
    r->Generate(*this);
  }

  *m_outputStream
           << "/* Generator by basalt */\n"
           << "#include <stdlib.h>\n"
           << "#include <stdint.h>\n"
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
    *m_outputStream << r.second.m_ctype 
                    << " "
                    << r.second.m_cname
                    << " = "
                    << r.second.m_initializer
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

int C_CodeGenerator::Generate(const AST::NodeList & expr)
{
  Closure & us = Top();
  for (auto & r : expr.m_list) {
    PushClosure();
    r->Generate(*this);
    us.Output() << TopOutput().str();
    PopClosure();
  }
  return 0;
}

int C_CodeGenerator::Generate(const AST::SourceLine & expr)
{
  TopOutput()  
        << "\n"
        << "#line " << expr.GetLineNumber() 
        << " \"" << m_inputFilename << "\"\n"
        << "  /* " << expr.GetLine() << " */\n";
  return 0;
}

int C_CodeGenerator::Generate(const AST::LineNumber & expr)
{
  // empty
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

////////////////////////////////////////////////////////////////

static bool CreateCVar(const AST::VarRef & var, 
                      C_CodeGenerator::CVarDef & cvar,
                      int tag)
{
  stringstream strm;

  strm << "USER_";
  for (auto r : var.GetName()) {
    if (isalnum(r))
      strm << r;
  }
  if (tag != 0)
    strm << "_" << tag;
  strm << "_";  

  cvar.m_type = var.GetType();
  switch (cvar.m_type) {
    case VarType::eNone:
      return false;
    case VarType::eInt16:
      strm << "int16";
      cvar.m_ctype = "int16_t";
      cvar.m_initializer = "0";
      break;
    case VarType::eInt32:
      strm << "int32";
      cvar.m_ctype = "int16_t";
      cvar.m_initializer = "0";
      break;
    case VarType::eSingle:
      strm << "single";
      cvar.m_ctype = "float";
      cvar.m_initializer = "0";
      break;
    case VarType::eDouble:
      strm << "double";
      cvar.m_ctype = "double";
      cvar.m_initializer = "0";
      break;
    case VarType::eString:
      strm << "string";
      cvar.m_ctype = "char *";
      cvar.m_initializer = "0";
      break;
  }

  cvar.m_cname = strm.str();

  return true;
} 

bool C_CodeGenerator::DeclareGlobalVar(const AST::VarRef & var, 
                                C_CodeGenerator::CVarDef & cvar)
{
  std::string name = var.GetName();

  if (m_globalVars.count(name) != 0) {
    cvar = m_globalVars[name];
  }
  else {
    int tag = 0;
    for (;;) {
      if (!CreateCVar(var, cvar, tag)) {
        cerr << "internal error: cannot map var to C type" << endl;
        return false;
      }
      if (m_cnames.count(cvar.m_cname) == 0)
        break;
      ++tag;  
    }
    m_globalVars[name] = cvar;
    m_cnames.insert(cvar.m_cname);
  }

  return true;
}

int C_CodeGenerator::Generate(const AST::Assign & expr)
{
  Closure & us = Top();

  C_CodeGenerator::CVarDef cvar;
  if (!DeclareGlobalVar(*expr.m_lhs, cvar))
    return -1;

  PushClosure();
  expr.m_rhs->Generate(*this);
  us.Output() << "  " << cvar.m_cname << " = " << TopOutput().str() << ";" << endl;
  PopClosure();

  return 0;
}