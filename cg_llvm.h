  
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

class CodegenLLVM : public CodeGenerator
{
  public:
    CodegenLLVM(const std::string & genType, const std::string & inputFilename, AST::SourceFileExprList & tree); 

    DECLARE_EXPR_VISIT_FUNCTIONS();

    // open generator
    virtual bool Open(int argc, const char ** argv) override;
    virtual bool Close(const std::string & m_outputFilename);
      
    // generate code
    virtual bool Generate(AST::AbstractDispatcher & dispatcher) override
    { return dispatcher.Generate(*this); }

    struct LLVMScope : public Scope
    {
      LLVMScope(Scope * parent = nullptr)
        : Scope(parent)
      { }
    };

    struct LLVMValueDef : public ValueDef
    {
      LLVMValueDef(const std::string & name, Variable::Type type, llvm::AllocaInst * alloc)
        : ValueDef(name, type)
        , m_alloc(alloc)
      { }

      llvm::AllocaInst * m_alloc;
    };
        
    virtual Scope * CreateScope(CodeGenerator & codeGen, Scope * parent = nullptr)
    { return new LLVMScope(parent); }
    
    virtual ValueDef * CreateValueDef(const std::string & name, Variable::Type type) override;

    virtual ValueDef * CreateValueDef(const std::string & name, Variable::Type type, llvm::AllocaInst * alloc)
    { return new LLVMValueDef(name, type, alloc); }
    
    virtual bool ReferenceVar(AST::VariableDefExpr & expr) override;
    virtual void AssignString(const std::string & lhName, const std::string & rhName) override;
    virtual void AssignVar(const ValueDef & lhs, const ValueDef & rhs) override;
    virtual void JoinStrings(const std::string & lhName, const std::string & rhName) override;
    virtual void BinaryOp(const ValueDef & lhs, char op, const ValueDef & rhs) override;
    virtual bool CallFunction(const std::string & returnTypeStr, const std::string & name, const std::vector<std::string> & args) override;

    virtual bool OnDeclareExternalFunc(const FunctionDef & fn) override;
    virtual bool OnDeclareConstString(const std::string & str, const std::string & name) override; 
    virtual bool OnDeclareVar(const AST::VariableDefExpr & expr, Scope & scope) override;
    
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
