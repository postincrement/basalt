
#ifndef AST_H
#define AST_H

#include <iostream>
#include <map>
#include <vector>
#include <memory>
#include <deque>
#include <sstream>

#include "common.h"

class CodegenDumper;
class CodegenCXX;
class CodegenLLVM;

#define DECLARE_EXPR_VISITOR() \
virtual bool Generate(AST::Visitor & visitor)

#define IMPLEMENT_EXPR_VISITOR() \
DECLARE_EXPR_VISITOR() { return visitor.Visit(*this); } \

#define DECLARE_EXPR_VISIT_FUNCTIONS() \
virtual bool Visit(AST::Expr & expr); \
virtual bool Visit(AST::ExprList & expr); \
virtual bool Visit(AST::SourceFileExprList & expr); \
\
virtual bool Visit(AST::VariableDefExpr & expr); \
virtual bool Visit(AST::VariableRefExpr & expr); \
virtual bool Visit(AST::UnaryExpr & expr); \
virtual bool Visit(AST::BinaryExpr & expr); \
\
virtual bool Visit(AST::StringVariableRefExpr & expr); \
virtual bool Visit(AST::StringConstantExpr & expr); \
virtual bool Visit(AST::StringBinaryExpr & expr); \
\
virtual bool Visit(AST::Int16VariableRefExpr & expr); \
virtual bool Visit(AST::ConstantIntExpr<int16_t> & expr); \
virtual bool Visit(AST::Int16BinaryExpr & expr); \
\
virtual bool Visit(AST::BIFExpr & expr); \
virtual bool Visit(AST::CallExpr & expr); \
virtual bool Visit(AST::LineMarkerExpr & expr) \

namespace AST {

  class AbstractDispatcher;

  class Expr;
  class ExprList;
  class SourceFileExprList;

  class VariableDefExpr;
  class VariableRefExpr;
  class BinaryExpr;
  class UnaryExpr;
  
  class StringVariableDefExpr;
  class StringVariableRefExpr;
  class StringConstantExpr;
  class StringBinaryExpr;
  
  template <typename> class IntVariableDefExpr;
  class Int16VariableRefExpr;
  template <typename> class ConstantIntExpr;  
  class Int16BinaryExpr;
  
  class BIFExpr;
  class CallExpr;
  class LineMarkerExpr;

  class Visitor
  {
    public:
      Visitor(SourceFileExprList & tree);

      virtual ~Visitor() { }

      // open generator
      virtual bool Open(const std::string & inputFilename, int argc, const char ** argv) = 0;

      // close generator
      virtual void Close() = 0;

      // generate code
      virtual bool Generate(AbstractDispatcher & dispatcher) = 0;

      // visit functions
      DECLARE_EXPR_VISIT_FUNCTIONS();

      SourceFileExprList & m_tree;
    };
  
  class AbstractDispatcher
  {
    public:
      virtual bool Generate(CodegenDumper & generator) = 0;  
      virtual bool Generate(CodegenCXX & generator) = 0;  
      virtual bool Generate(CodegenLLVM & generator) = 0;  
    };
} // namespace AST

#include "cg_dump.h"
#include "cg_cxx.h"
#include "cg_llvm.h"

namespace AST 
{
  class Dispatcher : public AbstractDispatcher
  {
    public:
      virtual bool Generate(CodegenDumper & dumper) override = 0;
      virtual bool Generate(CodegenCXX & generator) override = 0;  
      virtual bool Generate(CodegenLLVM & generator) override = 0;  
    };
    
//////////////////////////////////////////////////////////////////
//
// used to track variables
//

struct Variable
{
  enum Type {
    eString,
    eInt16,
    eSingle,
    eDouble
  };

  Variable(Type type, const std::string & name)
    : m_type(type)
    , m_name(name)
  { }

  Type m_type;
  std::string m_name;  
};


struct VariableDef : public Variable
{
  VariableDef(Type type, const std::string & name, const std::string & defName)
    : Variable(type, name)
    , m_defName(defName)
  { }

  std::string m_defName;
};

typedef std::map<std::string, VariableDef> VariableDefList;  

//////////////////////////////////////////////////////////////////
//
// Expr - Base class for all expression nodes.
// 

class Expr
{
  public:
    virtual ~Expr()
    { }

