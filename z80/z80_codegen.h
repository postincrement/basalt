#ifndef Z80_CODEGEN_H_
#define Z80_CODEGEN_H_

#include <cstdint>
#include <map>
#include <string>
#include <vector>

#include "../outputgen.h"

class Z80_OutputGenerator : public OutputGenerator
{
  public:
    explicit Z80_OutputGenerator(CodeGenerator & codeGenerator);

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

    void Prepare();
    void A(const std::string & text);
    void Lbl(const std::string & name);
    std::string Lab();
    VarType TypeOf(const std::string & name) const;
    int WidthOf(VarType type) const;
    std::string FloatConst(float value);
    void LoadFacc(const std::string & name);
    void StoreFacc(const std::string & name);
    void LoadHL(const std::string & name);
    void StoreHL(const std::string & name);
    void LoadString(const std::string & name);
    void ZeroTest(const std::string & name, VarType type, const std::string & ifZero);
    void StoreRelation(const std::string & op, const std::string & dest);
    void IncludeFile(std::ostream & strm, const std::string & filename);

    bool m_ready = false;
    bool m_useFloat = false;
    std::map<std::string, VarType> m_types;
    std::map<uint32_t, std::string> m_floats;
    int m_lab = 0;
    struct IfFrame {
      std::string m_elseLabel;
      std::string m_endLabel;
      bool m_sawElse = false;
    };
    std::vector<IfFrame> m_ifStack;
};

#endif
