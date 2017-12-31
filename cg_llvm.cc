#include "ast.h"

using namespace std;
using namespace AST;

static ValueDefBase * CreateValueDef(const std::string & name, Variable::Type type, llvm::Value * value)
{ 
  return new LLVMValueDef(name, type, value); 
}

static ValueDefBase * CreateValueDef(const std::string & name, Variable::Type type, llvm::AllocaInst * var)
{ 
  return new LLVMValueDef(name, type, var); 
}

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

CodegenLLVM::CodegenLLVM(const std::string & genType, const std::string & inputFilename, SourceFileExprList & tree)
  : CodeGeneratorBase(genType, inputFilename, tree)
  , m_builder(m_context)
{ 
  m_module.reset(new llvm::Module("basalt", m_context));

  // set debug info version
  m_module->addModuleFlag(llvm::Module::Warning, "Debug Info Version", llvm::DEBUG_METADATA_VERSION);  

  // Darwin only supports dwarf2.
  if (llvm::Triple(llvm::sys::getProcessTriple()).isOSDarwin())
    m_module->addModuleFlag(llvm::Module::Warning, "Dwarf Version", 2);  

  // create debug info
  m_debugBuilder = llvm::make_unique<llvm::DIBuilder>(*m_module);
}

bool CodegenLLVM::Open(int argc, const char ** argv)
{
  if (!CodeGeneratorBase::Open())
    return false;

  // create output filename
  Filename ifn(m_inputFilename);  
  m_objectFilename = Filename(ifn.GetDir() + ifn.GetBasename() + ".o");

  /*llvm::DICompileUnit * theCU = */m_debugBuilder->createCompileUnit(
    llvm::dwarf::DW_LANG_C, ifn.GetFilename(), ifn.GetDir(), "Basalt", 0, "", 0);  

  /* llvm::DIFile * unit = */m_debugBuilder->createFile(ifn.GetFilename(), ifn.GetDir());  

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

  return true;
} 

llvm::FunctionType * CodegenLLVM::CreateFunctionType(const FunctionDef & fn)
{
  // create function type list
  std::vector<llvm::Type *> argTypes;
  for (auto & r : fn.m_args) {
    llvm::Type * type;
    if (r == "int16_t") { 
      type = llvm::Type::getInt16Ty(m_context);
    }
    else if (r == "const char *") {
      type = llvm::Type::getInt8PtrTy(m_context)->getPointerTo();
    }
    else if (r == "float") {
      type = llvm::Type::getFloatTy(m_module->getContext());
    }
    else if (r == "double") {
      type = llvm::Type::getDoubleTy(m_module->getContext());
    }
    else
      InternalError("unknown argument type '" << r << "'");
    argTypes.push_back(type);  
  }

  // create return type  
  llvm::FunctionType * type = nullptr;
  if ((fn.m_returnType == "void") || fn.m_returnType.empty()) {
    type = llvm::FunctionType::get(
                    llvm::Type::getVoidTy(m_context),
                    argTypes,
                    false);
  }
  else {
    InternalError("unknown function return type '" << fn.m_returnType << "'");
  }

  return type; 
}

#if 0
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
#endif

bool CodegenLLVM::OnDeclareExternalFunc(const FunctionDef & fn)
{
  llvm::FunctionType * returnType = CreateFunctionType(fn);
  
  llvm::Function * func = llvm::Function::Create(returnType, 
                                                 llvm::GlobalValue::ExternalLinkage, 
                                                 fn.m_name, 
                                                 m_module.get()
                                                );
                                                
  llvm::CallInst * call = llvm::CallInst::Create(func, "", m_mainBlock);
  call->setCallingConv(llvm::CallingConv::C);
  call->setTailCall(false);

  return true;
}

bool CodegenLLVM::OnDeclareConstString(const AST::ConstantStringExpr & expr, const std::string & name) 
{ 
  llvm::Value * value = m_builder.CreateGlobalStringPtr(expr.m_value, name);

  // need a fake string variable to hold the binding
  Variable constStringVar(Variable::Type::eStringConst, name);
  AST::VariableDefExpr * var = new VariableDefExpr(constStringVar, 0, false);
  std::unique_ptr<VarDefExprBase> ptr(new LLVMVarDef(var, value));

  m_globalScope->m_vars.insert(
    VarDefExprMap::value_type(name, std::move(ptr))
  );

  return true; 
} 


#if 0
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
#endif

