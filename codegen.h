
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

namespace CodeGenerator 
{
	using namespace llvm;

	class Module
	{

		public:
			Module(const char * name = "top");
			Module(llvm::LLVMContext & context, const char * name = "top");

			llvm::IRBuilder<> & GetBuilder();
			llvm::Module & GetModule();
			llvm::LLVMContext & GetContext() { return m_context; }

			void Dump();

			std::string m_name;
			llvm::LLVMContext & m_context;
	  	llvm::IRBuilder<> m_builder; 
			llvm::Module * m_module;
	};

	class Variable
	{
		public:
			enum Type {
				eString,
				eSingle,
				eDouble,
				eInteger,
				eArray
			} m_type;

			Variable(const std::string & name)
				: m_name(name)
			{ }

			virtual void Generate(Module & module) = 0;

			std::string m_name;

			std::string m_string;
			float m_single;
			double m_double;
			int m_integer;
	};

	class ASTExpr
	{
		public:
			ASTExpr();
			virtual ~ASTExpr();
			virtual llvm::Value * Generate(Module & m_module) = 0;
	};

	class BinaryOpExpr : public ASTExpr
	{
		public:
			BinaryOpExpr(ASTExpr * lhs, ASTExpr * rhs)
				: m_lhs(lhs)
				, m_rhs(rhs)
			{ }

			virtual llvm::Value * Generate(Module & module)
			{
			  llvm::Value * l = m_lhs->Generate(module);
  			llvm::Value * r = m_rhs->Generate(module);

  			return module.GetBuilder().CreateFAdd(l, r, "addtmp");
			}

			ASTExpr * m_lhs;
			ASTExpr * m_rhs;
	};

	class ConstantIntExpr : public ASTExpr
	{
		public:
			ConstantIntExpr(int val)
			  : m_value(val)
			{ }

			virtual llvm::Value * Generate(Module & module)
			{
				return ConstantInt::get(module.GetBuilder().getInt32Ty(), APInt(32, m_value));
			}

			int m_value;
	};

	class IntVarExpr : public ASTExpr
	{
		public:
			IntVarExpr();
			virtual llvm::Value * Generate(Module & m_module);
	};

	class SingleVarExpr : public ASTExpr
	{
		public:
			SingleVarExpr();
			virtual llvm::Value * Generate(Module & m_module);
	};

	class DoubleVarExpr : public ASTExpr
	{
		public:
			DoubleVarExpr();
			virtual llvm::Value * Generate(Module & m_module);
	};

	class FunctionASTExpr
	{
		public:
			FunctionASTExpr(const std::string & name, ASTExpr * body);
			virtual llvm::Function * Generate(Module & module);

			std::string m_name;
			ASTExpr * m_body;
	};

	class GlobalVariableExpr : public ASTExpr
	{
		public:
			GlobalVariableExpr(const std::string & name)
				: ASTExpr()
				, m_name(name)
			{ }

			std::string m_name;
	};

	template <class IntType, class TypeFunc>
	class GlobalIntExpr : public GlobalVariableExpr
	{
		public:
			GlobalIntExpr(const std::string & name, IntType val)
				: GlobalVariableExpr(name)
				, m_value(val)
			{ }

			virtual llvm::Value * Generate(Module & module) override
			{
				return new llvm::GlobalVariable(module.GetModule(), 
																				m_func(module),
        																true,
        																GlobalValue::CommonLinkage,
        																0,
        																m_name);		
			}

			TypeFunc m_func;
			IntType m_value;
	};

	struct GetInt32Ty { llvm::Type * operator()(Module & module) { return module.GetBuilder().getInt32Ty(); } };

	typedef GlobalIntExpr<int32_t, GetInt32Ty> GlobalInt32Expr;

	class GlobalStringExpr : public GlobalVariableExpr
	{
		public:
			GlobalStringExpr(const std::string & name, const std::string & value)
			  : GlobalVariableExpr(name)
			  , m_value(value)
			{ }

			virtual llvm::Value * Generate(Module & module) override
			{
				std::cout << "creating global string " << std::endl;

				llvm::GlobalVariable * var = new llvm::GlobalVariable(
					                              module.GetModule(), 
																				module.GetBuilder().getInt8PtrTy(),
        																true,
        																GlobalValue::CommonLinkage,
        																0,
        																m_name);		

				// Constant Definitions
 				llvm::Constant * constArray = llvm::ConstantDataArray::getString(
 																								module.GetContext(), 
 																								m_value.c_str(), 
 																								true);
			 // Global Variable Definitions
 			 var->setInitializer(constArray);

 			 return var;
			}

			std::string m_value;
	};



} // namespace CodeGenerator


#endif // CODEGEN_H