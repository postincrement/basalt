#include "codegen.h"

#include <sstream>

using namespace std;

/*


*/

/// CreateEntryBlockAlloca - Create an alloca instruction in the entry block of
/// the function.  This is used for mutable variables etc.
llvm::AllocaInst * CodeGenerator::CreateEntryBlockAlloca(
                                         llvm::Function * TheFunction,
                                        const std::string & VarName)
{
  llvm::IRBuilder<> TmpB(&TheFunction->getEntryBlock(), TheFunction->getEntryBlock().begin());
  return TmpB.CreateAlloca(llvm::Type::getDoubleTy(m_context), nullptr, VarName.c_str());
}

llvm::BasicBlock * CodeGenerator::StartMain()
{
  //StructDef m_stringStruct;

  // declare argument list for main
  std::vector<llvm::Type*>FuncTy_1_args;

#if 0
  FuncTy_1_args.push_back(llvm::IntegerType::get(m_module->getContext(), 32));
  llvm::PointerType* PointerTy_3 = llvm::PointerType::get(llvm::IntegerType::get(m_module->getContext(), 8), 0);
  llvm::PointerType* PointerTy_2 = llvm::PointerType::get(PointerTy_3, 0);
  FuncTy_1_args.push_back(PointerTy_2);
#endif

  // create function prototype for main
  llvm::FunctionType* FuncTy_1 = llvm::FunctionType::get(
  /*Result=*/llvm::IntegerType::get(m_module->getContext(), 32),
  /*Params=*/FuncTy_1_args,
  /*isVarArg=*/false);  

  // create code for main  
  llvm::Function * func_main = llvm::Function::Create(
        /*Type=*/    FuncTy_1,
        /*Linkage=*/ llvm::GlobalValue::ExternalLinkage,
        /*Name=*/    "main", 
                     m_module.get()
                     );

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

  {
#if 0    
    llvm::Function::arg_iterator args = func_main->arg_begin();
    llvm::ilist_iterator<llvm::Argument> int32_argc = args++;
    int32_argc->setName("argc");
    llvm::ilist_iterator<llvm::Argument> ptr_argv = args++;
    ptr_argv->setName("argv");
#endif

    m_mainBlock = llvm::BasicBlock::Create(m_context, "", func_main,0);  

#if 0
    // Block  (label_10)
    llvm::AllocaInst* ptr_11 = new llvm::AllocaInst(llvm::IntegerType::get(m_context, 32), "", label_10);
    ptr_11->setAlignment(4);
    llvm::AllocaInst* ptr_12 = new llvm::AllocaInst(PointerTy_2, "", label_10);
    ptr_12->setAlignment(8);
    llvm::StoreInst* void_13 = new llvm::StoreInst(int32_argc, ptr_11, false, label_10);
    void_13->setAlignment(4);
    llvm::StoreInst* void_14 = new llvm::StoreInst(ptr_argv, ptr_12, false, label_10);
    void_14->setAlignment(8);
    llvm::StoreInst* void_15 = new llvm::StoreInst(const_int16_8, gvar_int16_a, false, label_10);
    void_15->setAlignment(2);
    llvm::LoadInst* int16_16 = new llvm::LoadInst(gvar_int16_a, "", false, label_10);
    int16_16->setAlignment(2);
    llvm::StoreInst* void_17 = new llm::StoreInst(int16_16, gvar_int16_b, false, label_10);
    void_17->setAlignment(2);
#endif
  }

  m_builder.SetInsertPoint(m_mainBlock);   

  return m_mainBlock;
}

void CodeGenerator::EndMain(llvm::BasicBlock * block)
{
  llvm::ConstantInt * const_int32_9 = llvm::ConstantInt::get(m_context, llvm::APInt(32, llvm::StringRef("0"), 10));
  llvm::ReturnInst::Create(m_context, const_int32_9, block);
}

