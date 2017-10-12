  
#ifndef CG_DUMP_H_
#define CG_DUMP_H_

#include "ast.h"
  
class CodegenDumper : public AST::Visitor
{
  public:
    CodegenDumper(AST::ExprList & tree, std::ostream & strm); 

    DECLARE_EXPR_VISIT_FUNCTIONS();      

    // open generator
    virtual bool Open(const std::string & inputFilename, int argc, const char ** argv) override;

    // generate code
    virtual bool Generate(AST::AbstractDispatcher & dispatcher) override
    { return dispatcher.Generate(*this); }

    AST::ExprList & m_tree;
    std::ostream & m_strm;
    std::deque<std::string> m_vars;
};
  
#endif // CG_DUMP_H_
