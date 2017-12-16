#ifndef CODEGEN_H_
#define CODEGEN_H_

#include <set>

#include "ast.h"

///////////////////////////////////////////////////////////////////////////

//
//  variable definitions
//

struct VarDefExprBase 
{
  VarDefExprBase(const AST::VariableDefExpr * expr)
    : m_expr(expr)
  { }

  virtual ~VarDefExprBase()
  { }

  const AST::VariableDefExpr * m_expr;
};

typedef std::map<std::string, std::unique_ptr<VarDefExprBase> > VarDefExprMap;

///////////////////////////////////////////////////////////////////////////

//
//  reference to a value 
//

struct ValueDefBase
{
  ValueDefBase(const std::string & name, Variable::Type type)
    : m_name(name)
    , m_type(type)
  { }
  std::string m_name;
  Variable::Type m_type;
};

///////////////////////////////////////////////////////////////////////////

struct ScopeBase 
{
  ScopeBase(ScopeBase * parent = nullptr)
    : m_parent(parent)
  { 
    m_scopeLevel = (parent == nullptr) ? 0 : (parent->GetScopeLevel()+1);
  }

  virtual ~ScopeBase()
  { }

  virtual void Enter()
  { }

  virtual void Leave() 
  { }
  
  virtual VarDefExprBase * FindVar(const std::string & name);

  virtual bool OnDeclareVar(const AST::VariableDefExpr & expr) { return false; }

  virtual unsigned GetScopeLevel() const 
  { return m_scopeLevel; }
  
  VarDefExprMap m_vars;

  ScopeBase * m_parent = nullptr;
  unsigned m_scopeLevel;
};

///////////////////////////////////////////////////////////////////////////

class CodeGeneratorBase : public AST::Visitor
{
  public:
    CodeGeneratorBase(const std::string & genType, const std::string & fn, AST::SourceFileExprList & tree);
    virtual bool Run(AST::SourceFileExprList & expr);
		    
    DECLARE_COMMON_EXPR_VISIT_FUNCTIONS();
    
    // required funcs in descendant generators
    public:
      // open/close generator
      virtual bool Open(int argc, const char ** argv) = 0;
      virtual bool Close(const std::string & m_outputFilename) = 0;

      virtual ScopeBase * CreateScope(CodeGeneratorBase & codeGen, ScopeBase * parent = nullptr) = 0;
      
      virtual bool ReferenceVar(AST::VariableDefExpr & expr) = 0;
      virtual bool ReferenceConstString(const std::string & name, const std::string & val) = 0;
      virtual void AssignString(const std::string & lhName, const std::string & rhName) = 0;
      virtual void AssignVar(const ValueDefBase & lhs, const ValueDefBase & rhs) = 0;
      virtual void JoinStrings(const std::string & lhName, const std::string & rhName) = 0;
      virtual void BinaryOp(const ValueDefBase & lhs, char op, const ValueDefBase & rhs) = 0;
      virtual bool CallFunction(const std::string & returnTypeStr, const std::string & name, const std::vector<std::string> & args) = 0;
    
      // optional functions
      virtual bool Open();

      virtual void EnterScope();  
      virtual void LeaveScope();  
      
      virtual bool OnDeclareConstString(const AST::ConstantStringExpr & expr, const std::string & name) { return true; } 
      virtual bool OnDeclareExternalFunc(const FunctionDef & fn) { return true; }
      virtual void OnLineMarker(const AST::LineMarkerExpr & expr) { }
      virtual bool OnDeclareVar(const AST::VariableDefExpr & expr, ScopeBase & scope) 
      { return scope.OnDeclareVar(expr); }

    // internal functions
    private:
      void DeclareVar(AST::VariableDefExpr & expr);
      void DeclareConstString(const std::string & str);
      bool DeclareExternalFunction(const FunctionDef & fn);
      bool DeclareRuntimeFunction(const std::string & name);
      bool CallRuntimeFunction(const std::string & name ...);
      bool VCreateFunctionCall(
        const char * returnTypeStr,
        const char * name,
        const char * argsStr_,
        va_list varg);
    
    // helper functions for descendant classes    
    protected:    
      std::string GetTempName(const std::string & prefix);
      
    protected:    
      Filename m_inputFilename;
      std::string m_genType;
      ScopeBase * m_globalScope = nullptr;
      ScopeBase * m_currentScope = nullptr;
      unsigned m_scopeLevel = 0;
      std::deque<ValueDefBase *> m_valueStack;    
      std::set<std::string> m_cleanupList;

      std::map<std::string, std::string> m_constStringMap;
      std::map<std::string, FunctionDef> m_externFunctionMap;
              
      unsigned m_tempCounter = 1;
      AST::LineMarkerExpr * m_currentLineMarkerExpr = nullptr;
};

#endif // CODEGEN_H_

