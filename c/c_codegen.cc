#include "../basalt.h"

#include "c_codegen.h"
#include <iostream>
#include <iomanip>
#include <typeinfo>
#include <cxxabi.h>

using namespace std;

#define STRING_CTYPE    "char *"
#define TEMP_PREFIX     "temp_"
#define BLOCKFN_PREFIX  "Block_"
#define BLOCK_FN        "Block_end"

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
  : CodeGenerator(
    {
      ".c", TEMP_PREFIX, "{", "}", 2, 2
    }
    )
{
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
  // map names of all variables to C
  for (auto & r : AST::g_globalVars) {
    AST::VarInfo & info = r.second;
    int tag = 0;
    CVarDef cvar;
    for (;;) {
      if (!CreateCVar(r.first, info, cvar, tag)) {
        InternalError("cannot map var to C type");
        return false;
      }
      if (m_cnames.count(cvar.m_cname) == 0)
        break;
      ++tag;  
    }
    m_globalVars[r.first] = cvar;
    m_cnames.insert(cvar.m_cname);
  }

  // create list of code blocks
  {
    for (size_t i = 0; i < m_program->m_list.size(); ++i) {
      if (!m_program->m_list[i])
        continue;

      const AST::SourceLine & r = *m_program->m_list[i];

      if (
        AST::g_jumpDestinationInfo.count(r.GetBasicLineNumber()) || 
        (m_codeBlocks.size() == 0)
        ) {
        StartBlock(r.GetBasicLineNumber());
      }

      TopOutput(false) << "\n  /* " << r.GetLine() << " */\n";
 
      if (!r.m_statements)
        continue;

      const AST::StatementList & s = *r.m_statements;
      if (s.m_list.size() == 0)
        continue;

      int j = 1;
      for (auto & t : s.m_list) {
        if ((m_codeBlocks.size() == 0) || m_codeBlocks[m_currentBlock].m_ended) {
          STRM_STR_DECL(ref, r.GetBasicLineNumber() << "_" << j++);
          StartBlock(ref);
        }
        t->Generate(*this);
      }
    }
    EndBlock();
  }

  // output code
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
           << "extern int basalt_print_int16(int16_t);\n"
           << "extern int basalt_print_int32(int32_t);\n"
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

  if (m_codeBlocks.size() == 1) {
    *m_outputStream 
            << "int main(int argc, char * argv[])\n"
            << "{\n"
            << "  basalt_init();\n"
            << m_codeBlocks[0].m_body.str()
            << "  exit(0);\n"
            << "}\n"
            ;
  }
  else {
    *m_outputStream << "/* declare structure used to link code blocks */\n"
                    << "struct BlockFunction {\n"
                    << "  struct BlockFunction (* m_func)();\n"
                    << "};\n"
                    << "\n"
                    ;
    if (m_blockQueueUsed) {
      *m_outputStream << "/* declare structure for GOSUB */\n"
                      << "struct BlockQueue {\n"
                      << "  struct BlockQueue * m_next;\n"
                      << "  struct BlockFunction (m_return)();\n"
                      << "};\n"
                      << "\n"
                      << "/* declare queue for GOSUB */\n"
                      << "struct BlockQueue * g_blockQueue;\n" 
                      << "\n"
                      << "/* GOSUB function */\n"
                      << "void Gosub(struct BlockFunction ret)\n"
                      << "{\n"
                      << "  struct BlockQueue * block = (struct BlockQueue *)malloc(sizeof(struct BlockQueue));\n"
                      << "  block->m_next = g_blockQueue;\n"
                      << "  block->m_return = ret;\n"
                      << "  g_blockQueue = block;\n"
                      << "}\n"
                      << "\n"
                      << "/* RETURN function */\n"
                      << "struct BlockFunction Return()\n"
                      << "{\n"
                      << "  if ((g_blockQueue == 0) || (g_blockQueue->m_return == 0)) {\n"
                      << "    /* mismatched return */;\n"
                      << "  }"
                      << "  struct BlockFunction ret;\n"
                      << "  ret.m_func = g_blockQueue->m_return;\n"
                      << "  struct BlockQueue * next = g_blockQueue->next;\n"
                      << "  free(g_blockQueue);\n"
                      << "  g_blockQueue = next;\n"
                      << "  return ret;\n"
                      << "}\n"
                      << "\n"
                      ;
    }

    *m_outputStream << "/* forward declare each block of code */\n";
    for (auto & r : m_codeBlocks) {
      *m_outputStream << "struct BlockFunction " BLOCKFN_PREFIX << r.m_ref << "();\n";
    }
    *m_outputStream << "\n";

    for (size_t i = 0; i < m_codeBlocks.size(); ++i) {
      CodeBlock & block = m_codeBlocks[i];

      *m_outputStream << "struct BlockFunction " BLOCKFN_PREFIX << block.m_ref << "()\n"
                      << "{\n"                    
                      << "  struct BlockFunction nextBlock;\n"
                      ;

      if (!block.m_endsWithJump) {
        *m_outputStream << "  nextBlock.m_func = ";
        if (i < m_codeBlocks.size()-1)
          *m_outputStream << "&" << BLOCKFN_PREFIX << m_codeBlocks[i+1].m_ref;
        else
          *m_outputStream << "0";
        *m_outputStream << ";\n";
      }

      *m_outputStream << block.m_body.str()
                      << "  return nextBlock;\n"
                      << "}\n"
                      << "\n"
                      ;                      
    }
    *m_outputStream 
            << "int main(int argc, char * argv[])\n"
            << "{\n"
            << "  basalt_init();\n"
            << "  struct BlockFunction block;\n"
            << "  block.m_func = &" BLOCKFN_PREFIX << 1 << ";\n"
            << "  while (block.m_func != 0) {\n" 
            << "    block = (*block.m_func)();\n"
            << "  }\n"
            << "  exit(0);\n"
            << "}\n"
            ;
  }

  return true;
}

