#ifndef CODEGEN_H_
#define CODEGEN_H_

#include <fstream>

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
};

#endif // CODEGEN_H_

