#ifndef C_CODEGEN_H_
#define C_CODEGEN_H_

#include "codegen.h"

class C_CodeGenerator : public CodeGenerator
{
  public:
    C_CodeGenerator(const std::string & inputFilename, 
                    const std::string & outputFilename, 
                    const AST::Program & program);

    virtual bool Body() override;

    virtual int Generate(const AST::Node & node) override;
    virtual int Generate(const AST::SourceLine & expr) override;
    virtual int Generate(const AST::Statement & statement) override;
    virtual int Generate(const AST::Print & expr) override;
    virtual int Generate(const AST::StringConstant & expr) override;
    virtual int Generate(const AST::Int16Constant & expr) override;
    virtual int Generate(const AST::Int32Constant & expr) override;
    virtual int Generate(const AST::SingleConstant & expr) override;
    virtual int Generate(const AST::DoubleConstant & expr) override;
    virtual int Generate(const AST::Assign & expr) override;
    virtual int Generate(const AST::VarRef & expr) override;
    virtual int Generate(const AST::Addition & expr) override;
    virtual int Generate(const AST::Subtraction & expr) override;
    virtual int Generate(const AST::Multiplication & expr) override;
    virtual int Generate(const AST::Division & expr) override;
    virtual int Generate(const AST::Negation & expr) override;
    virtual int Generate(const AST::Power & expr) override;

    virtual int Print(const AST::Node & node) override;
    virtual int Print(const AST::StringConstant & expr) override;
    virtual int Print(const AST::Int16Constant & expr) override;
    virtual int Print(const AST::Int32Constant & expr) override;
    virtual int Print(const AST::SingleConstant & expr) override;
    virtual int Print(const AST::DoubleConstant & expr) override;
    virtual int Print(const AST::PrintComma & expr) override;
    virtual int Print(const AST::PrintSemiColon & expr) override;
    virtual int Print(const AST::VarRef & expr) override;

    std::stringstream m_body;

    struct CVarDef {
      VarType m_type;
      std::string m_cname;
    };

    bool LookupGlobalVar(
      const AST::VarRef & var, 
      C_CodeGenerator::CVarDef & cvar
    );

    class Closure {
      public:
        std::stringstream & Output()
        { return m_output; }

      protected:  
        std::stringstream m_output;
    };

    Closure & Top()
    {
      return m_stack[m_stack.size()-1];      
    }

    std::stringstream & TopOutput()
    {
      return Top().Output();
    }

    void PushClosure()
    {
      m_stack.push_back(Closure());
    }

    void PopClosure()
    {
      m_stack.pop_back();
    }

    std::deque<Closure> m_stack;

  protected:
    int BinaryOperator(const std::string & op, const AST::BinaryOperation & expr);
    int UnaryOperator(const std::string & op, const AST::UnaryOperation & expr);

    typedef std::map<std::string, CVarDef> GlobalVarMap;
    std::set<std::string> m_cnames;
    GlobalVarMap m_globalVars;  
};

#endif // C_CODEGEN_H_