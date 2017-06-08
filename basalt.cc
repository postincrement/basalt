#include "codegen.h"



std::map<std::string, CodeGenerator::GlobalVariable *> g_globals;
std::map<std::string, CodeGenerator::FunctionASTExpr *> g_functions;
std::vector<CodeGenerator::ASTExpr *> g_expressions;

int main(int argc, char const *argv[])
{
//  g_functions["main"] = new CodeGenerator::FunctionASTExpr("main");

  g_globals["var1"] = new CodeGenerator::GlobalVariable("var1");  
  g_globals["var2"] = new CodeGenerator::GlobalVariable("var2");  
  g_globals["var3"] = new CodeGenerator::GlobalVariable("var3");  

  g_functions["main"] = new CodeGenerator::FunctionASTExpr("main",
                            new CodeGenerator::BinaryOpExpr(
                              new CodeGenerator::ConstantIntExpr(1), 
                              new CodeGenerator::ConstantIntExpr(2)
                            )
                           );   

	CodeGenerator::Module module;

  for (auto & r : g_functions)
    r.second->Generate(module);

  for (auto & r : g_expressions)
    r->Generate(module);

  module.Dump();

  return 0;
}