llvm::Value * CodeGenerator::LogError(const std::string & str)
{
  m_errorStream << str << endl;
  return nullptr;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Int16 functions
//

llvm::Value * CodeGenerator::Generate(AST::Int16ConstantExpr & expr)
{
  return llvm::ConstantInt::get(m_context, llvm::APInt(16, expr.m_intValue));
}


llvm::Value * CodeGenerator::Generate(AST::Int16VariableDefExpr & expr)
{
  llvm::AllocaInst * var = m_namedInt16Values[expr.m_name];

  if (!var) {
    var = new llvm::AllocaInst(llvm::IntegerType::get(m_context, 16), expr.m_name, m_mainBlock);
    m_namedInt16Values[expr.m_name] = var;
  }

  return nullptr; 
}

llvm::Value * CodeGenerator::Generate(AST::Int16VariableRefExpr & expr)
{
  llvm::AllocaInst * var = m_namedInt16Values[expr.m_name];

  if (!var) {
    std::stringstream strm;
    strm << "Reference to unknown int16 variable '" << expr.m_name << "'";
    return LogError(strm.str());      
  }

  // Load the value.
  return m_builder.CreateLoad(var, expr.m_name.c_str());
}


llvm::Value * CodeGenerator::Generate(AST::Int16BinaryExpr & expr)
{
  if (expr.m_op == '=') {
    AST::Int16VariableRefExpr * lhse = dynamic_cast<AST::Int16VariableRefExpr *>(expr.m_lhs);
    if (!lhse) {
      std::stringstream strm;
      strm << "LHS of assignment must be variable";
      return LogError(strm.str());      
    }
    llvm::Value * val = expr.m_rhs->Generate(*this);
    if (val == NULL)
      cout << "RHS of assignment is null" << endl;
    llvm::Value * var = m_namedInt16Values[lhse->m_name];
    if (!var) {
      std::stringstream strm;
      strm << "Unknown integer variable '" << lhse->m_name << "'";
      return LogError(strm.str());      
    }

    m_builder.CreateStore(val, var, false); 
    return var;    
  }

  if (!expr.m_lhs || !expr.m_rhs) {
    std::stringstream strm;
    strm << "Int16 binary expression '" << expr.m_op << "' failed";
    LogError(strm.str());      
    return nullptr;  
  }

  llvm::Value * lhs = expr.m_lhs->Generate(*this);
  llvm::Value * rhs = expr.m_rhs->Generate(*this);

  if (!lhs || !rhs)
    return nullptr;

  switch (expr.m_op) {
    case '+':
      return m_builder.CreateAdd(lhs, rhs, "addtmp");
    case '-':
      return m_builder.CreateSub(lhs, rhs, "subtmp");
    case '*':
      return m_builder.CreateMul(lhs, rhs, "multmp");
    case '/':
      return m_builder.CreateSDiv(lhs, rhs, "multmp");
    //case '<':
    //  lhs = m_builder.CreateCmpULT(lhs, rhs, "cmptmp");
    //  // Convert bool 0/1 to double 0.0 or 1.0
    //  return m_builder.CreateUIToFP(lhs, llvm::Type::getDoubleTy(m_context), "booltmp");
    default:
      {
        std::stringstream strm;
        strm << "Invalid binary operator '" << expr.m_op << "'";
        return LogError(strm.str());
      }
  }
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//  String functions
//


llvm::AllocaInst * CodeGenerator::CreateString(const std::string & name)
{
  // get, or create, struct structure definition
  llvm::StructType * stringStruct = m_module.get()->getTypeByName(STRING_VAR_TYPE);

  if (stringStruct == nullptr) {
    stringStruct = llvm::StructType::create(m_context, STRING_VAR_TYPE);

    std::vector<llvm::Type*> stringStructFields;

    // point to data
    stringStructFields.push_back(llvm::PointerType::get(llvm::IntegerType::get(m_context, 8), 0));

    // length
    stringStructFields.push_back(llvm::IntegerType::get(m_context, 8));

    // bool isStatic
    stringStructFields.push_back(llvm::IntegerType::get(m_context, 8));

    if (stringStruct->isOpaque())
      stringStruct->setBody(stringStructFields, /*isPacked=*/false);
  }

  // instantiate string
  llvm::AllocaInst * varPtr = new llvm::AllocaInst(stringStruct, name, m_mainBlock);
  varPtr->setAlignment(8);

  return varPtr;
}


llvm::Value * CodeGenerator::Generate(AST::StringVariableDefExpr & expr)
{
  llvm::AllocaInst * var = m_namedStringValues[expr.m_name];

  if (!var) {
    var = CreateString(expr.m_name);
    m_namedStringValues[expr.m_name] = var;
  }

  return nullptr; 
}


llvm::Value * CodeGenerator::Generate(AST::StringConstantExpr & expr)
{
  //if (expr.m_global)
    return m_builder.CreateGlobalStringPtr(expr.m_value);

  //return nullptr; 
}

llvm::Value * CodeGenerator::Generate(AST::StringVariableRefExpr & expr)
{
  llvm::AllocaInst * var = m_namedStringValues[expr.m_name];

  if (!var) {
    std::stringstream strm;
    strm << "Reference to unknown string variable '" << expr.m_name << "'";
    return LogError(strm.str());      
  }

  //llvm::ConstantInt * dataPtr = llvm::ConstantInt::get(m_context, llvm::APInt(32, llvm::StringRef("0"), 10));
  //llvm::ConstantInt * length  = llvm::ConstantInt::get(m_context, llvm::APInt(32, llvm::StringRef("0"), 10));

  /*
  llvm::GetElementPtrInst::Create(stringStruct, varPtr, {
        dataPtr, 
        length
  }, "", m_mainBlock);
  */

  // Load the value.
  return m_builder.CreateLoad(var, expr.m_name.c_str());
}


llvm::Value * CodeGenerator::Generate(AST::StringBinaryExpr & expr)
{
  if (expr.m_op == '=') {
    AST::StringVariableRefExpr * lhse = dynamic_cast<AST::StringVariableRefExpr *>(expr.m_lhs);
    if (!lhse) {
      std::stringstream strm;
      strm << "LHS of assignment must be variable";
      return LogError(strm.str());      
    }
    llvm::Value * val = expr.m_rhs->Generate(*this);
    if (val == NULL)
      cout << "RHS of assignment is null" << endl;
    llvm::Value * var = m_namedStringValues[lhse->m_name];
    if (!var) {
      std::stringstream strm;
      strm << "Unknown string variable '" << lhse->m_name << "'";
      return LogError(strm.str());      
    }

#if 0
    %TODO need assignment here
    ConstantInt * const_int32_7 = ConstantInt::get(mod->getContext(), APInt(32, StringRef("1"), 10));
    ConstantInt * const_int32_8 = ConstantInt::get(mod->getContext(), APInt(32, StringRef("0"), 10));

    GetElementPtrInst * ptr_22 = GetElementPtrInst::Create(StructTy_struct_String, ptr_a, {
     const_int32_8, 
     const_int32_7
    }, "", label_13);

    StoreInst* void_23 = new StoreInst(const_int8_12, ptr_22, false, label_13);    

    m_builder.CreateStore(val, var, false); 
#endif

    return var;    
  }

  if (!expr.m_lhs || !expr.m_rhs) {
    std::stringstream strm;
    strm << "String binary expression failed";
    LogError(strm.str());      
    return nullptr;
  }

  llvm::Value * lhs = expr.m_lhs->Generate(*this);
  llvm::Value * rhs = expr.m_rhs->Generate(*this);

  if (!lhs || !rhs)
    return nullptr;

  switch (expr.m_op) {
    case '+':
    case '-':
    case '*':
    case '/':
    default:
      {
        std::stringstream strm;
        strm << "Invalid binary operator '" << expr.m_op << "'";
        return LogError(strm.str());
      }
  }
}


llvm::Value * CodeGenerator::Generate(AST::BIFExpr & expr)
{
  if (expr.m_name == "print") {
    for (auto & r : *expr.m_args) {

      if (r == nullptr)
        continue;

      llvm::Value * val = r->Generate(*this);
      if (val == nullptr)
        continue;
      
      llvm::Type * type = val->getType();
      std::string funcName;

      std::vector<llvm::Value *> args;
      args.push_back(val);
      std::vector<llvm::Type *> argTypes;

      if (type == llvm::Type::getInt8PtrTy(m_context)) {
        argTypes.push_back(type);
        funcName = "basalt_print_string";
      }
      else if (type == llvm::Type::getInt16Ty(m_context)) {
        argTypes.push_back(type);
        funcName = "basalt_print_integer";  
      }
      else {
        std::string type_str;
        llvm::raw_string_ostream rso(type_str);
        type->print(rso);
        cerr << "error: unknown argument type to print '" << rso.str() << "'" << endl;
        return nullptr;
      }

      llvm::ArrayRef<llvm::Type*> argsRef(argTypes);
      llvm::FunctionType * funcType = llvm::FunctionType::get(m_builder.getVoidTy(), argsRef, false);
      llvm::Constant     * func     = m_module->getOrInsertFunction(funcName, funcType);  
      m_builder.CreateCall(func, args);  
    }
  }
  else {
    cerr << "error: unknown built-in function '" << expr.m_name << "'" << endl;
  }
  return nullptr;
}

llvm::Value * CodeGenerator::Generate(AST::Call1Expr<char *> & expr)
{
  //llvm::ArrayRef<llvm::Type*> argsRef(argTypes);
  //llvm::FunctionType * funcType = llvm::FunctionType::get(module.GetBuilder().getInt32Ty(), argsRef, false);
  //llvm::Constant * func         = module.GetModule().getOrInsertFunction(name, funcType);  
  //m_builder.CreateCall(func, arg);  

  return nullptr; 
}


llvm::Value * CodeGenerator::Generate(AST::Call1Expr<int16_t *> & expr)
{
  return nullptr; 
}

#if 0

llvm::Value * CodeGenerator::Generate(AST::Expr & expr)
{
  cout << "primitive generate called" << endl;
  return nullptr;
}




llvm::Value * CodeGenerator::Generate(AST::IntVariableDefExpr & expr)
{
  llvm::AllocaInst * var = m_namedValues[expr.m_name];

  if (!var) {
    var = CreateEntryBlockAlloca(TheFunction, expr.m_name);
    m_namedValues[expr.m_name] = var;
  }

  return nullptr;
}

llvm::Value * CodeGenerator::Generate(AST::IntVariableDefExpr & expr)
{
/*  
  AllocaInst * var = m_namedValues[expr.m_name];

  if (!value) {
    var = CreateEntryBlockAlloca(TheFunction, expr.m_name);
    m_namedValues[expr.m_name] = var;
  }

  // Load the value.
  return Builder.CreateLoad(V, Name.c_str());
  */
  return nullptr;
}

llvm::Value * CodeGenerator::Generate(AST::CallExpr & expr)
{
  // Look up the name in the global module table.
  llvm::Function * calleeF = m_module->getFunction(expr.m_callee);
  if (!calleeF) {
    std::stringstream strm;
    strm << "Unknown function '" << expr.m_callee << "' referenced";
    return LogError(strm.str());
  }

  // If argument mismatch error.
  if (calleeF->arg_size() != expr.m_args.size()) {
    std::stringstream strm;
    strm << "Incorrect number of arguments passed to '" << expr.m_callee
         << "' - should be " << calleeF->arg_size() << ", is " << expr.m_args.size();
    return LogError(strm.str());
  }

  std::vector<llvm::Value *> argsV;
  for (unsigned i = 0, e = expr.m_args.size(); i != e; ++i) {
    argsV.push_back(Generate(*expr.m_args[i]));
    if (!argsV.back())
      return nullptr;
  }

  return m_builder.CreateCall(calleeF, argsV, "calltmp");
}

typedef ConstantIntExpr<int16_t> ConstantInt16Expr;
typedef ConstantIntExpr<int32_t> ConstantInt32Expr;



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

void CodeGenerator::GenerateCall0(Module & module, const std::string & name)
{
  std::vector<llvm::Type *> argTypes;
  llvm::ArrayRef<llvm::Type*> argsRef(argTypes);
  llvm::FunctionType * funcType = llvm::FunctionType::get(module.GetBuilder().getInt32Ty(), argsRef, false);
  llvm::Constant * func         = module.GetModule().getOrInsertFunction(name, funcType);  
  module.GetBuilder().CreateCall(func);
}

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

CodeGenerator::FunctionExprAST::FunctionExprAST(const std::string & name, ExprAST * body)
  : m_name(name)
  , m_body(body)
{ }

llvm::Function * CodeGenerator::FunctionExprAST::Generate(Module & module)
{
  FunctionType * funcType = llvm::FunctionType::get(module.GetBuilder().getInt32Ty(), false);
  Function     * func     = llvm::Function::Create(funcType, 
                                                   Function::ExternalLinkage, 
                                                   m_name, 
                                                   &module.GetModule());

  // Create a new BasicBlockic block to start insertion into.
  //llvm::BasicBlock * BB = llvm::BasicBlock::Create(module.GetContext(), "entry", func);
  //module.GetBuilder().SetInsertPoint(BB); 

  //llvm::Value * retVal = m_body->Generate(module);

  //module.GetBuilder().CreateRet(retVal);     

  return func;
}

#endif
