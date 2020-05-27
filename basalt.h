
#ifndef BASALT_H_
#define BASALT_H_

#include <string>
#include <fstream>

#include "ast.h"
#include "common.h"


struct ArgDef 
{
  const char   m_short;
  const char * m_long;
  const char * m_type;
  void       * m_data;
  const char * m_usage;
};

class Basalt
{
  public:
    int Main(int argc, char const *argv[]);

    int ParseArguments(ArgDef * defs, int argc, char const *argv[], int index);
    void DecodeOpt(ArgDef * def, int & index, int argc, const char **argv);
    void Usage(const ArgDef * defs, bool keys = false);
    void DisplayHelp();
    char ReadNextChar();

    void OnError(const std::string & msg);
    void OnWarning(const std::string & msg);
    void DisplayError(const std::string & msg, const std::string & type);

    std::string GetCurrentLine() const { return m_line; }

  protected:    
    bool m_interactive;

    std::string m_progname;
    size_t m_lineOffs;
    std::string m_line;
    std::istream * m_inputStream = nullptr;
    std::ifstream m_inputFile;
    Filename m_inputFilename;    
};


extern int MBASIC_lex();
extern int MBASIC_parse();
extern void MBASIC_error(const char * msg);
//extern FILE * MBASIC_in;
extern int MBASIC_debug;
extern void MBASIC_yyinput(char * buf, int * result, int maxSize);

extern void InternalErrorFunc(const char * fn, unsigned ln, const std::string & str);
#define InternalError(expr) \
do { std::stringstream strm; strm << expr; InternalErrorFunc(__FILE__, __LINE__, strm.str()); } while (0)

extern void InternalWarningFunc(const char * fn, unsigned ln, const std::string & str);
#define InternalWarning(expr) \
do { std::stringstream strm; strm << expr; InternalWarningFunc(__FILE__, __LINE__, strm.str()); } while (0)

enum WarningCode {

  eWarning_Unknown                = 0x0000,

  // syntax warnings
  eWarning_Syntax                 = 0x1000, 
  eWarning_PrintUsingQuestionMark = eWarning_Syntax,
  eWarning_RemUsingQuote,
  eWarning_MixedExpression,
  eWarning_CannotEvaluate,
  eWarning_CannotAssignString,
  eWarning_UnsupportedStringOp,
  
  // LLVM code generation warning
  eWarning_LLVM                  = 0x2000, 
  eWarning_UnknownLLVMBIF        = eWarning_LLVM,

  // CXX code generation warning
  eWarning_CXX                   = 0x3000, 
  eWarning_UnknownCXXBIF         = eWarning_CXX
};

extern void SourceWarningFunc(WarningCode code, unsigned line, const std::string & marker, const std::string & str);
#define SourceWarning(code, expr) \
do { std::stringstream strm; strm << expr; \
     if (m_currentLineMarkerExpr == nullptr) \
       SourceWarningFunc(code, 0, "unknown", strm.str()); \
     else \
       SourceWarningFunc(code, m_currentLineMarkerExpr->m_lineNumber, m_currentLineMarkerExpr->m_marker, strm.str()); \
    } while (0)

extern void WarningFunc(WarningCode code, const std::string & str);
#define Warning(code, expr) \
do { std::stringstream strm; strm << expr; WarningFunc(code, strm.str()); } while (0)

extern Basalt g_application;
extern int g_lineNumber;
extern bool g_compileOnly;
extern bool g_dump;
extern bool g_enableDebugging;

#endif // BASALT_H_
