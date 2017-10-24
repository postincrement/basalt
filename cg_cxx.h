  
#ifndef CG_CXX_H_
#define CG_CXX_H_

#include <fstream>
#include <deque>

#include "ast.h"
 
class CodegenCXX : public CodeGenerator
{
  public:
    CodegenCXX(const std::string & genType, AST::SourceFileExprList & tree); 

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

    std::string StartOutput(); 
    void AssignString(const std::string & indent, const std::string & lhs, const std::string & rhs);
    
    Filename m_srcFilename;
    std::ostream * m_ostrm;
    std::ofstream m_outputFile;
    bool m_firstLine = true;
    AST::LineMarkerExpr * m_currentLineMarkerExpr = nullptr;

    struct ValueDef {
      ValueDef(const std::string & name, Variable::Type type)
        : m_name(name)
        , m_type(type)
      { }
      std::string m_name;
      Variable::Type m_type;
    };

    std::deque<ValueDef> m_valueStack;
};
  
#endif // CG_CXX_H_
