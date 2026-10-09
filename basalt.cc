#include <iostream>
#include <sstream>
#include <iomanip>
using namespace std;

#include <unistd.h>
#include <string.h>

#include "basalt.h"

#include "c/c_codegen.h"
#include "z80/z80_codegen.h"
#include "pretty/pretty_codegen.h"
#ifdef BASALT_WITH_LLVM
#include "llvm/llvm_codegen.h"
#include "llvm/native_codegen.h"
#endif

// declared as extern in basalt.h
Basalt g_application;
int  g_errorCount        = 0;
bool g_disableWarnings   = false;
bool g_enableLineNumbers = false;
bool g_debugInfo = false;
std::string g_printableInputFilename;

// declared as extern in ast.h
LanguageProfile * g_languageProfile = nullptr;

//////////////////////////////////////////////////////////

static int  g_warningCount      = 0;
static Factory<LanguageProfile> g_languageProfileFactory;
static Factory<OutputGenerator, CodeGenerator &> g_outputGenerators;

static LanguageProfileDef g_basicVariants[] = { 
// name      varlen tab defnum              defint
{ "BasicEx",   40,  14, VarType::eSingle,  VarType::eInt16  },
{ "Basic8k",    2,  14, VarType::eSingle,  VarType::eInt16  },
{ "DiskBasic", 40,  14, VarType::eSingle,  VarType::eInt16  },
{ 0 }
};

static LanguageProfileDef g_basicZ80Variants[] = { 
// name      varlen tab defnum              defint
{ "BasicEx",   40,  14, VarType::eSingle, VarType::eInt16  },
{ "Basic8k",    2,  14, VarType::eSingle, VarType::eInt16  },
{ "DiskBasic", 40,  14, VarType::eSingle, VarType::eInt16  },
{ 0 }
};

//////////////////////////////////////////////////////////

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

