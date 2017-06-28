
#ifndef AST_H
#define AST_H

#include <iostream>

namespace AST {

/// Expr - Base class for all expression nodes.
class Expr
{
  public:
    virtual ~Expr()
    { }
};

typedef std::vector<Expr *> ExprList;

/// NumberExpr - base Expression class for numeric literals
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
};

typedef ConstantIntExpr<uint16_t> ConstantInt16Expr;

/// VariableExpr - Expression class for referencing a variable, like "a".
class VariableExpr : public Expr
{
  public:
    enum Type
    {
      eInteger,
      eSingle,
      eDouble,
      eString
    };

    VariableExpr(Type type, const std::string & name, bool global)
      : m_type(type)
      , m_name(name)
      , m_global(global)
    { }

    Type GetType() const
    { return m_type; }

    const std::string & GetName() const
    { return m_name; }

    Type m_type;
    std::string m_name;
    bool m_global;
};

/// UnaryExpr - Expression class for a unary operator.
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

/// BinaryExpr - Expression class for a binary operator.
class BinaryExpr : public Expr
{
  public:
    BinaryExpr(char op, 
               std::unique_ptr<Expr> lhs,
               std::unique_ptr<Expr> rhs)
    : m_op(op) 
    , m_lhs(std::move(lhs))
    , m_rhs(std::move(rhs)) 
    {
    }   

    char m_op;
    std::unique_ptr<Expr> m_lhs;
    std::unique_ptr<Expr> m_rhs;
};

/// CallExpr - Expression class for function calls.
class CallExpr : public Expr
{
  public:
    CallExpr(const std::string & callee,
                std::vector<std::unique_ptr<Expr>> args)
      : m_callee(callee)
      , m_args(std::move(args))
    { }

    std::string m_callee;
    std::vector<std::unique_ptr<Expr>> m_args;
};

/// VarExpr - Expression class for declaring variables
class VarExpr : public Expr 
{
  public:
    VarExpr(const std::string & name, 
               std::unique_ptr<Expr> x,
               std::unique_ptr<Expr> body)
      : m_name(name)
      , m_x(std::move(x))
      , m_body(std::move(body))
    { }

    std::string m_name;
    std::unique_ptr<Expr> m_x;
    std::unique_ptr<Expr> m_body;
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

// new classes

#if 0

class Module
{
  public:
    Module(const char * name = "main");
    Module(llvm::LLVMContext & context, const char * name = "main");

    llvm::IRBuilder<> & GetBuilder();
    llvm::Module & GetModule();
    llvm::LLVMContext & GetContext() { return m_context; }

    void Dump();

    std::string m_name;
    llvm::LLVMContext & m_context;
    llvm::IRBuilder<> m_builder; 
    llvm::Module * m_module;
};


class Call0Expr : public Expr
{
  public:
    Call0Expr(const std::string & name)
      : Expr()
      , m_name(name)
    { }

    virtual llvm::Value * Generate(Module & module) override
    {
      GenerateCall0(module, m_name);
      return NULL;
    }

    std::string m_name;
};


/*
void GenerateCall0(Module & module, 
        const std::string & name);

void GenerateCall1(Module & module, 
                  const std::string & name, 
                  std::vector<llvm::Type *> & argTypes, 
                  llvm::Value * arg);


template<class ArgTypeFunc>
class Call1Expr : public Expr
{
  public:
    Call1Expr(const std::string & name, Expr * arg)
      : Expr()
      , m_name(name)
      , m_arg(arg)
    { }

    virtual llvm::Value * Generate(Module & module) override
    {
      llvm::Value * arg = m_arg->Generate(module);

      std::vector<llvm::Type *> argTypes;
      argTypes.push_back(m_argType(module));

      GenerateCall1(module, m_name, argTypes, arg);
      return NULL;
    }

    std::string m_name;
    ArgTypeFunc m_argType;
    Expr * m_arg;
};
*/

template <class IntType, class TypeFunc, int NumBits>
class IntVariable : public VariableExpr
{
  public:
    IntVariable(const std::string & name, bool global)
      : Variable(eInteger, name, global)
    {
    }

    virtual llvm::Value * GenerateOnce(Module & module)  override
    {
      GlobalVariable * var = new llvm::GlobalVariable(module.GetModule(), 
                                                m_func(module),
                                                false,
                                                GlobalValue::CommonLinkage,
                                                0, // has initializer, specified below
                                                m_name.c_str());
      var->setAlignment(NumBits / 8);

      llvm::ConstantInt * init = ConstantInt::get(module.GetContext(), APInt(NumBits, StringRef("0"), 10));
      var->setInitializer(init);

      return var;
    }

