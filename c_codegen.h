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
    virtual int Generate(const AST::NumericAssign & expr) override;
    virtual int Generate(const AST::NumericVarRef & expr) override;
    virtual int Generate(const AST::NumericAddition & expr) override;
    virtual int Generate(const AST::Subtraction & expr) override;
    virtual int Generate(const AST::Multiplication & expr) override;
    virtual int Generate(const AST::Division & expr) override;
    virtual int Generate(const AST::Negation & expr) override;
    virtual int Generate(const AST::Power & expr) override;
    virtual int Generate(const AST::NumericCast & expr) override;
    virtual int Generate(const AST::StringAssign & expr) override;
    virtual int Generate(const AST::StringVarRef & expr) override;
    virtual int Generate(const AST::Goto & expr) override;
    virtual int Generate(const AST::IntFunction & expr) override;
    virtual int Generate(const AST::SqrFunction & expr) override;
    virtual int Generate(const AST::LenFunction & expr) override;
    virtual int Generate(const AST::TabFunction & expr) override;
    virtual int Generate(const AST::LeftFunction & expr) override;
    virtual int Generate(const AST::MidFunction & expr) override;
    virtual int Generate(const AST::RightFunction & expr) override;
    virtual int Generate(const AST::ChrFunction & expr) override;
    virtual int Generate(const AST::StrFunction & expr) override;

    virtual int Print(const AST::Node & node) override;
    virtual int Print(const AST::StringConstant & expr) override;
    virtual int Print(const AST::Int16Constant & expr) override;
    virtual int Print(const AST::Int32Constant & expr) override;
    virtual int Print(const AST::SingleConstant & expr) override;
    virtual int Print(const AST::DoubleConstant & expr) override;
    virtual int Print(const AST::PrintComma & expr) override;
    virtual int Print(const AST::PrintSemiColon & expr) override;
    virtual int Print(const AST::NumericVarRef & expr) override;
    virtual int Print(const AST::StringVarRef & expr) override;

    std::stringstream m_body;

    struct CVarDef {
      VarType m_type;
      std::string m_cname;
    };

    bool LookupGlobalVar(
      const std::string & varName, 
      C_CodeGenerator::CVarDef & cvar
    );

    class Closure {
      public:
        Closure(bool open = false)
          : m_open(open)
        { 
          if (m_open)
            m_output << "{\n";
        }

        Closure(const Closure & obj)
          : m_open(obj.m_open)
        { 
          if (m_open)
            m_output << "{\n";
        }

        ~Closure()
        {
          if (m_open)
            m_output << "}\n";
        }

        std::stringstream & Output()
        { return m_output; }

      protected:
        bool m_open = false;  
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

    void PushClosure(bool open = false)
    {
      Closure closure(open);
      m_stack.push_back(closure);
    }

    void PopClosure()
    {
      m_stack.pop_back();
    }

    std::deque<Closure> m_stack;

  protected:
    int NumericBinaryOperator(const AST::Expr * lhs,
                       std::string & lhStr,
                       const AST::Expr * rhs,
                       std::string & rhStr);
                       
    int NumericBinaryOperator(const std::string & op, const AST::NumericBinaryOperation & expr);
    int UnaryOperator(const std::string & op, const AST::UnaryOperation & expr);

    typedef std::map<std::string, CVarDef> GlobalVarMap;
    std::set<std::string> m_cnames;
    GlobalVarMap m_globalVars;  
};

#endif // C_CODEGEN_H_