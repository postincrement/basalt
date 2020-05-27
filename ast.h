
#ifndef AST_H
#define AST_H

#include <iostream>
#include <map>
#include <vector>
#include <memory>
#include <deque>
#include <sstream>

#include "common.h"

namespace AST {

class Expr
{
  public:
    virtual ~Expr();
    virtual void PrintOn(std::ostream & strm) const = 0;
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

extern ExprList g_program;

////////////////////////////////////////////////////////////////////////////

class LineNumber : public Expr
{
  public:
    LineNumber(const std::string & str);
    virtual void PrintOn(std::ostream & strm) const;

  protected:
    std::string m_lineNumber;  
};

////////////////////////////////////////////////////////////////////////////

class Print : public ExprList
{
  public:
    Print(ExprList * exprList = nullptr);
    virtual void PrintOn(std::ostream & strm) const;

  protected:
};

class PrintComma : public Expr
{
  public:
    PrintComma() = default;
    virtual void PrintOn(std::ostream & strm) const;
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