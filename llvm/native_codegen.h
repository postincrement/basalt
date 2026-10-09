#ifndef NATIVE_CODEGEN_H_
#define NATIVE_CODEGEN_H_

#include "llvm_codegen.h"

// Host executable backend. The textual LLVM emitter still builds the program;
// this subclass compiles that IR and links it with the BASIC runtime.
class Native_OutputGenerator : public LLVM_OutputGenerator
{
  public:
    explicit Native_OutputGenerator(CodeGenerator & codeGenerator);

    bool EmitsBinary() const override { return true; }
    bool Run(const std::string & inputFilename, std::ostream * outputStream) override;
};

#endif
