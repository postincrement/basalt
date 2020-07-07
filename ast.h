
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

// if changed, change g_varTypeInfo in c_codegen.cc
// and g_basicVarSuffixes below
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
  VarType      m_defaultNumericType;
  VarType      m_defaultIntegerType;
};

struct LanguageProfile
{
  LanguageProfile(const LanguageProfileDef * def)
    : m_def(def)
  { }  

  virtual VarType GetIntegerType()        { return m_def->m_defaultIntegerType; }
  virtual VarType GetDefaultNumericType() { return m_def->m_defaultNumericType; }
  virtual int GetVarNameLen()             { return m_def->m_normalizedVarLen;   }

  const LanguageProfileDef * m_def;
};

extern LanguageProfile * g_languageProfile;

//////////////////////////////////////////////////////////////////

class CodeGenerator;

namespace AST {

////////////////////////////////////////////////////////////////////////////

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
    }

    virtual int Print(CodeGenerator & gen) const
    {
      for (auto & r : m_list)
        r->Print(gen);
    }

    std::vector<std::unique_ptr<N>> m_list;
};

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

  protected:
    VarType m_type;
};

using ExprList = NodeList<Expr>;

class NumericExpr : public Expr
{
  public:
    NumericExpr(VarType type = VarType::eNone)
      : Expr(type)
    {}
};

class StringExpr : public Expr
{
  public:
    StringExpr()
      : Expr(VarType::eString)
    {}


};

////////////////////////////////////////////////////////////////////////////

class Print : public NodeList<Expr>
{
  public:
    Print(NodeList<Expr> * exprList = nullptr);
    virtual int Generate(CodeGenerator & gen) const override;
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

    virtual bool IsVarRef() const
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

    virtual bool IsVarRef() const
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

class NumericBinaryOperation : public NumericOperator
{
  public:
    NumericBinaryOperation(const NumericExpr * lhs, const NumericExpr * rhs)
      : m_lhs(lhs)
      , m_rhs(rhs)
    { }

    virtual bool Validate();

    virtual int Evaluate(CodeGenerator & gen, std::string & result) const = 0;

    const NumericExpr * m_lhs;
    const NumericExpr * m_rhs;
};

class NumericAddition : public NumericBinaryOperation
{
  public:
    NumericAddition(const NumericExpr * lhs, const NumericExpr * rhs)
      : NumericBinaryOperation(lhs, rhs)
    { }  

    virtual int Evaluate(CodeGenerator & gen, std::string & result) const override;
};

class Subtraction : public NumericBinaryOperation
{
  public:
    Subtraction(const NumericExpr * lhs, const NumericExpr * rhs)
      : NumericBinaryOperation(lhs, rhs)
    { }  

    virtual int Evaluate(CodeGenerator & gen, std::string & result) const override;
};

class Multiplication : public NumericBinaryOperation
{
  public:
    Multiplication(const NumericExpr * lhs, const NumericExpr * rhs)
      : NumericBinaryOperation(lhs, rhs)
    { }  

    virtual int Evaluate(CodeGenerator & gen, std::string & result) const override;
};

class Division : public NumericBinaryOperation
{
  public:
    Division(const NumericExpr * lhs, const NumericExpr * rhs)
      : NumericBinaryOperation(lhs, rhs)
    { }  

    virtual int Evaluate(CodeGenerator & gen, std::string & result) const override;
};

////////////////////////////////////////////////////////////////////////////

class UnaryOperation : public NumericOperator
{
  public:
    UnaryOperation(const NumericExpr * expr)
      : NumericOperator(expr->GetType())
      , m_expr(expr)
    { }

    virtual int Evaluate(CodeGenerator & gen, std::string & result) const = 0;

    const Expr * m_expr;
};

class Negation : public UnaryOperation
{
  public:
    Negation(NumericExpr * expr)
      : UnaryOperation(expr)
    { }

    virtual int Evaluate(CodeGenerator & gen, std::string & result) const override;
};

class Power : public UnaryOperation
{
  public:
    Power(NumericExpr * expr)
      : UnaryOperation(expr)
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

    virtual bool IsConstant() const
    { return true; }
};

class NumericConstant : public Constant
{
  public:
    NumericConstant(VarType t)
      : Constant(t)
    { }  

