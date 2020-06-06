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
                  const AST::Program & program);

    bool Run();
    virtual bool Body() = 0;

    virtual int Generate(const AST::Node & node) { }
    virtual int Generate(const AST::SourceLine & expr) { }
    virtual int Generate(const AST::Statement & statement) { }
    virtual int Generate(const AST::Print & expr) { }
    virtual int Generate(const AST::StringConstant & expr) { }
    virtual int Generate(const AST::Int16Constant & expr) { }
    virtual int Generate(const AST::Int32Constant & expr) { }
    virtual int Generate(const AST::SingleConstant & expr) { }
    virtual int Generate(const AST::DoubleConstant & expr) { }
    virtual int Generate(const AST::NumericAssign & expr) { }
    virtual int Generate(const AST::NumericVarRef & expr) { }
    virtual int Generate(const AST::NumericAddition & expr) { }
    virtual int Generate(const AST::Subtraction & expr) { }
    virtual int Generate(const AST::Multiplication & expr) { }
    virtual int Generate(const AST::Division & expr) { }
    virtual int Generate(const AST::Negation & expr) { }
    virtual int Generate(const AST::Power & expr) { }
    virtual int Generate(const AST::NumericCast & expr) { }
    virtual int Generate(const AST::StringAssign & expr) { }
    virtual int Generate(const AST::StringVarRef & expr) { }
    virtual int Generate(const AST::Goto & expr) { }

    virtual int Print(const AST::Node & node) { }
    virtual int Print(const AST::StringConstant & expr) { }
    virtual int Print(const AST::Int16Constant & expr) { }
    virtual int Print(const AST::Int32Constant & expr) { }
    virtual int Print(const AST::SingleConstant & expr) { }
    virtual int Print(const AST::DoubleConstant & expr) { }
    virtual int Print(const AST::PrintComma & expr) { }
    virtual int Print(const AST::PrintSemiColon & expr) { }
    virtual int Print(const AST::NumericVarRef & expr) { }
    virtual int Print(const AST::StringVarRef & expr) { }

    std::string m_inputFilename;
    std::ostream * m_outputStream;

  protected:
    bool CheckVars();

    std::string m_outputFilename; 
    const AST::Program & m_program;
    std::ofstream m_outputFile;
};

#endif // CODEGEN_H_

