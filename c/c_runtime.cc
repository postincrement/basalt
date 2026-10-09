#include "c_codegen.h"

#include <ostream>

void C_CodeGenerator::OutputRuntimeDecls(std::ostream & strm)
{
  strm << "\n"
       << "int basalt_init();\n\n"
       ;
      
  if (m_funcsUsed.count("print_tab")) 
    strm << "int print_tab();\n";

  if (m_funcsUsed.count("print_newline")) 
    strm << "int print_newline();\n";

  if (m_funcsUsed.count("print_string")) 
    strm << "int print_string(const char *);\n";

  if (m_funcsUsed.count("print_int16")) 
   strm << "int print_int16(int16_t);\n";

  if (m_funcsUsed.count("print_int32")) 
   strm << "int print_int32(int32_t);\n";

  if (m_funcsUsed.count("print_single")) 
   strm << "int print_single(float);\n";

  if (m_funcsUsed.count("print_double")) 
   strm << "int print_double(double);\n";

  if (m_funcsUsed.count("input")) {
    strm << "double basalt_read_number(void);\n";
    strm << "void basalt_input_string(char **);\n";
    strm << "void basalt_line_input(char **);\n";
  }
  if (m_funcsUsed.count("rnd"))
    strm << "float basalt_rnd(float);\n";

#if 0
  if (m_funcsUsed.count("print_int32")) 
    strm << "int print_int32(int32_t);\n";

  if (m_funcsUsed.count("print_double")) 
   strm << "int print_double(double);\n";

  if (m_funcsUsed.count("strlen")) 
   strm << "int strlen(const char *);\n";

  if (m_funcsUsed.count("strdup")) 
   strm << "char * strdup(const char *);\n";

  if (m_funcsUsed.count("str_single")) 
    strm << "char * str_single(float);\n";

  if (m_funcsUsed.count("str_int16")) 
    strm << "char * str_int16(int16_t);\n";

  if (m_funcsUsed.count("str_int32")) 
    strm << "char * str_int32(int32_t);\n";
#endif

  strm << "\n";  
}

static const char * g_printTab = "\
int print_tab()\n\
{\n\
  int spaces = g_tabLen - (g_outputColumn % g_tabLen);\n\
  int i;\n\
  for (i = 0; i < spaces; ++i)\n\
    write(STDOUT_FILENO, \" \", 1);\n\
  g_outputColumn += spaces;\n\
  return 0;\n\
}\n\n\
";

static const char * g_printNewLine = "\
int print_newline()\n\
{\n\
  write(STDOUT_FILENO, \"\\r\\n\", 2);\n\
  g_outputColumn = 0;\n\
  return 0;\n\
}\n\n\
";

static const char * g_printString = "\
int print_string(const char * str)\n\
{\n\
  if (str == NULL)\n\
    return 0;\n\
  int len = strlen(str);\n\
  write(STDOUT_FILENO, str, len);\n\
  g_outputColumn += len;\n\
  return 0;\n\
}\n\n\
";

static const char * g_printSingle = "\
int print_single(float value)\n\
{\n\
  char buffer[20];\n\
  int len = sprintf(buffer, \"% .7g \", value);\n\
  write(STDOUT_FILENO, buffer, len);\n\
  g_outputColumn += len;\n\
  return 0;\n\
}\n\n\
";

static const char * g_printDouble = "\
int print_double(double value)\n\
{\n\
  char buffer[40];\n\
  int len = sprintf(buffer, \"% .13lg \", value);\n\
  write(STDOUT_FILENO, buffer, len);\n\
  g_outputColumn += len;\n\
  return 0;\n\
}\n\
";

static const char * g_printInt16 = "\
int print_int16(int16_t value)\n\
{\n\
  char buffer[20];\n\
  int len = sprintf(buffer, \"% d \", value);\n\
  write(STDOUT_FILENO, buffer, len);\n\
  g_outputColumn += len;\n\
  return 0;\n\
}\n\n\
";

