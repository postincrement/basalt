#include "../src/basalt.h"

#include "c_codegen.h"
#include <iostream>
#include <iomanip>
#include <typeinfo>

using namespace std;

#define INDENT() std::string(m_indent, ' ')

typedef void (*PrintFunc)(ostream & strm, int indent, const CodeGenerator::Node & node);

////////////////////////////////////////////////////////////

#if 0
static void print_numeric_var(ostream & strm, int indent, const CodeGenerator::Node & node)
{
  strm << std::string(indent, ' ');

  const CodeGenerator::PrintNumericVar * numConst = 
    dynamic_cast<const CodeGenerator::PrintNumericVar *>(&node);

  if (numConst == nullptr) {
    cerr << "internal error: " << endl;
    return;
  }  

  switch (numConst->m_type) {
    case VarType::eInt16:
      strm << "printf(\"%d\", " << numConst->m_value << ");" << endl;
      break;
    case VarType::eInt32:
      strm << "printf(\"%dn\", " << numConst->m_value << ");" << endl;
      break;
    case VarType::eSingle:
      strm << "printf(\"%f\", " << numConst->m_value << ");" << endl;
      break;
    case VarType::eDouble:
      strm << "printf(\"%d\", " << numConst->m_value << ");" << endl;
      break;
    case VarType::eString:
      strm << "printf(\"%d\", " << numConst->m_value << ");" << endl;
      break;
  }
}
#endif


static std::map<std::string, PrintFunc> g_nameToFunc = {
//  { "print_numeric_var",   &print_numeric_var   },
  { "block_end",           nullptr              },
  { "block_start",         nullptr              }
};

////////////////////////////////////////////////////////////

C_OutputGenerator::C_OutputGenerator(CodeGenerator & codegen)
  : OutputGenerator(
    {
      ".c", "temp", "", "", 2, 2
    }, codegen)
{
}

void C_OutputGenerator::OutputFilePrologue(ostream & strm)
{
  strm << "//\n"
       << "// Generated from " << m_inputFilename << "\n" 
       << "//\n\n"
       << "#include <stdio.h>\n"
       << "#include <stdlib.h>\n";

  const std::set<std::string> & funcsUsed = m_codeGenerator.GetFuncsUsed();

  if (funcsUsed.count("gosub"))
    strm << "#include <ucontext.h>\n";    

  strm << "\n";    

  const AST::VarList & globalVars = m_codeGenerator.GetGlobalVars();  
  if (globalVars.size() > 0) {
    strm << "//\n"
         << "// Global variables\n"
         << "//\n";
    for (auto & v : globalVars) {
      const AST::VarInfo & var = v.second;
      std::string ctype;
      std::string init;
      switch (var.m_type) {
        case VarType::eNone:
          break;
        case VarType::eInt16:
          ctype = "int16_t";
          init = "0";
          break;
        case VarType::eInt32:
          ctype = "int32_t"; 
          init = "0";
          break;
        case VarType::eSingle:
          ctype = "float"; 
          init = "0";
          break;
        case VarType::eDouble:
          ctype = "double"; 
          init = "0";
          break;
        case VarType::eString:
          ctype = "char * "; 
          init = "NULL";
          break;
      }
      if (!ctype.empty()) {
        strm << ctype << " " << v.first << " = " << init << ";\n";
      }
    }
    strm << "\n";
  }

  for (auto & r : funcsUsed) {
    strm << "// " << r << endl;
  }

  if (IsPrintUsed()) {
    strm << "\n//\n"
         << "// tab handling\n"
         << "//\n\n"
         << "int print_col = 0;\n"
         << "int print_tabstop = 14;\n"
         << "void print_repeat(int count, char ch)\n"
         << "{\n"
         << "  int i; for (i = 0; i < count; ++i) putchar(ch);\n"
         << "  print_col = (print_col + count) % print_tabstop;\n"
         << "}\n"
         << "void print_tab()\n"
         << "{\n"
         << "  print_repeat(print_tabstop - print_col, ' ');\n"
         << "}\n"
         << "void print_newline()\n"
         << "{\n"
         << "  print_col = 0; putchar('\\r\'); putchar('\\n\');\n"
         << "}\n"
         << "void print_string(const char * str)\n"
         << "{\n"
         << "  while (*str) {\n"
         << "    if (*str == 0x09) print_tab();\n"
         << "    else if (*str == 0x0d) print_newline();\n"
         << "    else if (*str >= 0x20) { putchar(*str); print_col = (print_col + 1) % print_tabstop; }\n"
         << "    ++str;\n"
         << "  }\n"
         << "}\n"
         ;

    if (funcsUsed.count("print_int16_var") || funcsUsed.count("print_int16_const"))         
      strm << "void print_int16_var(int16_t v)\n"
           << "{ printf((v < 0) ? \"%i \" : \" %i \", v); }\n"
           ;

    if (funcsUsed.count("print_int32_var") || funcsUsed.count("print_int32_const"))         
      strm << "void print_int32_var(int32_t v)\n"
           << "{ printf((v < 0) ? \"%i \" : \"%i \", v); }\n"
           ;

    if (funcsUsed.count("print_single_var") || funcsUsed.count("print_single_const"))       
      strm << "void print_single_var(float v)\n"
           << "{ printf((v < 0) ? \"%.0f \" : \" %.0f \", v); }\n"
           ;
    if (funcsUsed.count("print_double_var") || funcsUsed.count("print_double_const"))
      strm << "void print_double_var(double v)\n"
           << "{ printf((v < 0) ? \"%0.lf \" : \" %.0lf \", v); }\n"
           ;
  }

  strm << "\n";

  if (funcsUsed.count("gosub")) {
    strm << "static ucontext_t * return_context = 0;\n\n";
  }

  strm << "//\n"
        << "// main\n"
        << "//\n\n"
        << "int main(int argc, char *argv[])" << endl
       << "{" << endl; 
}

