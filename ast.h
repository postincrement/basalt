
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
virtual bool Visit(AST::VariableDefExpr & expr); \
virtual bool Visit(AST::VariableRefExpr & expr); \
virtual bool Visit(AST::BinaryExpr & expr); \
\
virtual bool Visit(AST::UnaryExpr & expr); \
\
virtual bool Visit(AST::ConstantStringExpr & expr); \
virtual bool Visit(AST::ConstantInt16Expr & expr); \
virtual bool Visit(AST::ConstantSingleExpr & expr); \
\
virtual bool Visit(AST::BIFExpr & expr); \
virtual bool Visit(AST::CallExpr & expr); \
virtual bool Visit(AST::LineMarkerExpr & expr) \

namespace AST {

  class AbstractDispatcher;

  class Expr;
  class ExprList;
  class SourceFileExprList;
  class LineMarkerExpr;
  
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

  class BIFExpr;
  class CallExpr;
  
  class Visitor
  {
    public:
      Visitor(SourceFileExprList & tree);

      virtual ~Visitor() { }

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

  class Dispatcher : public AbstractDispatcher
  {
    public:
      virtual bool Generate(CodegenDumper & dumper) override = 0;
      virtual bool Generate(CodegenCXX & generator) override = 0;  
      virtual bool Generate(CodegenLLVM & generator) override = 0;  
  };

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
  LineMarkerExpr(unsigned lineNumber)
    : m_lineNumber(lineNumber)
  { }

  LineMarkerExpr(unsigned lineNumber, const std::string & marker)
    : m_lineNumber(lineNumber)
    , m_marker(marker)
    { 
    }

  IMPLEMENT_EXPR_VISITOR();

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

    IMPLEMENT_EXPR_VISITOR();

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

    IMPLEMENT_EXPR_VISITOR();

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

    IMPLEMENT_EXPR_VISITOR();

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

    IMPLEMENT_EXPR_VISITOR();
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

    IMPLEMENT_EXPR_VISITOR();
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

    IMPLEMENT_EXPR_VISITOR();

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

    IMPLEMENT_EXPR_VISITOR();
    
    std::string m_name;
    ExprList * m_args = nullptr;
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

class CodeGenerator : public AST::Visitor
{
  public:
    CodeGenerator(AST::SourceFileExprList & tree);

    // open generator
    virtual bool Open(const std::string & inputFilename, int argc, const char ** argv) = 0;

    // close generator
    virtual bool Close() = 0;

    bool FindConstStrings();
    
    template <class Fn>
    bool Traverse(AST::ExprList & list, Fn & fn)
    {
      for (auto & r : list) {
        if (r != nullptr) {
          if (!fn(*r))
            return false;
        }
      }

      return true;
    }

    std::string GetTempName(const std::string & prefix);

    unsigned m_tempCounter = 1;
    std::map<std::string, std::string> m_constStrings;    
};

#include "cg_cxx.h"
#include "cg_llvm.h"

extern AST::SourceFileExprList g_expressions;
extern std::set<std::string> g_stringConstants;
extern AST::LineMarkerExpr * g_currentSourceFileMarker; 

#endif // CODEGEN_H