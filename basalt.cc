#include <iostream>
#include <sstream>
#include <iomanip>
using namespace std;

#include <unistd.h>

#include "basalt.h"

//#include "codegen.h"

int g_lineNumber   = 1;
int g_errorCount   = 0;
int g_warningCount = 0;

LanguageProfile * g_languageProfile = nullptr;
AST::SourceFileExprList  g_expressions;
std::set<std::string> g_stringConstants;
std::set<std::string> g_gotoTargets;

Basalt g_application;

int g_verbose = 0;
int g_displayHelp = 0;
std::string g_codeGeneratorName;
std::string g_outputFilename;
std::string g_languageProfileName;
bool g_dump = false;
bool g_compileOnly = false;
bool g_enableDebugging = false;

ArgDef g_argDefs[] = {
  { 'c',   "",          "b",  &g_compileOnly,         "compile only" },
  { 'd',   "dump",      "b",  &g_dump,                "dump output" },
  { 'v',   "verbose",   "",   &g_verbose,             "enable verbosity" },
  { 't',   "target",    "s",  &g_codeGeneratorName,   "set code generator" },
  { 'o',   "output",    "s",  &g_outputFilename,      "set output filename" },
  { 'p',   "profile",   "s",  &g_languageProfileName, "set language profile" },
  { ' ',   "yydebug",   "",   &MBASIC_debug,          "enable bison debugging"},
  { 'g',   "debug",     "b",  &g_enableDebugging,     "add debugging information to output file"},
  { 'h',   "help",      "",   &g_displayHelp,         "display help message"},
  {  0,    NULL,        NULL, NULL,                    NULL }
};

static Factory<CodeGenerator, const std::string &, const std::string, AST::SourceFileExprList &> g_codegeneratorFactory;
static Factory<LanguageProfile> g_languageProfileFactory;

const char * g_runtimeDefPrefix = "basalt_";

struct RuntimeFunctionDef g_runtimeDefs[] = {
  { nullptr,  "init",          nullptr        },
  { nullptr,  "print_string",  "const char *" },
  { nullptr,  "print_int16",   "uint16_t"     },
  { nullptr,  "print_single",  "float"        },
  { nullptr,  "print_double",  "double"       },
  { nullptr,  "print_tab",     nullptr        },
  { nullptr,  "print_eol",     nullptr        },
  { nullptr,  "assign_string", "const char *,const char *" },
  { nullptr, nullptr, nullptr }
};

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

void InternalErrorFunc(const char * fn, unsigned ln, const std::string & str)
{
  cerr << "error: internal error " << fn << "(" << ln << ") - " << str << endl;
  exit(1);
}

void InternalWarningFunc(const char * fn, unsigned ln, const std::string & str)
{
  cerr << "warning: error " << fn << "(" << ln << ") - " << str << endl;
}

void SourceWarningFunc(WarningCode code, unsigned line, const std::string & marker, const std::string & str)
{
  std::stringstream strm;
  strm << "line " << line << ": warning " << setw(4) << setfill('0') << hex << code << " - " << str << endl;
  cerr << strm.str();
}

void WarningFunc(WarningCode code, const std::string & str)
{
  std::stringstream strm;
  strm << "warning " << setw(4) << setfill('0') << hex << code << " - " << str << endl;
  cerr << strm.str();
}

FunctionDef::FunctionDef()
{
}

FunctionDef::FunctionDef(const std::string & name)
  : m_returnType("void")
  , m_name(name)
{
}

