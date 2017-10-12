using namespace std;

#include "ast.h"

using namespace AST;

////////////////////////////////////////////////////////
//
// CPP visit functions
//

static bool CXXError(Expr & expr)
{
  cout << "error: unimplemented C++ function for type " << typeid(expr).name() << endl;
  return true;
}

//////////////////////////////////////////////////////////////////////////

CodegenLLVM::CodegenLLVM(SourceFileExprList & tree)
  : Visitor(tree)
{  
}

bool CodegenLLVM::Open(const std::string & inputFilename, int argc, const char ** argv)
{
  Filename ifn(inputFilename);
  
  // create output filename
  Filename objectFilename(ifn.GetDir() + ifn.GetBasename() + ".o");

  cout << "info: creating '" << objectFilename.GetFilename() << "'" << endl;
  
  return true;
} 

void CodegenLLVM::Close()
{
} 

bool CodegenLLVM::Visit(Expr & expr)
{
  return CXXError(expr);
}

bool CodegenLLVM::Visit(SourceFileExprList & expr)
{
  for (auto & r : expr) {
    if (r != nullptr)
      r->Generate(*this);
  }
  return true;
}

bool CodegenLLVM::Visit(ExprList & expr)
{
  for (auto & r : expr) {
    r->Generate(*this);
  }
  return true;
}

bool CodegenLLVM::Visit(LineMarkerExpr & expr)
{
  return CXXError(expr);
}

bool CodegenLLVM::Visit(VariableDefExpr & expr)
{
  return CXXError(expr);
}

bool CodegenLLVM::Visit(VariableRefExpr & expr)
{
  return CXXError(expr);
}

bool CodegenLLVM::Visit(UnaryExpr & expr)
{
  return CXXError(expr);
}

bool CodegenLLVM::Visit(BinaryExpr & expr)
{
  return CXXError(expr);
}

///////////////////////////////////////////////////////////////////////

bool CodegenLLVM::Visit(StringConstantExpr & expr)
{
  return CXXError(expr);
}

bool CodegenLLVM::Visit(StringVariableRefExpr & expr)
{
  return CXXError(expr);
}

bool CodegenLLVM::Visit(StringBinaryExpr & expr)
{
  return CXXError(expr);
}

///////////////////////////////////////////////////////////////////////

bool CodegenLLVM::Visit(ConstantIntExpr<short int> & expr)
{
  return CXXError(expr);
}

bool CodegenLLVM::Visit(Int16VariableRefExpr & expr)
{ 
  return CXXError(expr);
}

bool CodegenLLVM::Visit(Int16BinaryExpr & expr)
{
  return CXXError(expr);
}

///////////////////////////////////////////////////////////////////////

bool CodegenLLVM::Visit(BIFExpr & expr)
{
  return CXXError(expr);
}

bool CodegenLLVM::Visit(CallExpr & expr)
{
  return CXXError(expr);
}

///////////////////////////////////////////////////////////////////////


#if 0
///////////////////////////////////////////////////////////////////////////////
//
//  code generation
//

if (g_verbose)
  cout << "info: generating code" << endl;

CodeGenerator cg(cerr);

llvm::BasicBlock * block = cg.StartMain();

{    
  std::vector<llvm::Value *> ArgsV;

  /*
  for (unsigned i = 0, e = Args.size(); i != e; ++i) {
    ArgsV.push_back(Args[i]->codegen());
    if (!ArgsV.back())
      return nullptr;
  }
  */
  llvm::FunctionType * FT = llvm::FunctionType::get(llvm::Type::getVoidTy(cg.m_context), false);
  llvm::Function * calleeF = llvm::Function::Create(FT, llvm::GlobalValue::ExternalLinkage, "basalt_init", cg.m_module.get());    

  llvm::CallInst* int64_4 = llvm::CallInst::Create(calleeF, "", block);
  int64_4->setCallingConv(llvm::CallingConv::C);
  int64_4->setTailCall(false);    
}

cout << "generating code for " << g_expressions.size() << " expressions" << endl;

/*
  llvm::StoreInst* void_10 = new llvm::StoreInst(const_ptr_6, ptr_9, false, label_8);
  void_10->setAlignment(8);
  llvm::GetElementPtrInst* ptr_11 = llvm::GetElementPtrInst::Create(StructTy_struct_String, ptr_a, {
   const_int32_5, 
   const_int32_4
  }, "", label_8);

  llvm::StoreInst* void_12 = new llvm::StoreInst(const_int8_7, ptr_11, false, label_8);
  void_12->setAlignment(8);
}
*/   

for (auto & r : g_expressions) {
  if (r != nullptr) {
    //cout << "generating code for non-null expression" << endl;
    r->Generate(cg);
  }
}

cg.EndMain(block);

//GenerateCall0(module, "basalt_init");    
//CodeGenerator::FunctionASTExpr * mainFunc = new CodeGenerator::FunctionASTExpr("main", NULL);

//llvm::BasicBlock * entry = llvm::BasicBlock::Create(module.GetContext(), "entrypoint", mainFunc->Generate(module));
//module.GetBuilder().SetInsertPoint(entry);

if (g_dumpAsm)
  cg.Dump();

/////////////////////////////////////////////////////////////
//
//  outputting object file
//

// create output filename
Filename objectFilename(g_inputFilename.GetDir() + g_inputFilename.GetBasename() + ".o");

if (g_verbose)
  cout << "info: creating '" << objectFilename.GetFilename() << "'" << endl;

// Initialize the target registry etc.
llvm::InitializeAllTargetInfos();
llvm::InitializeAllTargets();
llvm::InitializeAllTargetMCs();
llvm::InitializeAllAsmParsers();
llvm::InitializeAllAsmPrinters();  

std::string Error;
auto Target = llvm::TargetRegistry::lookupTarget(g_targetTripleStr, Error);

// Print an error and exit if we couldn't find the requested target.
// This generally occurs if we've forgotten to initialise the
// TargetRegistry or we have a bogus target triple.
if (!Target) {
  cerr << Error;
  return 1;
}  

auto CPU = "generic";
auto Features = "";

llvm::TargetOptions opt;
auto RM = llvm::Reloc::Model();  

auto TargetMachine = Target->createTargetMachine(g_targetTripleStr, CPU, Features, opt, RM);  

cg.m_module->setDataLayout(TargetMachine->createDataLayout());
cg.m_module->setTargetTriple(g_targetTripleStr);  

std::error_code EC;
llvm::raw_fd_ostream dest(objectFilename.c_str(), EC, llvm::sys::fs::F_None);

if (EC) {
  cerr << "Could not open file: " << EC.message();
  return 1;
}  

llvm::legacy::PassManager pass;
auto FileType = llvm::TargetMachine::CGFT_ObjectFile;

if (TargetMachine->addPassesToEmitFile(pass, dest, FileType)) {
  cerr << "TargetMachine can't emit a file of this type";
  return 1;
}

pass.run(*cg.m_module);

dest.flush();  

if (g_compileOnly)
  return 0;

// do the linker thing
Filename exeFilename(g_inputFilename.GetDir() + g_inputFilename.GetBasename());
std::stringstream cmd;
cmd << "clang " << objectFilename << " -L. -lbasaltrt -o " << exeFilename ;

int result = system(cmd.str().c_str());
if (result != 0)
  cerr << "error: linker failed" << endl;

return 0;
}

#endif    

