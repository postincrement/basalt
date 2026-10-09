#ifndef C_CODEGEN_H_
#define C_CODEGEN_H_

#include <set>
#include <string>
#include <vector>

#include "../outputgen.h"

void BasaltWriteCRuntimeDecls(std::ostream & strm, const std::set<std::string> & funcs);
void BasaltWriteCRuntime(std::ostream & strm, const std::set<std::string> & funcs);

class C_OutputGenerator : public OutputGenerator
{
  public:
    explicit C_OutputGenerator(CodeGenerator & codeGenerator);

  protected:
    void OutputFilePrologue(std::ostream & strm) override;
    void OutputFileEpilogue(std::ostream & strm) override;

    int Generate(CodeGenerator::GotoTarget & node) override;
    int Generate(CodeGenerator::Goto & node) override;
    int Generate(CodeGenerator::Gosub & node) override;
    int Generate(CodeGenerator::Return & node) override;
    int Generate(CodeGenerator::End & node) override;
    int Generate(CodeGenerator::System & node) override;
    int Generate(CodeGenerator::If & node) override;
    int Generate(CodeGenerator::Else & node) override;
    int Generate(CodeGenerator::EndIf & node) override;
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

    std::string Operand(const std::string & name) const;
    std::string StringPtr(const std::string & name) const;
    std::string IndexText(const CodeGenerator::IndexOp & node) const;
    void Line(const std::string & text);

    std::vector<int> m_gosubs;
    int m_gosubId = 0;
    bool m_needStrings = false;
};

#endif
