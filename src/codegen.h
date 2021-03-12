#ifndef CODEGEN_H_
#define CODEGEN_H_

#include "pass.h"

#define CompilerError(code, ln, expr) \
do { std::stringstream strm; strm << expr; \
  CompilerErrorInternal(ErrorCode::code, ln, strm.str()); \
} while (0)

#define InternalError(expr) \
do { std::stringstream strm; strm << expr; \
  g_application.InternalErrorInternal(ErrorCode::eInternalError, strm.str()); \
} while (0)

std::string DemangleTypeName(const std::type_info &r);

class CodeGenerator
{
  public:
    struct Config {
      const char * m_extension = "";
      const char * m_tempPrefix = "";
      const char * m_open = "";
      const char * m_close = "";
      int m_indent = 2;
      int m_indentInc = 2;
    };

    CodeGenerator(const Config & config, const AST::Parser & parser);

    const Config & GetConfig() const;

    bool Run(const std::string & inputFilename, std::ostream * outputStream);
    virtual bool Body() = 0;

    void SetLineNumber(int lineNumber)
    { m_lineNumber = lineNumber; }

    bool LookupGlobalVar(const std::string & varName, AST::VarInfo & var);

    // mandatory overrides
    virtual std::string CreateTempVar(VarType type) = 0;
    virtual void DestroyTempVar(const std::string & name) { };

    virtual int NumericAssign(const std::string & lhs, const std::string & rhs, VarType type) = 0;
    virtual std::string NumericFunction(const std::string & op, const std::string & arg, VarType type) = 0;
    virtual std::string StringFunction(const std::string & op, const std::string & arg) = 0;

    virtual int StringAssign(const std::string & lhs, const std::string & rhs) = 0;

    virtual int PrintNewLine() = 0;
    virtual int PrintTab() = 0;
    virtual int PrintNumericVar(const std::string & name, VarType type) = 0;
    virtual int PrintNumericConst(const std::string & name, VarType type) = 0;
    virtual int PrintStringVar(const std::string & name) = 0;
    virtual int PrintStringConst(const std::string & str) = 0;

    // optional overrides
    virtual int Generate(const AST::Node & node);
    virtual int Evaluate(const AST::Node & expr, std::string & result);
    virtual int Print(const AST::Node & node);

    virtual int Generate(const AST::SourceLine & expr);     // iterate through statements
    virtual int Generate(const AST::Statement & statement); // default for unimplemented statements
    virtual int Generate(const AST::Print & expr);          // iterate through expressions

    virtual int Generate(const AST::StringAssign & expr);

    virtual int Generate(const AST::NumericAssign & expr) { return 0; }
    virtual int Generate(const AST::GotoStatement & expr) { return 0; }
    virtual int Generate(const AST::End & expr) { return 0; }
    virtual int Generate(const AST::System & expr) { return 0; }
    virtual int Generate(const AST::Rem & expr) { return 0; }
    virtual int Generate(const AST::AssignStatement & expr) { return 0; }
    virtual int Generate(const AST::IfStatement & expr) { return 0; }
    virtual int Generate(const AST::ForStatement & expr) { return 0; }
    virtual int Generate(const AST::NextStatement & expr) { return 0; }
    virtual int Generate(const AST::GosubStatement & expr) { return 0; }
    virtual int Generate(const AST::ReturnStatement & expr) { return 0; }

    virtual int Evaluate(const AST::StringConstant & expr, std::string & result);
    virtual int Evaluate(const AST::StringVarRef & expr, std::string & result);

    virtual int Evaluate(const AST::NumericVarRef & expr, std::string & result);
    virtual int Evaluate(const AST::NumericConstant & expr, std::string & result);

    virtual int Evaluate(const AST::StrFunction & expr, std::string & result) { return 0; }
    virtual int Evaluate(const AST::NumericAddition & expr, std::string & result) { return 0; }
    virtual int Evaluate(const AST::Subtraction & expr, std::string & result) { return 0; }
    virtual int Evaluate(const AST::Multiplication & expr, std::string & result) { return 0; }
    virtual int Evaluate(const AST::Division & expr, std::string & result) { return 0; }
    virtual int Evaluate(const AST::NumericEquality & expr, std::string & result) { return 0; }
    virtual int Evaluate(const AST::NumericNotEquality & expr, std::string & result) { return 0; }
    virtual int Evaluate(const AST::NumericGreaterThan & expr, std::string & result) { return 0; }
    virtual int Evaluate(const AST::NumericGreaterThanEqual & expr, std::string & result) { return 0; }
    virtual int Evaluate(const AST::NumericLessThan & expr, std::string & result) { return 0; }
    virtual int Evaluate(const AST::NumericLessThanEqual & expr, std::string & result) { return 0; }
    virtual int Evaluate(const AST::Negation & expr, std::string & result) { return 0; }
    virtual int Evaluate(const AST::Power & expr, std::string & result) { return 0; }
    virtual int Evaluate(const AST::NumericCast & expr, std::string & result) { return 0; }

    virtual int Evaluate(const AST::IntFunction & expr, std::string & result);
    virtual int Evaluate(const AST::SqrFunction & expr, std::string & result);
    virtual int Evaluate(const AST::ChrFunction & expr, std::string & result);
    virtual int Evaluate(const AST::TabFunction & expr, std::string & result);

    virtual int Evaluate(const AST::LenFunction & expr, std::string & result) { return 0; }
    virtual int Evaluate(const AST::LeftFunction & expr, std::string & result) { return 0; }
    virtual int Evaluate(const AST::MidFunction & explepr, std::string & result) { return 0; }
    virtual int Evaluate(const AST::RightFunction & expr, std::string & result) { return 0; }
    virtual int Evaluate(const AST::StringAddition & expr, std::string & result) { return 0; }

    virtual int Print(const AST::NumericExpr & expr);
    virtual int Print(const AST::StringExpr & expr);

    virtual int Print(const AST::Int16Constant & ngexpr) { return 0; }
    virtual int Print(const AST::Int32Constant & expr) { return 0; }
    virtual int Print(const AST::SingleConstant & expr) { return 0; }
    virtual int Print(const AST::DoubleConstant & expr) { return 0; }
    virtual int Print(const AST::StringConstant & expr);
    virtual int Print(const AST::PrintComma & expr);
    virtual int Print(const AST::PrintSemiColon & expr);

  protected:
    void CompilerErrorInternal(ErrorCode code, unsigned len, const std::string & msg);

    bool CheckVars();
    bool CheckJumps();

    virtual int EvaluateNumericFunction(const std::string & op, const AST::NumericExpr * expr, std::string & result);
    virtual int EvaluateStringFunction(const std::string & op, const AST::NumericExpr * expr, std::string & result);

    Config m_config;

    std::string m_inputFilename;
    unsigned m_currentStatementLine;

    unsigned m_globalTempIndex = 1;

    const AST::Parser  & m_parser;
    const AST::Program & m_program;

    std::ostream * m_outputStream;

    std::set<std::string> m_funcsUsed;

    int m_lineNumber;

#if 0
    struct Closure;
    virtual Closure * CreateClosure(int indent) const;

    std::deque<Closure *> m_stack;
//    bool CreateBlocks();

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

    std::string GetGlobalTempName();

    int ResolveGotoDestination(const std::string & ref);

    bool AtTop() const
    { std::cerr << "AtTop() " << m_stack.size() << std::endl; return m_stack.size() == 1; }
#endif
};

#endif // CODEGEN_H_