bool CodegenLLVM::Close(const std::string & outputFilename)
{
  llvm::ConstantInt * const_int32_9 = llvm::ConstantInt::get(m_context, llvm::APInt(32, llvm::StringRef("0"), 10));
  llvm::ReturnInst::Create(m_context, const_int32_9, m_mainBlock);  

  m_debugBuilder->finalize();

  if (g_dump)
    m_module->dump();
    
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
  Filename exeFilename(m_inputFilename.GetDir() + m_inputFilename.GetBasename());
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
  return Run(expr);
}

bool CodegenLLVM::Visit(UnaryExpr & expr)
{
  return LLVMError(expr);
}

///////////////////////////////////////////////////////////////////////

LLVMVarDef * CodegenLLVM::FindVarRef(ScopeBase * scope, const std::string & name)
{
  auto r = scope->m_vars.find(name);
  if (r == scope->m_vars.end()) {
    if ((scope->m_parent == nullptr) || (scope == m_globalScope)) {
      InternalError("cannot find variable for '" << name << "'");
      return nullptr;
    }
    return FindVarRef(scope->m_parent, name);
  }

  cout << "found " << (void *)r->second.get() << endl;

  return dynamic_cast<LLVMVarDef *>(r->second.get());
}

bool CodegenLLVM::ReferenceConstString(const std::string & name, const std::string & val)
{
  auto r = m_globalScope->m_vars.find(name);
  if (r == m_globalScope->m_vars.end()) {
    InternalError("cannot find constant string for '" << name << "'");
    return false;
  }

  LLVMVarDef * rv = dynamic_cast<LLVMVarDef *>(r->second.get());

  if (rv->m_value == nullptr) {
    InternalError("constant string '" << name << "' is not a global variable");
    return false;    
  }

  auto v = CreateValueDef(name, Variable::Type::eString, rv->m_value);
  m_valueStack.push_back(v);
  return true;
}

bool CodegenLLVM::ReferenceVar(AST::VariableDefExpr & expr)
{
  cout << "looking for var " << expr.m_variable.m_normalizedName << " in " << (void *)m_globalScope << endl;
  LLVMVarDef * ref = FindVarRef(m_globalScope, expr.m_variable.m_normalizedName);
  if (ref == nullptr) {
    cout << "var failed" << endl;
    return false;
  }

  auto def = CreateValueDef(expr.m_variable.m_normalizedName, expr.m_variable.m_type, ref->m_value);
  m_valueStack.push_back(def);

  return true;
}

///////////////////////////////////////////////////////////////////////

#if 0

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

#endif

bool CodegenLLVM::Visit(CallExpr & expr)
{
  return LLVMError(expr);
}

bool CodegenLLVM::Visit(GotoExpr & expr)
{
  return LLVMError(expr);
}

///////////////////////////////////////////////////////////////////////

bool CodegenLLVM::Visit(ConstantExpr<float> & expr)
{
  std::string tempName(GetTempName("float_"));

  llvm::Type * type = llvm::Type::getFloatTy(m_module->getContext());
  llvm::Constant * val = llvm::ConstantFP::get(type, expr.m_value);
    
  llvm::AllocaInst * temp = new llvm::AllocaInst(type, tempName, m_mainBlock);
  temp->setAlignment(4);

  llvm::StoreInst * ins = new llvm::StoreInst(val, temp, false, m_mainBlock);
  ins->setAlignment(4);

  auto def = CreateValueDef(tempName, Variable::Type::eSingle, temp);
  m_valueStack.push_back(def);
  return true;
}

bool CodegenLLVM::Visit(ConstantExpr<short int> & expr)
{  
  std::string tempName(GetTempName("int16_"));
  llvm::ConstantInt * val = llvm::ConstantInt::get(m_context, llvm::APInt(16, expr.m_value));
    
  llvm::AllocaInst * temp = new llvm::AllocaInst(llvm::IntegerType::get(m_module->getContext(), 16), tempName, m_mainBlock);
  temp->setAlignment(4);

  llvm::StoreInst * ins = new llvm::StoreInst(val, temp, false, m_mainBlock);
  ins->setAlignment(8);

  auto def = CreateValueDef(tempName, Variable::Type::eInt16, temp);
  m_valueStack.push_back(def);
  return true;
}

bool CodegenLLVM::Visit(ConstantExpr<double> & expr)
{
  return LLVMError(expr);
}

