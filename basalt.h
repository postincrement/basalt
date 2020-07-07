
#ifndef BASALT_H_
#define BASALT_H_

#include <string>
#include <fstream>

#include "codegen.h"
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
    void Usage(bool keys = false);
    void DisplayHelp(const std::vector<ArgDef> & argDefs);
    char ReadNextChar();

    void OnError(unsigned lineNumber, const std::string & msg);
    void OnWarning(unsigned lineNumber, const std::string & msg);
    void DisplayError(unsigned lineNumber, const std::string & msg, const std::string & type);

    std::string GetCurrentLine() const { return m_line; }

  protected:    

    bool m_interactive;

    std::string m_progname;
    size_t m_lineOffs;
    std::string m_line;

    std::istream * m_inputStream = nullptr;
    std::ifstream m_inputFile;
    Filename m_inputFilename; 
    std::string m_printableInputFilename;

    int m_verbose = 0;
    int m_displayHelp = 0;
    std::string m_codeGeneratorName;
    std::string m_outputFilename;
    std::string m_languageProfileName;
    std::string m_arch;
    bool m_dump = false;
    bool m_compileOnly = false;
    bool m_enableDebugging = false;
};


extern int MBASIC_lex();
extern int MBASIC_parse();
extern int MBASIC_debug;
extern void MBASIC_yyinput(char * buf, int * result, int maxSize);

///////////////////////////////////////////////////////////////////////////////////////////////////

enum ErrorCode 
{
  eError_Unknown                 = 0x8000,
  eError_GotoDestinationNotFound
};

extern void InternalErrorFunc(const char * fn, unsigned ln, const std::string & str);
#define InternalError(expr) \
do { std::stringstream strm; strm << expr; InternalErrorFunc(__FILE__, __LINE__, strm.str()); } while (0)

extern void SourceErrorFunc(ErrorCode code, unsigned line, const std::string & str);
#define SourceError(code, line, expr) \
do { std::stringstream strm; strm << expr; \
       SourceErrorFunc(code, line, strm.str()); \
    } while (0)

extern void ErrorFunc(ErrorCode code, const std::string & str);

///////////////////////////////////////////////////////////////////////////////////////////////////

enum WarningCode {

  eWarning_Unknown                = 0x0000,

  // syntax warnings
  eWarning_Syntax                 = 0x1000, 
  eWarning_PrintUsingQuestionMark = eWarning_Syntax,
  eWarning_RemUsingQuote,
  eWarning_VarDefinedButNotUsed,
  eWarning_VarIsSynonym,
};

extern void InternalWarningFunc(const char * fn, unsigned ln, const std::string & str);
#define InternalWarning(expr) \
do { std::stringstream strm; strm << expr; InternalWarningFunc(__FILE__, __LINE__, strm.str()); } while (0)

extern void SourceWarningFunc(WarningCode code, unsigned line, const std::string & str); 
#define SourceWarning(code, line, expr) \
do { std::stringstream strm; strm << expr; \
  SourceWarningFunc(code, line, strm.str()); \
} while (0)

extern void WarningFunc(WarningCode code, const std::string & str);
#define Warning(code, expr) \
do { std::stringstream strm; strm << expr; WarningFunc(code, strm.str()); } while (0)

extern Basalt g_application;
extern int g_lexLineNumber;
extern bool g_compileOnly;
extern bool g_dump;
extern bool g_enableDebugging;
extern bool g_disableWarnings;
extern bool g_enableLineNumbers;

#endif // BASALT_H_
