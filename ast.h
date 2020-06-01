
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

    virtual int Generate(CodeGenerator & gen);
    virtual int Print(CodeGenerator & gen);
};

////////////////////////////////////////////////////////////////////////////

class NodeList : public Node
{
  public:
    virtual void Append(Node * node);    
    virtual void Append(NodeList * nodeList);

    std::vector<std::unique_ptr<Node>> m_list;
};

extern NodeList g_program;

////////////////////////////////////////////////////////////////////////////

class SourceLine : public Node
{
  public:
    SourceLine(int lineNumber, const std::string & line);

    virtual int Generate(CodeGenerator & gen) override;

    int GetLineNumber() const
    { return m_lineNumber; }

    std::string GetLine() const
    { return m_line; }

  protected:
    int m_lineNumber;  
    std::string m_line;  
};

////////////////////////////////////////////////////////////////////////////

class LineNumber : public Node
{
  public:
    LineNumber(const std::string & ref);
    virtual int Generate(CodeGenerator & gen) override;

  protected:
    std::string m_ref;  
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

////////////////////////////////////////////////////////////////////////////

typedef NodeList ExprList;

////////////////////////////////////////////////////////////////////////////

class Print : public ExprList
{
  public:
    Print(ExprList * exprList = nullptr);
    virtual int Generate(CodeGenerator & gen) override;
};

class PrintComma : public Expr
{
  public:
    PrintComma() = default;
    virtual int Print(CodeGenerator & gen) override;
};

class PrintSemiColon : public Expr
{
  public:
    PrintSemiColon() = default;
    virtual int Print(CodeGenerator & gen) override;
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
    virtual int Generate(CodeGenerator & gen) override;

    AST::VarRef * m_lhs;
    Expr * m_rhs;
};

////////////////////////////////////////////////////////////////////////////

class BinaryOperation : public Expr
{
  public:
    BinaryOperation(Expr * lhs, Expr * rhs)
      : m_lhs(lhs)
      , m_rhs(rhs)
    { }

    virtual int Generate(CodeGenerator & gen) = 0;

    Expr * m_lhs;
    Expr * m_rhs;
};

class Addition : public BinaryOperation
{
  public:
    Addition(Expr * lhs, Expr * rhs)
      : BinaryOperation(lhs, rhs)
    { }  

    virtual int Generate(CodeGenerator & gen) override;
};

class Subtraction : public BinaryOperation
{
  public:
    Subtraction(Expr * lhs, Expr * rhs)
      : BinaryOperation(lhs, rhs)
    { }  

    virtual int Generate(CodeGenerator & gen) override;
};

class Multiplication : public BinaryOperation
{
  public:
    Multiplication(Expr * lhs, Expr * rhs)
      : BinaryOperation(lhs, rhs)
    { }  

    virtual int Generate(CodeGenerator & gen) override;
};

class Division : public BinaryOperation
{
  public:
    Division(Expr * lhs, Expr * rhs)
      : BinaryOperation(lhs, rhs)
    { }  

    virtual int Generate(CodeGenerator & gen) override;
};

////////////////////////////////////////////////////////////////////////////

class UnaryOperation : public Expr
{
  public:
    UnaryOperation(Expr * expr)
      : m_expr(expr)
    { }

    virtual int Generate(CodeGenerator & gen) = 0;

    Expr * m_expr;
};

class Negation : public UnaryOperation
{
  public:
    Negation(Expr * expr)
      : UnaryOperation(expr)
    { }

    virtual int Generate(CodeGenerator & gen);
};


////////////////////////////////////////////////////////////////////////////

class VarRef : public Expr
{
  public:
    VarRef(VarType type, const std::string & m_id);
    virtual int Generate(CodeGenerator & gen) override;
    virtual int Print(CodeGenerator & gen) override;

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

    virtual int Generate(CodeGenerator & gen) override;
    virtual int Print(CodeGenerator & gen) override;

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