bool CodegenLLVM::Visit(VariableRefExpr & expr)
{
  LLVMVarDef * ref = FindVarRef(m_currentScope, expr.m_variable.m_normalizedName);
  if (ref == nullptr)
    return false;

  ValueDefBase * def = CreateValueDef(expr.m_variable.m_normalizedName, expr.m_variable.m_type, ref->m_value);
  m_valueStack.push_back(def);

  return true;
}

//////////////////////////////////////////////////////////////////////////////////

void CodegenLLVM::AssignString(const std::string & lhName, const std::string & rhName)
{
  // get variable associated with RHS
  // get variable associated with LHS
  //llvm::StoreInst * ins = new llvm::StoreInst(rhName, lhs, false, m_mainBlock);
  //ins->setAlignment(8);
}

void CodegenLLVM::AssignVar(const ValueDefBase & lhs, const ValueDefBase & rhs)
{
  const LLVMValueDef & lhsLLVM = static_cast<const LLVMValueDef &>(lhs);
  llvm::Value * rhsData = static_cast<const LLVMValueDef &>(rhs).m_value;

  ValueDefBase * def;
  llvm::StoreInst * ins;
  if (lhsLLVM.m_value != nullptr) {
    ins = new llvm::StoreInst(rhsData, lhsLLVM.m_value, false, m_mainBlock);
    def = CreateValueDef(lhs.m_name, lhs.m_type, lhsLLVM.m_value);
  }
  else
    InternalError("cannot find LHS of assignment");

  ins->setAlignment(4);
    
  m_valueStack.push_back(def);    
}

void CodegenLLVM::JoinStrings(const std::string & lhName, const std::string & rhName)
{}

void CodegenLLVM::BinaryOp(const ValueDefBase & lhs, char op, const ValueDefBase & rhs)
{
  CXXScope * scope = static_cast<CXXScope *>(m_currentScope);
  std::string indent = scope->GetIndent();

  std::string tempName(GetTempName("temp"));
  auto result = CreateValueDef(tempName, lhs.m_type);

  m_codeStrm << indent 
             << CTypeForVarType(lhs.m_type) 
             << " " << result->m_name
             << " = " << lhs.m_name 
             << " " << op 
             << " " << rhs.m_name 
             << ";\n"; 

  m_valueStack.push_back(result);  
}

bool CodegenLLVM::CallFunction(const std::string & returnTypeStr, const std::string & name, const std::vector<std::string> & args)
{
  return true;
}

bool CodegenLLVM::OnDeclareVar(const AST::VariableDefExpr & expr, ScopeBase & scope)
{
  if (scope.m_scopeLevel != 0) {
    InternalError("non-global vars not supported (" << m_scopeLevel << ")");
    return false;
  }

  const Variable & var = expr.m_variable;
  llvm::GlobalVariable * gvar = nullptr;
  
  switch (var.m_type) {
    case Variable::eString:
      {
        llvm::Type * type = llvm::Type::getInt8PtrTy(m_context)->getPointerTo();
        gvar = new llvm::GlobalVariable(
          /*Module=*/      *m_module.get(),
          /*Type=*/        type,
          /*isConstant=*/  false,
          /*Linkage=*/     llvm::GlobalValue::CommonLinkage,
          /*Initializer=*/ 0, // has initializer, specified below
          /*Name=*/        var.m_normalizedName);
          gvar->setAlignment(2);
          gvar->setInitializer(0);
        }    
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
        gvar->setInitializer(0);
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
        gvar->setInitializer(llvm::ConstantFP::get(m_module->getContext(), llvm::APFloat(0.0f)));
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
        gvar->setInitializer(llvm::ConstantFP::get(m_module->getContext(), llvm::APFloat(0.0)));
      }
      break;
    default:
      InternalError("unsupported global variable type " << var.m_type); 
  }

  std::unique_ptr<VarDefExprBase> ptr(new LLVMVarDef(&expr, gvar));
  scope.m_vars.insert(
    VarDefExprMap::value_type(expr.m_variable.m_normalizedName, std::move(ptr))
  );

  cout << "declared " << expr.m_variable.m_normalizedName << " into " << (void *)&scope << endl;

  return true;
}

///////////////////////////////////////////////////////////////////////
  
ScopeBase * CodegenLLVM::CreateScope(CodeGeneratorBase & codeGen, ScopeBase * parent)
{ 
  ScopeBase * ptr = new LLVMScope(codeGen, parent);
  return ptr;
}

LLVMScope::LLVMScope(CodeGeneratorBase & codeGen, ScopeBase * parent)
  : ScopeBase(parent)
{ }