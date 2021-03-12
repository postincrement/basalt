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
static CTypeInfoRec g_varTypeInfo[] ={
    { 0 },                        // none
    { "int16_t",    "0", "print_int16",  "basalt_str_int16",  "int16" },     // eInt16
    { "int32_t",    "0", "print_int32",  "basalt_str_int32",  "int32" },     // eInt32
    { "float",      "0", "print_single", "basalt_str_single", "single" },    // eSingle
    { "double",     "0", "print_double", "basalt_str_double", "double" },    // eDouble
    { STRING_CTYPE, "0", "print_string", 0,                   "string" }     // eString
};


////////////////////////////////////////////////////////////

C_CodeGenerator::C_CodeGenerator()
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

  Generate();

  // output code
  *m_outputStream
      << "/* C generator by basalt */\n"
      << "#include <stdlib.h>\n"
      << "#include <unistd.h>\n"
      << "#include <stdio.h>\n"
      << "#include <stdint.h>\n"
      << "#include <math.h>\n"
      << "#include <string.h>\n"
      ;

  OutputRuntimeDecls(*m_outputStream);

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

  OutputBlocks();

  OutputRuntime(*m_outputStream);

  return true;
}

void C_CodeGenerator::Generate()
{  
  // generate code for the list of source lines
  int ret = 0;

  m_startBlock = true;
  m_lastBlockHadReturn = true;

  // loop through the source lines
  // but ignore lines with no data
  for (size_t i = 0; i < m_program->m_list.size(); ++i) {
    if (m_program->m_list[i]) {
      GenerateLine(i, *m_program->m_list[i]);
    }
  }

  // finish last block
  EndBlock();
}

void C_CodeGenerator::GenerateLine(int index, const AST::SourceLine & sourceLine)
{
  // always start a block if the line number 
  // is the destination of a GOTO or GOSUB
  if (
      m_startBlock ||
      AST::g_jumpDestinationInfo.count(sourceLine.GetBasicLineNumber()) ||
      (m_codeBlocks.size() == 0)
      ) {
    //if (!m_lastBlockHadReturn) {
    //  TopOutput() << "return 0;\n";
    //  m_lastBlockHadReturn = true;
    //}  
    StartBlock(sourceLine.GetBasicLineNumber());
    m_startBlock = false;
  }

  // if no statements in this line, nothing to do
  if (!sourceLine.m_statements) {
    //cout << "\n" << sourceLine.GetLine() << " - no statements\n";
    return;
  }

  const AST::StatementList & statements = *sourceLine.m_statements;
  if (statements.m_list.size() == 0) {
    //cout << "\n" << sourceLine.GetLine() << " - empty list\n";
    return;
  }

  // insert text of BASIC line into source
  //TopOutput(false) << "\n  /* " << sourceLine.GetLine() << " */\n";

  // get next line in program
  if (index >= m_program->m_list.size()-1)
    m_nextLine = nullptr;
  else  
    m_nextLine = m_program->m_list[index+1].get();

  // loop through the statements
  for (int j = 0; j < statements.m_list.size(); ++j) {
    auto const & statement = *statements.m_list[j]; 
    //cout << "\n  " << statement.m_text << "\n";
    //TopOutput(false) << "// statement " << statement.m_text << "\n";

    int ret = GenerateStatement(j, 
                        sourceLine.GetBasicLineNumber(),
                        j == statements.m_list.size()-1, 
                        statement);

    bool finishLine = false;
    switch (ret) {
      case eOp_Next:
      case eOp_NextLine:
      case eOp_EndProgram:
      case eOp_Goto:
      case eOp_Return:
        finishLine = true;
        break;
      case eOp_NextStatement:
        break;
      default:
        cerr << "unknown code generator code " << ret 
              << " from '" << statement.m_text << "'" 
              << " and type " << DemangleTypeName(typeid(statement))
              << endl;
        break;  
    }

    if (finishLine) {
      if (j != statements.m_list.size()-1) {
        CompilerError(eWarning_UnreachableCode, statement.m_lineNumber, "unreachable code with code " << ret);
      }
      break;
    }
  }                        
}

int C_CodeGenerator::GenerateStatement(int index, 
                        const std::string & basicLineNumber,
                                       bool isLastStatementOnLine, 
                     const AST::Statement & statement)
{
  // start a new block if needed
  if (m_startBlock) {
    StartBlock(basicLineNumber);
    m_nextBlockRef = "";
    m_startBlock = false;
  }

  // set flags needed by code generation
  if (!isLastStatementOnLine)
    STRM_STR(m_nextBlockRef, basicLineNumber << "_" << index+1);
  else if (m_nextLine == nullptr) 
    m_nextBlockRef = "0";
  else { 
    m_nextBlockRef = m_nextLine->GetBasicLineNumber();
    STRM_STR(m_nextBlockRef, m_nextLine->GetBasicLineNumber());
  }

  // insert the statement text as a comment
  TopOutput(false) << ((m_codeBlocks.size() != 0) ? "\n" : "") 
                    << "  /* " << statement.m_text << " */\n";

  // generate code for statement
  return statement.Generate(*this);
}

