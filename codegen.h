
#ifndef CODEGEN_H
#define CODEGEN_H


#include "llvm/IR/Verifier.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/raw_os_ostream.h"



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
			void Dump();

			std::string m_name;
			llvm::LLVMContext & m_context;
	  		llvm::IRBuilder<> m_builder; 
			llvm::Module * m_module;
	};

	class ASTExpr
	{
		public:
			ASTExpr();
			virtual ~ASTExpr();
			virtual llvm::Value * Generate(Module & m_module) = 0;
	};

	class FunctionASTExpr
	{
		public:
			FunctionASTExpr(const std::string & name);
			virtual llvm::Function * Generate(Module & module);
			
			std::string m_name;
	};

} // namespace CodeGenerator


#endif // CODEGEN_G