void C_CodeGenerator::StartBlock(const std::string & ref, bool autoEnd)
{
  if (autoEnd)
    EndBlock();

  m_currentBlock = -1;
  for (int i = 0; i < m_codeBlocks.size(); ++i) {
    if (m_codeBlocks[i].m_ref == ref)
      m_currentBlock = i;
      break;
  }

  if (m_currentBlock < 0) {
    CodeBlock block;
    block.m_ref = ref;
    m_codeBlocks.push_back(std::move(block));
    m_currentBlock = m_codeBlocks.size()-1;
    Push();
  }
}

void C_CodeGenerator::EndBlock()
{
  if (m_codeBlocks.size() == 0)
    return;

  CodeBlock & block = m_codeBlocks[m_currentBlock];  
  block.m_endsWithJump = Top().HasJump();
  block.m_body << Pop();
  block.m_ended = true;
}

int C_CodeGenerator::Generate(const AST::SourceLine & line)
{
  TopOutput(false) << "  /* " << line.GetLine() << " */\n";
  return CodeGenerator::Generate(line);
}

int C_CodeGenerator::Generate(const AST::End & expr)
{
  Top().SetHasJump(true);
  TopOutput() << "nextBlock.m_func = 0;\n";
  return 0;
}

int C_CodeGenerator::Generate(const AST::System & expr)
{
  Top().SetHasJump(true);
  TopOutput() << "nextBlock.m_func = 0;\n";
  return 0;
}

int C_CodeGenerator::Generate(const AST::Rem & expr)
{
  return 0;
}

////////////////////////////////////////////////////////////////

int C_CodeGenerator::Generate(const AST::Print & expr)
{
  for (auto & r : expr.m_list) {
    r->Print(*this);
  }

  if ((expr.m_list.size() > 0) && !expr.m_list[expr.m_list.size()-1]->IsPrintSemiColon())
    TopOutput() << "basalt_print_newline();\n";
    
  return 0;
}

int C_CodeGenerator::Print(const AST::NumericVarRef & expr)
{
  if (m_globalVars.count(expr.GetName()) == 0) {
    CompilerError(Error_UndeclaredVariable, 0, "unknown variable \"" << expr.GetName() << "\"");
    return -1;
  }
  CVarDef & cvar = m_globalVars[expr.GetName()];
  std::string funcName = g_varTypeInfo[(int)expr.GetType()].m_printFn;
  TopOutput(true) << funcName << "(" << cvar.m_cname << ");\n";  
  return 0;
}


