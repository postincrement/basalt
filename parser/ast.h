
#ifndef AST_H
#define AST_H

#include <iostream>
#include <map>
#include <vector>
#include <memory>
#include <deque>
#include <sstream>
#include <set>
#include <stdint.h>
#include <queue>

// if changed, change varTypeInfo in ast.cc
// must be in order of precision
enum class VarType {
  eNone,
  eInt16,
  eInt32,
  eSingle,
  eDouble,
  eString
};

//////////////////////////////////////////////////////////////////

struct LanguageProfileDef 
{
  const char * m_name;
  int          m_normalizedVarLen;
  int          m_tabWidth;
  VarType      m_defaultNumericType;
  VarType      m_defaultIntegerType;
};

struct LanguageProfile
{
  LanguageProfile(const LanguageProfileDef * def)
    : m_def(def)
  { }  

  virtual VarType GetIntegerType() const        { return m_def->m_defaultIntegerType; }
  virtual VarType GetDefaultNumericType() const { return m_def->m_defaultNumericType; }
  virtual int GetVarNameLen() const             { return m_def->m_normalizedVarLen;   }
  virtual int GetTabWidth() const               { return m_def->m_tabWidth;   }

  const LanguageProfileDef * m_def;
};

//////////////////////////////////////////////////////////////////

class CodeGenerator;

namespace AST {

//////////////////////////////////////////////////////////////////

struct VarTypeInfoRec {
  const char * m_name;
  const char * m_suffix;
};

extern VarTypeInfoRec & GetVarTypeInfo(VarType type);

//////////////////////////////////////////////////////////////////

struct JumpDestinationInfo {
  unsigned m_count = 0;
  std::set<unsigned> m_usedLine;
};

typedef std::map<std::string, JumpDestinationInfo> JumpDestinationList;

//////////////////////////////////////////////////////////////////

struct VarInfo {
  VarType     m_type;
  std::string m_originalName;
  bool        m_lhs = false;
  unsigned    m_lhsLine = 0;
  bool        m_rhs = false;
  unsigned    m_rhsLine = 0;
};

typedef std::map<std::string, VarInfo>  VarList;
typedef std::map<std::string, unsigned> StringConstantList;
typedef std::map<std::string, unsigned> LineNumberInfo; 

//////////////////////////////////////////////////////////////////

class Node
{
  public:
    Node()
    {}

    virtual ~Node()
    {}

    virtual int Generate(CodeGenerator & gen) const;
    virtual int Evaluate(CodeGenerator & gen, std::string & result) const;
    virtual int Print(CodeGenerator & gen) const;
};

////////////////////////////////////////////////////////////////////////////

template <class N>
class NodeList : public Node
{
  public:
    virtual void Append(N * node)
    {
      if (node != nullptr) {
        m_list.push_back(std::unique_ptr<N>(node));
      }
    }

    virtual void Append(NodeList * nodeList)
    {
      if (nodeList != nullptr) {
        for (auto & r : nodeList->m_list) {
          Append(r.release());
        }
      }
    }

    virtual int Generate(CodeGenerator & gen) const
    {
      for (auto & r : m_list)
        r->Generate(gen);
      return 0;  
    }

    virtual int Print(CodeGenerator & gen) const
    {
      for (auto & r : m_list)
        r->Print(gen);
      return 0;  
    }

    std::vector<std::unique_ptr<N>> m_list;
};
////////////////////////////////////////////////////////////////////////////

class Statement;

using StatementList = NodeList<Statement>;

class SourceLine : public Node
{
  public:
    SourceLine(unsigned sourceLineNumber, 
              const std::string & m_basicLineNumber,
              const std::string & line);

    virtual int Generate(CodeGenerator & gen) const override;

    unsigned GetSourceLineNumber() const
    { return m_sourceLineNumber; }

    std::string GetBasicLineNumber() const
    { return m_basicLineNumber; }

    std::string GetLine() const
    { return m_line; }

    std::unique_ptr<StatementList> m_statements;

  protected:
    unsigned m_sourceLineNumber = 0;
    std::string m_basicLineNumber;
    std::string m_line;  
};

using Program = NodeList<SourceLine>;

////////////////////////////////////////////////////////////////////////////

struct Parser
{
  Parser();

