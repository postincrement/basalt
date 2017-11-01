  
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
    
    virtual Scope * CreateScope(Scope * parent = nullptr)
    { return new LLVMScope(parent); }
    
    virtual void AssignString(const std::string & lhName, const std::string & rhName) override;
    virtual void AssignVar(const std::string & lhs, const std::string & rhs) override;
    virtual void JoinStrings(const std::string & lhName, const std::string & rhName) override;
    virtual void BinaryOp(const ValueDef & result, const ValueDef & lhs, char op, const ValueDef & rhs) override;
    virtual bool CallFunction(const std::string & returnTypeStr, const std::string & name, const std::vector<std::string> & args) override;
    
    // internal functions
    llvm::FunctionType * CreateFunctionType(const char * typeStr);
    void CreateCallExternalFunc(RuntimeFunctionDef & funcDef);

    void CreateBIFCall(const std::string & name ...);
    
    void CreateFunctionCall(RuntimeFunctionDef & funcDef ...);
    void VCreateFunctionCall(RuntimeFunctionDef & funcDef, va_list varg);

    void CreateFunctionCall(const char * returnType, const char * name, const char * argsStr...);
    void VCreateFunctionCall(const char * returnType, const char * name, const char * argsStr, va_list varg);

    Filename m_objectFilename;

		std::unique_ptr<llvm::Module> m_module;	
    llvm::IRBuilder<> m_builder;
    llvm::LLVMContext m_context;    
    llvm::BasicBlock * m_mainBlock;

    std::map<std::string, llvm::Value *> m_constStringValues;
    std::map<std::string, llvm::Value *> m_globalVarValues;
};
  
#endif // CG_LLVM_H_
