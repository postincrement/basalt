#ifndef CODEGEN_H_
#define CODEGEN_H_

#include <set>

#include "ast.h"

class CodeGenerator : public AST::Visitor
{
  public:
		// common code
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
    void DeclareExternalFunction(const FunctionDef & fn);
    
		// funcs to define in descendant generators
    virtual void AssignString(const std::string & lhName, const std::string & rhName) = 0;
    virtual void AssignVar(const std::string & lhs, const std::string & rhs) = 0;
    virtual void JoinStrings(const std::string & lhName, const std::string & rhName) = 0;
    virtual void BinaryOp(const ValueDef & result, const ValueDef & lhs, char op, const ValueDef & rhs) = 0;
		
    // open generator
    virtual bool Open();
    virtual bool Open(int argc, const char ** argv) = 0;

    // close generator
    virtual bool Close(const std::string & m_outputFilename) = 0;

    std::string GetTempName(const std::string & prefix);

    typedef std::map<std::string, AST::VariableDefExpr *> VarDefExprMap;

    struct Scope 
    {
      Scope(Scope * parent = nullptr)
        : m_parent(parent)
      { }

      virtual AST::Expr * FindVar(const std::string & name);

      virtual void OnDeclareVar(const AST::Expr & expr) { } 
      
      VarDefExprMap m_vars;
      std::deque<ValueDef> m_valueStack;    
      std::set<std::string> m_cleanupList;

      Scope * m_parent = nullptr;
    };

    virtual Scope * CreateScope(Scope * parent = nullptr) = 0;
    virtual void OnDeclareConstString(const std::string & str, const std::string & name) { } 
    virtual void OnDeclareExternalFunc(const FunctionDef & fn) { }
    
		Filename m_inputFilename;
    std::string m_genType;
    Scope * m_globalScope = nullptr;
    Scope * m_currentScope = nullptr;

    std::map<std::string, std::string> m_constStringMap;
    std::map<std::string, FunctionDef> m_externFunctionMap;
            
    unsigned m_tempCounter = 1;
    AST::LineMarkerExpr * m_currentLineMarkerExpr = nullptr;
};

#endif // CODEGEN_H_

