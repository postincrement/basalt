#ifndef Z80_CODEGEN_H_
#define Z80_CODEGEN_H_

#include "../codegen.h"

class Z80_CodeGenerator : public CodeGenerator
{
  public:
    Z80_CodeGenerator();

    virtual std::string GetOutputFileExtension() const override;
                    
    struct AsmVarDef {
      VarType m_type;
      std::string m_aname;
    };

    bool LookupGlobalVar(
      const std::string & varName, 
      AsmVarDef & cvar
    );

  protected:
    typedef std::map<std::string, AsmVarDef> GlobalVarMap;
    std::set<std::string> m_anames;
    GlobalVarMap m_globalVars;  
  
  public:
    virtual bool Body() override;

//    virtual int Generate(const AST::SourceLine & expr) override;
    virtual int Generate(const AST::Statement & statement) override;
    virtual int Generate(const AST::End & expr) override;
/*    
    virtual int Generate(const AST::Print & expr) override;
    virtual int Generate(const AST::NumericAssign & expr) override;
    virtual int Generate(const AST::StringAssign & expr) override;
    virtual int Generate(const AST::Goto & expr) override;

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

    virtual int Print(const AST::StringConstant & expr) override;
    virtual int Print(const AST::Int16Constant & expr) override;
    virtual int Print(const AST::Int32Constant & expr) override;
    virtual int Print(const AST::SingleConstant & expr) override;
    virtual int Print(const AST::DoubleConstant & expr) override;
    virtual int Print(const AST::PrintComma & expr) override;
    virtual int Print(const AST::PrintSemiColon & expr) override;
    virtual int Print(const AST::NumericVarRef & expr) override;
    virtual int Print(const AST::StringVarRef & expr) override;
  */  
};

#endif // Z80_CODEGEN_H_