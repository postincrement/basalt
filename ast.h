
#ifndef AST_H
#define AST_H

#include <iostream>
#include <map>
#include <vector>
#include <memory>
#include <deque>
#include <sstream>

#define DECLARE_EXPR_VISITOR() \
virtual bool Accept(AST::Visitor & visitor)

#define IMPLEMENT_EXPR_VISITOR() \
DECLARE_EXPR_VISITOR() { return visitor.Visit(*this); } \

#define DECLARE_EXPR_VISIT_FUNCTIONS() \
virtual bool Visit(Expr & expr); \
virtual bool Visit(ExprList & expr); \
\
virtual bool Visit(VariableDefExpr & expr); \
virtual bool Visit(VariableRefExpr & expr); \
virtual bool Visit(UnaryExpr & expr); \
virtual bool Visit(BinaryExpr & expr); \
\
virtual bool Visit(StringVariableDefExpr & expr); \
virtual bool Visit(StringVariableRefExpr & expr); \
virtual bool Visit(StringConstantExpr & expr); \
virtual bool Visit(StringBinaryExpr & expr); \
\
virtual bool Visit(IntVariableDefExpr<int16_t> & expr); \
virtual bool Visit(Int16VariableRefExpr & expr); \
virtual bool Visit(ConstantIntExpr<int16_t> & expr); \
virtual bool Visit(Int16BinaryExpr & expr); \
\
virtual bool Visit(BIFExpr & expr); \
virtual bool Visit(PrintCommaExpr & expr); \
virtual bool Visit(PrintSemiColonExpr & expr); \
virtual bool Visit(CallExpr & expr) \

namespace AST {

  class AbstractDispatcher;

  class Expr;
  class ExprList;

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
  class PrintCommaExpr;
  class PrintSemiColonExpr;
  class CallExpr;

  class Visitor
  {
    public:
      virtual ~Visitor() { }

      // dispatcher functions
      virtual bool Accept(AbstractDispatcher & dispatcher) = 0;

      // visit functions
      DECLARE_EXPR_VISIT_FUNCTIONS();
  };

  class Dumper;
  
  class AbstractDispatcher
  {
    public:
      virtual bool Dispatch(Dumper & dumper) = 0;  
  };

  class Dumper : public Visitor
  {
    public:
      Dumper(ExprList & tree, std::ostream & strm); 
  
      DECLARE_EXPR_VISIT_FUNCTIONS();      

      virtual bool Accept(AbstractDispatcher & dispatcher) override
      { return dispatcher.Dispatch(*this); }

      ExprList & m_tree;
      std::ostream & m_strm;

      std::deque<std::string> m_vars;
  };
  
  class Dispatcher : public AbstractDispatcher
  {
    public:
      virtual bool Dispatch(Dumper & dumper);
  };
    
//////////////////////////////////////////////////////////////////
//
// used to track variables
//

struct VariableDef
{
  enum Type {
    eString,
    eInt16,
    eSingle,
    eDouble
  };

  VariableDef()
  { }

  VariableDef(Type type, const std::string & name, const std::string & defName)
    : m_type(type)
    , m_name(name)
    , m_defName(defName)
  { }

  Type m_type;
  std::string m_name;
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
  ExprList()
    : m_lineNumber(-1)
  { }

  virtual ~ExprList() { }
  
  IMPLEMENT_EXPR_VISITOR();

  signed m_lineNumber;  
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
    VariableDefExpr(const std::string & name, bool global)
      : m_name(name)
      , m_global(global)
    { }

    const std::string & GetName() const
    { return m_name; }

    IMPLEMENT_EXPR_VISITOR();

    std::string m_name;
    bool m_global;
};


template <class IntType>
class IntVariableDefExpr : public VariableDefExpr
{
  public:
    IntVariableDefExpr(const std::string & name, bool global)
      : VariableDefExpr(name, global)
    {
    }

    IMPLEMENT_EXPR_VISITOR();
};

typedef class IntVariableDefExpr<int16_t> Int16VariableDefExpr;

class StringVariableDefExpr : public VariableDefExpr
{
  public:
    StringVariableDefExpr(const std::string & name, bool global)
      : VariableDefExpr(name, global)
    { }

    IMPLEMENT_EXPR_VISITOR();
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
// PrintCommaExpr - placeholder for printing commas
//
class PrintCommaExpr : public Expr
{
  public:
    PrintCommaExpr()
    { }

    IMPLEMENT_EXPR_VISITOR();
};


//////////////////////////////////////////////////////////////////
//
// PrintSemiColonExpr - placeholder for concatenation, and disabling trailing newline
//
class PrintSemiColonExpr : public Expr
{
  public:
    PrintSemiColonExpr()
    { }   
    
    IMPLEMENT_EXPR_VISITOR();
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


extern AST::ExprList g_expressions;
extern AST::VariableDefList g_variables;


#endif // CODEGEN_H