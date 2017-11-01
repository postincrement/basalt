  
#ifndef CG_CXX_H_
#define CG_CXX_H_

#include <fstream>
#include <deque>
#include <set>

#include "ast.h"
 
class CodegenCXX : public CodeGenerator
{
  public:
    CodegenCXX(const std::string & genType, const std::string & inputFilename, AST::SourceFileExprList & tree); 

    DECLARE_EXPR_VISIT_FUNCTIONS();
    
    // open generator
    virtual bool Open(int argc, const char ** argv) override;
    virtual bool Close(const std::string & m_outputFilename) override;

    void Output(std::ostream & strm);
    
    // generate code
    virtual bool Generate(AST::AbstractDispatcher & dispatcher) override
    { return dispatcher.Generate(*this); }

    struct CXXScope : public Scope
    {
      CXXScope(Scope * parent = nullptr)
        : Scope(parent)
      { }
    };

    virtual Scope * CreateScope(Scope * parent = nullptr) override
    { return new CXXScope(parent); }

    virtual void AssignString(const std::string & lhName, const std::string & rhName) override;
    virtual void AssignVar(const std::string & lhs, const std::string & rhs) override;
    virtual void JoinStrings(const std::string & lhName, const std::string & rhName) override;
    virtual void BinaryOp(const ValueDef & result, const ValueDef & lhs, char op, const ValueDef & rhs) override;
    virtual bool CallFunction(const std::string & returnTypeStr, const std::string & name, const std::vector<std::string> & args) override;
    
    void CreateBIFCall(const std::string & name ...);

    void VCreateFunctionCall(                                      
      const char * returnTypeStr,
      const char * name,
      const char * argsStr_,
      va_list varg);

    Filename m_srcFilename;
    std::ofstream m_outputFile;
    bool m_firstLine = true;
    
    std::stringstream m_prefixStream;
    std::stringstream m_constStringStrm;
    std::stringstream m_externFuncStrm;
    std::stringstream m_globalVarsStrm;
    std::stringstream m_codeStrm;
  };
  
#endif // CG_CXX_H_
