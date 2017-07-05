#include "codegen.h"

llvm::Value * AST::IntVariableRefExpr::Generate(CodeGenerator & cg)
{
  return cg.Generate(*this);
}

llvm::Value * AST::BinaryExpr::Generate(CodeGenerator & cg)
{
  return cg.Generate(*this);
}

llvm::Value * AST::VariableRefExpr::Generate(CodeGenerator & cg)
{
  return cg.Generate(*this);
}

namespace AST {
template <>
llvm::Value * ConstantIntExpr<uint16_t>::Generate(CodeGenerator & cg)
{
  return cg.Generate(*this);
}

template <>
llvm::Value * IntVariableDefExpr<uint16_t>::Generate(CodeGenerator & cg)
{
  return cg.Generate(*this);
}

}

