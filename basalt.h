
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

extern int MBASIC_lex();
extern int MBASIC_parse();
extern void MBASIC_error(const char * msg);
extern FILE * MBASIC_in;
extern int MBASIC_debug;

extern int g_lineNumber;
extern Filename g_inputFilename;

extern CodeGenerator::ASTExprList g_expressions;


#endif // BASALT_H_
