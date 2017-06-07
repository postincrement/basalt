#include "codegen.h"

CodeGenerator::Module::Module(const char * name)
  : m_context(llvm::getGlobalContext())
  , m_builder(m_context) 
{
	 m_module = new llvm::Module(name, m_context);
}

CodeGenerator::Module::Module(llvm::LLVMContext & context, const char * name)
  : m_context(context)
  , m_builder(m_context) 
{
	 m_module = new llvm::Module(name, m_context);
}

llvm::IRBuilder<> & CodeGenerator::Module::GetBuilder()
{ 
	return m_builder; 
} 

llvm::Module & CodeGenerator::Module::GetModule()
{ 
	return *m_module;
}


void CodeGenerator::Module::Dump()
{
	return m_module->dump();
}

/////////////////////////////////////////////////////////////

CodeGenerator::ASTExpr::ASTExpr()
{ }

CodeGenerator::ASTExpr::~ASTExpr()
{ }

CodeGenerator::FunctionASTExpr::FunctionASTExpr(const std::string & name)
  : m_name(name)
{ }

llvm::Function * CodeGenerator::FunctionASTExpr::Generate(Module & module)
{
	FunctionType * funcType = llvm::FunctionType::get(module.GetBuilder().getInt32Ty(), false);
	Function     * func     = llvm::Function::Create(funcType, 
		                                               Function::ExternalLinkage, 
		                                               m_name, 
		                                               &module.GetModule());

	return func;
}

