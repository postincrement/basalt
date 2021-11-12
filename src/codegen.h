#ifndef CODEGEN_H_
#define CODEGEN_H_

#include <fstream>
#include <set>
#include <string>
#include <typeinfo>
#include <cxxabi.h>

#include "parser/ast.h"
#include "errorcode.h"


#define CompilerError(code, ln, expr) \
do { std::stringstream strm; strm << expr; \
  CompilerErrorInternal(ErrorCode::code, ln, strm.str()); \
} while (0)

#define InternalError(expr) \
do { std::stringstream strm; strm << expr; \
  g_application.InternalErrorInternal(ErrorCode::eInternalError, strm.str()); \
} while (0)

std::string DemangleTypeName(const std::type_info &r);

//////////////////////////////////////////////////////////////////////////////////////

class OutputGenerator;

class CodeGenerator
{
  public:
    struct Node
    {
      Node()
      {}

      Node(const std::string & func)
        : m_func(func)
      {}

      std::string GetFunc() const
      { return m_func; }

      virtual int Generate(OutputGenerator & gen);

      std::string m_func;
    };

    typedef std::deque<std::unique_ptr<Node>> Block;

    template <class Value>
    struct ValueNode : public Node
    {
      ValueNode(const std::string & func, VarType type, const Value & value)
        : Node(func)
        , m_type(type)
        , m_value(value)
      { }  
      VarType m_type;
      Value m_value;
    };

#define SIMPLE_NODE(name, func) \
    struct name : public Node \
    { \
      name() \
        : Node(func) \
      { }  \
      virtual int Generate(OutputGenerator & gen) override; \
    }; \

#define TYPED_VALUE_NODE(name, type, func) \
    struct name : public ValueNode<type> \
    { \
      name(VarType varType, const type & value) \
        : ValueNode(func, varType, value) \
      { }  \
      virtual int Generate(OutputGenerator & gen) override; \
    }; \

#define TYPED_STRING_NODE(name, func)  TYPED_VALUE_NODE(name, std::string, func)

#define VALUE_NODE(name, func, type) \
    struct name : public Node \
    { \
      name(const type & value) \
        : Node(func) \
        , m_value(value) \
        { }  \
      virtual int Generate(OutputGenerator & gen) override; \
      type m_value; \
    }; \

#define STRING_NODE(name, func) \
    VALUE_NODE(name, func, std::string)

    struct UnaryOperator : public Node
    {
      UnaryOperator(const std::string & func, VarType type, const std::string & ret, const std::string & arg) \
        : Node(func)
        , m_type(type)
        , m_ret(ret)
        , m_arg(arg)
        { } 
      virtual int Generate(OutputGenerator & gen) override;
      VarType m_type;
      std::string m_ret;
      std::string m_arg;
    };

    struct BinaryOperator : public Node 
    { 
      BinaryOperator(const std::string & func, VarType type, const std::string & ret, const std::string & arg1, const std::string & arg2)
        : Node(func)
        , m_type(type)
        , m_ret(ret)
        , m_arg1(arg1)
        , m_arg2(arg2)
        { }
      virtual int Generate(OutputGenerator & gen) override;
      VarType m_type;
      std::string m_ret;
      std::string m_arg1;
      std::string m_arg2;
    };

    SIMPLE_NODE(BlockStart,            "block_start");
    SIMPLE_NODE(BlockEnd,              "block_end");
    STRING_NODE(LineNumber,            "line_number");

    TYPED_STRING_NODE(CreateTempVar,  "create_temp_var");
    TYPED_STRING_NODE(DestroyTempVar, "destroy_temp_var");

    SIMPLE_NODE(PrintNewLine,            "print_newline");
    SIMPLE_NODE(PrintTab,                "print_tab");

    TYPED_STRING_NODE(PrintNumericVar,   "print_numeric_var");
    TYPED_STRING_NODE(PrintNumericConst, "print_numeric_const");

    STRING_NODE(PrintStringVar,          "print_string_var");
    STRING_NODE(PrintStringConst,        "print_string_const");

    CodeGenerator(AST::Parser & parser);

    bool Run(const std::string & inputFilename);

    void SetLineNumber(int lineNumber)
    { m_lineNumber = lineNumber; }

    bool LookupGlobalVar(const std::string & varName, AST::VarInfo & var);

    const AST::VarList & GetGlobalVars() const
    { return m_parser.m_globalVars; }

    template <class Type, class ... Args>
    Type & Add(Args... args)
    {
      m_code.push_back(std::make_unique<Type>(args...));
      Type & added = static_cast<Type &>(*m_code[m_code.size()-1]);
      std::string func = added.GetFunc();
      if (!func.empty())
        m_funcsUsed.insert(func);
      return added;  
    }

    // visitor interface for AST
    virtual int Generate(const AST::Node & node);
    virtual int Evaluate(const AST::Node & expr, std::string & result);
    virtual int Print(const AST::Node & node);

