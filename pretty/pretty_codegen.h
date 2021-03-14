#ifndef PRETTY_CODEGEN_H_
#define PRETTY_CODEGEN_H_

#include "../src/codegen.h"

class Pretty_CodeGenerator : public CodeGenerator
{
  public:
    Pretty_CodeGenerator(PseudoCodeGenerator & pseudo);

    virtual int Generate(PseudoCodeGenerator::Node & node) override;
    virtual int Generate(PseudoCodeGenerator::Block & node) override;

    virtual int Generate(PseudoCodeGenerator::CreateTempVar & node) override;
    virtual int Generate(PseudoCodeGenerator::DestroyTempVar & node) override;

    virtual int Generate(PseudoCodeGenerator::PrintNewLine & node) override;
    virtual int Generate(PseudoCodeGenerator::PrintTab & node) override;
    virtual int Generate(PseudoCodeGenerator::PrintStringConst & node) override;
    virtual int Generate(PseudoCodeGenerator::PrintStringVar & node) override;

    virtual int Generate(PseudoCodeGenerator::PrintNumericConst & node) override;
    virtual int Generate(PseudoCodeGenerator::PrintNumericVar & node) override;

    virtual int Generate(PseudoCodeGenerator::FunctionCHR & node) override;
    virtual int Generate(PseudoCodeGenerator::FunctionTAB & node) override;
    virtual int Generate(PseudoCodeGenerator::StringAssign & node) override;

    virtual int Generate(PseudoCodeGenerator::NumericAssign & node) override;

  protected:
    int m_indent = 0;    
};

#endif // PRETYY_CODEGEN_H_