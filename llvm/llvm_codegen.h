#ifndef LLVM_CODEGEN_H_
#define LLVM_CODEGEN_H_

#include "../codegen.h"

#include <map>
#include <sstream>
#include <string>
#include <vector>

class LLVM_CodeGenerator : public CodeGenerator
{
  public:
    LLVM_CodeGenerator();
    virtual bool Body() override;

    virtual int Generate(const AST::SourceLine & expr) override;
    virtual int Generate(const AST::End & expr) override;
    virtual int Generate(const AST::System & expr) override;
    virtual int Generate(const AST::Rem & expr) override;
    virtual int Generate(const AST::AssignStatement & expr) override;
    virtual int Generate(const AST::NumericAssign & expr) override;
    virtual int Generate(const AST::StringAssign & expr) override;
    virtual int Generate(const AST::IfStatement & expr) override;
    virtual int Generate(const AST::ForStatement & expr) override;
    virtual int Generate(const AST::NextStatement & expr) override;
    virtual int Generate(const AST::GotoStatement & expr) override;
    virtual int Generate(const AST::GosubStatement & expr) override;
    virtual int Generate(const AST::ReturnStatement & expr) override;
    virtual int Generate(const AST::Print & expr) override;
    virtual int Generate(const AST::OnGotoStatement & expr) override;
    virtual int Generate(const AST::ClearStatement & expr) override;
    virtual int Generate(const AST::WidthStatement & expr) override;
    virtual int Generate(const AST::InputStatement & expr) override;
    virtual int Generate(const AST::DimStatement & expr) override;
    virtual int Generate(const AST::DefStatement & expr) override;

    virtual int Evaluate(const AST::Int16Constant & expr, std::string & result) override;
    virtual int Evaluate(const AST::Int32Constant & expr, std::string & result) override;
    virtual int Evaluate(const AST::SingleConstant & expr, std::string & result) override;
    virtual int Evaluate(const AST::DoubleConstant & expr, std::string & result) override;
    virtual int Evaluate(const AST::StringConstant & expr, std::string & result) override;
    virtual int Evaluate(const AST::NumericVarRef & expr, std::string & result) override;
    virtual int Evaluate(const AST::StringVarRef & expr, std::string & result) override;
    virtual int Evaluate(const AST::NumericSubscript & expr, std::string & result) override;
    virtual int Evaluate(const AST::NumericAddition & expr, std::string & result) override;
    virtual int Evaluate(const AST::Subtraction & expr, std::string & result) override;
    virtual int Evaluate(const AST::Multiplication & expr, std::string & result) override;
    virtual int Evaluate(const AST::Division & expr, std::string & result) override;
    virtual int Evaluate(const AST::Power & expr, std::string & result) override;
    virtual int Evaluate(const AST::Negation & expr, std::string & result) override;
    virtual int Evaluate(const AST::NumericEquality & expr, std::string & result) override;
    virtual int Evaluate(const AST::NumericNotEquality & expr, std::string & result) override;
    virtual int Evaluate(const AST::NumericGreaterThan & expr, std::string & result) override;
    virtual int Evaluate(const AST::NumericGreaterThanEqual & expr, std::string & result) override;
    virtual int Evaluate(const AST::NumericLessThan & expr, std::string & result) override;
    virtual int Evaluate(const AST::NumericLessThanEqual & expr, std::string & result) override;
    virtual int Evaluate(const AST::LogicalAnd & expr, std::string & result) override;
    virtual int Evaluate(const AST::LogicalOr & expr, std::string & result) override;
    virtual int Evaluate(const AST::StringCompare & expr, std::string & result) override;
    virtual int Evaluate(const AST::IntFunction & expr, std::string & result) override;
    virtual int Evaluate(const AST::RndFunction & expr, std::string & result) override;
    virtual int Evaluate(const AST::NumericCast & expr, std::string & result) override;

    virtual int Print(const AST::NumericExpr & expr) override;
    virtual int Print(const AST::NumericVarRef & expr) override;
    virtual int Print(const AST::NumericSubscript & expr) override;
    virtual int Print(const AST::Int16Constant & expr) override;
    virtual int Print(const AST::Int32Constant & expr) override;
    virtual int Print(const AST::SingleConstant & expr) override;
    virtual int Print(const AST::DoubleConstant & expr) override;
    virtual int Print(const AST::StringConstant & expr) override;
    virtual int Print(const AST::StringVarRef & expr) override;
    virtual int Print(const AST::PrintComma & expr) override;

  private:
    struct Var {
      std::string global;
      VarType type = VarType::eNone;
      bool array = false;
      std::string agg;
      int count = 1;
    };
    struct ForFrame {
      std::string check;
      std::string after;
      std::string index;
      std::string toSlot;
      std::string stepSlot;
      VarType type = VarType::eSingle;
    };
    struct Cont {
      int id;
      std::string block;
    };

    std::stringstream m_decl;
    std::stringstream m_body;
    std::map<std::string, Var> m_vars;
    std::vector<ForFrame> m_fors;
    std::vector<Cont> m_conts;
    int m_tmp = 0;
    int m_blockId = 0;
    int m_forId = 0;
    int m_contId = 0;
    bool m_term = false;

    std::string Tmp();
    std::string Block(const std::string & hint);
    void Emit(const std::string & insn);
    void Term(const std::string & insn);
    std::string Eval(const AST::Expr & expr);
    std::string Conv(const std::string & value, VarType from, VarType to);
    std::string Bin(const char * iop, const char * fop, const AST::NumericBinaryOperation & expr);
    std::string Cmp(const char * iop, const char * fop, const AST::NumericBinaryOperation & expr);
    std::string Truth(const AST::NumericExpr & expr);
    std::string ElementPtr(const AST::NumericSubscript & expr);
    void StoreNumeric(const AST::NumericVarRef & lhs, const AST::NumericExpr & rhs);
    void PrintValue(VarType type, const std::string & value);
    void CallUser(const AST::NumericSubscript & expr, std::string & result, bool asFloat);
    static const char * Ty(VarType type);
    static bool IsFloat(VarType type);
    static std::string FLit(float value);
    static std::string DLit(double value);
    static std::string Zero(VarType type);
    static std::string Escape(const std::string & text);
};

#endif
