
#ifndef BASALT_H_
#define BASALT_H_

#include <string>
#include <fstream>

#include "codegen.h"
#include "common.h"
#include "errorcode.h"

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

    void InternalErrorInternal(ErrorCode code, const std::string & msg);
    void ParserErrorInternal(ErrorCode code, const std::string & msg);
    void CompilerErrorInternal(ErrorCode code, int ln, const std::string & msg);

    std::string GetCurrentLine() const { return m_line; }

    void SetStatementStart();
    std::string GetStatement();

  protected:    
    std::string FormatError(ErrorCode code, unsigned ln, int pos = -1);

    std::string m_progname;

    int m_verbose = 0;
    int m_displayHelp = 0;
    std::string m_codeGeneratorName;
    std::string m_outputFilename;
    std::string m_languageProfileName;
    std::string m_arch;
    bool g_disableWarnings = false;
    bool g_enableLineNumbers = false;

    // context for source files
    size_t m_lineOffs;
    size_t m_statementStart;
    std::string m_line;
    std::istream * m_inputStream = nullptr;
    std::ifstream m_inputFile;
    Filename m_inputFilename; 
    std::string g_printableInputFilename;
    unsigned m_errorCount = 0;
    unsigned m_warningCount = 0;
};


extern int MBASIC_lex();
extern int MBASIC_parse();
extern int MBASIC_debug;
extern void MBASIC_yyinput(char * buf, int * result, int maxSize);

///////////////////////////////////////////////////////////////////////////////////////////////////


// declared in basalt.cc
extern Basalt g_application;

// declared in mbasic.ypp
extern unsigned g_lexLineNumber;
extern std::string g_basicLineNumber;

#endif // BASALT_H_
