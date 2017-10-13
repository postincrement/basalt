
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
    char ReadNextChar();

    void OnError(const std::string & msg);
    void OnWarning(const std::string & msg);
    void DisplayError(const std::string & msg, const std::string & type);

    bool m_interactive;

    std::string m_progname;
    size_t m_lineOffs;
    std::string m_line;
    std::ifstream m_inputFile;
    Filename m_inputFilename;    
};


extern int MBASIC_lex();
extern int MBASIC_parse();
extern void MBASIC_error(const char * msg);
//extern FILE * MBASIC_in;
extern int MBASIC_debug;
extern void MBASIC_yyinput(char * buf, int * result, int maxSize);

extern Basalt g_application;
extern int g_lineNumber;
extern bool g_compileOnly;
extern bool g_dumpAsm;

#endif // BASALT_H_
