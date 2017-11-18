#ifndef CODEGEN_H_
#define CODEGEN_H_

#include <set>

#include "ast.h"

class CodeGenerator : public AST::Visitor
{
  public:
		// common code
    typedef std::map<std::string, AST::VariableDefExpr *> VarDefExprMap;

    struct Scope 
    {
      Scope(Scope * parent = nullptr)
        : m_parent(parent)
      { 
        m_scopeLevel = (parent == nullptr) ? 0 : (parent->GetScopeLevel()+1);
      }

      virtual ~Scope() { }

      virtual void Enter() {};
      virtual void Leave() {};
      
      virtual AST::Expr * FindVar(const std::string & name);

      virtual bool OnDeclareVar(const AST::VariableDefExpr & expr) { return true; } 

      virtual unsigned GetScopeLevel() const 
      { return m_scopeLevel; }
      
      VarDefExprMap m_vars;

      Scope * m_parent = nullptr;
      unsigned m_scopeLevel;
    };

    struct ValueDef {
      ValueDef(const std::string & name, Variable::Type type)
        : m_name(name)
        , m_type(type)
      { }
      std::string m_name;
      Variable::Type m_type;
    };

		CodeGenerator(const std::string & genType, const std::string & fn, AST::SourceFileExprList & tree);
		
    DECLARE_COMMON_EXPR_VISIT_FUNCTIONS();
    
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

    std::string GetTempName(const std::string & prefix);
        
    // funcs to define in descendant generators
    virtual ValueDef * CreateValueDef(const std::string & name, Variable::Type type)
    { return new ValueDef(name, type); }

    virtual void AssignString(const std::string & lhName, const std::string & rhName) = 0;
    virtual void AssignVar(const std::string & lhs, const std::string & rhs) = 0;
    virtual void JoinStrings(const std::string & lhName, const std::string & rhName) = 0;
    virtual void BinaryOp(const ValueDef & lhs, char op, const ValueDef & rhs) = 0;
    virtual bool CallFunction(const std::string & returnTypeStr, const std::string & name, const std::vector<std::string> & args) = 0;
    
    // open/close generator
    virtual bool Open();
    virtual bool Open(int argc, const char ** argv) = 0;
    virtual bool Close(const std::string & m_outputFilename) = 0;
    virtual bool Run(AST::SourceFileExprList & expr);

    virtual Scope * CreateScope(CodeGenerator & codeGen, Scope * parent = nullptr) = 0;
    virtual void EnterScope();  
    virtual void LeaveScope();  
    
    virtual bool OnDeclareConstString(const std::string & str, const std::string & name) { return true; } 
    virtual bool OnDeclareExternalFunc(const FunctionDef & fn) { return true; }
    virtual void OnLineMarker(const AST::LineMarkerExpr & expr) { }
    virtual bool OnDeclareVar(const AST::VariableDefExpr & expr, Scope & scope) { return scope.OnDeclareVar(expr); }
    
		Filename m_inputFilename;
    std::string m_genType;
    Scope * m_globalScope = nullptr;
    Scope * m_currentScope = nullptr;
    unsigned m_scopeLevel = 0;
    std::deque<ValueDef *> m_valueStack;    
    std::set<std::string> m_cleanupList;

    std::map<std::string, std::string> m_constStringMap;
    std::map<std::string, FunctionDef> m_externFunctionMap;
            
    unsigned m_tempCounter = 1;
    AST::LineMarkerExpr * m_currentLineMarkerExpr = nullptr;
};

#endif // CODEGEN_H_

