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

CodegenLLVM::CodegenLLVM(const std::string & genType, SourceFileExprList & tree)
  : CodeGenerator(genType, tree)
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
  m_mainBlock = llvm::BasicBlock::Create(m_context, "main", func_main, 0);  
  m_builder.SetInsertPoint(m_mainBlock);   

  // output global string definitions
  for (auto & r : m_constStrings)
    m_constStringValues[r.first] = m_builder.CreateGlobalStringPtr(r.first, r.second);

  // look for global variable definitions
  {
    for (auto & r : m_globalVars) {
      Variable & var = r.second->m_variable;
      llvm::GlobalVariable * gvar = nullptr;
        
      switch (var.m_type) {
        case Variable::eString:
          break;
        case Variable::eInt16:
          {
            llvm::Type * type = llvm::IntegerType::get(m_module->getContext(), 16);
            gvar = new llvm::GlobalVariable(
              /*Module=*/      *m_module.get(),
              /*Type=*/        type,
              /*isConstant=*/  false,
              /*Linkage=*/     llvm::GlobalValue::CommonLinkage,
              /*Initializer=*/ 0, // has initializer, specified below
              /*Name=*/        var.m_normalizedName);
            gvar->setAlignment(2);
            gvar->setInitializer(llvm::ConstantFP::get(type, 0));
          }
          break;
        case Variable::eSingle:
          {
            llvm::Type * type = llvm::Type::getFloatTy(m_module->getContext());
            gvar = new llvm::GlobalVariable(
              /*Module=*/      *m_module.get(),
              /*Type=*/        type,
              /*isConstant=*/  false,
              /*Linkage=*/     llvm::GlobalValue::CommonLinkage,
              /*Initializer=*/ 0, // has initializer, specified below
              /*Name=*/        var.m_normalizedName);
            gvar->setAlignment(4);
            gvar->setInitializer(llvm::ConstantFP::get(type, 0));
          }
          break;
        case Variable::eDouble:
          {
            llvm::Type * type = llvm::Type::getDoubleTy(m_module->getContext());
            gvar = new llvm::GlobalVariable(
              /*Module=*/      *m_module.get(),
              /*Type=*/        type,
              /*isConstant=*/  false,
              /*Linkage=*/     llvm::GlobalValue::CommonLinkage,
              /*Initializer=*/ 0, // has initializer, specified below
              /*Name=*/        var.m_normalizedName);
            gvar->setAlignment(8);
            gvar->setInitializer(llvm::ConstantFP::get(type, 0));
          }
          break;
        default:
          InternalError("unsupported global variable type " << var.m_type); 
      }
      m_globalVarValues[var.m_normalizedName] = gvar;
    }
  }

  // declare runtime init function
  CreateBIFCall("init");
  
  return true;
} 

llvm::FunctionType * CodegenLLVM::CreateFunctionType(const char * typeStr)
{
  llvm::FunctionType * type = nullptr;
  std::string str((typeStr == nullptr) ? "" : typeStr);

  if ((str == "void") || (str == "")) {
    type = llvm::FunctionType::get(
                    llvm::Type::getVoidTy(m_context),
                    false);
  }
  else {
    InternalError("unknown BIF return type '" << typeStr << "'");
  }

  return type; 
}

void CodegenLLVM::CreateCallExternalFunc(RuntimeFunctionDef & funcDef)
{
  llvm::FunctionType * returnType = CreateFunctionType(funcDef.m_returnType);
  
  std::string funcName(g_runtimeDefPrefix);
  funcName += funcDef.m_name;

  llvm::Function * func = llvm::Function::Create(returnType, 
                                                 llvm::GlobalValue::ExternalLinkage, 
                                                 funcName.c_str(), 
                                                 m_module.get()
                                                );
  llvm::CallInst * call = llvm::CallInst::Create(func, "", m_mainBlock);
  call->setCallingConv(llvm::CallingConv::C);
  call->setTailCall(false);    
}

void CodegenLLVM::CreateFunctionCall(RuntimeFunctionDef & funcDef ...)
{
  va_list argValues;
  va_start(argValues, funcDef);

  VCreateFunctionCall(funcDef.m_returnType, funcDef.m_name, funcDef.m_args, argValues);
}

void CodegenLLVM::CreateFunctionCall(
                                      const char * returnTypeStr,
                                      const char * name,
                                      const char * argsStr
                                      ...)
{
  va_list argValues;
  va_start(argValues, argsStr);

  VCreateFunctionCall(returnTypeStr, name, argsStr, argValues);
}