    virtual int Generate(const AST::SourceLine & expr);     // iterate through statements
    virtual int Generate(const AST::Statement & statement); // default for unimplemented statements
    virtual int Generate(const AST::Print & expr);          // iterate through expressions

    virtual int Generate(const AST::StringAssign & expr);
    virtual int Generate(const AST::AssignStatement & expr);
    virtual int Generate(const AST::NumericAssign & expr);

    virtual int Generate(const AST::GotoStatement & expr) { return 0; }
    virtual int Generate(const AST::End & expr) { return 0; }
    virtual int Generate(const AST::System & expr) { return 0; }
    virtual int Generate(const AST::Rem & expr) { return 0; }
    virtual int Generate(const AST::IfStatement & expr) { return 0; }
    virtual int Generate(const AST::ForStatement & expr) { return 0; }
    virtual int Generate(const AST::NextStatement & expr) { return 0; }
    virtual int Generate(const AST::GosubStatement & expr) { return 0; }
    virtual int Generate(const AST::ReturnStatement & expr) { return 0; }

    ////////////////////

    virtual int Evaluate(const AST::StringConstant & expr, std::string & result);
    virtual int Evaluate(const AST::StringVarRef & expr, std::string & result);

    virtual int Evaluate(const AST::NumericVarRef & expr, std::string & result);
    virtual int Evaluate(const AST::NumericConstant & expr, std::string & result);
    virtual int Evaluate(const AST::NumericAddition & expr, std::string & result);
    virtual int Evaluate(const AST::Subtraction & expr, std::string & result);
    virtual int Evaluate(const AST::Multiplication & expr, std::string & result);
    virtual int Evaluate(const AST::Division & expr, std::string & result);
    virtual int Evaluate(const AST::NumericCast & expr, std::string & result);

    virtual int Evaluate(const AST::StrFunction & expr, std::string & result) { return 0; }
    virtual int Evaluate(const AST::NumericEquality & expr, std::string & result) { return 0; }
    virtual int Evaluate(const AST::NumericNotEquality & expr, std::string & result) { return 0; }
    virtual int Evaluate(const AST::NumericGreaterThan & expr, std::string & result) { return 0; }
    virtual int Evaluate(const AST::NumericGreaterThanEqual & expr, std::string & result) { return 0; }
    virtual int Evaluate(const AST::NumericLessThan & expr, std::string & result) { return 0; }
    virtual int Evaluate(const AST::NumericLessThanEqual & expr, std::string & result) { return 0; }
    virtual int Evaluate(const AST::Negation & expr, std::string & result) { return 0; }
    virtual int Evaluate(const AST::Power & expr, std::string & result) { return 0; }

    virtual int Evaluate(const AST::IntFunction & expr, std::string & result);
    virtual int Evaluate(const AST::SqrFunction & expr, std::string & result);
    virtual int Evaluate(const AST::ChrFunction & expr, std::string & result);
    virtual int Evaluate(const AST::TabFunction & expr, std::string & result);
    virtual int Evaluate(const AST::RndFunction & expr, std::string & result);
    virtual int Evaluate(const AST::AbsFunction & expr, std::string & result);
    virtual int Evaluate(const AST::LenFunction & expr, std::string & result);

    virtual int Evaluate(const AST::LeftFunction & expr, std::string & result) { return 0; }
    virtual int Evaluate(const AST::MidFunction & explepr, std::string & result) { return 0; }
    virtual int Evaluate(const AST::RightFunction & expr, std::string & result) { return 0; }
    virtual int Evaluate(const AST::StringAddition & expr, std::string & result) { return 0; }

    virtual int Print(const AST::NumericExpr & expr);
    virtual int Print(const AST::StringExpr & expr);

    virtual int Print(const AST::Int16Constant & ngexpr) { return 0; }
    virtual int Print(const AST::Int32Constant & expr) { return 0; }
    virtual int Print(const AST::SingleConstant & expr);
    virtual int Print(const AST::DoubleConstant & expr) { return 0; }
    virtual int Print(const AST::StringConstant & expr);
    virtual int Print(const AST::PrintComma & expr);
    virtual int Print(const AST::PrintSemiColon & expr);

  protected:
    friend class OutputGenerator;

    void CompilerErrorInternal(ErrorCode code, unsigned len, const std::string & msg);

    bool CheckVars();
    bool CheckJumps();

    std::string GetTempName();

    int EvaluateUnaryStringOperator(
            const std::string & op, 
            const AST::NumericExpr & arg, 
            std::string & result);

    int EvaluateUnaryNumericOperator(
            const std::string & op, 
            const AST::UnaryNumericOperation & expr, 
            std::string & result);

    int EvaluateBinaryNumericOperator(
            const std::string & op,
            const AST::NumericBinaryOperation & expr, 
            std::string & result);

    std::string m_inputFilename;
    unsigned m_currentStatementLine;

    unsigned m_tempIndex = 1;

    AST::Parser & m_parser;
    const AST::Program & m_program;

    int m_lineNumber;

    std::set<std::string> m_funcsUsed;
    Block m_code;
};


#endif // CODEGEN_H_


