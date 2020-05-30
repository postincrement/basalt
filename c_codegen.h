#ifndef C_CODEGEN_H_
#define C_CODEGEN_H_

#include "codegen.h"

class C_CodeGenerator : public CodeGenerator
{
  public:
    C_CodeGenerator(const std::string & inputFilename, 
                    const std::string & outputFilename, 
                    AST::NodeList & program);

    virtual bool Body();

    virtual int Generate(const AST::Node & node);
    virtual int Generate(const AST::NodeList & expr);
    virtual int Generate(const AST::SourceLine & expr);
    virtual int Generate(const AST::LineNumber & expr);
    virtual int Generate(const AST::Print & expr);
    virtual int Generate(const AST::StringConstant & expr);
    virtual int Generate(const AST::Int16Constant & expr);
    virtual int Generate(const AST::Int32Constant & expr);
    virtual int Generate(const AST::SingleConstant & expr);
    virtual int Generate(const AST::DoubleConstant & expr);
    virtual int Generate(const AST::Assign & expr);

    virtual int Print(const AST::Node & node);
    virtual int Print(const AST::StringConstant & expr);
    virtual int Print(const AST::Int16Constant & expr);
    virtual int Print(const AST::Int32Constant & expr);
    virtual int Print(const AST::SingleConstant & expr);
    virtual int Print(const AST::DoubleConstant & expr);
    virtual int Print(const AST::PrintComma & expr);

    std::stringstream m_body;

    struct CVarDef {
      VarType m_type;
      std::string m_cname;
      std::string m_ctype;
      std::string m_initializer;
    };

    bool DeclareGlobalVar(
      const AST::VarRef & var, 
      C_CodeGenerator::CVarDef & cvar
    );

    class Closure {
      public:
        std::stringstream & Output()
        { return m_output; }

      protected:  
        std::stringstream m_output;
    };

    Closure & Top()
    {
      return m_stack[m_stack.size()-1];      
    }

    std::stringstream & TopOutput()
    {
      return Top().Output();
    }

    void PushClosure()
    {
      m_stack.push_back(Closure());
    }

    void PopClosure()
    {
      m_stack.pop_back();
    }

    std::deque<Closure> m_stack;

  protected:
    typedef std::map<std::string, CVarDef> GlobalVarMap;
    std::set<std::string> m_cnames;
    GlobalVarMap m_globalVars;  
};

#endif // C_CODEGEN_H_