
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

    virtual int Generate(CodeGenerator & gen);

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
    virtual int Generate(CodeGenerator & gen);

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
    virtual int Generate(CodeGenerator & gen);
};

class PrintComma : public Expr
{
  public:
    PrintComma() = default;
    virtual int Print(CodeGenerator & gen);
};

class PrintSemiColon : public Expr
{
  public:
    PrintSemiColon() = default;
    virtual int Print(CodeGenerator & gen);
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
    virtual int Generate(CodeGenerator & gen);

    AST::VarRef * m_lhs;
    Expr * m_rhs;
};

////////////////////////////////////////////////////////////////////////////

class VarRef : public Expr
{
  public:
    VarRef(VarType type, const std::string & m_id);

    std::string GetName() const;

  protected:
    std::string m_id;
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

    virtual int Generate(CodeGenerator & gen);
    virtual int Print(CodeGenerator & gen);

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