  int AddStringConstant(const std::string & str);

  void AddJumpTo(int sourceLineNumber, const std::string & basicLineRef);

  std::string DefineVar(
                 const std::string & name,
                 const std::string & originalName,
                 VarType varType,
                 int leftRef,
                 int rightRef);

  VarInfo * GetVar(const std::string & name);

  bool StartLine(const std::string * lineRef);

  Program             m_program;
  JumpDestinationList m_jumpDestinationInfo;
  LanguageProfile     * m_languageProfile;
  VarList             m_globalVars;
  StringConstantList  m_stringConstants;
  unsigned            m_stringConstantIndex = 1;
  LineNumberInfo      m_lineNumberInfo;

  unsigned m_lexLineNumber     = 1;  // corrected source line number
  std::string m_basicLineNumber;     // BASIC line number
};

extern Parser * g_parser;

////////////////////////////////////////////////////////////////////////////

class Expr : public Node
{
  public:
    Expr(VarType type = VarType::eNone);
    VarType GetType() const;

    virtual bool IsConstant() const
    { return false; } 

    virtual bool IsVarRef() const
    { return false; } 

    virtual bool IsPrintSemiColon() const
    { return false; }

  protected:
    VarType m_type;
};

using ExprList = NodeList<Expr>;

////////////////////////////////////////////////////////////////////////////

class Statement : public Node
{
  public:
    Statement(unsigned lineNumber)
      : m_lineNumber(lineNumber)
    { }

    virtual bool IsJump() const
    { return false; }

    unsigned m_lineNumber;
    std::string m_text;
};


using StatementList = NodeList<Statement>;


////////////////////////////////////////////////////////////////////////////

class NumericExpr : public Expr
{
  public:
    NumericExpr(VarType type = VarType::eNone)
      : Expr(type)
    {}

    virtual int Print(CodeGenerator & gen) const override;
};

class StringExpr : public Expr
{
  public:
    StringExpr()
      : Expr(VarType::eString)
    {}

    virtual int Print(CodeGenerator & gen) const override;
};

////////////////////////////////////////////////////////////////////////////

class Print : public Statement
{
  public:
    Print(unsigned lineNumber, NodeList<Expr> * exprList = nullptr);
    virtual int Generate(CodeGenerator & gen) const override;

    ExprList * m_list = nullptr;
};

class PrintComma : public Expr
{
  public:
    PrintComma() = default;
    virtual int Print(CodeGenerator & gen) const override;
};

class PrintSemiColon : public Expr
{
  public:
    PrintSemiColon() = default;
    virtual int Print(CodeGenerator & gen) const override;
    virtual bool IsPrintSemiColon() const override
    { return true; }
};

////////////////////////////////////////////////////////////////////////////

class String : public StringExpr
{
  public:
    String() = default;
    String(const std::string * str)
    {
      if (str != nullptr)
        m_val = *str;
    }

  protected:
    std::string m_val;
};

////////////////////////////////////////////////////////////////////////////

class NumericVarRef : public NumericExpr
{
  public:
    NumericVarRef(VarType type, const std::string & m_id);
    virtual int Evaluate(CodeGenerator & gen, std::string & result) const override;
    virtual int Print(CodeGenerator & gen) const override;

    virtual bool IsVarRef() const override
    { return true; } 

    std::string GetName() const
    {
      return m_name;
    }

    std::string GetOriginalName() const
    {
      return m_originalName;
    }

  protected:
    std::string m_name;
    std::string m_originalName;
};

class StringVarRef : public StringExpr
{
  public:
    StringVarRef(const std::string & m_id);
    virtual int Evaluate(CodeGenerator & gen, std::string & result) const override;
    virtual int Print(CodeGenerator & gen) const override;

    virtual bool IsVarRef() const override
    { return true; } 

    std::string GetName() const
    {
      return m_name;
    }

    std::string GetOriginalName() const
    {
      return m_originalName;
    }

  protected:
    std::string m_name;
    std::string m_originalName;
};

////////////////////////////////////////////////////////////////////////////

class StringAssign : public StringExpr
{
  public:
    StringAssign(const StringVarRef * lhs, const StringExpr * rhs);
    virtual int Generate(CodeGenerator & gen) const override;