void C_CodeGenerator::OutputBlocks()
{
  *m_outputStream 
      << "/* declare structure used to link code blocks */\n"
      << "struct BlockFunction {\n"
      << "  int (* m_func)(struct BlockFunction *);\n"
      << "};\n"
      << "\n"
      << "int execute(struct BlockFunction * block);\n\n"
      ;
#if 0
  if (m_funcsUsed.count("for")) {
    *m_outputStream 
        << "/* declare structure for FOR */\n"
        << "struct ForInfo {\n"
        << "  struct BlockFunction (* m_func)();\n"
        << "  struct ForInfo * g_next;\n"
        << "  union {\n"
        << "    int16_t m_int16[3];\n"
        << "    float   m_float[3];\n"
        << "    double  m_double[3];\n"
        << "  } m_val;\n"
        << "};\n\n"
        << "/* declare queue for FOR */\n"
        << "struct ForInfo g_forQueueEntry;\n"
        << "struct ForInfo * g_forQueue = NULL;\n"
        << "\n";
  }

  if (m_funcsUsed.count("gosub")) {
    *m_outputStream 
        << "/* declare structure for GOSUB */\n"
        << "struct BlockQueue {\n"
        << "  struct BlockQueue * m_next;\n"
        << "  struct BlockFunction m_return;\n"
        ;
    if (m_funcsUsed.count("for")) {
      *m_outputStream 
          << "  struct ForInfo * m_forQueue;\n"
          ;
    }     
    *m_outputStream 
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
        ;
    if (m_funcsUsed.count("for")) {
      *m_outputStream 
          << "  block->m_forQueue = NULL;\n"
          ;
    }
    *m_outputStream 
        << "  g_blockQueue = block;\n"
        << "}\n"
        << "\n"
        << "/* RETURN function */\n"
        << "struct BlockFunction Return()\n"
        << "{\n"
        << "  if (g_blockQueue == 0) {\n"
        << "    /* mismatched return */;\n"
        << "  }"
        << "  struct BlockFunction ret;\n"
        << "  ret = g_blockQueue->m_return;\n"
        << "  struct BlockQueue * next = g_blockQueue->m_next;\n"
        << "  free(g_blockQueue);\n"
        << "  g_blockQueue = next;\n"
        << "  return ret;\n"
        << "}\n"
        << "\n"
        ;
  }
#endif

  // output forward declarations of each block
  *m_outputStream << "/* forward declare each block of code */\n";
  for (auto & r : m_codeBlocks) {
    *m_outputStream << "int " BLOCKFN_PREFIX << r.m_ref << "(struct BlockFunction *);\n";
  }
  *m_outputStream << "\n";

  // output each block
  for (size_t i = 0; i < m_codeBlocks.size(); ++i) {
    CodeBlock & block = m_codeBlocks[i];

    // function header
    *m_outputStream << "int " BLOCKFN_PREFIX << block.m_ref << "(struct BlockFunction * block)\n"
      << "{\n"
      ;

    // set pointer to next block, unless we know this block ends
    // with a jump to another block
    *m_outputStream << "  block->m_func = ";
    if (i < m_codeBlocks.size()-1)
      *m_outputStream << "&" << BLOCKFN_PREFIX << m_codeBlocks[i+1].m_ref;
    else
      *m_outputStream << "0";
    *m_outputStream << ";\n";

    // output the block body
    *m_outputStream << block.m_body.str() 
//        << "  return 0;\n"
        << "}\n"
        << "\n"
        ;
}

*m_outputStream
    << "int execute(struct BlockFunction * block)\n"
    << "{\n"
    << "  int ret = 1;\n"
    << "  while (block->m_func != 0) {\n"
    << "    int ret = (*block->m_func)(block);\n"
    << "    if (ret != 0)\n"
    << "      break;\n"
    << "  }\n"
    << "  if (ret <= 1)\n"
    << "    return ret;\n"
    << "  return 0; // return\n"
    << "}\n"
    << "\n"
    << "int main(int argc, char * argv[])\n"
    << "{\n"
    << "  basalt_init();\n"
    << "  struct BlockFunction block;\n"
    << "  block.m_func = &" BLOCKFN_PREFIX << 1 << ";\n"
    << "  execute(&block);\n"
    << "  exit(0);\n"
    << "}\n"
    ;
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
  TopOutput() << "return -1;\n";
  return eOp_EndProgram;
}

