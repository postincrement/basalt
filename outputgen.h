#ifndef OUTPUTGEN_H_
#define OUTPUTGEN_H_

#include <ostream>
#include <sstream>
#include <string>
#include <vector>

#include "codegen.h"

class OutputGenerator
{
  public:
    struct Config {
      const char * m_extension = "";
      int m_indent = 0;
      int m_indentInc = 2;
    };

    OutputGenerator(const Config & config, CodeGenerator & codeGenerator)
      : m_config(config)
      , m_codeGenerator(codeGenerator)
    { }

    virtual ~OutputGenerator() = default;

    const Config & GetConfig() const { return m_config; }

    virtual bool EmitsBinary() const { return false; }
    void SetOutputPath(const std::string & path) { m_outputPath = path; }

    virtual bool Run(const std::string & inputFilename, std::ostream * outputStream);
    virtual void BeginNode(const CodeGenerator::Node &) {}

    virtual int Generate(CodeGenerator::Node & node);
    virtual int Generate(CodeGenerator::BlockStart & node);
    virtual int Generate(CodeGenerator::BlockEnd & node);
    virtual int Generate(CodeGenerator::GotoTarget & node);
    virtual int Generate(CodeGenerator::Goto & node);
    virtual int Generate(CodeGenerator::Gosub & node);
    virtual int Generate(CodeGenerator::Return & node);
    virtual int Generate(CodeGenerator::End & node);
    virtual int Generate(CodeGenerator::System & node);
    virtual int Generate(CodeGenerator::If & node);
    virtual int Generate(CodeGenerator::Else & node);
    virtual int Generate(CodeGenerator::EndIf & node);
    virtual int Generate(CodeGenerator::CreateTempVar & node);
    virtual int Generate(CodeGenerator::PrintNewLine & node);
    virtual int Generate(CodeGenerator::PrintTab & node);
    virtual int Generate(CodeGenerator::PrintStringConst & node);
    virtual int Generate(CodeGenerator::PrintStringVar & node);
    virtual int Generate(CodeGenerator::PrintNumber & node);
    virtual int Generate(CodeGenerator::UnaryOperator & node);
    virtual int Generate(CodeGenerator::BinaryOperator & node);
    virtual int Generate(CodeGenerator::IndexOp & node);
    virtual int Generate(CodeGenerator::Input & node);
    virtual int Generate(CodeGenerator::OnGoto & node);
    virtual int Generate(CodeGenerator::Clear & node);
    virtual int Generate(CodeGenerator::Width & node);

  protected:
    virtual void OutputFilePrologue(std::ostream & strm);
    virtual void OutputFileEpilogue(std::ostream & strm);
    int Unimplemented(const char * kind);

    Config m_config;
    CodeGenerator & m_codeGenerator;
    std::string m_inputFilename;
    std::string m_outputPath;
    std::ostringstream * m_outputStream = nullptr;
    int m_indent = 0;
};

#endif