int C_CodeGenerator::Print(const AST::NumericExpr & expr) 
{ 
  std::string str;    
  expr.Evaluate(*this, str);
  std::string funcName = g_varTypeInfo[(int)expr.GetType()].m_printFn;
  TopOutput(true) << funcName << "(" << str << ");\n";  
  return 0;
}

int C_CodeGenerator::Print(const AST::StringVarRef & expr)
{
  if (m_globalVars.count(expr.GetName()) == 0) {
    CompilerError(Error_UndeclaredVariable, 0, "variable \"" << expr.GetName() << "\" not declared");
    return -1;
  }
  CVarDef & cvar = m_globalVars[expr.GetName()];
  TopOutput(true) << "basalt_print_string(" <<  cvar.m_cname << ");\n";  
  return 0;
}

int C_CodeGenerator::Print(const AST::StringConstant & expr)
{
  TopOutput(true) << "basalt_print_string(" << QuoteLiteral(expr.GetValue()) << ");\n";
  return 0;
}

int C_CodeGenerator::Print(const AST::Int16Constant & expr)
{
  TopOutput(true) << "basalt_print_int16(" << expr.GetValue() << ");\n";
  return 0;
}

int C_CodeGenerator::Print(const AST::Int32Constant & expr)
{
  TopOutput(true) << "basalt_print_int32(" << expr.GetValue() << ");\n";
  return 0;
}

int C_CodeGenerator::Print(const AST::SingleConstant & expr)
{
  TopOutput(true) << "basalt_print_single(" << std::setprecision(7) << expr.GetValue() << ");\n";
  return 0;
}

int C_CodeGenerator::Print(const AST::DoubleConstant & expr)
{
  TopOutput(true) << "basalt_print_double(" << /* std::setprecision(10) << */ expr.GetValue() << ");\n";
  return 0;
}

int C_CodeGenerator::Print(const AST::PrintComma & expr)
{
  TopOutput(true) << "basalt_print_tab();\n";
  return 0;
}

////////////////////////////////////////////////////////////////

int C_CodeGenerator::Generate(const AST::GotoStatement & expr)
{
  Top().SetHasJump(true);
  TopOutput() << "nextBlock.m_func = &" BLOCKFN_PREFIX << expr.GetRef() << ";\n";  
  return 0;
}

////////////////////////////////////////////////////////////////

int C_CodeGenerator::Generate(const AST::GosubStatement & expr)
{
  // create symbol for return entry
  std::string returnRef = GetGlobalTempName();

  // create a new entry for the block queue
  m_blockQueueUsed = true;
  TopOutput() << "struct BlockFunction target;\n";
  TopOutput() << "target.m_func = &" BLOCKFN_PREFIX << returnRef << ";\n";
  TopOutput() << "Gosub(target);\n";
  
  // set "gosub" to routine
  TopOutput() << "nextBlock.m_func = &" BLOCKFN_PREFIX << expr.GetRef() << ";\n";  

  // finish this block
  Top().SetHasJump(true);

  // start new block for return
  StartBlock(returnRef);
  return 0;
}

