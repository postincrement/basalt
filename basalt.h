
#ifndef BASALT_H_
#define BASALT_H_

#include <string>

#include "codegen.h"

class Filename : public std::string
{
  public:
    Filename()
    { }

    Filename(const std::string & str)
      : std::string(str)
    { }

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
        m_fullVarNames = true;
        break;
      case Basic80_5_0_Disk:
        m_fullVarNames = true;
        break;
      case Basic80_5_0_8k:
      default:
        m_fullVarNames = false;
        break;
    } 
  }

  bool m_fullVarNames;  
};

extern int MBASIC_lex();
extern int MBASIC_parse();
extern void MBASIC_error(const char * msg);
extern FILE * MBASIC_in;
extern int MBASIC_debug;

extern int g_lineNumber;
extern Filename g_inputFilename;

extern LanguageProfile g_profile;

extern std::map<std::string, CodeGenerator::ASTExpr *> m_globalStringConstants;
extern CodeGenerator::VariableList g_globals;
extern CodeGenerator::ASTExprList g_expressions;

#endif // BASALT_H_