int C_CodeGenerator::Generate(const AST::System & expr)
{
  TopOutput() << "return -1;\n";
  return eOp_EndProgram;
}

int C_CodeGenerator::Generate(const AST::Rem & expr)
{
  return eOp_NextLine;
}

////////////////////////////////////////////////////////////////

int C_CodeGenerator::Generate(const AST::Print & printExpr)
{
  AST::ExprList & expr = *printExpr.m_list;
  for (auto & r : expr.m_list) {
    r->Print(*this);
  }

  if ((expr.m_list.size() > 0) && !expr.m_list[expr.m_list.size()-1]->IsPrintSemiColon()) {
    m_funcsUsed.insert("print_newline");
    TopOutput() << "print_newline();\n";
  }

  return eOp_NextStatement;
}

int C_CodeGenerator::Print(const AST::NumericVarRef & expr)
{
  if (m_globalVars.count(expr.GetName()) == 0) {
    CompilerError(Error_UndeclaredVariable, 0, "unknown variable \"" << expr.GetName() << "\"");
    return eOp_NextLine;
  }
  CVarDef & cvar = m_globalVars[expr.GetName()];
  std::string funcName = g_varTypeInfo[(int)expr.GetType()].m_printFn;
  m_funcsUsed.insert(funcName);
  TopOutput(true) << funcName << "(" << cvar.m_cname << ");\n";
  return eOp_NextStatement;
}


int C_CodeGenerator::Print(const AST::NumericExpr & expr)
{
  std::string str;
  expr.Evaluate(*this, str);
  std::string funcName = g_varTypeInfo[(int)expr.GetType()].m_printFn;
  m_funcsUsed.insert(funcName);
  TopOutput(true) << funcName << "(" << str << ");\n";
  return eOp_NextStatement;
}

int C_CodeGenerator::Print(const AST::StringVarRef & expr)
{
  if (m_globalVars.count(expr.GetName()) == 0) {
    CompilerError(Error_UndeclaredVariable, 0, "variable \"" << expr.GetName() << "\" not declared");
    return -1;
  }
  CVarDef & cvar = m_globalVars[expr.GetName()];
  m_funcsUsed.insert("print_string");
  TopOutput(true) << "print_string(" <<  cvar.m_cname << ");\n";
  return eOp_NextStatement;
}

int C_CodeGenerator::Print(const AST::StringConstant & expr)
{
  m_funcsUsed.insert("print_string");
  TopOutput(true) << "print_string(" << QuoteLiteral(expr.GetValue()) << ");\n";
  return eOp_NextStatement;
}

int C_CodeGenerator::Print(const AST::Int16Constant & expr)
{
  m_funcsUsed.insert("print_int16");
  TopOutput(true) << "print_int16(" << expr.GetValue() << ");\n";
  return eOp_NextStatement;
}

int C_CodeGenerator::Print(const AST::Int32Constant & expr)
{
  m_funcsUsed.insert("print_int32");
  TopOutput(true) << "print_int32(" << expr.GetValue() << ");\n";
  return eOp_NextStatement;
}

int C_CodeGenerator::Print(const AST::SingleConstant & expr)
{
  m_funcsUsed.insert("print_single");
  TopOutput(true) << "print_single(" << std::setprecision(13) << std::noshowpoint << expr.GetValue() << ");\n";
  return eOp_NextStatement;
}

int C_CodeGenerator::Print(const AST::DoubleConstant & expr)
{
  m_funcsUsed.insert("print_double");
  TopOutput(true) << "print_double(" << std::setprecision(13) << std::noshowpoint << expr.GetValue() << ");\n";
  return eOp_NextStatement;
}

int C_CodeGenerator::Print(const AST::PrintComma & expr)
{
  m_funcsUsed.insert("print_tab");
  TopOutput(true) << "print_tab();\n";
  return eOp_NextStatement;
}

////////////////////////////////////////////////////////////////

int C_CodeGenerator::Generate(const AST::GotoStatement & expr)
{
  TopOutput() << "block->m_func = &" BLOCKFN_PREFIX << expr.GetRef() << ";\n";
  TopOutput() << "return 0;\n";
  return eOp_Goto;
}

////////////////////////////////////////////////////////////////

