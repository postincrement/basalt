#ifndef CODEGEN_H_
#define CODEGEN_H_

#include "pseudo.h"

//////////////////////////////////////////////////////////////////////////////////////

class CodeGenerator
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

    CodeGenerator(const Config & config, PseudoCodeGenerator & pseudoGenerator);

    const Config & GetConfig() const;

    bool Run(const std::string & inputFilename, std::ostream * outputStream);

    virtual int Generate(PseudoCodeGenerator::Node & node) = 0;

    virtual int Generate(PseudoCodeGenerator::LineNumber & node) = 0;
    virtual int Generate(PseudoCodeGenerator::BlockStart & node) = 0;
    virtual int Generate(PseudoCodeGenerator::BlockEnd & node) = 0;

    virtual int Generate(PseudoCodeGenerator::CreateTempVar & node) = 0;

    virtual int Generate(PseudoCodeGenerator::PrintNewLine & node) = 0;
    virtual int Generate(PseudoCodeGenerator::PrintTab & node) = 0;
    virtual int Generate(PseudoCodeGenerator::PrintStringConst & node) = 0;
    virtual int Generate(PseudoCodeGenerator::PrintStringVar & node) = 0;
    virtual int Generate(PseudoCodeGenerator::PrintNumericConst & node) = 0;
    virtual int Generate(PseudoCodeGenerator::PrintNumericVar & node) = 0;

    virtual int Generate(PseudoCodeGenerator::FunctionCHR & node) = 0;
    virtual int Generate(PseudoCodeGenerator::FunctionTAB & node) = 0;;

    virtual int Generate(PseudoCodeGenerator::StringAssign & node) = 0;

    virtual int Generate(PseudoCodeGenerator::NumericAssign & node) = 0;


  protected:
    Config m_config;
    PseudoCodeGenerator & m_pseudoGenerator;
    std::string m_inputFilename;
    std::stringstream * m_outputStream = nullptr;    
};


#endif // CODEGEN_H_

