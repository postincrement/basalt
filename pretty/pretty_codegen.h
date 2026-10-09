#ifndef PRETTY_CODEGEN_H_
#define PRETTY_CODEGEN_H_

#include "../outputgen.h"

class Pretty_OutputGenerator : public OutputGenerator
{
  public:
    explicit Pretty_OutputGenerator(CodeGenerator & codeGenerator);

  protected:
    void OutputFilePrologue(std::ostream & strm) override;
    void OutputFileEpilogue(std::ostream & strm) override;
    int Generate(CodeGenerator::Node & node) override;
    int Generate(CodeGenerator::GotoTarget & node) override;
    int Generate(CodeGenerator::Goto & node) override;
    int Generate(CodeGenerator::Gosub & node) override;
    int Generate(CodeGenerator::Return & node) override;
    int Generate(CodeGenerator::End & node) override;
    int Generate(CodeGenerator::System & node) override;
    int Generate(CodeGenerator::If & node) override;
    int Generate(CodeGenerator::Else & node) override;
    int Generate(CodeGenerator::EndIf & node) override;
    int Generate(CodeGenerator::CreateTempVar & node) override;
    int Generate(CodeGenerator::PrintNewLine & node) override;
    int Generate(CodeGenerator::PrintTab & node) override;
    int Generate(CodeGenerator::PrintStringConst & node) override;
    int Generate(CodeGenerator::PrintStringVar & node) override;
    int Generate(CodeGenerator::PrintNumber & node) override;
    int Generate(CodeGenerator::UnaryOperator & node) override;
    int Generate(CodeGenerator::BinaryOperator & node) override;
    int Generate(CodeGenerator::IndexOp & node) override;
    int Generate(CodeGenerator::Input & node) override;
    int Generate(CodeGenerator::OnGoto & node) override;
    int Generate(CodeGenerator::Clear & node) override;
    int Generate(CodeGenerator::Width & node) override;

    void Line(const std::string & text);
};

#endif
