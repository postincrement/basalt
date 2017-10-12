#include <iostream>
#include <sstream>
using namespace std;

#include <unistd.h>

#include "basalt.h"

//#include "codegen.h"

int g_lineNumber   = 1;
int g_errorCount   = 0;
int g_warningCount = 0;

LanguageProfile          g_profile;
AST::SourceFileExprList  g_expressions;

Basalt g_application;

int g_verbose = 0;
std::string g_codeGenerator;
std::string g_outputFilename;
bool g_dumpAsm = false;
bool g_compileOnly = false;

ArgDef g_argDefs[] = {
  { 'c',   "",          "b",  &g_compileOnly,     "compile only" },
  { 'd',   "dump",      "b",  &g_dumpAsm,         "dump assembly" },
  { 'v',   "verbose",   "",   &g_verbose,         "enable verbosity" },
  { 't',   "target",    "s",  &g_codeGenerator,   "set code generator" },
  { 'o',   "output",    "s",  &g_outputFilename,  "set output filename" },
  { ' ',   "yydebug",   "",   &MBASIC_debug,      "enable bison debugging"},
  {  0,    NULL,        NULL, NULL,                NULL }
};

template <class Abstract, typename ... TArgs>
class Factory
{
  public:
    Factory()
    { }

    struct AbstractWorker
    {
      virtual Abstract * Create(TArgs ... args) = 0; 
    };

    template <class Concrete>
    struct Worker : public AbstractWorker
    {
      Worker()
      { }

      virtual Abstract * Create(TArgs ... args) override
      { return new Concrete(args...); }
    };

    template<class Concrete>
    void Register(const std::string & key)
    {
      m_workers[key] = new Worker<Concrete>();
    }

    std::vector<std::string> GetList() const
    {
      std::vector<std::string> types;
      for (auto & r : m_workers)
        types.push_back(r.first);

      return types;
    }

    Abstract * CreateInstance(const std::string & key, TArgs ... args)
    {
      typename WorkerListType::iterator r = m_workers.find(key);
      if (r == m_workers.end())
        return nullptr;
      AbstractWorker * worker = r->second;  
      return worker->Create(args ...);  
    }

    bool Contains(const std::string & key) const
    {
      return m_workers.count(key) != 0;
    }

    typedef std::map<std::string, AbstractWorker *> WorkerListType;    
    WorkerListType m_workers;
};

static Factory<AST::Visitor, AST::SourceFileExprList &> g_codegeneratorFactory;

void Basalt::Usage(const ArgDef * defs, bool showKeys)
{
  if (showKeys) {
    std::vector<std::string> keys = g_codegeneratorFactory.GetList();
    for (auto & r : keys)
      cout << "  " << r << "\n";
  }
  exit(1);
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


int Basalt::Main(int argc, char const *argv[])
{
  m_interactive = false;

  // register code generators
  g_codegeneratorFactory.Register<CodegenDumper>("dump");
  g_codegeneratorFactory.Register<CodegenCXX>   ("cxx");
  g_codegeneratorFactory.Register<CodegenLLVM>  ("llvm");
  
  // parse options and arguments
  int index = ParseArguments(g_argDefs, argc, argv);

  // set default code generator if no output file specified 
  if (g_codeGenerator.empty() && g_outputFilename.empty())
    g_codeGenerator = "dump";

  // see if input filename exists  
  if (index >= argc) {
    cerr << "error: no input filename specified" << endl;
    exit(1);
  }
  
  // see if the code generator exists
  if (!g_codegeneratorFactory.Contains(g_codeGenerator)) {
    cerr << "error: code generator " << g_codeGenerator << "not known. Use one of:\n";
    Usage(g_argDefs, true);
    exit(1);
  }

  // open input file
  g_inputFilename = Filename(argv[index]);
  std::string ext = g_inputFilename.GetExtension();
  for (auto & r : ext) {
    r = tolower(r);
  }
  if (ext != ".bas") {
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

  AST::Visitor * generator = g_codegeneratorFactory.CreateInstance(g_codeGenerator, g_expressions);
  if (generator == nullptr) {
    cerr << "internal error: cannot instantiate generator with name '" << g_codeGenerator << "'" << endl;
    return -1;
  }

  if (!generator->Open(g_inputFilename, argc, argv))
    return -1;
  
  if (!g_expressions.Generate(*generator)) {
    return -1;
  }

  generator->Close();

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
