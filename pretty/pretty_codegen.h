#ifndef PRETTY_CODEGEN_H_
#define PRETTY_CODEGEN_H_

#include "../src/codegen.h"

class Pretty_CodeGenerator : public CodeGenerator
{
  public:
    Pretty_CodeGenerator(PseudoCodeGenerator & pseudo);

    virtual int Generate(PseudoCodeGenerator::Node & node) override;

    virtual int Generate(PseudoCodeGenerator::LineNumber & node) override;
    virtual int Generate(PseudoCodeGenerator::BlockStart & node) override;
    virtual int Generate(PseudoCodeGenerator::BlockEnd & node) override;

    virtual int Generate(PseudoCodeGenerator::CreateTempVar & node) override;

    virtual int Generate(PseudoCodeGenerator::PrintNewLine & node) override;
    virtual int Generate(PseudoCodeGenerator::PrintTab & node) override;
    virtual int Generate(PseudoCodeGenerator::PrintStringConst & node) override;
    virtual int Generate(PseudoCodeGenerator::PrintStringVar & node) override;

    virtual int Generate(PseudoCodeGenerator::PrintNumericConst & node) override;
    virtual int Generate(PseudoCodeGenerator::PrintNumericVar & node) override;

    virtual int Generate(PseudoCodeGenerator::UnaryOperator & node) override;
    virtual int Generate(PseudoCodeGenerator::BinaryOperator & node) override;

  protected:
    int m_indent = 0;   
    std::queue<std::stringstream *> m_blockStack; 
};

#endif // PRETYY_CODEGEN_H_