    virtual bool Validate();

    const AST::StringVarRef * m_lhs;
    const StringExpr * m_rhs;
};

////////////////////////////////////////////////////////////////////////////

class StringAddition : public StringExpr
{
  public:
    StringAddition(const StringExpr * lhs, const StringExpr * rhs)
      : m_lhs(lhs)
      , m_rhs(rhs)
    { }

    virtual int Evaluate(CodeGenerator & gen, std::string & result) const override;

    const StringExpr * m_lhs;
    const StringExpr * m_rhs;
};

////////////////////////////////////////////////////////////////////////////

class NumericAssign : public Expr
{
  public:
    NumericAssign(const NumericVarRef * lhs, const NumericExpr * rhs);
    virtual int Generate(CodeGenerator & gen) const override;

    virtual bool Validate();

    const AST::NumericVarRef * m_lhs;
    const NumericExpr * m_rhs;
};

////////////////////////////////////////////////////////////////////////////

class NumericCast : public NumericExpr
{
  public:
    NumericCast(VarType type, const NumericExpr * expr)
      : NumericExpr(type)
      , m_from(expr)
    { }

    virtual int Evaluate(CodeGenerator & gen, std::string & result) const override;

    const AST::NumericExpr * m_from;
};

////////////////////////////////////////////////////////////////////////////

class NumericOperator : public NumericExpr
{
  public:
    NumericOperator() = default;

    NumericOperator(VarType t)
      : NumericExpr(t)
    {}
};

class StringOperator : public StringExpr
{
  public:
    StringOperator()
      : StringExpr()
    {}
};

class NumericBinaryOperation : public NumericOperator
{
  public:
    NumericBinaryOperation(VarType type, const NumericExpr * lhs, const NumericExpr * rhs)
      : NumericOperator(type)
      , m_lhs(lhs)
      , m_rhs(rhs)
    { }
    NumericBinaryOperation(const NumericExpr * lhs, const NumericExpr * rhs)
      : m_lhs(lhs)
      , m_rhs(rhs)
    { }

    virtual bool Validate();

    virtual int Evaluate(CodeGenerator & gen, std::string & result) const override = 0;

    virtual bool IsConstant() const override
    { return m_lhs->IsConstant() && m_rhs->IsConstant(); }

    const NumericExpr * m_lhs;
    const NumericExpr * m_rhs;
};

#define DEFINE_NUMERICBINARYOP(name) \
class name : public NumericBinaryOperation \
{ \
  public: \
    name(const NumericExpr * lhs, const NumericExpr * rhs) \
      : NumericBinaryOperation(lhs, rhs) \
    { }  \
    virtual int Evaluate(CodeGenerator & gen, std::string & result) const override; \
};

DEFINE_NUMERICBINARYOP(NumericAddition)
DEFINE_NUMERICBINARYOP(Subtraction)
DEFINE_NUMERICBINARYOP(Multiplication)
DEFINE_NUMERICBINARYOP(Division)

class NumericComparisonOperation : public NumericBinaryOperation
{
  public:
    NumericComparisonOperation(VarType type, const NumericExpr * lhs, const NumericExpr * rhs)
      : NumericBinaryOperation(type, lhs, rhs)
    { }
    NumericComparisonOperation(const NumericExpr * lhs, const NumericExpr * rhs)
      : NumericBinaryOperation(lhs, rhs)
    { }

    virtual int Evaluate(CodeGenerator & gen, std::string & result) const = 0;
};


#define DEFINE_NUMERICCOMPARISONOP(name) \
class name : public NumericComparisonOperation \
{ \
  public: \
    name(const NumericExpr * lhs, const NumericExpr * rhs) \
      : NumericComparisonOperation(VarType::eInt16, lhs, rhs) \
    { }  \
    virtual int Evaluate(CodeGenerator & gen, std::string & result) const override; \
};

DEFINE_NUMERICCOMPARISONOP(NumericEquality)
DEFINE_NUMERICCOMPARISONOP(NumericNotEquality)
DEFINE_NUMERICCOMPARISONOP(NumericGreaterThan)
DEFINE_NUMERICCOMPARISONOP(NumericGreaterThanEqual)
DEFINE_NUMERICCOMPARISONOP(NumericLessThan)
DEFINE_NUMERICCOMPARISONOP(NumericLessThanEqual)

////////////////////////////////////////////////////////////////////////////

class UnaryNumericOperation : public NumericOperator
{
  public:
    UnaryNumericOperation(VarType type, const Expr * arg)
      : NumericOperator(type)
      , m_arg(arg)
    { }

