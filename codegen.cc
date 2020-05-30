#include <iostream>
#include <typeinfo>

using namespace std;

#include "codegen.h"

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
  Prologue();
  for (auto & r : m_program.m_list) {
    Dispatch(r.get());
  }
  Epilogue();
  return true;
}

void CodeGenerator::Prologue()
{}

void CodeGenerator::Epilogue()
{}

////////////////////////////////////////////////////////////

C_CodeGenerator::C_CodeGenerator (const std::string & inputFilename, 
                                  const std::string & outputFilename,
                                      AST::NodeList & program)
  : CodeGenerator(inputFilename, outputFilename, program)
{
}

void C_CodeGenerator::Prologue()
{
  *m_outputStream
           << "/* Generator by basalt */\n"
           << "#include <stdlib.h>\n"
           << "#include <stdint.h>\n"
           << "extern int basalt_init();\n"
           << "extern int basalt_print_string(const char *);\n"
           << "extern int basalt_print_tab();\n"
           << "extern int basalt_print_newline();\n"
           ;
}

void C_CodeGenerator::Epilogue()
{
  for (auto & r : m_globalVars) {
    *m_outputStream << r.second.m_ctype 
                    << " "
                    << r.second.m_cname
                    << " = "
                    << r.second.m_initializer
                    << "; /* " << r.first << " */\n"
                    ;
  }

  *m_outputStream 
           << "int main(int argc, char * argv[])\n"
           << "{\n"
           << "  basalt_init();\n" 
           << m_body.str()
           << "  exit(0);\n"
           << "}\n"
           ;
}
void C_CodeGenerator::Dispatch(AST::Node * node)
{
  node->C_Generate(*this);
}

void AST::ExprList::C_Generate(C_CodeGenerator & gen)
{
  for (auto & r : m_list)
    r->C_Generate(gen);
}

void AST::Node::C_Generate(C_CodeGenerator & gen)
{
  const std::type_info & r = typeid(*this);
  cerr << "warning: unimplemented C_Generate for " << r.name() << "\n";
}

void AST::Node::C_Print(C_CodeGenerator & gen)
{
  const std::type_info & r = typeid(*this);
  cerr << "warning: unimplemented C_Print for " << r.name() << "\n";
}

std::string AST::Node::C_Evaluate() const
{
  const std::type_info & r = typeid(*this);
  cerr << "warning: unimplemented C_Evaluate for " << r.name() << "\n";
  return "";
}


////////////////////////////////////////////////////////////////

void AST::SourceLine::C_Generate(C_CodeGenerator & gen)
{
  gen.m_body  
        << "#line " << m_lineNumber << " \"" << gen.m_inputFilename << "\"\n"
        << "  /* " << m_line << " */\n";
}

void AST::LineNumber::C_Generate(C_CodeGenerator & gen)
{
  // empty
}

////////////////////////////////////////////////////////////////

void AST::Print::C_Generate(C_CodeGenerator & gen)
{
  for (auto & r : m_list) {
    r->C_Print(gen);
  }
  gen.m_body << "  basalt_print_newline();\n";
}

void AST::StringValue::C_Print(C_CodeGenerator & gen)
{
  gen.m_body << "  basalt_print_string(\"" << m_value << "\");\n";
}

void AST::PrintComma::C_Print(C_CodeGenerator & gen)
{
  gen.m_body << "  basalt_print_tab();\n";
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
  if (cvar.m_type == AST::VarType::eDefault)
    cvar.m_type = AST::VarType::eInteger;

  switch (cvar.m_type) {
    case AST::VarType::eNone:
    case AST::VarType::eDefault:
      return false;
    case AST::VarType::eInteger:
      strm << "int";
      cvar.m_ctype = "int16_t";
      cvar.m_initializer = "0";
      break;
    case AST::VarType::eSingle:
      strm << "single";
      cvar.m_ctype = "float";
      cvar.m_initializer = "0";
      break;
    case AST::VarType::eDouble:
      strm << "double";
      cvar.m_ctype = "double";
      cvar.m_initializer = "0";
      break;
    case AST::VarType::eString:
      strm << "string";
      cvar.m_ctype = "char *";
      cvar.m_initializer = "\"\"";
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

std::string AST::IntegerValue::C_Evaluate() const
{
  std::stringstream strm;
  strm << m_value;
  return strm.str();
}

void AST::Assign::C_Generate(C_CodeGenerator & gen)
{
  C_CodeGenerator::CVarDef cvar;
  if (!gen.DeclareGlobalVar(*m_lhs, cvar))
    return;

  std::stringstream value;
  //value << cvar.m_initializer;
  value << m_rhs->C_Evaluate();

  if (value.str().empty()) {
    cerr << "warning: could not evaluate assigment of " << m_lhs->GetName() << endl;
    value << cvar.m_initializer;
  }

  gen.m_body << "  " << cvar.m_cname << " = " << value.str() << ";" << endl; 
}