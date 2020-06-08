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
    virtual int Generate(const AST::NumericAssign & expr) override;
    virtual int Generate(const AST::StringAssign & expr) override;
    virtual int Generate(const AST::Goto & expr) override;

    virtual int Evaluate(const AST::Node & expr, std::string & result) override;
    virtual int Evaluate(const AST::StringConstant & expr, std::string & result) override;
    virtual int Evaluate(const AST::StringVarRef & expr, std::string & result) override;
    virtual int Evaluate(const AST::StrFunction & expr, std::string & result) override;
    virtual int Evaluate(const AST::Int16Constant & expr, std::string & result) override;
    virtual int Evaluate(const AST::Int32Constant & expr, std::string & result) override;
    virtual int Evaluate(const AST::SingleConstant & expr, std::string & result) override;
    virtual int Evaluate(const AST::DoubleConstant & expr, std::string & result) override;
    virtual int Evaluate(const AST::NumericVarRef & expr, std::string & result) override;
    virtual int Evaluate(const AST::NumericAddition & expr, std::string & result) override;
    virtual int Evaluate(const AST::Subtraction & expr, std::string & result) override;
    virtual int Evaluate(const AST::Multiplication & expr, std::string & result) override;
    virtual int Evaluate(const AST::Division & expr, std::string & result) override;
    virtual int Evaluate(const AST::Negation & expr, std::string & result) override;
    virtual int Evaluate(const AST::Power & expr, std::string & result) override;
    virtual int Evaluate(const AST::NumericCast & expr, std::string & result) override;
    virtual int Evaluate(const AST::IntFunction & expr, std::string & result) override;
    virtual int Evaluate(const AST::SqrFunction & expr, std::string & result) override;
    virtual int Evaluate(const AST::LenFunction & expr, std::string & result) override;
    virtual int Evaluate(const AST::TabFunction & expr, std::string & result) override;
    virtual int Evaluate(const AST::LeftFunction & expr, std::string & result) override;
    virtual int Evaluate(const AST::MidFunction & expr, std::string & result) override;
    virtual int Evaluate(const AST::RightFunction & expr, std::string & result) override;
    virtual int Evaluate(const AST::ChrFunction & expr, std::string & result) override;
    virtual int Evaluate(const AST::StringAddition & expr, std::string & result) override;

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
        Closure(int indent)
          : m_indent(indent)
        { 
        }

        ~Closure()
        {
        }

        std::string Indent(int n = 0) const
        {
          return std::string(m_indent + n, ' ');
        }

        int GetIndent() const
        { return m_indent; }

        std::stringstream & Output()
        { return m_output; }

      protected:
        int m_indent;
        std::stringstream m_output;
    };

    Closure & Top()
    {
      return m_stack[m_stack.size()-1];      
    }

    std::stringstream & TopOutput(bool indent = true)
    {
      if (indent)
        Top().Output() << Top().Indent();
      return Top().Output();
    }

    void PushClosure()
    {
      int indent = (m_stack.size() < 1) ? 2 : (Top().GetIndent()+2); 
      m_stack.emplace_back(indent);
      if (m_stack.size() > 1)
        TopOutput(false) << Top().Indent(-2) << "{\n";
    }

    std::string PopClosure()
    {
      std::string str;
      if (m_stack.size() > 1) {
        TopOutput(false) << Top().Indent(-2) << "}\n";
      }
      str = TopOutput(false).str();
      if (m_stack.size() > 0) {
        m_stack.pop_back();
      }
      if (m_stack.size() > 0) {
        TopOutput(false) << str;
      }
      return str;  
    }

    std::deque<Closure> m_stack;

  protected:
    int NumericBinaryOperator(const std::string & op, const AST::NumericBinaryOperation * expr, std::string & result);
    int UnaryOperator        (const std::string & op, const AST::UnaryOperation * expr,         std::string & result);
    int NumericExpr          (const std::string & op, const AST::NumericExpr * expr,            std::string & result);
    int StringExpr           (const std::string & op, const AST::StringExpr * expr,             std::string & result);

    typedef std::map<std::string, CVarDef> GlobalVarMap;
    std::set<std::string> m_cnames;
    GlobalVarMap m_globalVars;  
};

#endif // C_CODEGEN_H_