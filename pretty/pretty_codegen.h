#ifndef PRETTY_CODEGEN_H_
#define PRETTY_CODEGEN_H_

#include "../src/outputgen.h"

class Pretty_OutputGenerator : public OutputGenerator
{
  public:
    Pretty_OutputGenerator(CodeGenerator & pseudo);

    virtual int Generate(CodeGenerator::Node & node) override;

    virtual int Generate(CodeGenerator::LineNumber & node) override;
    virtual int Generate(CodeGenerator::BlockStart & node) override;
    virtual int Generate(CodeGenerator::BlockEnd & node) override;

    virtual int Generate(CodeGenerator::CreateTempVar & node) override;

    virtual int Generate(CodeGenerator::PrintNewLine & node) override;
    virtual int Generate(CodeGenerator::PrintTab & node) override;
    virtual int Generate(CodeGenerator::PrintStringConst & node) override;
    virtual int Generate(CodeGenerator::PrintStringVar & node) override;

    virtual int Generate(CodeGenerator::PrintNumericConst & node) override;
    virtual int Generate(CodeGenerator::PrintNumericVar & node) override;

    virtual int Generate(CodeGenerator::UnaryOperator & node) override;
    virtual int Generate(CodeGenerator::BinaryOperator & node) override;

  protected:
    int m_indent = 0;   
    std::queue<std::stringstream *> m_blockStack; 
};

#endif // PRETYY_CODEGEN_H_