    TypeFunc m_func;
};

typedef IntVariable<int16_t, GetInt16Ty, 2> Int16Variable;

class StringVariable : public Variable
{
  public:
    StringVariable(const std::string & name, bool global)
      : Variable(eString, name, global)
    {
    }

    virtual llvm::Value * GenerateOnce(Module & m_module) override
    {
      return nullptr;
    }
};


class ConstantStringExpr : public ValuedExpr
{
  public:
    ConstantStringExpr(const std::string & value, bool global)
      : m_global(global)
      , m_value(value)
    { }

    virtual llvm::Value * GenerateOnce(Module & module) override
    {
      if (m_global)
        return module.GetBuilder().CreateGlobalStringPtr(m_value.c_str());

      return nullptr; 
    }

    bool m_global;
    std::string m_value;
};


class FunctionExpr
{
  public:
    FunctionExpr(const std::string & name, Expr * body);
    virtual llvm::Function * Generate(Module & module);

    std::string m_name;
    Expr * m_body;
  };

class NumericAssignExpr : public Expr
{
  public:
    NumericAssignExpr(Variable * variable, Expr * expr)
      : Expr()
      , m_lhs(variable)
      , m_rhs(expr)
    {       
    }

    virtual llvm::Value * Generate(Module & module) override
    {
      llvm::Value * rhs = m_rhs->Generate(module);
      llvm::Value * lhs = m_lhs->Generate(module);

      llvm::StoreInst * val = new llvm::StoreInst(rhs, lhs, false, module.GetBuilder().GetInsertBlock());
      val->setAlignment(2);

      return val;
    }

    Variable * m_lhs;
    Expr * m_rhs;
};

class StringAssignExpr : public Expr
{
  public:
    StringAssignExpr(Variable * variable, Expr * expr)
      : Expr()
      , m_lhs(variable)
      , m_rhs(expr)
    { }

    virtual llvm::Value * Generate(Module & module) override
    {
      return NULL;
    }

    Variable * m_lhs;
    Expr * m_rhs;
};

struct VariableList 
{
  template<class VarT>
  Variable * Add(const std::string & name, bool global)
  {
    auto r = m_vars.find(name);
    if (r != m_vars.end()) { 
      if (
          (dynamic_c<VarT *>(r->second) != NULL) &&
          (r->second->m_global == global)
          )
          return r->second;
      else
        return nullptr;
    }

    Variable * var = new VarT(name, global);
    m_vars[name] = var;
    return var;
  }

  std::map<std::string, Variable *> m_vars;
};

struct CodeGen
{
  CodeGen(const std::string & name = "basalt")
    : m_module(llvm::make_unique<llvm::Module>(name, m_context))
    , m_builder(m_context)
  { 
  }

  std::unique_ptr<llvm::Module> m_module; 
  llvm::LLVMContext m_context;
  llvm::IRBuilder<> m_builder;

  std::map<std::string, llvm::AllocaInst *> m_namedValues;

  //VariableList g_variables;
  //ExprList g_expressions;  
};


struct GetInt16Ty     { llvm::Type * operator()(CodeGen & cg) { return cg.m_builder.getInt16Ty(); } };
struct GetInt32Ty     { llvm::Type * operator()(CodeGen & cg) { return cg.m_builder.getInt32Ty(); } };

struct GetPtrToInt8Ty  { llvm::Type * operator()(CodeGen & cg) { return cg.m_builder.getInt8Ty()->getPointerTo(); } };
struct GetPtrToInt16Ty { llvm::Type * operator()(CodeGen & cg) { return cg.m_builder.getInt16Ty()->getPointerTo(); } };


/// NumberTypeExpr - Expression class for typed numeric literals
template<class IntType, class LLVMTypeFunc>
class IntNumberTypeExpr : public NumberExpr 
{
  public:
    IntNumberTypeExpr(IntType val) 
      : m_val(val) {}

    llvm::Value * Generate(CodeGen & cg) override
    {
      return llvm::ConstantInt::get(m_typeFunc(), m_val, true);
    }

    IntType m_val;
    LLVMTypeFunc m_typeFunc;
};

typedef IntNumberTypeExpr<uint16_t, GetInt16Ty> Int16NumberTypeExpr;




} // namespace AST


struct PrintElement 
{
  PrintElement()
    : m_isTab(false)
    , m_expr(nullptr)
  { }
  bool m_isTab;
  CodeGenerator::Expr * m_expr;
};

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

typedef std::vector<PrintElement *> PrintElementList;

#endif



#endif // CODEGEN_H