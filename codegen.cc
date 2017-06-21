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

void CodeGenerator::GenerateCall1(Module & module, 
                  const std::string & name, 
                  std::vector<llvm::Type *> & argTypes, 
                  llvm::Value * arg)
{
  llvm::ArrayRef<llvm::Type*> argsRef(argTypes);
  llvm::FunctionType * funcType = llvm::FunctionType::get(module.GetBuilder().getInt32Ty(), argsRef, false);
  llvm::Constant * func         = module.GetModule().getOrInsertFunction(name, funcType);  
  module.GetBuilder().CreateCall(func, arg);
}

/////////////////////////////////////////////////////////////

CodeGenerator::FunctionASTExpr::FunctionASTExpr(const std::string & name, ASTExpr * body)
  : m_name(name)
  , m_body(body)
{ }

llvm::Function * CodeGenerator::FunctionASTExpr::Generate(Module & module)
{
  FunctionType * funcType = llvm::FunctionType::get(module.GetBuilder().getInt32Ty(), false);
  Function     * func     = llvm::Function::Create(funcType, 
                                                   Function::ExternalLinkage, 
                                                   m_name, 
                                                   &module.GetModule());

  // Create a new basic block to start insertion into.
  //llvm::BasicBlock * BB = llvm::BasicBlock::Create(module.GetContext(), "entry", func);
  //module.GetBuilder().SetInsertPoint(BB); 

  //llvm::Value * retVal = m_body->Generate(module);

  //module.GetBuilder().CreateRet(retVal);     

  return func;
}