void C_OutputGenerator::OutputFileEpilogue(ostream & strm)
{
  strm << "}" << endl;
}

int C_OutputGenerator::Generate(CodeGenerator::Node & node)
{
  *m_outputStream << INDENT() << "A node!" << endl;
  return 0;
}

int C_OutputGenerator::Generate(CodeGenerator::GotoTarget & node)
{
  *m_outputStream << INDENT() << node.m_value << ":" << endl;
  return 0;
}

int C_OutputGenerator::Generate(CodeGenerator::If & node)
{
  *m_outputStream << INDENT() << "if (" << node.m_condVar << ") // " << m_indent << endl; 
  return 0;
}

int C_OutputGenerator::Generate(CodeGenerator::Else & node)
{
  *m_outputStream << INDENT() << "else\n";
  return 0;
}

int C_OutputGenerator::Generate(CodeGenerator::BlockStart & node)
{
  //cerr << "start:\n" << m_outputStream->str() << "----\n";
  m_blockStack.push_back(m_outputStream);
  m_outputStream = new std::ostringstream(std::ios_base::ate);
  m_indent += m_config.m_indentInc;
  return 0;
}

int C_OutputGenerator::Generate(CodeGenerator::BlockEnd & node)
{
  std::string oldCode = m_blockStack.back()->str();
  std::string newCode = m_outputStream->str();

  delete m_outputStream;
  m_outputStream = m_blockStack.back();
  m_blockStack.pop_back();

  m_indent -= m_config.m_indentInc;

  //*m_outputStream << block << INDENT() << "}" << endl;
  if (!newCode.empty())
    *m_outputStream << INDENT() << "{\n" 
                    << newCode
                    << INDENT() << "}\n";

  //cerr << "endold:\n" << oldCode << "new:\n" << newCode << "final:\n" << m_outputStream->str() << "----\n";
  //cerr << "final2:\n" << m_outputStream->str() << "----\n";
  return 0;
}

int C_OutputGenerator::Generate(CodeGenerator::Goto & node)
{
  *m_outputStream << INDENT() << "goto " << node.m_value << ";\n";
  return 0;
}

int C_OutputGenerator::Generate(CodeGenerator::Gosub & node)
{
  std::string cname = "savedContext";
  *m_outputStream << INDENT() << "ucontext_t " << cname << ";\n"
                  << INDENT() << "get_context(&" << cname <<");\n"
                  << INDENT() << "return_context = &" << cname << ";\n"
                  << INDENT() << "goto line_" << node.m_value << ";\n";
  return 0;
}

