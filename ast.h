
#ifndef AST_H
#define AST_H

#include <iostream>

class CodeGenerator;

namespace AST {

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

    virtual llvm::Value * Generate(CodeGenerator & cg) = 0;
};

typedef std::vector<Expr *> ExprList;

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

    virtual llvm::Value * Generate(CodeGenerator & cg);
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

    virtual llvm::Value * Generate(CodeGenerator & cg);

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

    virtual llvm::Value * Generate(CodeGenerator & cg);
};

typedef IntVariableDefExpr<int16_t> Int16VariableDefExpr;

class StringVariableDefExpr : public VariableDefExpr
{
  public:
    StringVariableDefExpr(const std::string & name, bool global)
      : VariableDefExpr(name, global)
    { }

    virtual llvm::Value * Generate(CodeGenerator & cg);
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

    virtual llvm::Value * Generate(CodeGenerator & cg);
};

class StringVariableRefExpr : public VariableRefExpr
{
  public:
    StringVariableRefExpr(const std::string & name)
      : VariableRefExpr(name)
    {
    }

    virtual llvm::Value * Generate(CodeGenerator & cg);
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

    virtual llvm::Value * Generate(CodeGenerator & cg);
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

    virtual llvm::Value * Generate(CodeGenerator & cg);
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

    virtual llvm::Value * Generate(CodeGenerator & cg);

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

    virtual llvm::Value * Generate(CodeGenerator & cg);
};


} // namespace AST

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