void CodegenLLVM::VCreateFunctionCall(                                      
                                      const char * returnTypeStr,
                                      const char * name,
                                      const char * argsStr_,
                                      va_list varg)
{
  // create return type
  llvm::FunctionType * returnType = CreateFunctionType(returnTypeStr);

  std::vector<llvm::Value *> args;

  // create argument type list
  std::vector<llvm::Type *> argTypes;
  if (argsStr_ != nullptr) {
    std::string argStr(argsStr_);
    std::vector<std::string> tokens;
    Tokenize(tokens, argStr, ',');
    for (auto & r : tokens) {
      llvm::Type * type;
      llvm::Value * val;
      if (r == "int16_t") { 
        type = llvm::Type::getInt16Ty(m_context);
        val = llvm::ConstantInt::get(m_context, llvm::APInt(16, va_arg(varg, int)));
      }
      else if (r == "const char *") {
        type = llvm::Type::getInt8PtrTy(m_context)->getPointerTo();
        val = va_arg(varg, llvm::Value *);
      }
      else if (r == "float") {
        type = llvm::Type::getFloatTy(m_module->getContext()),
        val = va_arg(varg, llvm::Value *);
      }
      else if (r == "double") {
        llvm::Value * var = va_arg(varg, llvm::Value *);
        llvm::LoadInst * loadInst = new llvm::LoadInst(var, "", false, m_mainBlock);
        loadInst->setAlignment(8);
        type = llvm::Type::getDoubleTy(m_module->getContext()),
        val = loadInst;
      }
      else
        InternalError("unknown argument type '" << r << "'");
      argTypes.push_back(type);  
      args.push_back(val);
    }
  }

  // create function call
  llvm::ArrayRef<llvm::Type*> argsRef(argTypes);
  llvm::Constant * func = m_module->getOrInsertFunction(name, returnType);  
  m_builder.CreateCall(func, args);
}

void CodegenLLVM::CreateBIFCall(const std::string & name ...)
{
  va_list argValues;
  va_start(argValues, name);

  std::string bifName(g_runtimeDefPrefix);
  bifName += name;
  
  int i = 0;
  while (g_runtimeDefs[i].m_name != nullptr) {
    RuntimeFunctionDef & funcDef = g_runtimeDefs[i];
    if (name == funcDef.m_name) {
      VCreateFunctionCall(funcDef.m_returnType, bifName.c_str(), funcDef.m_args, argValues);
      return;
    }
    ++i;
  }

  InternalError("unknown BIF '" << name << "'");
}

bool CodegenLLVM::Close()
{
  llvm::ConstantInt * const_int32_9 = llvm::ConstantInt::get(m_context, llvm::APInt(32, llvm::StringRef("0"), 10));
  llvm::ReturnInst::Create(m_context, const_int32_9, m_mainBlock);  

  if (g_dump) {
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
  //m_builder.SetCurrentDebugLocation(llvm::DebugLoc::get(AST->getLine(), AST->getCol(), Scope));  
  return true;
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

bool CodegenLLVM::Visit(ConstantStringExpr & expr)
{
  return LLVMError(expr);
}

bool CodegenLLVM::Visit(ConstantExpr<float> & expr)
{
  return LLVMError(expr);
}

bool CodegenLLVM::Visit(ConstantExpr<short int> & expr)
{
  return LLVMError(expr);
}

bool CodegenLLVM::Visit(ConstantExpr<double> & expr)
{
  return LLVMError(expr);
}

///////////////////////////////////////////////////////////////////////

bool CodegenLLVM::Visit(BIFExpr & expr)
{
  if (
      (expr.m_name == "print_eol") ||
      (expr.m_name == "print_tab")
     )  
    CreateBIFCall(expr.m_name);

  else if (expr.m_name == "print_expr") {
    if ((expr.m_args == nullptr) || (expr.m_args->size() != 1))
      InternalError("BIF print has invalid args");
    else {      
      AST::Expr * printExpr = (*expr.m_args)[0];

      // string constant
      AST::ConstantStringExpr * constantString = dynamic_cast<AST::ConstantStringExpr *>(printExpr);
      if (constantString != nullptr) {
        llvm::Value * value = m_constStringValues[constantString->m_value];
        CreateBIFCall("print_string", value);
        return true;
      }

      // variable reference
      AST::VariableRefExpr * varRef = dynamic_cast<AST::VariableRefExpr *>(printExpr);
      if (varRef != nullptr) {
        auto r = m_globalVarValues.find(varRef->m_variable.m_normalizedName);
        if (r == m_globalVarValues.end())
          InternalError("cannot find global variable def for '" << varRef->m_variable.m_name << "'");
        switch (varRef->m_variable.m_type) {
          case Variable::eString:
            CreateBIFCall("print_string", r->second);
            break;          
          case Variable::eInt16:
            CreateBIFCall("print_int16",  r->second);
            break;          
          case Variable::eSingle:
            CreateBIFCall("print_single",  r->second);
            break;          
          case Variable::eDouble:
            CreateBIFCall("print_double",  r->second);
            break;          
         case Variable::eUntyped:
            InternalError("unknown print variable type " << varRef->m_variable.m_type);
        }
        return true;
      }
      
      // unknown
      else {
        InternalError("unknown print expression type " << typeid(printExpr).name());
      }
    }  
  } 

  else {
    Warning(eWarning_UnknownLLVMBIF, "unknown BIF '" << expr.m_name << "'");
  }

  return true;
}

bool CodegenLLVM::Visit(CallExpr & expr)
{
  return LLVMError(expr);
}

bool CodegenLLVM::Visit(GotoExpr & expr)
{
  return LLVMError(expr);
}


///////////////////////////////////////////////////////////////////////

