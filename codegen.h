
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

namespace CodeGenerator  {

using namespace llvm;

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

class ASTExpr
{
	public:
		ASTExpr()
		{ }

		virtual ~ASTExpr()
		{ }

		virtual llvm::Value * Generate(Module & module) = 0;
};

struct GetInt16Ty     { llvm::Type * operator()(Module & module) { return module.GetBuilder().getInt16Ty(); } };
struct GetInt32Ty     { llvm::Type * operator()(Module & module) { return module.GetBuilder().getInt32Ty(); } };

struct GetPtrToInt8Ty  { llvm::Type * operator()(Module & module) { return module.GetBuilder().getInt8Ty()->getPointerTo(); } };
struct GetPtrToInt16Ty { llvm::Type * operator()(Module & module) { return module.GetBuilder().getInt16Ty()->getPointerTo(); } };


class ValuedASTExpr : public ASTExpr
{
	public:
		ValuedASTExpr()
		  : m_llvmValue(nullptr)
		{ }

		virtual llvm::Value * Generate(Module & module)
		{
			if (m_llvmValue == nullptr)
				m_llvmValue = GenerateOnce(module);

			return m_llvmValue;
		}

		virtual llvm::Value * GenerateOnce(Module & module) = 0;

  private:
  	llvm::Value * m_llvmValue;	
};


typedef std::vector<ASTExpr *> ASTExprList;

class Variable : public ValuedASTExpr
{
	public:
		enum Type
		{
			eInteger,
			eSingle,
			eDouble,
			eString
		};

		Variable(Type type, const std::string & name, bool global)
			: m_type(type)
			, m_name(name)
			, m_global(global)
		{ }

		Type GetType() const
		{ return m_type; }

		Type m_type;
		std::string m_name;
		bool m_global;
};

template <class IntType, class TypeFunc, int NumBits>
class IntVariable : public Variable
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
class ConstantIntExpr : public ASTExpr
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

class ConstantStringExpr : public ValuedASTExpr
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


void GenerateCall0(Module & module, 
	      const std::string & name);

void GenerateCall1(Module & module, 
                  const std::string & name, 
                  std::vector<llvm::Type *> & argTypes, 
                  llvm::Value * arg);

class FunctionASTExpr
{
	public:
		FunctionASTExpr(const std::string & name, ASTExpr * body);
		virtual llvm::Function * Generate(Module & module);

		std::string m_name;
		ASTExpr * m_body;
	};

class Call0Expr : public ASTExpr
{
	public:
		Call0Expr(const std::string & name)
			: ASTExpr()
			, m_name(name)
		{ }

		virtual llvm::Value * Generate(Module & module) override
		{
		  GenerateCall0(module, m_name);
		  return NULL;
		}

		std::string m_name;
};

template<class ArgTypeFunc>
class Call1Expr : public ASTExpr
{
	public:
		Call1Expr(const std::string & name, ASTExpr * arg)
			: ASTExpr()
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
		ASTExpr * m_arg;
};

class NumericAssignExpr : public ASTExpr
{
	public:
		NumericAssignExpr(Variable * variable, ASTExpr * expr)
			: ASTExpr()
			, m_lhs(variable)
			, m_rhs(expr)
		{ 			
		}

		virtual llvm::Value * Generate(Module & module) override
		{
			llvm::Value * lhs = m_lhs->Generate(module);
			llvm::Value * rhs = m_rhs->Generate(module);

			llvm::StoreInst * val = new llvm::StoreInst(rhs, lhs, false, module.GetBuilder().GetInsertBlock());
      val->setAlignment(2);

			return val;
		}

		Variable * m_lhs;
		ASTExpr * m_rhs;
};

class StringAssignExpr : public ASTExpr
{
	public:
		StringAssignExpr(Variable * variable, ASTExpr * expr)
			: ASTExpr()
			, m_lhs(variable)
			, m_rhs(expr)
		{ }

		virtual llvm::Value * Generate(Module & module) override
		{
			/*
			llvm::Value * arg = m_arg->Generate(module);

		  // generator code for void puts(i8 *)
		  std::vector<llvm::Type *> argTypes;
		  argTypes.push_back(module.GetBuilder().getInt8Ty()->getPointerTo());

		  GenerateCall1(module, "puts", argTypes, arg);
      */

		  return NULL;
		}

		Variable * m_lhs;
		ASTExpr * m_rhs;
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

} // namespace CodeGenerator


struct PrintElement 
{
  PrintElement()
    : m_isTab(false)
    , m_expr(nullptr)
  { }
  bool m_isTab;
  CodeGenerator::ASTExpr * m_expr;
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



#endif // CODEGEN_H