int C_CodeGenerator::Generate(const AST::ReturnStatement & expr)
{
  TopOutput() << "nextBlock = Return();\n";
  Top().SetHasJump(true);

  //EndBlock();

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
  CVarDef cvar;
  if (!LookupGlobalVar(expr.m_lhs->GetName(), cvar))
    return -1;

  if (expr.m_rhs == nullptr) {
    InternalError("expression missing rhs");
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

  return 0;
}

////////////////////////////////////////////////////////////////

int C_CodeGenerator::Generate(const AST::NumericAssign & expr)
{
  Closure & us = Top();

  CVarDef cvar;
  if (!LookupGlobalVar(expr.m_lhs->GetName(), cvar))
    return -1;

  if (expr.m_rhs == nullptr) {
    InternalError("expression missing rhs");
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
    CompilerError(Error_UndeclaredVariable, 0, "variable \"" << expr.GetName() << "\" not declared");
    return -1;
  }
  CVarDef & cvar = m_globalVars[expr.GetName()];
  result = cvar.m_cname;  
  return 0;  
}

////////////////////////////////////////////////////////////////

int C_CodeGenerator::Evaluate(const AST::Int32Constant & expr, std::string & result)
{
  stringstream strm;
  strm << expr.GetValue();
  result = strm.str();
  return 0;
}

////////////////////////////////////////////////////////////////

int C_CodeGenerator::Evaluate(const AST::SingleConstant & expr, std::string & result)
{
  stringstream strm;
  strm << /* std::setprecision(7) << */ expr.GetValue();
  result = strm.str();
  return 0;
}

////////////////////////////////////////////////////////////////

int C_CodeGenerator::Evaluate(const AST::DoubleConstant & expr, std::string & result)
{
  stringstream strm;
  strm << /* std::setprecision(10) << */ expr.GetValue();
  result = strm.str();
  return 0;
}

////////////////////////////////////////////////////////////////

bool C_CodeGenerator::LookupGlobalVar(const std::string & varName, CVarDef & cvar)
{
  if (m_globalVars.count(varName) == 0)
    return false;

  cvar = m_globalVars[varName];
  return true;
}

int C_CodeGenerator::Evaluate(const AST::NumericVarRef & expr, std::string & result)
{
  if (m_globalVars.count(expr.GetName()) == 0) {
    CompilerError(Error_UndeclaredVariable, 0, "variable \"" << expr.GetName() << "\" not declared");
    return -1;
  }
  CVarDef & cvar = m_globalVars[expr.GetName()];
  result = cvar.m_cname;  
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
  // always evaluate LHS first, to ensure left to right evaluation
  expr->m_rhs->Evaluate(*this, rhs);
  expr->m_lhs->Evaluate(*this, lhs);
  TopOutput() << info.m_ctype << " " << temp1 << " = " << lhs << " " << op << " " << rhs << ";\n";

  result = temp1;

  return 0;
}

int C_CodeGenerator::NumericComparisonOperator(const std::string & op, const AST::NumericBinaryOperation * expr, std::string & result)
{
  if ((expr == nullptr) || (expr->m_lhs == nullptr) || (expr->m_rhs == nullptr)) {
    InternalError("numeric comparison failed");
    return -1;
  }

  std::string temp1 = Top().GetTempName();
  CTypeInfoRec & info = g_varTypeInfo[(int)expr->GetType()];

  std::string lhs, rhs;
  expr->m_lhs->Evaluate(*this, lhs);
  expr->m_rhs->Evaluate(*this, rhs);

  TopOutput() << info.m_ctype << " " << temp1 << " = (" << lhs << " " << op << " " << rhs << ") ? -1 : 0;\n";

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

int C_CodeGenerator::Evaluate(const AST::NumericEquality & expr, std::string & result)
{
  return NumericComparisonOperator("==", &expr, result);
}

int C_CodeGenerator::Evaluate(const AST::NumericNotEquality & expr, std::string & result)
{
  return NumericComparisonOperator("!=", &expr, result);
}

int C_CodeGenerator::Evaluate(const AST::NumericGreaterThan & expr, std::string & result)
{
  return NumericComparisonOperator(">", &expr, result);
}

int C_CodeGenerator::Evaluate(const AST::NumericGreaterThanEqual & expr, std::string & result)
{
  return NumericComparisonOperator(">=", &expr, result);
}

int C_CodeGenerator::Evaluate(const AST::NumericLessThan & expr, std::string & result)
{
  return NumericComparisonOperator("<", &expr, result);
}

int C_CodeGenerator::Evaluate(const AST::NumericLessThanEqual & expr, std::string & result)
{
  return NumericComparisonOperator("<=", &expr, result);
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
    InternalError("cannot find str function for type " << (int)expr.GetArg1()->GetType());
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

////////////////////////////////////////////////////////////////

int C_CodeGenerator::Generate(const AST::IfStatement & expr)
{
  if (expr.m_cond == nullptr)
    return -1;
  if (expr.m_trueStatements == nullptr)
    return -1;

  std::string str;    
  expr.m_cond->Evaluate(*this, str);

  TopOutput(true) << "if (" << str << ")\n";

  std::string gotoLine = expr.m_trueStatements->m_lineNumber;
  if (!gotoLine.empty()) {
    Push();
    TopOutput() << "nextBlock.m_func = &" BLOCKFN_PREFIX << gotoLine << ";\n";  
    TopOutput() << "return nextBlock;\n";
    Pop();  
  }
  else {
    Push();
    expr.m_trueStatements->m_statements->Generate(*this);
    Pop();
    if (expr.m_falseStatements) {
      TopOutput(true) << "else\n";
    }
    Push();
  }
  if (expr.m_falseStatements) {
    expr.m_falseStatements->Generate(*this);
  }
  if (gotoLine.empty()) {
    Pop();
  }
  return 0;
}

////////////////////////////////////////////////////////////////

int C_CodeGenerator::Generate(const AST::ForStatement & expr)
{
  std::string forRef = GetGlobalTempName();

  // get index variable
  CVarDef cvar;
  if (!LookupGlobalVar(expr.m_var->GetName(), cvar))
    return -1;

  // create FOR queue entry
  ForBlock forBlock;
  forBlock.m_ref   = forRef;
  forBlock.m_type  = expr.m_var->GetType();
  forBlock.m_index = cvar.m_cname;
  forBlock.m_isConst = false;

  // get initial value
  std::string fromValue;
  expr.m_fromVal->Evaluate(*this, fromValue);

  // get TO value
  std::string toValue;
  expr.m_toVal->Evaluate(*this, toValue);

  // get STEP value
  std::string stepValue;
  if (expr.m_stepVal)
    expr.m_stepVal->Evaluate(*this, stepValue);

  // compare initial and final values
  bool fromIsConst = expr.m_fromVal->IsConstant();
  bool toIsConst   = expr.m_toVal->IsConstant();
  bool stepIsConst = stepValue.empty() || expr.m_stepVal->IsConstant();

  if (fromIsConst && toIsConst && stepIsConst) {
    int dir;
    switch (forBlock.m_type) {
      case VarType::eInt16:
      case VarType::eInt32:
        {
          unsigned long from = strtoul(fromValue.c_str(), NULL, 10);
          unsigned long to   = strtoul(toValue.c_str(), NULL, 10);
          dir = (from <= to) ? 1 : -1;
        }
        break;
      case VarType::eSingle:
        {
          float from = strtof(fromValue.c_str(), NULL);
          float to   = strtof(toValue.c_str(), NULL);
          dir = (from <= to) ? 1 : -1;
        }
        break;
      case VarType::eDouble:
        {
          double from = strtod(fromValue.c_str(), NULL);
          double to   = strtod(toValue.c_str(), NULL);
          dir = (from <= to) ? 1 : -1;
        }
        break;
    }
    if (stepValue.empty()) {
      stepValue = (dir < 0) ? "-1" : "1";
    }
    forBlock.m_step    = stepValue;
    forBlock.m_to      = toValue;
    forBlock.m_isConst = true;
  }
  else {
    TopOutput() << "if (" << toValue << " <= " << fromValue << ")";
    Push();
    if (stepValue.empty()) {
      stepValue = "1";
    }
    else {
    }
    Pop();
    TopOutput() << "else";
    Push();
    if (stepValue.empty()) {
      stepValue = "-1";
    }
    else {
    }
    Pop();
  }

  // assign initial value of index variable
  TopOutput() << cvar.m_cname << " = " << fromValue << ";\n";

  // jump to start of FOR block
  TopOutput() << "nextBlock.m_func = &" BLOCKFN_PREFIX << forRef << ";\n";  

  // finish this block
  Top().SetHasJump(true);
  m_forQueue.push_back(forBlock);

  // start new block
  StartBlock(forRef);
  return 0;
}

int C_CodeGenerator::Generate(const AST::NextStatement & expr)
{
  // ensure we are in a FOR loop
  if (m_forQueue.size() == 0) {
    CompilerError(Error_MismatchedNext, expr.m_lineNumber, "Mismatched next");
    exit(-1);
  }

  // get information for topmost FOR
  ForBlock forBlock = m_forQueue.back();
  m_forQueue.pop_back();

  // output simple code when using constants
  if (forBlock.m_isConst) {
    TopOutput() << forBlock.m_index << " += " << forBlock.m_step << ";\n";
    TopOutput() << "if (" << forBlock.m_index << " <= " << forBlock.m_to << ")\n";
    Push();
    TopOutput() << "nextBlock.m_func = &" BLOCKFN_PREFIX << forBlock.m_ref << ";\n";
    TopOutput() << "return nextBlock;\n";
    Pop();
  }
  else {

  }
  return 0;
}
