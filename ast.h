
#ifndef AST_H
#define AST_H

#include <iostream>
#include <map>
#include <vector>
#include <memory>
#include <deque>
#include <sstream>

#include <stdint.h>

#include "common.h"

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

  protected:
    VarType m_type;
};

using ExprList = NodeList<Expr>;

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

class String : public Expr
{
  public:
    String() = default;
    String(const std::string * str);

  protected:
    std::string m_val;
};

////////////////////////////////////////////////////////////////////////////

class VarRef;

class Assign : public Expr
{
  public:
    Assign(VarRef * lhs, Expr * rhs);
    virtual int Generate(CodeGenerator & gen) const override;

    AST::VarRef * m_lhs;
    Expr * m_rhs;
};

////////////////////////////////////////////////////////////////////////////

class Operator : public Expr
{
  public:
    virtual bool Validate(std::string & msg) = 0;
};

class BinaryOperation : public Operator
{
  public:
    BinaryOperation(Expr * lhs, Expr * rhs)
      : m_lhs(lhs)
      , m_rhs(rhs)
    { }

    virtual bool Validate(std::string & msg);

    virtual int Generate(CodeGenerator & gen) const = 0;

    Expr * m_lhs;
    Expr * m_rhs;
};

class Addition : public BinaryOperation
{
  public:
    Addition(Expr * lhs, Expr * rhs)
      : BinaryOperation(lhs, rhs)
    { }  

    virtual int Generate(CodeGenerator & gen) const override;
};

class Subtraction : public BinaryOperation
{
  public:
    Subtraction(Expr * lhs, Expr * rhs)
      : BinaryOperation(lhs, rhs)
    { }  

    virtual int Generate(CodeGenerator & gen) const override;
};

class Multiplication : public BinaryOperation
{
  public:
    Multiplication(Expr * lhs, Expr * rhs)
      : BinaryOperation(lhs, rhs)
    { }  

    virtual int Generate(CodeGenerator & gen) const override;
};

class Division : public BinaryOperation
{
  public:
    Division(Expr * lhs, Expr * rhs)
      : BinaryOperation(lhs, rhs)
    { }  

    virtual int Generate(CodeGenerator & gen) const override;
};

////////////////////////////////////////////////////////////////////////////

class UnaryOperation : public Operator
{
  public:
    UnaryOperation(Expr * expr)
      : m_expr(expr)
    { }

    virtual bool Validate(std::string & msg)
    { return true; }

    virtual int Generate(CodeGenerator & gen) const = 0;

    Expr * m_expr;
};

class Negation : public UnaryOperation
{
  public:
    Negation(Expr * expr)
      : UnaryOperation(expr)
    { }

    virtual int Generate(CodeGenerator & gen) const override;
};

class Power : public UnaryOperation
{
  public:
    Power(Expr * expr)
      : UnaryOperation(expr)
    { }

    virtual int Generate(CodeGenerator & gen) const override;
};

////////////////////////////////////////////////////////////////////////////

class VarRef : public Expr
{
  public:
    VarRef(VarType type, const std::string & m_id);
    virtual int Generate(CodeGenerator & gen) const override;
    virtual int Print(CodeGenerator & gen) const override;

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

template<VarType t, typename N>
class Constant : public Expr
{
  public:
    Constant(const N & v)
      : Expr(t)
      , m_value(v)
    { }  

    static AST::Expr * Create(const std::string & str);

    virtual int Generate(CodeGenerator & gen) const override;
    virtual int Print(CodeGenerator & gen) const override;

    N GetValue() const
    { return m_value; }

  protected:
    N m_value;
};

using StringConstant = Constant<VarType::eString, std::string>;
using Int16Constant  = AST::Constant<VarType::eInt16,  int16_t>;
using Int32Constant  = Constant<VarType::eInt32,  int32_t>;
using SingleConstant = Constant<VarType::eSingle, float>;
using DoubleConstant = Constant<VarType::eDouble, double>;

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

////////////////////////////////////////////////////////////////////////////

using Statement     = NodeList<Expr>;
using StatementList = NodeList<Statement>;

class SourceLine : public Node
{
  public:
    SourceLine(int sourceLineNumber, 
              const std::string & m_basicLineNumber,
              const std::string & line);

    virtual int Generate(CodeGenerator & gen) const override;

    int GetSourceLineNumber() const
    { return m_sourceLineNumber; }

    std::string GetBasicLineNumber() const
    { return m_basicLineNumber; }

    std::string GetLine() const
    { return m_line; }

    std::unique_ptr<StatementList> m_statements;

  protected:
    int m_sourceLineNumber = 0;
    std::string m_basicLineNumber;
    std::string m_line;  
};

using Program = NodeList<SourceLine>;

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