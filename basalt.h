
#ifndef BASALT_H_
#define BASALT_H_

#include <string>
#include <fstream>

#include "codegen.h"
#include "ast.h"

class Filename : public std::string
{
  public:
    Filename()
    { }

    Filename(const std::string & str)
      : std::string(str)
    { }

    Filename & operator =(const std::string & str)
    { this->std::string::operator=(str); return *this; }

    std::string GetDir() const
    { 
      size_t pos = find_last_of('/');
      if (pos == std::string::npos)
        return "";
      return substr(0, pos);
    }

    std::string GetFilename() const
    { 
      std::string fn;
      size_t pos = find_last_of('/');
      if (pos == std::string::npos)
        fn = *this;
      else
        fn = substr(pos+1);
      return fn;
    }

    std::string GetBasename() const
    { 
      std::string base = GetFilename();      
      size_t pos = base.find_last_of('.');      
      if (pos != std::string::npos)
        base = base.substr(0, pos);
      return base;
    }

    std::string GetExtension() const
    { 
      std::string ext = GetFilename();      
      size_t pos = ext.find_last_of('.');      
      if (pos == std::string::npos)
        return "";
      return ext.substr(pos);
    }
};

struct LanguageProfile
{
  enum Dialect {
    Basic80_5_0_8k,
    Basic80_5_0_Extended,
    Basic80_5_0_Disk
  };

  LanguageProfile(Dialect dialect = Basic80_5_0_8k)
  {
    Set(dialect);
  }

  void Set(Dialect dialect = Basic80_5_0_8k)
  {
    switch (dialect) {
      case Basic80_5_0_Extended:
        m_identifierLen = 40;
        break;
      case Basic80_5_0_Disk:
        m_identifierLen = 40;
        break;
      case Basic80_5_0_8k:
      default:
        m_identifierLen = 2;
        break;
    } 
  }

  size_t m_identifierLen;
};

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
    void DecodeOpt(ArgDef * def);
    void Usage(const ArgDef * defs);
    char ReadNextChar();

    void OnError(const std::string & msg);
    void OnWarning(const std::string & msg);
    void DisplayError(const std::string & msg, const std::string & type);

    bool m_interactive;

    std::string m_progname;
    size_t m_lineOffs;
    std::string m_line;
    std::ifstream m_inputFile;
};


extern int MBASIC_lex();
extern int MBASIC_parse();
extern void MBASIC_error(const char * msg);
//extern FILE * MBASIC_in;
extern int MBASIC_debug;
extern void MBASIC_yyinput(char * buf, int * result, int maxSize);

extern Basalt g_application;
extern int g_lineNumber;
extern Filename g_inputFilename;

extern LanguageProfile g_profile;

#endif // BASALT_H_