void C_CodeGenerator::OutputRuntime(std::ostream & strm)
{
  strm << "\n/* run time functions */\n\n";

  strm << "int basalt_init()\n"
       << "{\n"
       << "  return 0;\n"
       << "}\n"
       << "\n"
       ;

  if (m_funcsUsed.count("print_tab")) 
    strm << g_printTab;

  if (m_funcsUsed.count("print_newline")) 
    strm << g_printNewLine;

  if (m_funcsUsed.count("print_string")) 
    strm << g_printString;

  if (m_funcsUsed.count("print_int16")) 
    strm << g_printInt16;

  if (m_funcsUsed.count("print_int32")) 
    strm << "int print_int32(int32_t value)\n"
            "{\n"
            "  char buffer[20];\n"
            "  int len = sprintf(buffer, \"% d \", value);\n"
            "  write(STDOUT_FILENO, buffer, len);\n"
            "  g_outputColumn += len;\n"
            "  return 0;\n"
            "}\n\n";

  if (m_funcsUsed.count("print_single")) 
    strm << g_printSingle;

  if (m_funcsUsed.count("print_double")) 
    strm << g_printDouble;

  if (m_funcsUsed.count("input")) {
    strm <<
      "static char g_inLine[512];\n"
      "static char * g_inPtr;\n"
      "static void basalt_pull_line(void)\n"
      "{\n"
      "  if (g_inPtr && *g_inPtr && *g_inPtr != '\\n' && *g_inPtr != '\\r')\n"
      "    return;\n"
      "  if (!fgets(g_inLine, (int)sizeof g_inLine, stdin)) {\n"
      "    g_inLine[0] = 0;\n"
      "    g_inPtr = g_inLine;\n"
      "    return;\n"
      "  }\n"
      "  g_inPtr = g_inLine;\n"
      "}\n"
      "double basalt_read_number(void)\n"
      "{\n"
      "  char * end = 0;\n"
      "  basalt_pull_line();\n"
      "  double value = strtod(g_inPtr, &end);\n"
      "  if (end == g_inPtr) {\n"
      "    value = 0;\n"
      "    while (*g_inPtr && *g_inPtr != ',' && *g_inPtr != '\\n' && *g_inPtr != '\\r')\n"
      "      g_inPtr++;\n"
      "  } else\n"
      "    g_inPtr = end;\n"
      "  while (*g_inPtr == ' ' || *g_inPtr == '\\t')\n"
      "    g_inPtr++;\n"
      "  if (*g_inPtr == ',')\n"
      "    g_inPtr++;\n"
      "  return value;\n"
      "}\n"
      "static void basalt_store_string(char ** out, const char * start, int length)\n"
      "{\n"
      "  char * copy = (char *)malloc((size_t)length + 1);\n"
      "  if (length > 0)\n"
      "    memcpy(copy, start, (size_t)length);\n"
      "  copy[length] = 0;\n"
      "  if (*out)\n"
      "    free(*out);\n"
      "  *out = copy;\n"
      "}\n"
      "void basalt_input_string(char ** out)\n"
      "{\n"
      "  basalt_pull_line();\n"
      "  char * start = g_inPtr;\n"
      "  int length = 0;\n"
      "  while (start[length] && start[length] != ',' && start[length] != '\\n' && start[length] != '\\r')\n"
      "    length++;\n"
      "  basalt_store_string(out, start, length);\n"
      "  g_inPtr = start + length;\n"
      "  while (*g_inPtr == ' ' || *g_inPtr == '\\t')\n"
      "    g_inPtr++;\n"
      "  if (*g_inPtr == ',')\n"
      "    g_inPtr++;\n"
      "}\n"
      "void basalt_line_input(char ** out)\n"
      "{\n"
      "  basalt_pull_line();\n"
      "  char * start = g_inPtr;\n"
      "  int length = 0;\n"
      "  while (start[length] && start[length] != '\\n' && start[length] != '\\r')\n"
      "    length++;\n"
      "  basalt_store_string(out, start, length);\n"
      "  g_inPtr = start + length;\n"
      "  if (*g_inPtr == '\\r') g_inPtr++;\n"
      "  if (*g_inPtr == '\\n') g_inPtr++;\n"
      "}\n";
  }

  if (m_funcsUsed.count("rnd")) {
    strm <<
      "static unsigned long g_rndSeed = 327680ul;\n"
      "float basalt_rnd(float x)\n"
      "{\n"
      "  if (x < 0)\n"
      "    g_rndSeed = (unsigned long)(-x * 1000.0f) | 1ul;\n"
      "  if (x != 0.0f)\n"
      "    g_rndSeed = g_rndSeed * 214013ul + 2531011ul;\n"
      "  return (float)((g_rndSeed >> 16) & 32767ul) / 32768.0f;\n"
      "}\n";
  }
}

#if 0
int basalt_print_string(const char * str)
{
  if (str == NULL)
    return 0;
  int len = strlen(str);
  write(STDOUT_FILENO, str, len);
  g_outputColumn += len;
  return 0;
}

int basalt_print_int16(int16_t value)
{
  char buffer[20];
  int len = sprintf(buffer, "% d ", value);
  write(STDOUT_FILENO, buffer, len);
  g_outputColumn += len;
  return 0;
}

int basalt_print_int32(int32_t value)
{
  char buffer[20];
  int len = sprintf(buffer, "% d ", value);
  write(STDOUT_FILENO, buffer, len);
  g_outputColumn += len;
  return 0;
}

int basalt_print_single(float value)
{
  char buffer[20];
  int len = sprintf(buffer, "% .7g ", value);
  write(STDOUT_FILENO, buffer, len);
  g_outputColumn += len;
  return 0;
}

int basalt_print_double(double value)
{
  char buffer[20];
  int len = sprintf(buffer, "% lf ", value);
  write(STDOUT_FILENO, buffer, len);
  g_outputColumn += len;
  return 0;
}

int basalt_print_newline()
{
  write(STDOUT_FILENO, "\r\n", 2);
  g_outputColumn = 0;
  return 0;
}

int basalt_strlen(const char * str)
{
  if (str == NULL)
    return 0;
  return strlen(str);
}

char * basalt_strdup(const char * str)
{
  if (str == NULL)
    return NULL;
  return strdup(str);
}

char * basalt_str_single(float value)
{
  char buffer[20];
  int len = sprintf(buffer, "% .10lg ", value);
  return strdup(buffer);
}

char * basalt_str_double(double value)
{
  char buffer[20];
  int len = sprintf(buffer, "% .10lg ", value);
  return strdup(buffer);
}

char * basalt_str_int16(int16_t value)
{
  char buffer[20];
  int len = sprintf(buffer, "% d ", value);
  return strdup(buffer);
}

char * basalt_str_int32(int32_t value)
{
  char buffer[20];
  int len = sprintf(buffer, "% d ", value);
  return strdup(buffer);
}

#endif