FunctionDef::FunctionDef(const RuntimeFunctionDef & def)
  : m_name(def.m_name)
{
  if (def.m_returnType != nullptr)
    m_returnType = def.m_returnType;

  if (def.m_args != nullptr) {    
    std::string argStr(def.m_args);
    Tokenize(m_args, argStr, ',');
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

  return index;
}


void Basalt::Usage(const ArgDef * defs, bool showKeys)
{
  if (showKeys) {
    cout << "where generator is one of:\n";
    std::vector<std::string> keys = g_codegeneratorFactory.GetList();
    for (auto & r : keys)
      cout << "  " << r << "\n";
    cout << "where profile is one of:";
    keys = g_languageProfileFactory.GetList();
    for (auto & r : keys)
      cout << "  " << r << "\n";
  }
  exit(1);
}

void Basalt::DisplayHelp()
{
  std::vector<std::string> col1;
  std::vector<std::string> col2;
  std::vector<std::string> col3;
  std::vector<std::string> col4;

  int col1Width = 0;
  int col2Width = 0;
  int col3Width = 0;
  int col4Width = 0;

  for (ArgDef * arg = g_argDefs; arg->m_short != 0; ++arg) {    
    stringstream strm;
    if (arg->m_short != ' ') {
      strm << "-" << arg->m_short;
    }
    col1.push_back(strm.str());
    col1Width = std::max<int>(col1Width, strm.str().length());
    strm.str("");

    if ((arg->m_long != nullptr) && (arg->m_long[0] != '\0')) {
      strm << "--" << arg->m_long;
    }
    col2.push_back(strm.str());
    col2Width = std::max<int>(col2Width, strm.str().length());
    strm.str("");

    switch (arg->m_type[0]) {
      case 's':
        strm << "string";
        break;
      default:
        break;
    }
    col3.push_back(strm.str());
    col3Width = std::max<int>(col3Width, strm.str().length());

    col4.push_back(arg->m_usage);
    col4Width = std::max<int>(col4Width, strlen(arg->m_usage));
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
  m_interactive = false;

  // register code generators
  g_codegeneratorFactory.Register<CodegenCXX>   ("c");
  g_codegeneratorFactory.Register<CodegenCXX>   ("cc");
  g_codegeneratorFactory.Register<CodegenCXX>   ("cpp");
  g_codegeneratorFactory.Register<CodegenCXX>   ("cxx");
  g_codegeneratorFactory.Register<CodegenLLVM>  ("llvm");

  // register language profiles
  g_languageProfileFactory.Register<Basic_8k_LanguageProfile>      ("basic-8k");
  g_languageProfileFactory.Register<Basic_Extended_LanguageProfile>("basic-ext");
  g_languageProfileFactory.Register<Basic_Disk_LanguageProfile>    ("basic-disk");
  
  // parse options and arguments
  int index = ParseArguments(g_argDefs, argc, argv);

  if (g_displayHelp) {
    DisplayHelp();
    return 0;
  }

  // set default code generator if no output file specified 
  if (g_codeGeneratorName.empty() && g_outputFilename.empty())
    g_codeGeneratorName = "c";

  // set default code generator if no output file specified 
  if (g_languageProfileName.empty())
    g_languageProfileName = "basic-8k";

  // see if input filename exists  
  if (index >= argc) {
    cerr << "error: no input filename specified" << endl;
    exit(1);
  }
  
  // see if the code generator exists
  if (!g_codegeneratorFactory.Contains(g_codeGeneratorName)) {
    cerr << "error: code generator " << g_codeGeneratorName << "not known.\n";
    Usage(g_argDefs, true);
    exit(1);
  }

  // see if the language profile exists
  if (!g_languageProfileFactory.Contains(g_languageProfileName)) {
    cerr << "error: code generator " << g_languageProfileName << "not known.\n";
    Usage(g_argDefs, true);
    exit(1);
  }

  // open input file
  m_inputFilename = Filename(argv[index]);
  std::string ext = m_inputFilename.GetExtension();
  for (auto & r : ext) {
    r = tolower(r);
  }
  if (ext != ".bas") {
    cerr << "error: unknown input file extension '" << m_inputFilename.GetExtension() << "'" << endl;
    return -1;
  }

  g_languageProfile = g_languageProfileFactory.CreateInstance(g_languageProfileName);
  if (g_languageProfile == nullptr) {
    cerr << "internal error: cannot instantiate language profile with name '" << g_languageProfileName << "'" << endl;
    return -1;
  }

  m_inputFile.open(m_inputFilename);
  if (!m_inputFile) {
    cerr << "error: cannot open input file '" << m_inputFilename << "'" << endl;
    return -1;
  }

  if (g_verbose)
    cout << "info: parsing '" << m_inputFilename.GetFilename() << "'" << endl;

  m_lineOffs = 2;
  MBASIC_parse();

  if (g_errorCount > 0) {
    cout << "error: " << g_errorCount << " errors - compile stopped" << endl;
    return -1;
  }

  CodeGenerator * generator = 
      g_codegeneratorFactory.CreateInstance(g_codeGeneratorName, g_codeGeneratorName, m_inputFilename, g_expressions);

  if (generator == nullptr) {
    cerr << "internal error: cannot instantiate generator with name '" << g_codeGeneratorName << "'" << endl;
    return -1;
  }

  if (
      !generator->Open(argc, argv) ||
      !generator->TopLevel(g_expressions) ||
      !generator->Close(g_outputFilename)
     )
     return -1;

  return 0;
}

////////////////////////////////////////////////////////////////////////

BasicLanguageProfile::BasicLanguageProfile(int normVarLen)
{
  m_normalizedVarLen = normVarLen;
}

bool BasicLanguageProfile::NormalizeVariableName(Variable & var, int dim)
{
  std::string rawName = var.m_name;

#if 0  
  
  // take out unprintables
  for (auto & r : rawName)
    if (!isalnum(r) && (r != '_'))
      r = '_';
    else
      r = tolower(r);  

  std::stringstream strm;
  strm << rawName;
    
  switch (var.m_type) {
    case Variable::eString:
      strm << "_string";
      break;
    case Variable::eInt16:
      strm << "_int";
      break;
    case Variable::eSingle:
      strm << "_single";
      break;
    case Variable::eDouble:
      strm << "_double";
      break;
    case Variable::eUntyped:
      return false;
  };

  if (dim > 0)
    strm << "_array_" << dim;

#endif

  var.m_normalizedName = rawName;

  return true;
}

////////////////////////////////////////////////////////////////////////

Basic_8k_LanguageProfile::Basic_8k_LanguageProfile()
 : BasicLanguageProfile(2)
{
}

Variable::Type Basic_8k_LanguageProfile::GetDefaultNumericType()
{
  return Variable::Type::eSingle;
}

////////////////////////////////////////////////////////////////////////

Basic_Extended_LanguageProfile::Basic_Extended_LanguageProfile()
  : BasicLanguageProfile(40)
{
}

Variable::Type Basic_Extended_LanguageProfile::GetDefaultNumericType()
{
  return Variable::Type::eInt16;
}

////////////////////////////////////////////////////////////////////////

Basic_Disk_LanguageProfile::Basic_Disk_LanguageProfile()
  : BasicLanguageProfile(40)
{  
}

Variable::Type Basic_Disk_LanguageProfile::GetDefaultNumericType()
{
  return Variable::Type::eInt16;
}

////////////////////////////////////////////////////////////////////////

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
  cout << m_inputFilename << ":" << g_lineNumber << ":" << m_lineOffs << ": " << type << " - " << msg << endl;
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
