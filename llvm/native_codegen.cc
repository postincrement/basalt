#include "native_codegen.h"

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "llvm/Analysis/CGSCCPassManager.h"
#include "llvm/Analysis/LoopAnalysisManager.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Metadata.h"
#include "llvm/IR/PassManager.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Module.h"
#include "llvm/IRReader/IRReader.h"
#include "llvm/MC/TargetRegistry.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/SourceMgr.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/TargetParser/Host.h"

#if defined(__APPLE__)
#include "llvm/Support/Program.h"
#endif

using namespace std;

Native_OutputGenerator::Native_OutputGenerator(CodeGenerator & codeGenerator)
  : LLVM_OutputGenerator(codeGenerator)
{
  m_config.m_extension = "";
}

#if defined(__APPLE__)

static string Version3(string version)
{
  int dots = 0;
  for (char ch : version)
    if (ch == '.')
      ++dots;
  if (dots == 0)
    version += ".0.0";
  else if (dots == 1)
    version += ".0";
  return version;
}

static bool LinkExecutable(const string & objectPath, const string & outputPath, string & error)
{
#ifndef BASALT_LD
  error = "native linking was not configured";
  return false;
#else
  string arch = llvm::Triple(llvm::sys::getDefaultTargetTriple()).getArchName().str();
  if (arch == "aarch64")
    arch = "arm64";

  string minVersion = Version3(BASALT_MACOS_VERSION);
  string sdkVersion = Version3(BASALT_SDK_VERSION);
  vector<string> storage = {
    BASALT_LD,
    "-arch", arch,
    "-platform_version", "macos", minVersion, sdkVersion,
    "-syslibroot", BASALT_SYSROOT,
    "-dead_strip",
    "-o", outputPath,
    objectPath,
    BASALT_RUNTIME_OBJECT,
    "-lSystem"
  };
  vector<llvm::StringRef> args;
  args.reserve(storage.size());
  for (const string & arg : storage)
    args.push_back(arg);

  string errMsg;
  bool failed = false;
  int rc = llvm::sys::ExecuteAndWait(storage[0], args, nullopt, {}, 0, 0, &errMsg, &failed);
  if (failed || rc != 0) {
    error = "linker failed";
    if (!errMsg.empty())
      error += ": " + errMsg;
    return false;
  }
  return true;
#endif
}

static bool WriteDsym(const string & executable, string & error)
{
#ifndef BASALT_DSYMUTIL
  error = "dsymutil was not configured";
  return false;
#else
  if (string(BASALT_DSYMUTIL).empty()) {
    error = "dsymutil was not found";
    return false;
  }
  vector<string> storage = { BASALT_DSYMUTIL, executable };
  vector<llvm::StringRef> args;
  for (const string & arg : storage)
    args.push_back(arg);
  string errMsg;
  bool failed = false;
  int rc = llvm::sys::ExecuteAndWait(storage[0], args, nullopt, {}, 0, 0, &errMsg, &failed);
  if (failed || rc != 0) {
    error = "dsymutil failed";
    if (!errMsg.empty())
      error += ": " + errMsg;
    return false;
  }
  return true;
#endif
}

static void OptimizeModule(llvm::Module & module, bool debug)
{
  llvm::LoopAnalysisManager loops;
  llvm::FunctionAnalysisManager functions;
  llvm::CGSCCAnalysisManager cgscc;
  llvm::ModuleAnalysisManager modules;
  llvm::PassBuilder builder;
  builder.registerModuleAnalyses(modules);
  builder.registerCGSCCAnalyses(cgscc);
  builder.registerFunctionAnalyses(functions);
  builder.registerLoopAnalyses(loops);
  builder.crossRegisterProxies(loops, functions, cgscc, modules);

  llvm::OptimizationLevel level = debug ? llvm::OptimizationLevel::O0 : llvm::OptimizationLevel::O2;
  llvm::ModulePassManager passes = debug
    ? builder.buildO0DefaultPipeline(level)
    : builder.buildPerModuleDefaultPipeline(level);
  passes.run(module, modules);
}

