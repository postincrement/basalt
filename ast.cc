#include "codegen.h"

llvm::Value * AST::Int16VariableRefExpr::Generate(CodeGenerator & cg)
{
  return cg.Generate(*this);
}

llvm::Value * AST::Int16BinaryExpr::Generate(CodeGenerator & cg)
{
  return cg.Generate(*this);
}



llvm::Value * AST::StringVariableRefExpr::Generate(CodeGenerator & cg)
{
  return cg.Generate(*this);
}

llvm::Value * AST::StringConstantExpr::Generate(CodeGenerator & cg)
{
  return cg.Generate(*this);
}

llvm::Value * AST::StringVariableDefExpr::Generate(CodeGenerator & cg)
{
  return cg.Generate(*this);
}

llvm::Value * AST::StringBinaryExpr::Generate(CodeGenerator & cg)
{
  return cg.Generate(*this);
}



llvm::Value * AST::BIFExpr::Generate(CodeGenerator & cg)
{
  return cg.Generate(*this);
}


namespace AST {

template <>
llvm::Value * ConstantIntExpr<int16_t>::Generate(CodeGenerator & cg)
{
  return cg.Generate(*this);
}

template <>
llvm::Value * IntVariableDefExpr<int16_t>::Generate(CodeGenerator & cg)
{
  return cg.Generate(*this);
}

template <>
llvm::Value * AST::Call1Expr<char *>::Generate(CodeGenerator & cg)
{
  return cg.Generate(*this);
}

template <>
llvm::Value * AST::Call1Expr<int16_t *>::Generate(CodeGenerator & cg)
{
  return cg.Generate(*this);
}

}