int C_OutputGenerator::Generate(CodeGenerator::Return & node)
{
  *m_outputStream << INDENT() << "set_context(return_context);\n";
  return 0;
}

int C_OutputGenerator::Generate(CodeGenerator::End & node)
{
  *m_outputStream << INDENT() << "exit(0);\n";
  return 0;
}

int C_OutputGenerator::Generate(CodeGenerator::System & node)
{
  *m_outputStream << INDENT() << "exit(0);\n";
  return 0;
}

int C_OutputGenerator::Generate(CodeGenerator::PrintNewLine & node) 
{
  *m_outputStream << INDENT() << "print_newline();\n"; 
  return 0;
}

int C_OutputGenerator::Generate(CodeGenerator::PrintInt16Var & node) 
{
  *m_outputStream << INDENT() << "print_int16_var(" << node.m_value << ");\n";
  return 0;
}

int C_OutputGenerator::Generate(CodeGenerator::PrintInt32Var & node) 
{
  *m_outputStream << INDENT() << "print_int32_var(" << node.m_value << ");\n";
  return 0;
}

int C_OutputGenerator::Generate(CodeGenerator::PrintSingleVar & node) 
{
  *m_outputStream << INDENT() << "print_single_var(" << fixed << node.m_value << ");\n";
  return 0;
}

int C_OutputGenerator::Generate(CodeGenerator::PrintDoubleVar & node) 
{
  *m_outputStream << INDENT() << "print_double_var(" << fixed << node.m_value << ");\n";
  return 0;
}

int C_OutputGenerator::Generate(CodeGenerator::PrintInt16Const & node) 
{
  *m_outputStream << INDENT() << "print_int16_var(" << std::stoi(node.m_value) << ");\n";
  return 0;
}

int C_OutputGenerator::Generate(CodeGenerator::PrintInt32Const & node) 
{
  *m_outputStream << INDENT() << "print_int32_var(" << std::stoi(node.m_value) << ");\n";
  return 0;
}

int C_OutputGenerator::Generate(CodeGenerator::PrintSingleConst & node) 
{
  *m_outputStream << INDENT() << "print_single_var(" << fixed << std::stof(node.m_value) << ");\n";
  return 0;
}

int C_OutputGenerator::Generate(CodeGenerator::PrintDoubleConst & node) 
{
  *m_outputStream << INDENT() << "print_double_var(" << fixed << std::stod(node.m_value) << ");\n";
  return 0;
}

int C_OutputGenerator::Generate(CodeGenerator::PrintStringConst & node)
{
  *m_outputStream << INDENT() << "print_string(\"" << node.m_value << "\");\n"; 
  return 0;
}

int C_OutputGenerator::Generate(CodeGenerator::PrintTab & node)
{
  *m_outputStream << INDENT() << "print_tab();\n"; 
  return 0;
}

int C_OutputGenerator::Generate(CodeGenerator::PrintStringVar & node) 
{ 
  *m_outputStream << INDENT() << node.GetFunc() << " \"" << node.m_value << "\"" << endl; 
  return 0; 
} 

int C_OutputGenerator::Generate(CodeGenerator::CreateTempVar & node)
{
  //cerr << "CreateTempVar before\n" << m_outputStream->str() << "---\n";
  *m_outputStream << INDENT() << AST::GetVarTypeInfo(node.m_type).m_name << " " << node.m_value << ";\n";
  //cerr << "CreateTempVar after\n" << m_outputStream->str() << "---\n";
  return 0;
} 

int C_OutputGenerator::Generate(CodeGenerator::UnaryOperator & node)
{
  if (node.m_func != "=")
    *m_outputStream << INDENT() << node.m_ret << " = " << node.m_func << "(" << node.m_arg << ");\n";
  else  
    *m_outputStream << INDENT() << node.m_ret << " " << node.m_func << " " << node.m_arg << ";\n";
  return 0;
}

int C_OutputGenerator::Generate(CodeGenerator::BinaryOperator & node)
{
  if (isalpha(node.m_func[0]))
    *m_outputStream << INDENT() << node.m_ret << " = " << node.m_func << "(" << node.m_arg1 << ", " << node.m_arg2 << ");\n";
  else  
    *m_outputStream << INDENT() << node.m_ret << " = " << node.m_arg1 << " " << node.m_func << " " << node.m_arg2 << ";\n";
  return 0;
}