static bool WriteModule(llvm::Module & module, const string & path, string & error)
{
  error_code openError;
  llvm::raw_fd_ostream out(path, openError);
  if (openError) {
    error = openError.message();
    return false;
  }
  module.print(out, nullptr);
  return true;
}

static bool EmitObject(const string & ir, const string & objectPath, const string & outputPath, string & error, bool & debug)
{
  static bool ready = false;
  if (!ready) {
    if (llvm::InitializeNativeTarget() || llvm::InitializeNativeTargetAsmPrinter()) {
      error = "this build of LLVM has no native code generator";
      return false;
    }
    ready = true;
  }

  llvm::LLVMContext context;
  llvm::SMDiagnostic diagnostic;
  auto buffer = llvm::MemoryBuffer::getMemBufferCopy(ir, "basalt.ll");
  unique_ptr<llvm::Module> module = llvm::parseIR(*buffer, diagnostic, context);
  if (!module) {
    string message;
    llvm::raw_string_ostream stream(message);
    diagnostic.print("basalt", stream);
    error = message;
    return false;
  }

  string tripleName = llvm::sys::getDefaultTargetTriple();
  llvm::Triple triple(tripleName);
  module->setTargetTriple(triple);

  string lookupError;
  const llvm::Target * target = llvm::TargetRegistry::lookupTarget(triple, lookupError);
  if (target == nullptr) {
    error = lookupError;
    return false;
  }

  llvm::TargetOptions options;
  debug = false;
  if (llvm::NamedMDNode * units = module->getNamedMetadata("llvm.dbg.cu"))
    debug = units->getNumOperands() > 0;
  auto level = debug ? llvm::CodeGenOptLevel::None : llvm::CodeGenOptLevel::Default;
  unique_ptr<llvm::TargetMachine> machine(target->createTargetMachine(
      triple, "generic", "", options, llvm::Reloc::PIC_, nullopt, level));
  if (!machine) {
    error = "could not create a target machine for " + tripleName;
    return false;
  }
  module->setDataLayout(machine->createDataLayout());
  OptimizeModule(*module, debug);
  if (!WriteModule(*module, outputPath + ".ll", error))
    return false;

  error_code openError;
  llvm::raw_fd_ostream object(objectPath, openError, llvm::sys::fs::OF_None);
  if (openError) {
    error = openError.message();
    return false;
  }

  llvm::legacy::PassManager passes;
  if (machine->addPassesToEmitFile(passes, object, nullptr, llvm::CodeGenFileType::ObjectFile)) {
    error = "the native target cannot emit an object file";
    return false;
  }
  passes.run(*module);
  object.flush();
  return true;
}

#endif

bool Native_OutputGenerator::Run(const string & inputFilename, ostream *)
{
  if (m_outputPath.empty()) {
    cerr << "error: native output needs a filename (-o program)\n";
    return false;
  }

  ostringstream ir;
  if (!LLVM_OutputGenerator::Run(inputFilename, &ir))
    return false;

#if !defined(__APPLE__)
  cerr << "error: native executables are produced on macOS\n";
  return false;
#else
  llvm::SmallString<128> objectPath;
  error_code tempError = llvm::sys::fs::createTemporaryFile("basalt", "o", objectPath);
  if (tempError) {
    cerr << "error: " << tempError.message() << "\n";
    return false;
  }

  string error;
  bool debug = false;
  bool ok = EmitObject(ir.str(), objectPath.str().str(), m_outputPath, error, debug);
  if (ok)
    ok = LinkExecutable(objectPath.str().str(), m_outputPath, error);
  if (ok && debug)
    ok = WriteDsym(m_outputPath, error);
  llvm::sys::fs::remove(objectPath);
  if (!ok)
    cerr << "error: " << error << "\n";
  return ok;
#endif
}