int C_CodeGenerator::Generate(const AST::GosubStatement & expr)
{
  // create symbol for return entry
  std::string returnRef;
  
  returnRef = m_nextBlockRef;

  // create a new entry for the block queue
  TopOutput() << "struct BlockFunction target;\n";
  TopOutput() << "target.m_func = &" BLOCKFN_PREFIX << returnRef << ";\n";
  TopOutput() << "Gosub(target);\n";

  // set "gosub" to routine
  TopOutput() << "nextBlock.m_func = &" BLOCKFN_PREFIX << expr.GetRef() << ";\n";

  m_funcsUsed.insert("gosub");

  return eOp_NextStatement;
}

int C_CodeGenerator::Generate(const AST::ReturnStatement & expr)
{
  TopOutput() << "return Return();\n";

  return eOp_Return;
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
    return eOp_NextLine;

  if (expr.m_rhs == nullptr) {
    InternalError("expression missing rhs");
    return eOp_NextLine;
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

  return eOp_NextStatement;
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
    break;
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

int C_CodeGenerator::Generate(const AST::AssignStatement & expr)
{
  m_currentStatementLine = expr.m_lineNumber;
  return expr.m_expr->Generate(*this);
}

int C_CodeGenerator::Generate(const AST::NumericAssign & expr)
{
  Closure & us = Top();

  CVarDef cvar;
  if (!LookupGlobalVar(expr.m_lhs->GetName(), cvar))
    return eOp_NextLine;

  if (expr.m_rhs == nullptr) {
    InternalError("expression missing rhs");
    return eOp_NextLine;
  }

  std::string rhs;
  expr.m_rhs->Evaluate(*this, rhs);
  us.Output() << "  " << cvar.m_cname << " = " << rhs << ";" << endl;

  return eOp_NextStatement;
}

////////////////////////////////////////////////////////////////

int C_CodeGenerator::Evaluate(const AST::StringVarRef & expr, std::string & result)
{
  if (m_globalVars.count(expr.GetName()) == 0) {
    CompilerError(Error_UndeclaredVariable, 0, "variable \"" << expr.GetName() << "\" not declared");
    return eOp_NextLine;
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
    return eOp_NextLine;
  if (expr.m_trueStatements == nullptr)
    return eOp_NextLine;

  std::string str;
  expr.m_cond->Evaluate(*this, str);

  TopOutput(true) << "if (" << str << ")\n";

  std::string gotoLine = expr.m_trueStatements->m_lineNumber;
  if (!gotoLine.empty()) {
    Push();
    TopOutput() << "block->m_func = &" BLOCKFN_PREFIX << gotoLine << ";\n";
    TopOutput() << "return 0;\n";
    Pop();
  }
  else {
    Push();
    expr.m_trueStatements->m_statements->Generate(*this);
    Pop();
  }
  if (expr.m_falseStatements) {
    TopOutput(true) << "else\n";
    Push();
    expr.m_falseStatements->Generate(*this);
    Pop();
  }
  return eOp_NextStatement;
}

////////////////////////////////////////////////////////////////

int C_CodeGenerator::Generate(const AST::ForStatement & expr)
{
  std::string forRef = GetGlobalTempName();

  // get index variable
  CVarDef cvar;
  if (!LookupGlobalVar(expr.m_var->GetName(), cvar))
    return eOp_NextLine;

  VarType forType = cvar.m_type;

  // create FOR queue entry
//  ForBlock forBlock;
//  forBlock.m_isConst = false;
//  
//  forBlock.m_ref     = forRef;
//  forBlock.m_index   = cvar.m_cname;

  std::string forCType   = g_varTypeInfo[(int)forType].m_ctype;

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

  // determine type of loop
  bool isConst = fromIsConst && toIsConst && stepIsConst;
  if (isConst) {
    int dir;
    switch (forType) {
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
  }

  // set initial value of index variable
  TopOutput() << cvar.m_cname << " = " << fromValue.c_str() << ";\n";

  // set TO value
  TopOutput() << forCType << " " << forRef << "_to = " << toValue << ";\n";

  // set STEP value
  TopOutput() << forCType << " " << forRef << "_step = ";
  if (stepValue.empty()) {
    TopOutput(false) << "(" << fromValue << " < " << toValue << ") ? 1 : -1";
  }
  else {
    TopOutput(false) << Trim(stepValue);
  }
  TopOutput(false) << ";\n";

  // output start of FOR
  TopOutput(true) << "do\n";

  // body of FOR
  StartBlock("for", false);


  return eOp_NextStatement;
}

int C_CodeGenerator::Generate(const AST::NextStatement & expr)
{
  TopOutput() << "return " << (int)eOp_Next << ";\n";

  EndBlock();
  
  // do STEP
  TopOutput() << cvar.m_cname << " += " << forRef << "_step;\n";

  // loop
  TopOutput(true) << "while (" << cvar.m_cname << " <= " << forRef << "_to);\n";
  return eOp_Next;
}
