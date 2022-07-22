#ifndef C_CODEGEN_H_
#define C_CODEGEN_H_

#include <deque>

#include "outputgen.h"

class C_OutputGenerator : public OutputGenerator
{
  public:
    C_OutputGenerator(CodeGenerator & pseudo);

    virtual void OutputFilePrologue(std::ostream & strm) override;
    virtual void OutputFileEpilogue(std::ostream & strm) override;

    virtual int Generate(CodeGenerator::Node & node) override;

    virtual int Generate(CodeGenerator::BlockStart & node) override;
    virtual int Generate(CodeGenerator::BlockEnd & node) override;
    virtual int Generate(CodeGenerator::Goto & node) override;
    virtual int Generate(CodeGenerator::GotoTarget & node) override;
    virtual int Generate(CodeGenerator::Gosub & node) override;
    virtual int Generate(CodeGenerator::Return & node) override;
    virtual int Generate(CodeGenerator::End & node) override;
    virtual int Generate(CodeGenerator::System & node) override;

    virtual int Generate(CodeGenerator::CreateTempVar & node) override;

    virtual int Generate(CodeGenerator::PrintNewLine & node) override;
    virtual int Generate(CodeGenerator::PrintTab & node) override;

    virtual int Generate(CodeGenerator::PrintStringConst & node) override;
    virtual int Generate(CodeGenerator::PrintInt16Const & node) override;
    virtual int Generate(CodeGenerator::PrintInt32Const & node) override;
    virtual int Generate(CodeGenerator::PrintSingleConst & node) override;
    virtual int Generate(CodeGenerator::PrintDoubleConst & node) override;

    virtual int Generate(CodeGenerator::PrintStringVar & node) override;
    virtual int Generate(CodeGenerator::PrintInt16Var & node) override;
    virtual int Generate(CodeGenerator::PrintInt32Var & node) override;
    virtual int Generate(CodeGenerator::PrintSingleVar & node) override;
    virtual int Generate(CodeGenerator::PrintDoubleVar & node) override;

    virtual int Generate(CodeGenerator::UnaryOperator & node) override;
    virtual int Generate(CodeGenerator::BinaryOperator & node) override;

    virtual int Generate(CodeGenerator::If & node) override;
    virtual int Generate(CodeGenerator::Else & node) override;

  protected:
};

#endif // C_CODEGEN_H_