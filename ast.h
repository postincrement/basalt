
#ifndef AST_H
#define AST_H

#include <iostream>
#include <map>
#include <vector>
#include <memory>
#include <deque>
#include <sstream>

#include "common.h"

class CodeGenerator;
class C_CodeGenerator;

namespace AST {

////////////////////////////////////////////////////////////////////////////

class Node
{
  public:
    Node();
    virtual ~Node();
    virtual void PrintOn(std::ostream & strm) const = 0;

    virtual void C_Generate(C_CodeGenerator & gen);
    virtual void C_Print(C_CodeGenerator & gen);
};

////////////////////////////////////////////////////////////////////////////

class NodeList : public Node
{
  public:
    virtual void Append(Node * node);    
    virtual void Append(NodeList * nodeList);
    virtual void PrintOn(std::ostream & strm) const;

    std::vector<std::unique_ptr<Node>> m_list;
};

extern NodeList g_program;

////////////////////////////////////////////////////////////////////////////

class SourceLine : public Node
{
  public:
    SourceLine(int lineNumber, const std::string & line);
    virtual void PrintOn(std::ostream & strm) const;
    virtual void C_Generate(C_CodeGenerator & gen);

  protected:
    int m_lineNumber;  
    std::string m_line;  
};

////////////////////////////////////////////////////////////////////////////

class LineNumber : public Node
{
  public:
    LineNumber(const std::string & ref);
    virtual void PrintOn(std::ostream & strm) const;

  protected:
    std::string m_ref;  
};

////////////////////////////////////////////////////////////////////////////

enum class VarType {
  eNone,
  eDefault,
  eInteger,
  eSingle,
  eDouble,
  eString
};

class Expr : public Node
{
  public:
    Expr(VarType type = VarType::eNone);
    virtual void PrintOn(std::ostream & strm) const;
  protected:
    VarType m_type;
};

////////////////////////////////////////////////////////////////////////////

class ExprList : public Expr
{
  public:
    virtual void Append(Expr * expr);    
    virtual void Append(ExprList * exprList);
    virtual void PrintOn(std::ostream & strm) const;
    virtual size_t Length() const;
  protected:
    std::vector<std::unique_ptr<Expr>> m_list;
};

////////////////////////////////////////////////////////////////////////////

class Print : public ExprList
{
  public:
    Print(ExprList * exprList = nullptr);
    virtual void PrintOn(std::ostream & strm) const;
    virtual void C_Generate(C_CodeGenerator & gen);

  protected:
};

class PrintComma : public Expr
{
  public:
    PrintComma() = default;
    virtual void PrintOn(std::ostream & strm) const;

    virtual void C_Print(C_CodeGenerator & gen);
};

class PrintSemiColon : public Expr
{
  public:
    PrintSemiColon() = default;
    virtual void PrintOn(std::ostream & strm) const;
};

////////////////////////////////////////////////////////////////////////////

class String : public Expr
{
  public:
    String() = default;
    String(const std::string * str);
    virtual void PrintOn(std::ostream & strm) const;

  protected:
    std::string m_val;
};

////////////////////////////////////////////////////////////////////////////

class Assign : public Expr
{
  public:
    Assign(Expr * lhs, Expr * rhs);
    virtual void PrintOn(std::ostream & strm) const;
  protected:
    Expr * m_lhs;
    Expr * m_rhs;
};

////////////////////////////////////////////////////////////////////////////

class VarRef : public Expr
{
  public:
    VarRef(VarType type, const std::string & m_id);
    virtual void PrintOn(std::ostream & strm) const;
  protected:
    std::string m_id;
};

class DefaultValue : public Expr
{
  public:
    DefaultValue(const std::string & str);
    virtual void PrintOn(std::ostream & strm) const;
  protected:
    std::string m_value;  
};

class StringValue : public Expr
{
  public:
    StringValue(const std::string & str);
    virtual void PrintOn(std::ostream & strm) const;

    virtual void C_Print(C_CodeGenerator & gen);
  protected:
    std::string m_value;  
};

class IntegerValue : public Expr
{
  public:
    IntegerValue(int value);
    virtual void PrintOn(std::ostream & strm) const;
  protected:
    int m_value;  
};

class SingleValue : public Expr
{
  public:
    SingleValue(double value);
    virtual void PrintOn(std::ostream & strm) const;
  protected:
    double m_value;  
};

class DoubleValue : public Expr
{
  public:
    DoubleValue(double value);
    virtual void PrintOn(std::ostream & strm) const;
  protected:
    double m_value;  
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