    virtual bool IsConstant() const override
    { return m_arg->IsConstant(); }

    virtual int Evaluate(CodeGenerator & gen, std::string & result) const override = 0;

    const Expr * m_arg;
};

template<class ArgType>
class UnaryStringOperation : public StringOperator
{
  public:
    UnaryStringOperation(const ArgType * expr)
      : StringOperator()
      , m_expr(expr)
    { }

    virtual int Evaluate(CodeGenerator & gen, std::string & result) const = 0;

    const ArgType * m_expr;
};

////////////////////////////////////////////////////////////////////////////

class Negation : public UnaryNumericOperation
{
  public:
    Negation(NumericExpr * expr)
      : UnaryNumericOperation(expr->GetType(), expr)
    { }

    virtual int Evaluate(CodeGenerator & gen, std::string & result) const override;
};

class Power : public UnaryNumericOperation
{
  public:
    Power(NumericExpr * expr)
      : UnaryNumericOperation(expr->GetType(), expr)
    { }

    virtual int Evaluate(CodeGenerator & gen, std::string & result) const override;
};

////////////////////////////////////////////////////////////////////////////

class Constant: public NumericExpr
{
  public:
    Constant(VarType t)
      : NumericExpr(t)
    { }  

    virtual bool IsConstant() const override
    { return true; }
};

class NumericConstant : public Constant
{
  public:
    NumericConstant(VarType t)
      : Constant(t)
    { }  

    virtual std::string AsString() const = 0;
    virtual double AsDouble() const = 0;
    virtual float AsSingle() const = 0;
    virtual int32_t AsInt32() const = 0;
    virtual int16_t AsInt16() const = 0;

    virtual int Evaluate(CodeGenerator & gen, std::string & result) const override;

    virtual void Negate() = 0;
};

template<VarType t, typename N>
class ConstantType : public NumericConstant
{
  public:
    ConstantType(const N & v)
      : NumericConstant(t)
      , m_value(v)
    { }  

    static AST::NumericConstant * Create(const std::string & str);

    virtual int Print(CodeGenerator & gen) const override;

    virtual bool IsConstant() const override
    { return true; }

    virtual void Negate() override
    { m_value = -m_value; }

    N GetValue() const
    { return m_value; }

    virtual double AsDouble() const override { return m_value; }
    virtual float AsSingle() const override  { return m_value; }
    virtual int32_t AsInt32() const override { return m_value; }
    virtual int16_t AsInt16() const override { return m_value; }
    virtual std::string AsString() const override
    {
      std::stringstream strm;
      strm << std::fixed << GetValue();
      return strm.str();
    }


  protected:
    N m_value;
};

using Int16Constant  = ConstantType<VarType::eInt16,  int16_t>;
using Int32Constant  = ConstantType<VarType::eInt32,  int32_t>;
using SingleConstant = ConstantType<VarType::eSingle, float>;
using DoubleConstant = ConstantType<VarType::eDouble, double>;

////////////////////////////////////////////////////////////////////////////

class AssignStatement : public Statement
{
  public:
    AssignStatement(unsigned lineNumber, Expr * expr)
      : Statement(lineNumber)
      , m_expr(expr)
    {}

    virtual int Generate(CodeGenerator & gen) const override;

    Expr * m_expr;
};

////////////////////////////////////////////////////////////////////////////

class StringConstant : public StringExpr
{
  public:
    StringConstant(const std::string & str);

    virtual bool IsConstant() const override
    { return true; }

    std::string GetValue() const
    { return m_value; }

    virtual int Evaluate(CodeGenerator & gen, std::string & result) const override;

    virtual int Print(CodeGenerator & gen) const override;

  protected:   
    std::string m_value;  
    unsigned m_index;
};

////////////////////////////////////////////////////////////////////////////

class End : public Statement
{
  public:
    End(unsigned lineNumber)
      : Statement(lineNumber)
    {}

