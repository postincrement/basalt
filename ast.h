
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

namespace AST {

#if 0

class Expr;
class ExprList;
class SourceFileExprList;
class LineMarkerExpr;
class GotoExpr;

class VariableDefExpr;
class ArrayVariableDefExpr;
class VariableRefExpr;
class BinaryExpr;
class UnaryExpr;
  
template <typename> class IntVariableDefExpr;

template <typename> class ConstantExpr;  
typedef ConstantExpr<std::string> ConstantStringExpr;  
typedef ConstantExpr<int16_t> ConstantInt16Expr;
typedef ConstantExpr<float> ConstantSingleExpr;
typedef ConstantExpr<double> ConstantDoubleExpr;

class BIFExpr;
class CallExpr;

#endif

#define DECLARE_GENERATOR() \
  virtual bool Dispatch(CodeGenerator & generator);

//////////////////////////////////////////////////////////////////
//
// Expr - Base class for all expression nodes.
// 

class Expr
{
  public:
    virtual ~Expr()
    { }

    virtual bool Dispatch(CodeGenerator & generator);
  };

struct ExprList : public std::vector<Expr *> 
{
  ExprList(Expr * expr = nullptr)
  { 
    if (expr != nullptr)
      push_back(expr);
  }

  virtual ~ExprList() { }
};

struct SourceFileExprList : public ExprList 
{
  SourceFileExprList()
  { }

  virtual ~SourceFileExprList() { }

  DECLARE_GENERATOR()
};


//////////////////////////////////////////////////////////////////
//
// Used to mark lines, i.e for BASIC line numbers
//

struct LineMarkerExpr : public Expr 
{
  LineMarkerExpr(unsigned lineNumber)
    : m_lineNumber(lineNumber)
  { }

  LineMarkerExpr(unsigned lineNumber, const std::string & marker)
    : m_lineNumber(lineNumber)
    , m_marker(marker)
    { 
    }

  unsigned m_lineNumber;
  std::string m_marker;
  std::string m_line;
};

//////////////////////////////////////////////////////////////////
//
//  VariableDefExpr - Expression class for creating a variable on the LHS of an expression
//

class BaseVariableDefExpr : public Expr
{
  public:
    BaseVariableDefExpr(const Variable & variable, bool global)
      : m_variable(variable)
      , m_global(global)
    {       
    }

    Variable m_variable;
    bool m_global;
};

//////////////////////////////////////////////////////////////////
//
//  VariableDefExpr - Expression class for creating a variable on the LHS of an expression
//

class VariableDefExpr : public BaseVariableDefExpr
{
  public:
    VariableDefExpr(const Variable & variable, int dim, bool global)
      : BaseVariableDefExpr(variable, global)
      , m_dim(dim)
    {       
    }

    int m_dim;
};

//////////////////////////////////////////////////////////////////
//
//  VariableRefExpr - Expression class for referencing a variable, like "a".
//
class VariableRefExpr : public BaseVariableDefExpr
{
  public:
    VariableRefExpr(const Variable & variable, AST::ExprList * args, bool global)
      : BaseVariableDefExpr(variable, global)
      , m_args(args)
      { }

    int GetDim() const
    { return (m_args == nullptr) ? 0 : m_args->size(); }

    AST::ExprList * m_args;
};

//////////////////////////////////////////////////////////////////
//
// ArrayDim - Expression class for an array dimension request
//
class ArrayDimExpr : public VariableRefExpr
{
  public:
    ArrayDimExpr(const Variable & variable, AST::ExprList * args, bool global)
      : VariableRefExpr(variable, args, global)
      { }
};

//////////////////////////////////////////////////////////////////
//
// BinaryExpr - Expression class for a binary operator.
//
class BinaryExpr : public Expr
{
  public:
    BinaryExpr(char op, 
               Expr * lhs,
               Expr * rhs)
    : m_op(op) 
    , m_lhs(lhs)
    , m_rhs(rhs) 
    {
    }  
    
    char m_op;
    Expr * m_lhs;
    Expr * m_rhs;
};

//////////////////////////////////////////////////////////////////
//
// NumberExpr - base Expression class for numeric literals
//
class NumberExpr : public Expr 
{
};

template <class Type>
class ConstantExpr : public NumberExpr
{
  public:
    ConstantExpr(const Type & val)
      : m_value(val)
    { }

    Type m_value;
};

typedef ConstantExpr<std::string> ConstantStringExpr;
typedef ConstantExpr<int16_t> ConstantInt16Expr;
typedef ConstantExpr<float> ConstantSingleExpr;

//////////////////////////////////////////////////////////////////
//
// UnaryExpr - Expression class for a unary operator.
//
class UnaryExpr : public Expr 
{
  public:
    UnaryExpr(char op, std::unique_ptr<Expr> operand)
      : m_op(op)
      , m_operand(std::move(operand))
    { }

    char m_op;
    std::unique_ptr<Expr> m_operand;
};


//////////////////////////////////////////////////////////////////
//
// BIFExpr - Expression class for built in functions
//
class BIFExpr : public Expr
{
  public:
    BIFExpr(const std::string & name, ExprList * args)
      : m_name(name)
      , m_args(args)
    { }

    std::string m_name;
    ExprList * m_args = nullptr;
};

//////////////////////////////////////////////////////////////////
//
//  GotoExpr
//

class GotoExpr : public Expr
{
  public:
    GotoExpr(const std::string & marker)
      : m_marker(marker)
    {       
    }

    std::string m_marker;
};


//////////////////////////////////////////////////////////////////
//
// CallExpr - Expression class for function calls.
//
class CallExpr : public Expr
{
  public:
    CallExpr(const std::string & callee)
      : m_callee(callee)
    { }

    CallExpr(const std::string & callee, std::vector<std::unique_ptr<Expr>> args)
      : m_callee(callee)
      , m_args(std::move(args))
    { }

    std::string m_callee;
    std::vector<std::unique_ptr<Expr>> m_args;
};


template<class ArgType>
class Call1Expr : public CallExpr
{
  public:
    Call1Expr(const std::string & name, Expr * arg)
      : CallExpr(name)
    {
      m_args.push_back(std::unique_ptr<Expr>(arg));
    }
};

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

#include "codegen.h"
#include "cg_cxx.h"
#include "cg_llvm.h"

extern AST::SourceFileExprList g_expressions;
extern std::set<std::string> g_stringConstants;
extern std::set<std::string> g_gotoTargets;
extern AST::LineMarkerExpr * g_currentSourceFileMarker; 

#endif // CODEGEN_H