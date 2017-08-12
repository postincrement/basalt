
#ifndef CODEGEN_H
#define CODEGEN_H

#include <iostream>

#include "llvm/IR/Verifier.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Verifier.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/raw_os_ostream.h"
#include "llvm/Support/TargetRegistry.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/Host.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/TargetRegistry.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Target/TargetOptions.h"
#include "llvm/Target/TargetOptions.h"
#include "llvm/Passes/PassBuilder.h"

#include "ast.h"

#define	STRING_VAR_TYPE			"basalt_StringType"


class CodeGenerator  
{
	public:
		CodeGenerator(std::ostream & errorStream, const std::string & name = "basalt")
	  	: m_module(llvm::make_unique<llvm::Module>(name, m_context))
			, m_builder(m_context)
			, m_errorStream(errorStream)
		{ 
  	}

  	//StructDef m_stringStruct;

		llvm::BasicBlock * StartMain();
		void EndMain(llvm::BasicBlock * block);

		llvm::AllocaInst * CreateEntryBlockAlloca(
                                         llvm::Function * TheFunction,
                                        const std::string & VarName);

  	void Dump()
  	{
	    m_module->dump();
  	}

		llvm::Value * LogError(const std::string & str);

		//virtual llvm::Value * Generate() = 0;

		llvm::Value * Generate(AST::Int16VariableRefExpr & expr);
		llvm::Value * Generate(AST::Int16ConstantExpr & expr);
		llvm::Value * Generate(AST::Int16VariableDefExpr & expr);
		llvm::Value * Generate(AST::Int16BinaryExpr & expr);

		llvm::Value * Generate(AST::StringVariableRefExpr & expr);
	  llvm::Value * Generate(AST::StringVariableDefExpr & expr);
	  llvm::Value * Generate(AST::StringConstantExpr & expr);
		llvm::Value * Generate(AST::StringBinaryExpr & expr);

	  llvm::Value * Generate(AST::BIFExpr & expr);

		llvm::Value * Generate(AST::Call1Expr<char *> & expr);
		llvm::Value * Generate(AST::Call1Expr<int16_t *> & expr);

		llvm::AllocaInst * CreateString(const std::string & name);

		llvm::LLVMContext m_context;
		std::unique_ptr<llvm::Module> m_module;	
		llvm::IRBuilder<> m_builder;
		std::ostream & m_errorStream;
		std::map<std::string, llvm::AllocaInst *> m_namedInt16Values;
		std::map<std::string, llvm::AllocaInst *> m_namedStringValues;

		llvm::BasicBlock * m_mainBlock;
};

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


class Call0Expr : public ExprAST
{
	public:
		Call0Expr(const std::string & name)
			: ExprAST()
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
class Call1Expr : public ExprAST
{
	public:
		Call1Expr(const std::string & name, ExprAST * arg)
			: ExprAST()
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
		ExprAST * m_arg;
};
*/

template <class IntType, class TypeFunc, int NumBits>
class IntVariable : public VariableExprAst
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

template <class IntType, class TypeFunc, int NumBits>
class ConstantIntExpr : public ExprAST
{
	public:
		ConstantIntExpr(IntType val)
		  : m_intValue(val)
		{ }

		virtual llvm::Value * Generate(Module & module) override
		{
			llvm::Value * val = llvm::ConstantInt::get(m_func(module), APInt(NumBits, m_intValue));			
			return val;
		}

		TypeFunc m_func;
		IntType m_intValue;
};


typedef ConstantIntExpr<int16_t, GetInt16Ty, 16> ConstantInt16Expr;

typedef ConstantIntExpr<int32_t, GetInt32Ty, 32> ConstantInt32Expr;

class ConstantStringExpr : public ValuedExprAST
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


class FunctionExprAST
{
	public:
		FunctionExprAST(const std::string & name, ExprAST * body);
		virtual llvm::Function * Generate(Module & module);

		std::string m_name;
		ExprAST * m_body;
	};

class NumericAssignExpr : public ExprAST
{
	public:
		NumericAssignExpr(Variable * variable, ExprAST * expr)
			: ExprAST()
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
		ExprAST * m_rhs;
};

class StringAssignExpr : public ExprAST
{
	public:
		StringAssignExpr(Variable * variable, ExprAST * expr)
			: ExprAST()
			, m_lhs(variable)
			, m_rhs(expr)
		{ }

		virtual llvm::Value * Generate(Module & module) override
		{
		  return NULL;
		}

		Variable * m_lhs;
		ExprAST * m_rhs;
};

struct VariableList 
{
	template<class VarT>
	Variable * Add(const std::string & name, bool global)
	{
		auto r = m_vars.find(name);
		if (r != m_vars.end()) { 
			if (
				  (dynamic_cast<VarT *>(r->second) != NULL) &&
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

#endif


#if 0

struct GetInt16Ty     { llvm::Type * operator()(CodeGen & cg) { return cg.m_builder.getInt16Ty(); } };
struct GetInt32Ty     { llvm::Type * operator()(CodeGen & cg) { return cg.m_builder.getInt32Ty(); } };

struct GetPtrToInt8Ty  { llvm::Type * operator()(CodeGen & cg) { return cg.m_builder.getInt8Ty()->getPointerTo(); } };
struct GetPtrToInt16Ty { llvm::Type * operator()(CodeGen & cg) { return cg.m_builder.getInt16Ty()->getPointerTo(); } };


/// NumberTypeExprAST - Expression class for typed numeric literals
template<class IntType, class LLVMTypeFunc>
class IntNumberTypeExprAST : public NumberExprAST 
{
	public:
  	IntNumberTypeExprAST(IntType val) 
  		: m_val(val) {}

  	llvm::Value * Generate(CodeGen & cg) override
  	{
  		return llvm::ConstantInt::get(m_typeFunc(), m_val, true);
		}

  	IntType m_val;
  	LLVMTypeFunc m_typeFunc;
};

typedef IntNumberTypeExprAST<uint16_t, GetInt16Ty> Int16NumberTypeExprAST;


struct PrintElement 
{
  PrintElement()
    : m_isTab(false)
    , m_expr(nullptr)
  { }
  bool m_isTab;
  CodeGenerator::ExprAST * m_expr;
};

typedef std::vector<PrintElement *> PrintElementList;

#endif 

#endif // CODEGEN_H