void Basalt::DecodeOpt(ArgDef * def, int & index, int argc, const char **argv)
{
  std::string type(def->m_type);

  // empty type means increment value, if any
  if (type.length() == 0) {
    if (def->m_data == NULL)
      OptionError(def);
    else
      (*(int *)(def->m_data))++;
  }

  else if (type == "b") {
    if (def->m_data == NULL)
      OptionError(def);
    else
      (*(bool *)(def->m_data)) = true;
  }

  else if (type == "s") {
    if ((def->m_data == NULL) || (index >= argc))
      OptionError(def);
    else {
      (*(std::string *)(def->m_data)) = argv[++index];
    }
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

  // Options may appear before or after the source file.
  int inputIndex = argc;
  while (index < argc) {
    const char * ptr = argv[index];

    if (*ptr++ != '-') {
      if (inputIndex != argc) {
        cerr << "error: unexpected argument '" << argv[index] << "'\n";
        return -1;
      }
      inputIndex = index;
      ++index;
      continue;
    }

    // parse long options
    if (*ptr == '-') {
      ptr++;
      std::string opt(ptr);
      ArgDef * def = defs;
      while (def->m_type != 0) {
        if (opt == def->m_long) {
          DecodeOpt(def, index, argc, argv);
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
            DecodeOpt(def, index, argc, argv);
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

  return inputIndex;
}

void Basalt::Usage(bool showKeys)
{
  if (showKeys) {
    std::vector<std::string> keys;
/*
    cout << "where generator is one of:\n";
    keys = g_codegeneratorFactory.GetList();
    for (auto & r : keys)
      cout << "  " << r << "\n";
*/      
    cout << "where profile is one of:";
    keys = g_languageProfileFactory.GetList();
    for (auto & r : keys)
      cout << "  " << r << "\n";
  }
  exit(1);
}

void Basalt::DisplayHelp(const std::vector<ArgDef> & argDefs)
{
  std::vector<std::string> col1;
  std::vector<std::string> col2;
  std::vector<std::string> col3;
  std::vector<std::string> col4;

  int col1Width = 0;
  int col2Width = 0;
  int col3Width = 0;
  int col4Width = 0;

  for (auto & arg : argDefs) {    
    stringstream strm;
    if (arg.m_short != ' ') {
      strm << "-" << arg.m_short;
    }
    col1.push_back(strm.str());
    col1Width = std::max<int>(col1Width, strm.str().length());
    strm.str("");

    if ((arg.m_long != nullptr) && (arg.m_long[0] != '\0')) {
      strm << "--" << arg.m_long;
    }
    col2.push_back(strm.str());
    col2Width = std::max<int>(col2Width, strm.str().length());
    strm.str("");

    switch (arg.m_type[0]) {
      case 's':
        strm << "string";
        break;
      default:
        break;
    }
    col3.push_back(strm.str());
    col3Width = std::max<int>(col3Width, strm.str().length());

    col4.push_back(arg.m_usage);
    col4Width = std::max<int>(col4Width, strlen(arg.m_usage));
  }

  for (size_t i = 0; i < col1.size(); ++i) {
    cout << std::left << setw(col1Width) << col1[i] 
         << " " << std::left << setw(col2Width) << col2[i] 
         << " " << std::left << setw(col3Width) << col3[i] 
         << "  " << col4[i] << endl;
  }
}

int Basalt::Main(int argc, char const *argv[])
{
  // register language profiles
  g_outputGenerators.Register<C_OutputGenerator>("ansi-c");
  g_outputGenerators.Register<Z80_OutputGenerator>("z80");
  g_outputGenerators.Register<Pretty_OutputGenerator>("pretty");
#ifdef BASALT_WITH_LLVM
  g_outputGenerators.Register<LLVM_OutputGenerator>("llvm");
  g_outputGenerators.Register<Native_OutputGenerator>("native");
#endif

#ifdef BASALT_WITH_LLVM
  m_arch = "native";
  const char * archHelp = "set output architecture (default: native)";
#else
  m_arch = "ansi-c";
  const char * archHelp = "set output architecture (default: ansi-c)";
#endif
  g_disableWarnings = false;
  g_enableLineNumbers = false;
  g_debugInfo = false;
  
  std::vector<ArgDef> argDefs = {
    { 'v',   "verbose",   "",   &m_verbose,             "enable verbosity" },
    { 't',   "target",    "s",  &m_codeGeneratorName,   "set code generator" },
    { 'o',   "output",    "s",  &m_outputFilename,      "set output filename" },
    { 'p',   "profile",   "s",  &m_languageProfileName, "set language profile" },
    { ' ',   "yydebug",   "",   &MBASIC_debug,          "enable bison debugging"},
    { 'h',   "help",      "",   &m_displayHelp,         "display help message"},
    { 'W',   "warnings",  "b",  &g_disableWarnings,     "disable warnings"},
    { 'L',   "linenum",   "b",  &g_enableLineNumbers,   "enable line numbers"},
    { 'g',   "debug",     "b",  &g_debugInfo,           "emit source locations in the native executable"},
    { 'a',   "arch",      "s",  &m_arch,                archHelp }
  };

  // parse options and arguments
  int index = ParseArguments(&argDefs[0], argc, argv);
  if (index < 0)
    return -1;

  if (m_displayHelp) {
    DisplayHelp(argDefs);
    return 0;
  }

  // One lowering pass feeds every backend.
  CodeGenerator lowered;
  OutputGenerator * codeGen = g_outputGenerators.CreateInstance(m_arch, lowered);
  if (codeGen == nullptr) {
    cerr << "error: unknown arch '" << m_arch << "'" << endl;
    return -1;
  }

  // set language profile 
  LanguageProfileDef * profiles = (m_arch == "z80") ? g_basicZ80Variants : g_basicVariants;
  int languageProfileIndex = 0;
  m_languageProfileName = profiles[languageProfileIndex].m_name;
  g_languageProfile = nullptr;
  int i = 0;
  while (profiles[i].m_name != 0) {
    if (m_languageProfileName == profiles[i].m_name) {
      g_languageProfile = new LanguageProfile(&profiles[i]);
      languageProfileIndex = i;
      break;
    }
    ++i;
  }

  if (!g_languageProfile) {
    cerr << "error: language profile '" << m_languageProfileName << "' not known.\n";
    Usage(true);
    exit(1);
  }
  

  // see if using stdin or file as input  
  if (index == argc) {
    if (m_verbose)
      cerr << "info: parsing stdin" << endl;
    m_inputStream = &std::cin;
    g_printableInputFilename = "<stdin>";
  }
  else {
    m_inputStream = &m_inputFile;
    
    // open input file
    m_inputFilename = Filename(argv[index]);
    g_printableInputFilename = m_inputFilename;
    std::string ext = m_inputFilename.GetExtension();
    for (auto & r : ext) {
      r = tolower(r);
    }
    if (ext != ".bas") {
      cerr << "error: unknown input file extension '" << m_inputFilename.GetExtension() << "'" << endl;
      return -1;
    }

    m_inputFile.open(m_inputFilename);
    if (!m_inputFile) {
      cerr << "error: cannot open input file '" << m_inputFilename << "'" << endl;
      return -1;
    }
  }


  std::ostream * outputStream = nullptr;
  std::ofstream outputFile;
  std::string outputPath;

  if (m_outputFilename == "-") {
    if (codeGen->EmitsBinary()) {
      cerr << "error: native output needs a filename (-o program)\n";
      return -1;
    }
    outputStream = &std::cout;
  }
  else {
    if (!m_outputFilename.empty())
      outputPath = m_outputFilename;
    else {
      std::string dir = m_inputFilename.GetDir();
      if (!dir.empty())
        dir += "/";
      outputPath = dir + m_inputFilename.GetBasename() + codeGen->GetConfig().m_extension;
    }
    if (codeGen->EmitsBinary()) {
      if (outputPath.empty()) {
        cerr << "error: native output needs a filename (-o program)\n";
        return -1;
      }
      codeGen->SetOutputPath(outputPath);
    }
    else {
      outputFile.open(outputPath, std::ofstream::out | std::ofstream::trunc);
      if (!outputFile.is_open()) {
        cerr << "error: cannot create output file '" << outputPath << "'" << endl;
        return false;
      }
      outputStream = &outputFile;
    }
  }

  if (m_verbose)
    cerr << "info: parsing '" << g_printableInputFilename << "'" << endl;

  m_lineOffs = 2;
  MBASIC_parse();

  if (m_verbose) {
    cerr << "info: parsing finished" << endl;
  }

  if (g_errorCount > 0) {
    cout << "error: " << g_errorCount << " errors - compile stopped" << endl;
    return -1;
  }

  if (!lowered.Build(AST::g_program)) {
    cerr << "error: code generation failed" << endl;
    return -1;
  }

  if (!codeGen->Run(g_printableInputFilename, outputStream)) {
    cerr << "error: code generation failed" << endl;
    return -1;
  }

  if (g_errorCount > 0)
    return -1;

  return 0;
}

void Basalt::SetStatementStart()
{
  m_statementStart = m_lineOffs;
}

std::string Basalt::GetStatement()
{
  std::string str = Trim(m_line.substr(m_statementStart, m_lineOffs - m_statementStart));
  return str;
}

////////////////////////////////////////////////////////////////////////

char Basalt::ReadNextChar()
{
  if (m_lineOffs > m_line.length()) {
    if (!getline(*m_inputStream, m_line))
      return 0;
    m_lineOffs = 0;
    if (m_line.length() > 0) {
      char * start = &m_line[0];
      char * ptr   = start + m_line.length() - 1;
      while ((ptr > start) && (isspace(*ptr)))
        --ptr;
      m_line = m_line.substr(0, (ptr - start) + 1);
    }
  }

  if (m_lineOffs == m_line.length()) {
    //m_line.clear();
    m_lineOffs++;
    return '\n';
  }

  return m_line[m_lineOffs++];
}

std::string Basalt::FormatError(ErrorCode code, 
                                unsigned ln, 
                                int pos)
{
  std::string type;
  if (code < ErrorCode::eWarning_First) {
    m_errorCount++;
    g_errorCount++;
    type = "internal error";
  }
  else if (code <= ErrorCode::eError_First) {
    m_warningCount++;
    type = "warning";
  }
  else {
    m_errorCount++;
    g_errorCount++;
    type = "error";
  }

  stringstream strm;
  strm << g_printableInputFilename << ":" << ln << ":";
  if (pos >= 0)
    strm << pos << ":";
  strm << " " << type << " " << HEXFORMAT4(code) << " : ";
  return strm.str();
}

void Basalt::CompilerErrorInternal(ErrorCode code, int ln, const std::string & msg)
{
  cerr << FormatError(code, ln) << msg << "\n";
}

void Basalt::InternalErrorInternal(ErrorCode code, const std::string & msg)
{
  cerr << FormatError(code, 0) << msg << "\n";
  exit(-1);
}

void Basalt::ParserErrorInternal(ErrorCode code, const std::string & msg)
{
  size_t p = std::min(m_lineOffs, m_line.length());
  cerr << FormatError(code, g_lexLineNumber, p) << msg << "\n";
  cerr << m_line << endl;
  size_t i;
  for (i = 0; i < p; i++)
    cout << " ";
  cout << "^" << endl;
}

void MBASIC_yyinput(char * buf, size_t * result, size_t maxSize)
{
  char ch = g_application.ReadNextChar();

  if (ch == 0)
    *result = 0;
  else {
    buf[0] = ch;
    *result = 1;
  }
}

int main(int argc, char const *argv[])
{
  return g_application.Main(argc, argv);
}