    virtual bool IsJump() const override
    { return true; }

    virtual int Generate(CodeGenerator & gen) const override;
};

class System : public Statement
{
  public:
    System(unsigned lineNumber)
      : Statement(lineNumber)
    {}

    virtual bool IsJump() const override
    { return true; }

    virtual int Generate(CodeGenerator & gen) const override;
};

class Rem : public Statement
{
  public:
    Rem(unsigned lineNumber, const std::string & comment)
      : Statement(lineNumber)
      , m_comment(comment)
    {}

    virtual int Generate(CodeGenerator & gen) const override;

    std::string m_comment;
};

////////////////////////////////////////////////////////////////////////////

class JumpStatement : public Statement
{
  public:
    JumpStatement(unsigned lineNumber, const std::string & ref);

    std::string GetRef() const
    { return m_ref; } 

    std::string m_ref;
};

class GotoStatement : public JumpStatement
{
  public:
    GotoStatement(unsigned lineNumber, const std::string & ref)
      : JumpStatement(lineNumber, ref)
    { }

    virtual bool IsJump() const override
    { return true; }

    virtual int Generate(CodeGenerator & gen) const override;
};


class GosubStatement : public JumpStatement
{
  public:
    GosubStatement(unsigned lineNumber, const std::string & ref)
      : JumpStatement(lineNumber, ref)
    { }

    virtual bool IsJump() const override
    { return true; }

    virtual int Generate(CodeGenerator & gen) const override;
};

class ReturnStatement : public Statement
{
  public:
    ReturnStatement(unsigned lineNumber)
      : Statement(lineNumber)
    {}

    virtual bool IsJump() const override
    { return true; }

    virtual int Generate(CodeGenerator & gen) const override;
};

////////////////////////////////////////////////////////////////////////////

typedef std::deque<std::string> OnRefList;

class OnGotoStatement : public Statement
{
  public:
    OnGotoStatement(unsigned lineNumber, const OnRefList & onRefs);

    virtual bool IsJump() const override
    { return true; }

    virtual int Generate(CodeGenerator & gen) const override;

    OnRefList m_onRefs;    
};

////////////////////////////////////////////////////////////////////////////

class IntFunction : public UnaryNumericOperation
{
  public:
    IntFunction(const NumericExpr * arg)
      : UnaryNumericOperation(arg->GetType(), arg)
    {}

    virtual int Evaluate(CodeGenerator & gen, std::string & result) const override;
};

class SqrFunction : public UnaryNumericOperation
{
  public:
    SqrFunction(const NumericExpr * arg)
      : UnaryNumericOperation(arg->GetType(), arg)
    {
    }

    virtual int Evaluate(CodeGenerator & gen, std::string & result) const override;
};

class RndFunction : public UnaryNumericOperation
{
  public:
    RndFunction(const NumericExpr * arg)
      : UnaryNumericOperation(arg->GetType(), arg)
    {
    }

    virtual int Evaluate(CodeGenerator & gen, std::string & result) const override;
};

class AbsFunction : public UnaryNumericOperation
{
  public:
    AbsFunction(const NumericExpr * arg)
      : UnaryNumericOperation(arg->GetType(), arg)
    {
    }

    virtual int Evaluate(CodeGenerator & gen, std::string & result) const override;
};

class LenFunction : public UnaryNumericOperation
{
  public:
    LenFunction(const StringExpr * arg1)
      : UnaryNumericOperation(g_parser->m_languageProfile->GetIntegerType(), arg1)
    {}

    virtual int Evaluate(CodeGenerator & gen, std::string & result) const override;
};


////////////////////////////////////////////////////////////////////////////

class TabFunction : public StringExpr
{
  public:
    TabFunction(const NumericExpr * arg1)
      : StringExpr()
      , m_arg1(arg1)
    {}

    const NumericExpr * GetArg1() const
    { return m_arg1; }

    virtual int Evaluate(CodeGenerator & gen, std::string & result) const override;

  protected:
    const NumericExpr * m_arg1;
};

class StrFunction : public StringExpr
{
  public:
    StrFunction(const NumericExpr * arg1)
      : m_arg1(arg1)
    {}

