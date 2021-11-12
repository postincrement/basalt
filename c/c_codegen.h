#ifndef C_CODEGEN_H_
#define C_CODEGEN_H_

#include <deque>

#include "outputgen.h"

class C_OutputGenerator : public OutputGenerator
{
  public:
    C_OutputGenerator(CodeGenerator & pseudo);

    virtual void OutputFilePrologue(std::ostream & strm) override;
    virtual void OutputFileEpilogue(std::ostream & strm) override;

    virtual int Generate(CodeGenerator::Node & node) override;

    virtual int Generate(CodeGenerator::LineNumber & node) override;
    virtual int Generate(CodeGenerator::BlockStart & node) override;
    virtual int Generate(CodeGenerator::BlockEnd & node) override;

    virtual int Generate(CodeGenerator::CreateTempVar & node) override;

    virtual int Generate(CodeGenerator::PrintNewLine & node) override;
    virtual int Generate(CodeGenerator::PrintTab & node) override;
    virtual int Generate(CodeGenerator::PrintStringConst & node) override;
    virtual int Generate(CodeGenerator::PrintStringVar & node) override;

    virtual int Generate(CodeGenerator::PrintNumericConst & node) override;
    virtual int Generate(CodeGenerator::PrintNumericVar & node) override;

    virtual int Generate(CodeGenerator::UnaryOperator & node) override;
    virtual int Generate(CodeGenerator::BinaryOperator & node) override;

  protected:
    std::queue<std::stringstream *> m_blockStack; 

    int OutputFunc(CodeGenerator::Node & node);
   
#if 0
    std::stringstream m_body;

    enum {
      eOp_NextStatement = 100,
      eOp_NextLine,
      eOp_EndProgram,
      eOp_Goto,
      eOp_Next,
      eOp_Return
    };

    struct CVarDef {
      VarType m_type;
      std::string m_cname;
    };

    bool LookupGlobalVar(
      const std::string & varName, 
      CVarDef & cvar
    );

    struct CodeBlock
    {
      std::stringstream m_body;
      std::string m_ref;
      bool m_ended = false;
    };

    void Generate();
    void OutputRuntimeDecls(std::ostream & strm);
    void OutputRuntime(std::ostream & strm);
    void OutputBlocks();

  protected:
    void GenerateLine(int index, const AST::SourceLine & line);
    int GenerateStatement(int index, 
           const std::string & basicLineNumber,
                          bool isLastStatementOnLine, 
        const AST::Statement & statement);

    const AST::SourceLine * m_nextLine;
    bool m_lastBlockHadReturn;
    bool m_startBlock = false;
    std::string m_nextBlockRef;

    void StartBlock(const std::string & ref, bool autoEnd = true);
    void EndBlock();

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

    std::vector<CodeBlock> m_codeBlocks;
    int m_currentBlock = -1;

    struct ForBlock {
      std::string m_index;  // name of index variable
      std::string m_ctype;
      bool m_isConst;
      std::string m_ref;
    };

    std::deque<ForBlock> m_forQueue;
    bool m_isLastStatementOnLine = false;
    bool m_isLastStatement = false;

    std::set<std::string> m_funcsUsed;

  public:   
    virtual bool Body() override;

    virtual int Generate(const AST::SourceLine & expr) override;
    //virtual int Generate(const AST::Statement & statement) override;

    virtual int Generate(const AST::Print & expr) override;
    virtual int Generate(const AST::NumericAssign & expr) override;
    virtual int Generate(const AST::StringAssign & expr) override;
    virtual int Generate(const AST::GotoStatement & expr) override;
    virtual int Generate(const AST::End & expr) override;
    virtual int Generate(const AST::System & expr) override;
    virtual int Generate(const AST::Rem & expr) override;
    virtual int Generate(const AST::AssignStatement & expr) override;
    virtual int Generate(const AST::IfStatement & expr) override;
    virtual int Generate(const AST::ForStatement & expr) override;
    virtual int Generate(const AST::NextStatement & expr) override;
    virtual int Generate(const AST::GosubStatement & expr) override;
    virtual int Generate(const AST::ReturnStatement & expr) override;

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
#endif    
};

#endif // C_CODEGEN_H_