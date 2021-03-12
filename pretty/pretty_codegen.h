#ifndef PRETTY_CODEGEN_H_
#define PRETTY_CODEGEN_H_

#include "../src/codegen.h"

class Pretty_CodeGenerator : public CodeGenerator
{
  public:
    Pretty_CodeGenerator(const AST::Parser & parser);

    virtual bool Body() override;

    std::stringstream m_output;

    virtual std::string CreateTempVar(VarType type) override;

    virtual int PrintNewLine() override;
    virtual int PrintTab() override;

    virtual int PrintStringVar(const std::string & name) override;
    virtual int PrintStringConst(const std::string & str) override;

    virtual int PrintNumericVar(const std::string & name, VarType type) override;
    virtual int PrintNumericConst(const std::string & value, VarType type) override;

    virtual int NumericAssign(const std::string & lhs, const std::string & rhs, VarType type) override;    
    virtual std::string NumericFunction(const std::string & op, const std::string & arg, VarType type) override;

    virtual int StringAssign(const std::string & lhs, const std::string & rhs) override;
    virtual std::string StringFunction(const std::string & op, const std::string & arg) override;

#if 0
    virtual int Generate(const AST::SourceLine & expr) override;
//    virtual int Generate(const AST::Statement & statement) override;
    virtual int Generate(const AST::End & expr) override;
    virtual int Generate(const AST::Rem & expr) override;
    virtual int Generate(const AST::AssignStatement & expr) override;
    
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

    virtual int Print(const AST::Int16Constant & expr) override;
    virtual int Print(const AST::Int32Constant & expr) override;
    virtual int Print(const AST::SingleConstant & expr) override;
    virtual int Print(const AST::DoubleConstant & expr) override;
#endif

};

#endif // Z80_CODEGEN_H_