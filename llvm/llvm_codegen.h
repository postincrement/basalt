#ifndef LLVM_CODEGEN_H_
#define LLVM_CODEGEN_H_

#include <map>
#include <set>
#include <string>
#include <vector>

#include "../outputgen.h"

class LLVM_OutputGenerator : public OutputGenerator
{
  public:
    explicit LLVM_OutputGenerator(CodeGenerator & codeGenerator);

  protected:
    void BeginNode(const CodeGenerator::Node & node) override;
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
    void Emit(const std::string & text);
    void NoteStatement();
    int DebugLoc(unsigned line);
    void WriteInstr(std::ostream & strm, const std::string & text, unsigned line);
    std::string MetaQuoted(const std::string & text) const;
    void WriteDebugMetadata(std::ostream & strm);
    void EnsureOpen();
    void Close(const std::string & next);
    std::string Tmp();
    std::string Lab(const std::string & prefix);
    const char * Ty(VarType type) const;
    std::string Ptr(const std::string & name) const;
    std::string Literal(VarType type, const std::string & text) const;
    std::string FLit(float value) const;
    std::string DLit(double value) const;
    std::string LoadAs(VarType type, const std::string & name);
    std::string Cast(const std::string & value, VarType from, VarType to);
    void Store(VarType type, const std::string & value, const std::string & name);
    std::string StringPtr(const std::string & name, bool nullable);
    std::string LlvmString(const std::string & text) const;
    VarType TypeOf(const std::string & name) const;

    bool m_ready = false;
    bool m_terminated = false;
    int m_ssa = 0;
    int m_lab = 0;
    std::map<std::string, VarType> m_types;
    std::vector<int> m_gosubs;
    int m_gosubId = 0;
    struct IfFrame {
      std::string m_elseLabel;
      std::string m_endLabel;
      bool m_sawElse = false;
    };
    std::vector<IfFrame> m_ifStack;

    unsigned m_debugLine = 0;
    std::string m_debugText;
    int m_nextDebug = 10;
    std::map<unsigned, int> m_debugLocs;
    std::set<unsigned> m_labeledLines;
    struct DebugLabel {
      unsigned m_line;
      std::string m_text;
      int m_id;
    };
    std::vector<DebugLabel> m_debugLabels;
};

#endif
