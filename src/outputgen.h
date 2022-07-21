#ifndef OUTPUTGEN_H_
#define OUTPUTGEN_H_

#include "codegen.h"

//////////////////////////////////////////////////////////////////////////////////////

class OutputGenerator
{
  public:
    struct Config {
      const char * m_extension = "";
      const char * m_tempPrefix = "";
      const char * m_open = "";
      const char * m_close = "";
      int m_indent = 2;
      int m_indentInc = 2;
    };

    OutputGenerator(const Config & config, CodeGenerator & codeGenerator);

    const Config & GetConfig() const;

    virtual void OutputFilePrologue(std::ostream & strm);
    virtual void OutputFileEpilogue(std::ostream & strm);

    bool Run(const std::string & inputFilename, std::ostream * outputStream);

    virtual int Generate(CodeGenerator::Node & node) = 0;

    virtual int Generate(CodeGenerator::LineNumber & node) = 0;
    virtual int Generate(CodeGenerator::BlockStart & node) = 0;
    virtual int Generate(CodeGenerator::BlockEnd & node) = 0;

    virtual int Generate(CodeGenerator::CreateTempVar & node) = 0;

    virtual int Generate(CodeGenerator::PrintNewLine & node) = 0;
    virtual int Generate(CodeGenerator::PrintTab & node) = 0;

    virtual int Generate(CodeGenerator::PrintStringConst & node) = 0;
    virtual int Generate(CodeGenerator::PrintInt16Const & node) = 0;
    virtual int Generate(CodeGenerator::PrintInt32Const & node) = 0;
    virtual int Generate(CodeGenerator::PrintSingleConst & node) = 0;
    virtual int Generate(CodeGenerator::PrintDoubleConst & node) = 0;

    virtual int Generate(CodeGenerator::PrintStringVar & node) = 0;
    virtual int Generate(CodeGenerator::PrintInt16Var & node) = 0;
    virtual int Generate(CodeGenerator::PrintInt32Var & node) = 0;
    virtual int Generate(CodeGenerator::PrintSingleVar & node) = 0;
    virtual int Generate(CodeGenerator::PrintDoubleVar & node) = 0;

    virtual int Generate(CodeGenerator::UnaryOperator & node) = 0;
    virtual int Generate(CodeGenerator::BinaryOperator & node) = 0;;

    virtual bool IsPrintUsed() const;

  protected:
    Config m_config;
    CodeGenerator & m_codeGenerator;
    std::string m_inputFilename;
    std::stringstream * m_outputStream = nullptr;    
    int m_indent = 0;   
};


#endif // OUTPUTGEN_H_

