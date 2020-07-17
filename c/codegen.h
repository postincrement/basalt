#ifndef C_CODEGEN_H_
#define C_CODEGEN_H_

#include "../codegen.h"

class C_CodeGenerator : public CodeGenerator
{
  public:
    C_CodeGenerator();

    std::stringstream m_body;

    struct CVarDef {
      VarType m_type;
      std::string m_cname;
    };

    bool LookupGlobalVar(
      const std::string & varName, 
      CVarDef & cvar
    );

  protected:
    int NumericBinaryOperator    (const std::string & op, const AST::NumericBinaryOperation * expr, std::string & result);
    int NumericComparisonOperator(const std::string & op, const AST::NumericBinaryOperation * expr, std::string & result);
    int UnaryOperator        (const std::string & op, const AST::UnaryOperation * expr,         std::string & result);
    int NumericExpr          (const std::string & op, const AST::NumericExpr * expr,            std::string & result);
    int StringExpr           (const std::string & op, const AST::StringExpr * expr,             std::string & result);

    void CatStrings(const std::string & tempName,
                    const std::string & lhs, 
                    const std::string & rhs, 
                    const std::string & pre,
                    bool indent = true);

    typedef std::map<std::string, CVarDef> GlobalVarMap;
    std::set<std::string> m_cnames;
    GlobalVarMap m_globalVars;  

  public:   
    virtual bool Body() override;

    virtual int Generate(const AST::SourceLine & expr) override;
    //virtual int Generate(const AST::Statement & statement) override;

    virtual int Generate(const AST::Print & expr) override;
    virtual int Generate(const AST::NumericAssign & expr) override;
    virtual int Generate(const AST::StringAssign & expr) override;
    virtual int Generate(const AST::Goto & expr) override;
    virtual int Generate(const AST::End & expr) override;
    virtual int Generate(const AST::Rem & expr) override;
    virtual int Generate(const AST::IfStatement & expr) override;

    //virtual int Evaluate(const AST::StringConstant & expr, std::string & result) override;
    virtual int Evaluate(const AST::StringVarRef & expr, std::string & result) override;
    virtual int Evaluate(const AST::StrFunction & expr, std::string & result) override;
    //virtual int Evaluate(const AST::Int16Constant & expr, std::string & result) override;
    virtual int Evaluate(const AST::Int32Constant & expr, std::string & result) override;
    virtual int Evaluate(const AST::SingleConstant & expr, std::string & result) override;
    virtual int Evaluate(const AST::DoubleConstant & expr, std::string & result) override;
    virtual int Evaluate(const AST::NumericVarRef & expr, std::string & result) override;
    virtual int Evaluate(const AST::NumericAddition & expr, std::string & result) override;
    virtual int Evaluate(const AST::Subtraction & expr, std::string & result) override;
    virtual int Evaluate(const AST::Multiplication & expr, std::string & result) override;
    virtual int Evaluate(const AST::Division & expr, std::string & result) override;
    virtual int Evaluate(const AST::NumericEquality & expr, std::string & result) override;
    virtual int Evaluate(const AST::NumericNotEquality & expr, std::string & result) override;
    virtual int Evaluate(const AST::NumericGreaterThan & expr, std::string & result) override;
    virtual int Evaluate(const AST::NumericGreaterThanEqual & expr, std::string & result) override;
    virtual int Evaluate(const AST::NumericLessThan & expr, std::string & result) override;
    virtual int Evaluate(const AST::NumericLessThanEqual & expr, std::string & result) override;
    
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

    virtual int Print(const AST::NumericExpr & expr) override;
    virtual int Print(const AST::StringConstant & expr) override;
    virtual int Print(const AST::Int16Constant & expr) override;
    virtual int Print(const AST::Int32Constant & expr) override;
    virtual int Print(const AST::SingleConstant & expr) override;
    virtual int Print(const AST::DoubleConstant & expr) override;
    virtual int Print(const AST::PrintComma & expr) override;
    virtual int Print(const AST::NumericVarRef & expr) override;
    virtual int Print(const AST::StringVarRef & expr) override;
};

#endif // C_CODEGEN_H_