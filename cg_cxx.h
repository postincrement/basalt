  
#ifndef CG_CXX_H_
#define CG_CXX_H_

#include <fstream>

#include "ast.h"
 
class CodegenCXX : public AST::Visitor
{
  public:
    CodegenCXX(AST::SourceFileExprList & tree); 

    DECLARE_EXPR_VISIT_FUNCTIONS();      

    // open generator
    virtual bool Open(const std::string & inputFilename, int argc, const char ** argv) override;
    virtual bool Close();
    
    // generate code
    virtual bool Generate(AST::AbstractDispatcher & dispatcher) override
    { return dispatcher.Generate(*this); }

    Filename m_srcFilename;
    std::ofstream m_ostrm;
};
  
#endif // CG_CXX_H_
