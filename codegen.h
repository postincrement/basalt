#ifndef CODEGEN_H_
#define CODEGEN_H_

#include <fstream>
#include <set>
#include <string>

#include "ast.h"

class CodeGenerator
{
  public:
    CodeGenerator(const char * tempPrefix = nullptr);

    struct Closure;

    virtual std::string GetOutputFileExtension() const = 0;              
    virtual Closure * CreateClosure(int indent) const;

    bool Run(const std::string & inputFilename, std::ostream * outputStream, const AST::Program & program);
    virtual bool Body() = 0;

    virtual int Generate(const AST::Node & node);
    virtual int Evaluate(const AST::Node & expr, std::string & result);
    virtual int Print(const AST::Node & node);

    virtual int Generate(const AST::SourceLine & expr);     // iterate through statements
    virtual int Generate(const AST::Statement & statement); // iterate through expressions 

    virtual int Generate(const AST::Print & expr) { }
    virtual int Generate(const AST::NumericAssign & expr) { }
    virtual int Generate(const AST::StringAssign & expr) { }
    virtual int Generate(const AST::Goto & expr) { }
    virtual int Generate(const AST::End & expr) { }

    virtual int Evaluate(const AST::StringConstant & expr, std::string & result) { }
    virtual int Evaluate(const AST::StringVarRef & expr, std::string & result) { }
    virtual int Evaluate(const AST::StrFunction & expr, std::string & result) { }
    virtual int Evaluate(const AST::Int16Constant & expr, std::string & result) { }
    virtual int Evaluate(const AST::Int32Constant & expr, std::string & result) { }
    virtual int Evaluate(const AST::SingleConstant & expr, std::string & result) { }
    virtual int Evaluate(const AST::DoubleConstant & expr, std::string & result) { }
    virtual int Evaluate(const AST::NumericVarRef & expr, std::string & result) { }
    virtual int Evaluate(const AST::NumericAddition & expr, std::string & result) { }
    virtual int Evaluate(const AST::Subtraction & expr, std::string & result) { }
    virtual int Evaluate(const AST::Multiplication & expr, std::string & result) { }
    virtual int Evaluate(const AST::Division & expr, std::string & result) { }
    virtual int Evaluate(const AST::Negation & expr, std::string & result) { }
    virtual int Evaluate(const AST::Power & expr, std::string & result) { }
    virtual int Evaluate(const AST::NumericCast & expr, std::string & result) { }
    virtual int Evaluate(const AST::IntFunction & expr, std::string & result) { }
    virtual int Evaluate(const AST::SqrFunction & expr, std::string & result) { }
    virtual int Evaluate(const AST::LenFunction & expr, std::string & result) { }
    virtual int Evaluate(const AST::TabFunction & expr, std::string & result) { }
    virtual int Evaluate(const AST::LeftFunction & expr, std::string & result) { }
    virtual int Evaluate(const AST::MidFunction & expr, std::string & result) { }
    virtual int Evaluate(const AST::RightFunction & expr, std::string & result) { }
    virtual int Evaluate(const AST::ChrFunction & expr, std::string & result) { }
    virtual int Evaluate(const AST::StringAddition & expr, std::string & result) { }

    virtual int Print(const AST::StringConstant & expr) { }
    virtual int Print(const AST::Int16Constant & expr) { }
    virtual int Print(const AST::Int32Constant & expr) { }
    virtual int Print(const AST::SingleConstant & expr) { }
    virtual int Print(const AST::DoubleConstant & expr) { }
    virtual int Print(const AST::PrintComma & expr) { }
    virtual int Print(const AST::PrintSemiColon & expr) { }
    virtual int Print(const AST::NumericVarRef & expr) { }
    virtual int Print(const AST::StringVarRef & expr) { }

    class Closure {
      public:
        Closure(const std::string & tempPrefix, int indent);
        ~Closure();

        std::string Indent(int n = 0) const;
        int GetIndent() const;

        std::stringstream & Output();
        std::string GetTempName();

      protected:
        std::string m_tempPrefix;
        int m_indent;
        std::stringstream m_output;
        int m_tempIndex = 1;
    };

    Closure & Top();
    std::stringstream & TopOutput(bool indent = true);
    void Push();
    std::string Pop();

  protected:
    bool CheckVars();

    std::string m_tempPrefix;
    std::string m_inputFilename;

    const AST::Program * m_program;
    std::ostream * m_outputStream;
    std::deque<Closure *> m_stack;
};

#endif // CODEGEN_H_

