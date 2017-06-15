#include <iostream>
using namespace std;

#include "codegen.h"


struct ArgDef 
{
  const char   m_short;
  const char * m_long;
  const char * m_type;
  void       * m_data;
  const char * m_usage;
};

class Basalt
{
  public:
    int Main(int argc, char const *argv[]);


    int ParseArguments(ArgDef * defs, int argc, char const *argv[], int index);
    void DecodeOpt(ArgDef * def);
    void Usage(const ArgDef * defs);

    std::string m_progname;
};

Basalt g_application;

std::map<std::string, CodeGenerator::GlobalVariable *> g_globals;
std::map<std::string, CodeGenerator::FunctionASTExpr *> g_functions;
std::vector<CodeGenerator::ASTExpr *> g_expressions;


int m_verbose = 0;
std::string m_targetTripleStr;

ArgDef g_argDefs[] = {
  { 'v',   "verbose",   "",   &m_verbose,         "enable verbosity" },
  { ' ',   "target",    "s",  &m_targetTripleStr, "set compiler target triplet" },
  {  0,    NULL,        NULL, NULL,                NULL }
};

void Basalt::Usage(const ArgDef * defs)
{

}

void Basalt::DecodeOpt(ArgDef * def)
{
  std::string type(def->m_type);

  // empty type means increment value, if any
  if (type.length() == 0) {
    if (def->m_data != NULL)
      (*(int *)(def->m_data))++;
  }

  else {
    cerr << "warning: unknown argument type '" << type << "'" << endl;
  }
}

int Basalt::ParseArguments(ArgDef * defs, int argc, char const *argv[], int index = 1)
{
  // extract program nameindex
  m_progname = std::string(argv[0]);
  size_t pos = m_progname.find_last_of('/');
  if (pos != string::npos)
    m_progname = m_progname.substr(pos+1);

  // parse the arguments
  while (index < argc) {
    const char * ptr = argv[index];

    // if not an option, move on to parsing arguments
    if (*ptr++ != '-')
      break;

    // parse long options
    if (*ptr == '-') {
      ptr++;
      std::string opt(ptr);
      ArgDef * def = defs;
      while (def->m_type != 0) {
        if (opt == def->m_long) {
          DecodeOpt(def);
          break;
        }
        ++def;
      }
      if (def->m_type == 0) {
        cerr << "warning: unknown option long '" << ptr << "'" << endl;
      }
    }

    // parse short options
    else {
      while (*ptr != '\0') {
        ArgDef * def = defs;
        while (def->m_type != 0) {
          if (*ptr == def->m_short) {
            DecodeOpt(def);
            ++ptr;
            break;
          }
          ++def;
        }
        if (def->m_type == 0) {
          cerr << "warning: unknown short option '" << *ptr << "'" << endl;
          ++ptr;
        }
      }
    }
    ++index;
  }

  return index;
}

int Basalt::Main(int argc, char const *argv[])
{
  {
    auto targetTriple = llvm::sys::getDefaultTargetTriple();
    m_targetTripleStr = targetTriple; 
  }

  // parse options and arguments
  int index = ParseArguments(g_argDefs, argc, argv);
  if (index < 0) {
    Usage(g_argDefs);
    return -1;
  }

  cout << m_targetTripleStr << endl;

  llvm::StringRef targetTriple(m_targetTripleStr);

  // get source filename
  //if (index < argc) {
  //  cout << argv[index] << endl;
  //  return 0;
  //}

//  g_functions["main"] = new CodeGenerator::FunctionASTExpr("main");

  CodeGenerator::Module module;

  g_globals["var1"] = new CodeGenerator::GlobalVariable("var1");  
  g_globals["var2"] = new CodeGenerator::GlobalVariable("var2");  
  g_globals["var3"] = new CodeGenerator::GlobalVariable("var3");  

#if 0  

  g_functions["main"] = new CodeGenerator::FunctionASTExpr("main",
                            new CodeGenerator::BinaryOpExpr(
                              new CodeGenerator::ConstantIntExpr(1), 
                              new CodeGenerator::ConstantIntExpr(2)
                            )
                           );   

#endif

  for (auto & r : g_functions)
    r.second->Generate(module);

  for (auto & r : g_expressions)
    r->Generate(module);

  module.Dump();

  /////////////////////////////////////////////////////////////

  // Initialize the target registry etc.
  llvm::InitializeAllTargetInfos();
  llvm::InitializeAllTargets();
  llvm::InitializeAllTargetMCs();
  llvm::InitializeAllAsmParsers();
  llvm::InitializeAllAsmPrinters();  

  std::string Error;
  auto Target = llvm::TargetRegistry::lookupTarget(targetTriple, Error);

  // Print an error and exit if we couldn't find the requested target.
  // This generally occurs if we've forgotten to initialise the
  // TargetRegistry or we have a bogus target triple.
  if (!Target) {
    cerr << Error;
    return 1;
  }  

  auto CPU = "generic";
  auto Features = "";

  llvm::TargetOptions opt;
  auto RM = llvm::Reloc::Model();  

//  llvm::TargetOptions opt;
//  auto RM = llvm::Optional<llvm::Reloc::Model>();

  auto TargetMachine = Target->createTargetMachine(targetTriple, CPU, Features, opt, RM);  

  module.GetModule().setDataLayout(TargetMachine->createDataLayout());
  module.GetModule().setTargetTriple(targetTriple);  

  auto Filename = "output.o";
  std::error_code EC;
  llvm::raw_fd_ostream dest(Filename, EC, llvm::sys::fs::F_None);

  if (EC) {
    cerr << "Could not open file: " << EC.message();
    return 1;
  }  

  llvm::legacy::PassManager pass;
  auto FileType = llvm::TargetMachine::CGFT_ObjectFile;

  if (TargetMachine->addPassesToEmitFile(pass, dest, FileType)) {
    cerr << "TargetMachine can't emit a file of this type";
    return 1;
  }

#if 1
  pass.run(module.GetModule());
#endif 

  dest.flush();  

  return 0;
}

int main(int argc, char const *argv[])
{
  return g_application.Main(argc, argv);
}
