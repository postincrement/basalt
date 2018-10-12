  
#ifndef CG_LLVM_H_
#define CG_LLVM_H_

#include "ast.h"
#include "basalt.h"

#include "llvm/IR/Verifier.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Verifier.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/raw_os_ostream.h"
#include "llvm/Support/TargetRegistry.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/Host.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/TargetRegistry.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Target/TargetOptions.h"
#include "llvm/Target/TargetOptions.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/IR/DIBuilder.h"

///////////////////////////////////////////////////////////////////////////

class CodegenLLVM;

struct LLVMVarDef : public VarDefExprBase 
{
/*
  LLVMVarDef(const AST::VariableDefExpr * expr, llvm::AllocaInst * var)
    : VarDefExprBase(expr)
    , m_value(var)
  { }

  LLVMVarDef(const AST::VariableDefExpr * expr, llvm::GlobalVariable * glob)
    : VarDefExprBase(expr)
    , m_value(glob)
  { }
*/

  LLVMVarDef(const AST::VariableDefExpr * expr, llvm::Value * value)
    : VarDefExprBase(expr)
    , m_value(value)
  { }

  llvm::Value * m_value = nullptr;
};

///////////////////////////////////////////////////////////////////////////

struct LLVMValueDef : public ValueDefBase
{
  LLVMValueDef(const std::string & name, Variable::Type type, llvm::Value * value)
    : ValueDefBase(name, type)
    , m_value(value)
  { }

  /*
  LLVMValueDef(const std::string & name, Variable::Type type, llvm::AllocaInst * var)
    : ValueDefBase(name, type)
    , m_value(var)
  { }
  */

  llvm::Value * m_value = nullptr;
};

///////////////////////////////////////////////////////////////////////////

struct LLVMScope : public ScopeBase
{
  LLVMScope(CodeGenerator & codeGen, ScopeBase * parent = nullptr);

  //virtual bool OnDeclareVar(const AST::VariableDefExpr & expr) override;
};

///////////////////////////////////////////////////////////////////////////

class CodegenLLVM : public CodeGenerator
{
  public:
    CodegenLLVM(const std::string & genType, const std::string & inputFilename, AST::SourceFileExprList & tree); 

    // required funcs
    virtual bool Open(int argc, const char ** argv) override;
    virtual bool Close(const std::string & m_outputFilename) override;

    virtual ScopeBase * CreateScope(CodeGenerator & codeGen, ScopeBase * parent = nullptr) override;
    
#if 0     
    virtual bool ReferenceVar(AST::VariableDefExpr & expr) override;
    virtual bool ReferenceConstString(const std::string & name, const std::string & val) override;
    virtual void AssignString(const std::string & lhName, const std::string & rhName) override;
    virtual void AssignVar(const ValueDefBase & lhs, const ValueDefBase & rhs) override;
    virtual void JoinStrings(const std::string & lhName, const std::string & rhName) override;
    virtual void BinaryOp(const ValueDefBase & lhs, char op, const ValueDefBase & rhs) override;
    virtual bool CallFunction(const std::string & returnTypeStr, const std::string & name, const std::vector<std::string> & args) override;

    virtual bool OnDeclareExternalFunc(const FunctionDef & fn) override;
    virtual bool OnDeclareConstString(const AST::ConstantStringExpr & expr, const std::string & name) override; 
    virtual bool OnDeclareVar(const AST::VariableDefExpr & expr, ScopeBase & scope) override;
#endif

    // internal functions
    llvm::FunctionType * CreateFunctionType(const FunctionDef & fn);

#if 0    
    void CreateCallExternalFunc(RuntimeFunctionDef & funcDef);

    void CreateBIFCall(const std::string & name ...);
    
    void CreateFunctionCall(RuntimeFunctionDef & funcDef ...);
    void VCreateFunctionCall(RuntimeFunctionDef & funcDef, va_list varg);

    void CreateFunctionCall(const char * returnType, const char * name, const char * argsStr...);
    void VCreateFunctionCall(const char * returnType, const char * name, const char * argsStr, va_list varg);
 #endif   

    LLVMVarDef * FindVarRef(ScopeBase * scope, const std::string & name);

    Filename m_objectFilename;

    std::unique_ptr<llvm::Module> m_module;	
    llvm::IRBuilder<> m_builder;
    std::unique_ptr<llvm::DIBuilder> m_debugBuilder;
    llvm::LLVMContext m_context;    
    llvm::BasicBlock * m_mainBlock;

    std::map<std::string, llvm::Value *> m_constStringValues;
    std::map<std::string, llvm::Value *> m_globalVarValues;
};
  
#endif // CG_LLVM_H_
