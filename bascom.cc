#include "codegen.h"

int main(int argc, char const *argv[])
{
	CodeGenerator::FunctionASTExpr mainFunc("main");

	CodeGenerator::Module module;
	mainFunc.Generate(module);
  module.Dump();

  return 0;
}