    virtual double AsDouble() const = 0;
    virtual float AsSingle() const = 0;
    virtual int32_t AsInt32() const = 0;
    virtual int16_t AsInt16() const = 0;
};

template<VarType t, typename N>
class ConstantType : public NumericConstant
{
  public:
    ConstantType(const N & v)
      : NumericConstant(t)
      , m_value(v)
    { }  

    static AST::NumericExpr * Create(const std::string & str);

    virtual int Evaluate(CodeGenerator & gen, std::string & result) const override;
    virtual int Print(CodeGenerator & gen) const override;

    virtual bool IsConstant() const
    { return true; }

    N GetValue() const
    { return m_value; }

    virtual double AsDouble() const override { return m_value; }
    virtual float AsSingle() const override  { return m_value; }
    virtual int32_t AsInt32() const override { return m_value; }
    virtual int16_t AsInt16() const override { return m_value; }

  protected:
    N m_value;
};

using Int16Constant  = ConstantType<VarType::eInt16,  int16_t>;
using Int32Constant  = ConstantType<VarType::eInt32,  int32_t>;
using SingleConstant = ConstantType<VarType::eSingle, float>;
using DoubleConstant = ConstantType<VarType::eDouble, double>;

struct VarInfo {
  VarType m_type;
  std::string m_originalName;
  bool     m_lhs = false;
  unsigned m_lhsLine = 0;
  bool     m_rhs = false;
  unsigned m_rhsLine = 0;
};

typedef std::map<std::string, VarInfo> VarList;
extern VarList g_globalVars;

typedef std::map<std::string, unsigned> StringConstantList;
extern StringConstantList g_stringConstants;

extern unsigned g_stringConstantIndex;

////////////////////////////////////////////////////////////////////////////

class StringConstant : public StringExpr
{
  public:
    StringConstant(const std::string & str);

    virtual bool IsConstant() const
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

using Statement     = NodeList<Expr>;
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

typedef std::map<std::string, unsigned> LineNumberInfo; 
extern LineNumberInfo g_lineNumberInfo;

////////////////////////////////////////////////////////////////////////////

class End : public Statement
{
  public:
    End()
    {}

    virtual int Generate(CodeGenerator & gen) const override;
};

////////////////////////////////////////////////////////////////////////////

struct GotoDestinationInfo {
  unsigned m_count = 0;
  std::set<unsigned> m_usedLine;
};

typedef std::map<std::string, GotoDestinationInfo> GotoDestinationList;
extern GotoDestinationList g_gotoDestinationInfo;

class Goto : public Statement
{
  public:
    Goto(const std::string & ref, unsigned sourceLineNumber);

    std::string GetRef() const
    { return m_ref; } 

    virtual int Generate(CodeGenerator & gen) const override;

    std::string m_ref;
    unsigned m_sourceLineNumber;
};

////////////////////////////////////////////////////////////////////////////

class IntFunction : public NumericExpr
{
  public:
    IntFunction(const NumericExpr * arg1)
      : NumericExpr(g_languageProfile->GetIntegerType())
      , m_arg1(arg1)
    {}

    const NumericExpr * GetArg1() const
    { return m_arg1; }

    virtual int Evaluate(CodeGenerator & gen, std::string & result) const override;

  protected:
    const NumericExpr * m_arg1;
};

class SqrFunction : public NumericExpr
{
  public:
    SqrFunction(const NumericExpr * arg1)
      : NumericExpr(arg1->GetType())
      , m_arg1(arg1)
    {
    }

    const NumericExpr * GetArg1() const
    { return m_arg1; }

    virtual int Evaluate(CodeGenerator & gen, std::string & result) const override;

  protected:
    const NumericExpr * m_arg1;
};

class LenFunction : public NumericExpr
{
  public:
    LenFunction(const StringExpr * arg1)
      : NumericExpr(g_languageProfile->GetIntegerType())
      , m_arg1(arg1)
    {}

    const StringExpr * GetArg1() const
    { return m_arg1; }

    virtual int Evaluate(CodeGenerator & gen, std::string & result) const override;

  protected:
    const StringExpr * m_arg1;
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

extern Program g_program;

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
  double m_value;
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

#endif // CODEGEN_H