  
#ifndef CG_CXX_H_
#define CG_CXX_H_

#include <fstream>

#include "ast.h"
 
class CodegenCXX : public CodeGenerator
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

    void CreateBIFCall(const std::string & name ...);

    void VCreateFunctionCall(                                      
      const char * returnTypeStr,
      const char * name,
      const char * argsStr_,
      va_list varg);
          
    Filename m_srcFilename;
    std::ostream * m_ostrm;
    std::ofstream m_outputFile;

};
  
#endif // CG_CXX_H_
