#include "ast.h"

using namespace std;
using namespace AST;

////////////////////////////////////////////////////////
//
// LLVM visit functions
//

static bool LLVMError(const std::string & str)
{
  cout << "error: unimplemented LLVM function for type " << str << endl;
  return true;
}

static bool LLVMError(Expr & expr)
{
  return LLVMError(typeid(expr).name());
}

//////////////////////////////////////////////////////////////////////////

CodegenLLVM::CodegenLLVM(SourceFileExprList & tree)
  : CodeGenerator(tree)
  , m_builder(m_context)
{ 
  m_module.reset(new llvm::Module("basalt", m_context));
}

bool CodegenLLVM::Open(const std::string & inputFilename, int argc, const char ** argv)
{
  m_srcFilename = inputFilename;

  // create output filename
  Filename ifn(inputFilename);  
  m_objectFilename = Filename(ifn.GetDir() + ifn.GetBasename() + ".o");

  // create function prototype for main
  std::vector<llvm::Type*>FuncTy_1_args;
  llvm::FunctionType * FuncTy_1 = llvm::FunctionType::get(
       /*Result=*/   llvm::IntegerType::get(m_module->getContext(), 32),
       /*Params=*/   FuncTy_1_args,
       /*isVarArg=*/ false
      );  				
      
      
  // create code for main  
  llvm::Function * func_main = llvm::Function::Create(
        /*Type=*/    FuncTy_1,
        /*Linkage=*/ llvm::GlobalValue::ExternalLinkage,
        /*Name=*/    "main", 
                     m_module.get()
      );
  
  // set calling convention and other attributes    
  func_main->setCallingConv(llvm::CallingConv::C);
  llvm::AttributeSet func_main_PAL;
  {
    llvm::SmallVector<llvm::AttributeSet, 4> Attrs;
    llvm::AttributeSet PAS;
    {
      llvm::AttrBuilder B;
      B.addAttribute(llvm::Attribute::NoUnwind);
      B.addAttribute(llvm::Attribute::UWTable);
      PAS = llvm::AttributeSet::get(m_context, ~0U, B);
    }

    Attrs.push_back(PAS);
    func_main_PAL = llvm::AttributeSet::get(m_context, Attrs);
  }
  func_main->setAttributes(func_main_PAL);

  // create top level block
  m_mainBlock = llvm::BasicBlock::Create(m_context, "", func_main, 0);  
  m_builder.SetInsertPoint(m_mainBlock);   

  // look for global string definitions
  {
    FindConstStrings fn(*this);
    Traverse<FindConstStrings>(m_tree, fn);
    for (auto & r : m_constStrings) {
      m_builder.CreateGlobalStringPtr(r.first, r.second);
    }
  }

  
  // make call to runtime init function
  {    
    std::vector<llvm::Value *> ArgsV;

    llvm::FunctionType * FT = llvm::FunctionType::get(llvm::Type::getVoidTy(m_context), false);
    llvm::Function * calleeF = llvm::Function::Create(FT, llvm::GlobalValue::ExternalLinkage, "basalt_init", m_module.get());    

    llvm::CallInst* int64_4 = llvm::CallInst::Create(calleeF, "", m_mainBlock);
    int64_4->setCallingConv(llvm::CallingConv::C);
    int64_4->setTailCall(false);    
  }

  return true;
} 

bool CodegenLLVM::Close()
{
  llvm::ConstantInt * const_int32_9 = llvm::ConstantInt::get(m_context, llvm::APInt(32, llvm::StringRef("0"), 10));
  llvm::ReturnInst::Create(m_context, const_int32_9, m_mainBlock);  

  if (g_dumpAsm) {
    m_module->dump();
    return true;
  }
    
  /////////////////////////////////////////////////////////////
  //
  //  outputting object file
  //

  // Initialize the target registry etc.
  llvm::InitializeAllTargetInfos();
  llvm::InitializeAllTargets();
  llvm::InitializeAllTargetMCs();
  llvm::InitializeAllAsmParsers();
  llvm::InitializeAllAsmPrinters();  

  std::string Error;
  std::string targetTripleStr = llvm::sys::getDefaultTargetTriple();
  auto Target = llvm::TargetRegistry::lookupTarget(targetTripleStr, Error);

  // Print an error and exit if we couldn't find the requested target.
  // This generally occurs if we've forgotten to initialise the
  // TargetRegistry or we have a bogus target triple.
  if (!Target) {
    cerr << Error << endl;
    return false;
  }  

  if (g_compileOnly) 
    cout << "info: creating '" << m_objectFilename.GetFilename() << "'" << endl;

  auto CPU = "generic";
  auto Features = "";

  llvm::TargetOptions opt;
  auto RM = llvm::Reloc::Model();  

  auto TargetMachine = Target->createTargetMachine(targetTripleStr, CPU, Features, opt, RM);  

  m_module->setDataLayout(TargetMachine->createDataLayout());
  m_module->setTargetTriple(targetTripleStr);  

  std::error_code EC;
  llvm::raw_fd_ostream dest(m_objectFilename.c_str(), EC, llvm::sys::fs::F_None);

  if (EC) {
    cerr << "Could not open file: " << EC.message();
    return false;
  }  

  llvm::legacy::PassManager pass;
  auto FileType = llvm::TargetMachine::CGFT_ObjectFile;

  if (TargetMachine->addPassesToEmitFile(pass, dest, FileType)) {
    cerr << "TargetMachine can't emit a file of this type";
    return false;
  }

  pass.run(*m_module);

  dest.flush();  

  if (g_compileOnly) 
    return true;

  // do the linker thing
  Filename exeFilename(m_srcFilename.GetDir() + m_srcFilename.GetBasename());
  cout << "info: creating '" << exeFilename.GetFilename() << "'" << endl;
  
  std::stringstream cmd;
  cmd << "clang " << m_objectFilename << " -L. -lbasaltrt -o " << exeFilename ;

  int result = system(cmd.str().c_str());
  if (result != 0)
    cerr << "error: linker failed with command:\n"
         << cmd.str() << endl;

  return true;
}

bool CodegenLLVM::Visit(Expr & expr)
{
  return LLVMError(expr);
}

bool CodegenLLVM::Visit(ExprList & expr)
{
  return LLVMError(typeid(expr).name());
}

bool CodegenLLVM::Visit(SourceFileExprList & expr)
{
  for (auto & r : expr) {
    if (r != nullptr)
      r->Generate(*this);
  }
  return true;
}

bool CodegenLLVM::Visit(LineMarkerExpr & expr)
{
  return LLVMError(expr);
}

bool CodegenLLVM::Visit(VariableDefExpr & expr)
{
  return LLVMError(expr);
}

bool CodegenLLVM::Visit(VariableRefExpr & expr)
{
  return LLVMError(expr);
}

bool CodegenLLVM::Visit(UnaryExpr & expr)
{
  return LLVMError(expr);
}

bool CodegenLLVM::Visit(BinaryExpr & expr)
{
  return LLVMError(expr);
}

///////////////////////////////////////////////////////////////////////

bool CodegenLLVM::Visit(StringConstantExpr & expr)
{
  return LLVMError(expr);
}

///////////////////////////////////////////////////////////////////////

bool CodegenLLVM::Visit(ConstantIntExpr<short int> & expr)
{
  return LLVMError(expr);
}

///////////////////////////////////////////////////////////////////////

bool CodegenLLVM::Visit(BIFExpr & expr)
{
  return LLVMError(expr);
}

bool CodegenLLVM::Visit(CallExpr & expr)
{
  return LLVMError(expr);
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

