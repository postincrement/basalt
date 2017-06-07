

#include "llvm/IR/Verifier.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/raw_os_ostream.h"


namespace CodeGenerator 
{
	class Module
	{
		public:
			Module(const char * name = "top")
			  : m_context(llvm::getGlobalContext())
			  , m_builder(m_context) 
			{
	  		 m_module = new llvm::Module(name, m_context);
			}

			Module(llvm::LLVMContext & context, const char * name = "top")
			  : m_context(context)
			  , m_builder(m_context) 
			{
	  		 m_module = new llvm::Module(name, m_context);
			}

			llvm::IRBuilder<> & GetBuilder()
			{ 
				return m_builder; 
			} 

			llvm::Module & GetModule()
			{ 
				return *m_module;
			}


			void Dump()
			{
				return m_module->dump();
			}


			llvm::LLVMContext & m_context;
	  	llvm::IRBuilder<> m_builder; 
			llvm::Module * m_module;
	};

	class Statement
	{
		public:
			Statement(Module & module)
				: m_module(module)
			{ }

			Module & m_module;
	};

	class Expression : public Statement
	{
		public:

	};

	class Function : public Statement
	{
		public:
			Function(Module & module)
				: Statement(module)
				, m_func(NULL)
			{ }

			llvm::FunctionType * Get()
			{
				if (m_func == NULL)
					m_func = llvm::FunctionType::get(m_module.GetBuilder().getInt32Ty(), false);
				return m_func;
			}

			llvm::FunctionType * m_func;
	};


} // namespace CodeGenerator


int main(int argc, char const *argv[])
{
	CodeGenerator::Module module;

  CodeGenerator::Function mainFunction(module);

  llvm::Function * mainFunc = llvm::Function::Create(mainFunction.Get(), 
  	                                                 llvm::Function::ExternalLinkage, 
  	                                                 "main", 
  	                                                 &module.GetModule());  
 
  module.Dump();

  return 0;
}
