#include <iostream>
#include <sstream>
using namespace std;

#include "basalt.h"
#include "codegen.h"

Filename g_inputFilename;
int g_lineNumber = 1;
int g_errorCount = 0;

LanguageProfile g_profile;
AST::ExprList g_expressions;

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

int g_verbose = 0;
std::string g_targetTripleStr;
bool g_dumpAsm = false;
bool g_compileOnly = false;

ArgDef g_argDefs[] = {
  { 'c',   "",          "b",  &g_compileOnly,     "compile only" },
  { 'd',   "dump",      "b",  &g_dumpAsm,         "dump assembly" },
  { 'v',   "verbose",   "",   &g_verbose,         "enable verbosity" },
  { ' ',   "yydebug",   "",   &MBASIC_debug,      "enable bison debugging"},
  { ' ',   "target",    "s",  &g_targetTripleStr, "set compiler target triplet" },
  {  0,    NULL,        NULL, NULL,                NULL }
};

void Basalt::Usage(const ArgDef * defs)
{
}

void OptionError(const ArgDef * def)
{
  cerr << "warning: no data for option ";
  if (def->m_short != ' ')
    cerr << "-" << def->m_short;
  if (def->m_long != NULL) {
    if (def->m_short != ' ')
      cerr << "/";
    cerr << "--" << def->m_long << endl;
  }
}


void Basalt::DecodeOpt(ArgDef * def)
{
  std::string type(def->m_type);

  // empty type means increment value, if any
  if (type.length() == 0) {
    if (def->m_data != NULL)
      (*(int *)(def->m_data))++;
    else
      OptionError(def);
  }

  else if (type == "b") {
    if (def->m_data != NULL)
      (*(bool *)(def->m_data)) = true;
    else
      OptionError(def);
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
    g_targetTripleStr = targetTriple; 
  }

  // parse options and arguments
  int index = ParseArguments(g_argDefs, argc, argv);
  if (index >= argc) {
    Usage(g_argDefs);
    return -1;
  }

  ///////////////////////////////////////////////////////////////////////////////
  //
  // parse input file
  //

  // open input file
  g_inputFilename = Filename(argv[index]);

  if (g_inputFilename.GetExtension() != ".bas") {
    cerr << "error: unknown input file extension '" << g_inputFilename.GetExtension() << "'" << endl;
    return -1;
  }

  MBASIC_in = fopen(g_inputFilename.c_str(), "r");
  if (MBASIC_in == NULL) {
    cerr << "error: cannot open input file '" << g_inputFilename << "'" << endl;
    return -1;
  }

  if (g_verbose)
    cout << "info: parsing '" << g_inputFilename.GetFilename() << "'" << endl;

  MBASIC_parse();

  if (g_errorCount > 0) {
    cout << "error: " << g_errorCount << " errors - compile stopped" << endl;
    return -1;
  }

  ///////////////////////////////////////////////////////////////////////////////
  //
  //  code generation
  //

  if (g_verbose)
    cout << "info: generating code" << endl;

  CodeGenerator cg(cerr);

  llvm::BasicBlock * block = cg.StartMain();

  {    
    std::vector<llvm::Value *> ArgsV;

    /*
    for (unsigned i = 0, e = Args.size(); i != e; ++i) {
      ArgsV.push_back(Args[i]->codegen());
      if (!ArgsV.back())
        return nullptr;
    }
    */
    llvm::FunctionType * FT = llvm::FunctionType::get(llvm::Type::getVoidTy(cg.m_context), false);
    llvm::Function * calleeF = llvm::Function::Create(FT, llvm::GlobalValue::ExternalLinkage, "basalt_init", cg.m_module.get());    
    cg.m_builder.CreateCall(calleeF, ArgsV, "calltmp");    

    llvm::CallInst* int64_4 = llvm::CallInst::Create(calleeF, "", block);
    int64_4->setCallingConv(llvm::CallingConv::C);
    int64_4->setTailCall(false);    
  }

  for (auto & r : g_expressions) {
    if (r != nullptr)
      cg.Generate(*r);
  }

  cg.EndMain(block);

  //GenerateCall0(module, "basalt_init");    
  //CodeGenerator::FunctionASTExpr * mainFunc = new CodeGenerator::FunctionASTExpr("main", NULL);

  //llvm::BasicBlock * entry = llvm::BasicBlock::Create(module.GetContext(), "entrypoint", mainFunc->Generate(module));
  //module.GetBuilder().SetInsertPoint(entry);

  if (g_dumpAsm)
    cg.Dump();

  /////////////////////////////////////////////////////////////
  //
  //  outputting object file
  //

  // create output filename
  Filename objectFilename(g_inputFilename.GetDir() + g_inputFilename.GetBasename() + ".o");

  if (g_verbose)
    cout << "info: creating '" << objectFilename.GetFilename() << "'" << endl;

  // Initialize the target registry etc.
  llvm::InitializeAllTargetInfos();
  llvm::InitializeAllTargets();
  llvm::InitializeAllTargetMCs();
  llvm::InitializeAllAsmParsers();
  llvm::InitializeAllAsmPrinters();  

  std::string Error;
  auto Target = llvm::TargetRegistry::lookupTarget(g_targetTripleStr, Error);

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

  auto TargetMachine = Target->createTargetMachine(g_targetTripleStr, CPU, Features, opt, RM);  

  cg.m_module->setDataLayout(TargetMachine->createDataLayout());
  cg.m_module->setTargetTriple(g_targetTripleStr);  

  std::error_code EC;
  llvm::raw_fd_ostream dest(objectFilename.c_str(), EC, llvm::sys::fs::F_None);

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

  pass.run(*cg.m_module);

  dest.flush();  

  if (g_compileOnly)
    return 0;

  // do the linker thing
  Filename exeFilename(g_inputFilename.GetDir() + g_inputFilename.GetBasename());
  std::stringstream cmd;
  cmd << "clang " << objectFilename << " -L. -lbasaltrt -o " << exeFilename ;


  int result = system(cmd.str().c_str());
  if (result != 0)
    cerr << "error: linker failed" << endl;

  return 0;
}

void MBASIC_error(const char * msg)
{
  g_errorCount++;
  cout << g_inputFilename << " (" << g_lineNumber << "): " << msg << endl;
}

int main(int argc, char const *argv[])
{
  return g_application.Main(argc, argv);
}
