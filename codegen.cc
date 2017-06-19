#include "codegen.h"

std::map<std::string, llvm::Value *> CodeGenerator::m_globalStrings;


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

/*
  // Create a new pass manager attached to it.fpm
  llvm::FunctionPassManager * fpm = module->get();

  // Do simple "peephole" optimizations and bit-twiddling optzns.
  fpm->add(createInstructionCombiningPass());
  // Reassociate expressions.
  fpm->add(createReassociatePass());
  // Eliminate Common SubExpressions.
  fpm->add(createGVNPass());
  // Simplify the control flow graph (deleting unreachable blocks, etc).
  fpm->add(createCFGSimplificationPass());

  fpm->doInitialization();
*/  
} 

/*
void CodeGenerator::Module::Optimize()
{
  //if (m_fpm != NULL)
  //  m_fpm->run(*TheFunction);
}
*/

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

CodeGenerator::ASTExpr::ASTExpr()
{ }

CodeGenerator::ASTExpr::~ASTExpr()
{ }

/////////////////////////////////////////////////////////////

CodeGenerator::IntVarExpr::IntVarExpr()
{
}

llvm::Value * CodeGenerator::IntVarExpr::Generate(Module & m_module)
{
  // Look this variable up in the function.
  //llvm::Value * v = NamedValues[Name];
  //if (v == NULL)
  //  LogErrorV("Unknown variable name");

  //return v; 
  return NULL;
}


/////////////////////////////////////////////////////////////

CodeGenerator::SingleVarExpr::SingleVarExpr()
{
}

llvm::Value * CodeGenerator::SingleVarExpr::Generate(Module & m_module)
{
  return NULL;
}


/////////////////////////////////////////////////////////////

CodeGenerator::DoubleVarExpr::DoubleVarExpr()
{
}

llvm::Value * CodeGenerator::DoubleVarExpr::Generate(Module & m_module)
{
  return NULL;
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
