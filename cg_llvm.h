  
#ifndef CG_LLVM_H_
#define CG_LLVM_H_

#include "ast.h"

class CodegenLLVM : public AST::Visitor
{
  public:
    CodegenLLVM(AST::SourceFileExprList & tree); 

    DECLARE_EXPR_VISIT_FUNCTIONS();      

    // open generator
    virtual bool Open(const std::string & inputFilename, int argc, const char ** argv) override;
    virtual void Close();
      
    // generate code
    virtual bool Generate(AST::AbstractDispatcher & dispatcher) override
    { return dispatcher.Generate(*this); }
    
    Filename m_srcFilename;
};
  
#endif // CG_LLVM_H_
