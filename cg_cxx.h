  
#ifndef CG_CXX_H_
#define CG_CXX_H_

#include <fstream>
#include <deque>
#include <set>

#include "ast.h"

class CodegenCXX;

///////////////////////////////////////////////////////////////////////////

struct CXXScope : public ScopeBase
{
  CXXScope(CodeGenerator & codeGen, ScopeBase * parent = nullptr);

  virtual void Enter() override;
  virtual void Leave() override;

  std::string GetIndent() const
  { return m_indent; }

  //virtual bool OnDeclareVar(const AST::VariableDefExpr & expr) override;

  CodegenCXX * m_owner;
  std::string m_indent;
};

///////////////////////////////////////////////////////////////////////////
 
class CodegenCXX : public CodeGenerator
{
  public:
    CodegenCXX(const std::string & genType, const std::string & inputFilename, AST::SourceFileExprList & tree); 

    // required funcs
    virtual bool Open(int argc, const char ** argv) override;
    virtual bool Close(const std::string & m_outputFilename) override;

    virtual ScopeBase * CreateScope(CodeGenerator & codeGen, ScopeBase * parent = nullptr) override;
    virtual void OnLineMarker(const AST::LineMarkerExpr & expr) override;

    virtual bool TopLevel(AST::SourceFileExprList & expr) override;
\
#if 0
    virtual bool ReferenceVar(AST::VariableDefExpr & expr) override;
    virtual bool ReferenceConstString(const std::string & name, const std::string & val) override;
    virtual void AssignString(const std::string & lhName, const std::string & rhName) override;
    virtual void AssignVar(const ValueDefBase & lhs, const ValueDefBase & rhs) override;
    virtual void JoinStrings(const std::string & lhName, const std::string & rhName) override;
    virtual void BinaryOp(const ValueDefBase & lhs, char op, const ValueDefBase & rhs) override;
    virtual bool CallFunction(const std::string & returnTypeStr, const std::string & name, const std::vector<std::string> & args) override;
#endif

    #if 0
    void CreateBIFCall(const std::string & name ...);

    void VCreateFunctionCall(                                      
      const char * returnTypeStr,
      const char * name,
      const char * argsStr_,
      va_list varg);

    #endif

    // new functions
    void Output(std::ostream & strm);

    std::string DeclareFunction(const FunctionDef & func);
    std::string DeclareVariable(VarDefExprBase & varDef);
    
    std::stringstream m_prefixStream;
    std::stringstream m_constStringStrm;
    std::stringstream m_externFuncStrm;
    std::stringstream m_globalVarsStrm;
    std::stringstream m_codeStrm;

    protected:  
      Filename m_srcFilename;
      std::ofstream m_outputFile;
      bool m_firstLine = true;
      
      bool m_needCleanup = false;
};
  
#endif // CG_CXX_H_