    const NumericExpr * GetArg1() const
    { return m_arg1; }

    virtual int Evaluate(CodeGenerator & gen, std::string & result) const override;

  protected:
    const NumericExpr * m_arg1;
};

class ChrFunction : public StringExpr
{
  public:
    ChrFunction(const NumericExpr * arg1)
      : m_arg1(arg1)
    {}

    const NumericExpr * GetArg1() const
    { return m_arg1; }

    virtual int Evaluate(CodeGenerator & gen, std::string & result) const override;

  protected:
    const NumericExpr * m_arg1;
};

class LeftFunction : public StringExpr
{
  public:
    LeftFunction(const StringExpr * arg1, const NumericExpr * arg2)
      : m_arg1(arg1)
      , m_arg2(arg2)
    {}

    const StringExpr * GetArg1() const
    { return m_arg1; }

    const NumericExpr * GetArg2() const
    { return m_arg2; }

    virtual int Evaluate(CodeGenerator & gen, std::string & result) const override;

  protected:
    const StringExpr * m_arg1;
    const NumericExpr * m_arg2;
};

class MidFunction : public StringExpr
{
  public:
    MidFunction(const StringExpr * arg1, const NumericExpr * arg2, const NumericExpr * arg3)
      : m_arg1(arg1)
      , m_arg2(arg2)
      , m_arg3(arg3)
    {}

    const StringExpr * GetArg1() const
    { return m_arg1; }

    const NumericExpr * GetArg2() const
    { return m_arg2; }

    const NumericExpr * GetArg3() const
    { return m_arg3; }

    virtual int Evaluate(CodeGenerator & gen, std::string & result) const override;

  protected:
    const StringExpr * m_arg1;
    const NumericExpr * m_arg2;
    const NumericExpr * m_arg3;
};

class RightFunction : public StringExpr
{
  public:
    RightFunction(const StringExpr * arg1, const NumericExpr * arg2)
      : m_arg1(arg1)
      , m_arg2(arg2)
    {}

    const StringExpr * GetArg1() const
    { return m_arg1; }

    const NumericExpr * GetArg2() const
    { return m_arg2; }

    virtual int Evaluate(CodeGenerator & gen, std::string & result) const override;

  protected:
    const StringExpr * m_arg1;
    const NumericExpr * m_arg2;
};

////////////////////////////////////////////////////////////////////////////

struct IfConditional : public Expr
{
  StatementList * m_statements = nullptr;
  std::string m_lineNumber;
};

class IfStatement : public Statement 
{
  public:
    IfStatement(unsigned lineNumber,
                const NumericExpr * cond, 
                const IfConditional * trueStatements,
                const IfConditional * falseStatements
                );

    virtual int Generate(CodeGenerator & gen) const;

    const NumericExpr * m_cond; 
    const IfConditional * m_trueStatements = nullptr;
    const IfConditional * m_falseStatements = nullptr;
};

////////////////////////////////////////////////////////////////////////////

class ForStatement : public Statement 
{
  public:
    ForStatement(unsigned lineNumber, 
                 const NumericVarRef * var, 
                 const NumericExpr * fromVal,
                 const NumericExpr * toVal,
                 const NumericExpr * stepVal);

    virtual int Generate(CodeGenerator & gen) const;

    VarType m_type;
    const NumericVarRef * m_var;
    const NumericExpr * m_fromVal;
    const NumericExpr * m_toVal;
    const NumericExpr * m_stepVal;                 
};

class NextStatement : public Statement 
{
  public:
    NextStatement(unsigned lineNumber, const NumericVarRef * var);

    virtual int Generate(CodeGenerator & gen) const;

    const NumericVarRef * m_var;
};

////////////////////////////////////////////////////////////////////////////

} // namespace AST

////////////////////////////////////////////////////////////////////////////

struct SingleFloat 
{
  SingleFloat(const std::string & str)
    : m_lexeme(str)
  {
    m_value = atof(str.c_str());
  }
  std::string m_lexeme;
  float m_value;
};

struct DoubleFloat 
{
  DoubleFloat(const std::string & str)
    : m_lexeme(str)
  {
    m_value = atof(str.c_str());
  } 
  std::string m_lexeme;
  double m_value;
};

#endif // AST_H_