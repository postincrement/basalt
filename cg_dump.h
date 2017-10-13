  
#ifndef CG_DUMP_H_
#define CG_DUMP_H_

#include "ast.h"
  
class CodegenDumper : public CodeGenerator
{
  public:
    CodegenDumper(AST::SourceFileExprList & tree); 

    DECLARE_EXPR_VISIT_FUNCTIONS();      

    // open generator
    virtual bool Open(const std::string & inputFilename, int argc, const char ** argv) override;
    virtual bool Close();
    
    // generate code
    virtual bool Generate(AST::AbstractDispatcher & dispatcher) override
    { return dispatcher.Generate(*this); }
    
    std::ostream & m_strm;
    std::deque<std::string> m_vars;
};
  
#endif // CG_DUMP_H_