    IMPLEMENT_EXPR_VISITOR();
  };

struct ExprList : public std::vector<Expr *> 
{
  ExprList(Expr * expr = nullptr)
  { 
    if (expr != nullptr)
      push_back(expr);
  }

  virtual ~ExprList() { }
  
  IMPLEMENT_EXPR_VISITOR();
};

struct SourceFileExprList : public ExprList 
{
  SourceFileExprList()
  { }

  virtual ~SourceFileExprList() { }
  
  IMPLEMENT_EXPR_VISITOR();
};


//////////////////////////////////////////////////////////////////
//
// Used to mark lines, i.e for BASIC line numbers
//

struct LineMarkerExpr : public Expr 
{
  LineMarkerExpr()
  { }

  LineMarkerExpr(const std::string & marker)
    : m_marker(marker)
  { }

  IMPLEMENT_EXPR_VISITOR();

  std::string m_marker;
  std::string m_line;
};

//////////////////////////////////////////////////////////////////
//
// NumberExpr - base Expression class for numeric literals
//
class NumberExpr : public Expr 
{
};

template <class IntType>
class ConstantIntExpr : public NumberExpr
{
  public:
    ConstantIntExpr(IntType val)
      : m_intValue(val)
    { }

    IntType m_intValue;

    IMPLEMENT_EXPR_VISITOR();
};

typedef ConstantIntExpr<int16_t> Int16ConstantExpr;

//////////////////////////////////////////////////////////////////
//
// StringExpr - Expression class for string literals
//
class StringConstantExpr : public Expr
{
  public:
    StringConstantExpr(const std::string & value, bool global)
      : m_global(global)
      , m_value(value)
    { }

    IMPLEMENT_EXPR_VISITOR();
  
    bool m_global;
    std::string m_value;
};


//////////////////////////////////////////////////////////////////
//
//  VariableDefExpr - Expression class for creating a variable on the LHS of an expression
//

class VariableDefExpr : public Expr
{
  public:
    VariableDefExpr(Variable & variable, bool global)
      : m_variableName(variable)
      , m_global(global)
    { }

    AST::Variable::Type GetType() const
    { return m_variableName.m_type; }

    const std::string & GetName() const
    { return m_variableName.m_name; }

    IMPLEMENT_EXPR_VISITOR();

    Variable m_variableName;
    bool m_global;
};


//////////////////////////////////////////////////////////////////
//
//  VariableRefExpr - Expression class for referencing a variable, like "a".
//
class VariableRefExpr : public Expr
{
  public:
    VariableRefExpr(const std::string & name)
      : m_name(name)
    { }

    IMPLEMENT_EXPR_VISITOR();

    const std::string & GetName() const
    { return m_name; }

    std::string m_name;
};

class Int16VariableRefExpr : public VariableRefExpr
{
  public:
    Int16VariableRefExpr(const std::string & name)
      : VariableRefExpr(name)
    {
    }

    IMPLEMENT_EXPR_VISITOR();
};

class StringVariableRefExpr : public VariableRefExpr
{
  public:
    StringVariableRefExpr(const std::string & name)
      : VariableRefExpr(name)
    {
    }

    IMPLEMENT_EXPR_VISITOR();
};

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

    IMPLEMENT_EXPR_VISITOR();

    char m_op;
    std::unique_ptr<Expr> m_operand;
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
    
    IMPLEMENT_EXPR_VISITOR();
    
    char m_op;
    Expr * m_lhs;
    Expr * m_rhs;
};

class Int16BinaryExpr : public BinaryExpr
{
  public:
    Int16BinaryExpr(char op, 
               Expr * lhs,
               Expr * rhs)
    : BinaryExpr(op, lhs, rhs) 
    {
    }   

    IMPLEMENT_EXPR_VISITOR();
};

class StringBinaryExpr : public BinaryExpr
{
  public:
    StringBinaryExpr(char op, 
               Expr * lhs,
               Expr * rhs)
    : BinaryExpr(op, lhs, rhs) 
    {
    }   

    IMPLEMENT_EXPR_VISITOR();
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

    IMPLEMENT_EXPR_VISITOR();
    
    std::string m_name;
    ExprList * m_args;
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

    IMPLEMENT_EXPR_VISITOR();

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

    IMPLEMENT_EXPR_VISITOR();
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

extern AST::SourceFileExprList g_expressions;


#endif // CODEGEN_H