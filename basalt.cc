#include <iostream>
#include <sstream>
using namespace std;

#include <unistd.h>

#include "basalt.h"

//#include "codegen.h"

Filename g_inputFilename;
int g_lineNumber   = 1;
int g_errorCount   = 0;
int g_warningCount = 0;

LanguageProfile          g_profile;
AST::ExprList            g_expressions;
AST::VariableDefList     g_variables;

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
  m_interactive = false;

  {
    //auto targetTriple = llvm::sys::getDefaultTargetTriple();
    //g_targetTripleStr = targetTriple; 
  }

  // parse options and arguments
  int index = ParseArguments(g_argDefs, argc, argv);
  if (index >= argc) {
    m_interactive = true;
    g_inputFilename = "<stdin>";
    cout << "> " << flush;
    while (getline(cin, m_line)) {
      m_lineOffs = 0;
      g_errorCount = 0;
      g_expressions.clear();
      MBASIC_parse();
      if (g_errorCount == 0) {
        cout << "compile here" << endl;
      }
      cout << "> " << flush;
    }
    return 0;
  }

  //  Usage(g_argDefs);
  //  return -1;
  //}

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

  m_inputFile.open(g_inputFilename);
  if (!m_inputFile) {
    cerr << "error: cannot open input file '" << g_inputFilename << "'" << endl;
    return -1;
  }

  if (g_verbose)
    cout << "info: parsing '" << g_inputFilename.GetFilename() << "'" << endl;

  m_lineOffs = 2;
  MBASIC_parse();

  if (g_errorCount > 0) {
    cout << "error: " << g_errorCount << " errors - compile stopped" << endl;
    return -1;
  }

  AST::Dumper dumper(g_expressions, cout);
  g_expressions.Accept(dumper);

#if 0
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

    llvm::CallInst* int64_4 = llvm::CallInst::Create(calleeF, "", block);
    int64_4->setCallingConv(llvm::CallingConv::C);
    int64_4->setTailCall(false);    
  }

  cout << "generating code for " << g_expressions.size() << " expressions" << endl;

/*
    llvm::StoreInst* void_10 = new llvm::StoreInst(const_ptr_6, ptr_9, false, label_8);
    void_10->setAlignment(8);
    llvm::GetElementPtrInst* ptr_11 = llvm::GetElementPtrInst::Create(StructTy_struct_String, ptr_a, {
     const_int32_5, 
     const_int32_4
    }, "", label_8);

    llvm::StoreInst* void_12 = new llvm::StoreInst(const_int8_7, ptr_11, false, label_8);
    void_12->setAlignment(8);
  }
 */   

  for (auto & r : g_expressions) {
    if (r != nullptr) {
      //cout << "generating code for non-null expression" << endl;
      r->Generate(cg);
    }
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

#endif    

  return 0;
}



char Basalt::ReadNextChar()
{
  if (m_lineOffs > m_line.length()) {
    if (m_interactive)
      return 0;
    if (!getline(m_inputFile, m_line))
      return 0;
    m_lineOffs = 0;
  }

  if (m_lineOffs == m_line.length()) {
    m_lineOffs++;
    return '\n';
  }

  return m_line[m_lineOffs++];
}


void Basalt::OnError(const std::string & msg)
{
  g_errorCount++;
  DisplayError(msg, "error");
}

void Basalt::OnWarning(const std::string & msg)
{
  g_warningCount++;
  DisplayError(msg, "error");
}

void Basalt::DisplayError(const std::string & msg, const std::string & type)
{
  cout << g_inputFilename << ":" << g_lineNumber << ":" << m_lineOffs << ": " << type << " - " << msg << endl;
  cout << m_line << endl;
  size_t i;
  for (i = 0; i < m_lineOffs-2; i++)
    cout << " ";
  cout << "^" << endl;
}


void MBASIC_yyinput(char * buf, int * result, int maxSize)
{
  char ch = g_application.ReadNextChar();

  if (ch == 0)
    *result = 0;
  else {
    buf[0] = ch;
    *result = 1;
  }
}

void MBASIC_error(const char * msg)
{
  g_application.OnError(msg);
}


int main(int argc, char const *argv[])
{
  return g_application.Main(argc, argv);
}
