#ifndef Z80_CODEGEN_H_
#define Z80_CODEGEN_H_

#include "../codegen.h"

class Z80_CodeGenerator : public CodeGenerator
{
  public:
    Z80_CodeGenerator();

    struct AsmVarDef {
      VarType m_type;
      std::string m_aname;
    };

    bool LookupGlobalVar(
      const std::string & varName, 
      AsmVarDef & cvar
    );

    void LoadHL(const std::string & val = "")
    { LoadRegPair("hl", val); }

    void LoadDE(const std::string & val = "")
    { LoadRegPair("de", val); }

    void LoadBC(const std::string & val = "")
    { LoadRegPair("bc", val); }

    void LoadB(const std::string & val = "")
    { LoadReg('b', val); }

    void LoadC(const std::string & val = "")
    { LoadReg('c', val); }

    void LoadD(const std::string & val = "")
    { LoadReg('d', val); }

    void LoadE(const std::string & val = "")
    { LoadReg('e', val); }

    void LoadH(const std::string & val = "")
    { LoadReg('h', val); }

    void LoadL(const std::string & val = "")
    { LoadReg('l', val); }

    void LoadReg(char reg, const std::string & val);
    void LoadRegPair(const std::string & regPair, const std::string & val);

    void AssignExprToHL(const AST::Expr & expr)
    { return AssignExprToRegPair("hl", expr); }

    void AssignExprToRegPair(const std::string & regPair, const AST::Expr & expr);

    int EvaluateBinaryOperands(const AST::NumericBinaryOperation & expr, bool commutative);
    int NumericComparisonOperator(const AST::NumericBinaryOperation & expr, const std::string & funcBase);

  protected:
    typedef std::map<std::string, AsmVarDef> GlobalVarMap;
    std::set<std::string> m_anames;
    GlobalVarMap m_globalVars;

    std::set<std::string> m_funcsUsed;
    std::map<std::string, std::string> m_regs;

    struct ForBlock {
      std::string m_ref;
      VarType m_type;
      std::string m_index;
      const AST::Expr * m_to;
      const AST::Expr * m_step;
    };

    std::deque<ForBlock> m_forQueue;    

  public:
    virtual bool Body() override;

    virtual int Generate(const AST::SourceLine & expr) override;
//    virtual int Generate(const AST::Statement & statement) override;
    virtual int Generate(const AST::End & expr) override;
    virtual int Generate(const AST::Rem & expr) override;
    
    virtual int Generate(const AST::Print & expr) override;
    virtual int Generate(const AST::NumericAssign & expr) override;
    //virtual int Generate(const AST::StringAssign & expr) override;    
    virtual int Generate(const AST::GotoStatement & expr) override;
    virtual int Generate(const AST::IfStatement & expr) override;
    virtual int Generate(const AST::ForStatement & expr) override;
    virtual int Generate(const AST::NextStatement & expr) override;

    //virtual int Evaluate(const AST::StringConstant & expr, std::string & result) override;
    //virtual int Evaluate(const AST::StringVarRef & expr, std::string & result) override;
    //virtual int Evaluate(const AST::StrFunction & expr, std::string & result) override;
    //virtual int Evaluate(const AST::Int16Constant & expr, std::string & result) override;
    //virtual int Evaluate(const AST::Int32Constant & expr, std::string & result) override;
    //virtual int Evaluate(const AST::SingleConstant & expr, std::string & result) override;
    //virtual int Evaluate(const AST::DoubleConstant & expr, std::string & result) override;
    virtual int Evaluate(const AST::NumericVarRef & expr, std::string & result) override;
    virtual int Evaluate(const AST::NumericAddition & expr, std::string & result) override;
    virtual int Evaluate(const AST::Subtraction & expr, std::string & result) override;
/*    virtual int Evaluate(const AST::Multiplication & expr, std::string & result) override;
    virtual int Evaluate(const AST::Division & expr, std::string & result) override;
*/
    virtual int Evaluate(const AST::NumericEquality & expr, std::string & result) override;
    virtual int Evaluate(const AST::NumericNotEquality & expr, std::string & result) override;
    virtual int Evaluate(const AST::NumericGreaterThan & expr, std::string & result) override;
    virtual int Evaluate(const AST::NumericGreaterThanEqual & expr, std::string & result) override;
    virtual int Evaluate(const AST::NumericLessThan & expr, std::string & result) override;
    virtual int Evaluate(const AST::NumericLessThanEqual & expr, std::string & result) override;
/*
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
*/
    virtual int Print(const AST::NumericExpr & expr) override;
    virtual int Print(const AST::StringConstant & expr) override;
    virtual int Print(const AST::Int16Constant & expr) override;
    //virtual int Print(const AST::Int32Constant & expr) override;
    //virtual int Print(const AST::SingleConstant & expr) override;
    //virtual int Print(const AST::DoubleConstant & expr) override;
    virtual int Print(const AST::PrintComma & expr) override;
    virtual int Print(const AST::NumericVarRef & expr) override;
    //virtual int Print(const AST::StringVarRef & expr) override;
};

#endif // Z80_CODEGEN_H_