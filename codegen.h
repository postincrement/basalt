#ifndef CODEGEN_H_
#define CODEGEN_H_

#include <fstream>
#include <set>
#include <string>

#include "ast.h"

class CodeGenerator
{
  public:
    CodeGenerator(const std::string & inputFilename, 
                  const std::string & outputFilename, 
                  AST::NodeList & program);

    bool Run();

    std::string m_inputFilename;
    std::ostream * m_outputStream;

    virtual void Prologue();
    virtual void Epilogue();
    virtual void Dispatch(AST::Node * node) = 0;

  protected:
    std::string m_outputFilename; 
    AST::NodeList & m_program;
    std::ofstream m_outputFile;
};


class C_CodeGenerator : public CodeGenerator
{
  public:
    C_CodeGenerator(const std::string & inputFilename, 
                    const std::string & outputFilename, 
                    AST::NodeList & program);
    virtual void Dispatch(AST::Node * node);
    virtual void Prologue();
    virtual void Epilogue();

    std::stringstream m_body;

    struct CVarDef {
      AST::VarType m_type;
      std::string m_cname;
      std::string m_ctype;
      std::string m_initializer;
    };

    bool DeclareGlobalVar(
      const AST::VarRef & var, 
      C_CodeGenerator::CVarDef & cvar
    );

  protected:
    typedef std::map<std::string, CVarDef> GlobalVarMap;
    std::set<std::string> m_cnames;
    GlobalVarMap m_globalVars;  
};

#endif // CODEGEN_H_

