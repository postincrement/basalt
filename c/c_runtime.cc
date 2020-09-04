#include "c_codegen.h"

#include <ostream>

void C_CodeGenerator::OutputRuntimeDecls(std::ostream & strm)
{
  if (m_funcsUsed.count("print_tab")) 
    strm << "extern int print_tab();\n";

  if (m_funcsUsed.count("print_newline")) 
    strm << "extern int print_newline();\n";

  if (m_funcsUsed.count("print_string")) 
    strm << "extern int print_string(const char *);\n";

  if (m_funcsUsed.count("print_int16")) 
   strm << "extern int print_int16(int16_t);\n";

  if (m_funcsUsed.count("print_int32")) 
    strm << "extern int print_int32(int32_t);\n";

  if (m_funcsUsed.count("print_single")) 
   strm << "extern int print_single(float);\n";

  if (m_funcsUsed.count("print_double")) 
   strm << "extern int print_double(double);\n";

  if (m_funcsUsed.count("strlen")) 
   strm << "extern int strlen(const char *);\n";

  if (m_funcsUsed.count("strdup")) 
   strm << "extern char * strdup(const char *);\n";

  if (m_funcsUsed.count("str_single")) 
    strm << "extern char * str_single(float);\n";

  if (m_funcsUsed.count("str_int16")) 
    strm << "extern char * str_int16(int16_t);\n";

  if (m_funcsUsed.count("str_int32")) 
    strm << "extern char * str_int32(int32_t);\n";

  strm << "\n";  
}

static const char * g_printTab = "\n\
int print_tab()\n\
{\n\
  int spaces = g_tabLen - (g_outputColumn % g_tabLen);\n\
  int i;\n\
  for (i = 0; i < spaces; ++i)\n\
    write(STDOUT_FILENO, \" \", 1);\n\
  g_outputColumn += spaces;\n\
  return 0;\n\
}\n\
";

static const char * g_printNewLine = "\n\
int print_newline()\n\
{\n\
  write(STDOUT_FILENO, \"\\r\\n\", 2);\n\
  g_outputColumn = 0;\n\
  return 0;\n\
}\n\
";

static const char * g_printString = "\n\
int print_string(const char * str)\n\
{\n\
  if (str == NULL)\n\
    return 0;\n\
  int len = strlen(str);\n\
  write(STDOUT_FILENO, str, len);\n\
  g_outputColumn += len;\n\
  return 0;\n\
}\n\
";

static const char * g_printSingle = "\n\
int print_single(float value)\n\
{\n\
  char buffer[20];\n\
  int len = sprintf(buffer, \"% .7g \", value);\n\
  write(STDOUT_FILENO, buffer, len);\n\
  g_outputColumn += len;\n\
  return 0;\n\
}\n\
";

static const char * g_printInt16 = "\n\
int print_int16(int16_t value)\n\
{\n\
  char buffer[20];\n\
  int len = sprintf(buffer, \"% d \", value);\n\
  write(STDOUT_FILENO, buffer, len);\n\
  g_outputColumn += len;\n\
  return 0;\n\
}\n\
";

void C_CodeGenerator::OutputRuntime(std::ostream & strm)
{
  strm << "\n"
       << "int basalt_init()\n"
       << "{}\n"
       << "\n"
       ;
  if (
      m_funcsUsed.count("print_tab") 
      || m_funcsUsed.count("print_newline") 
      || m_funcsUsed.count("print_string") 
      || m_funcsUsed.count("print_int16") 
      || m_funcsUsed.count("print_single") 
     ) {
    strm << "static int g_tabLen       = 14;\n"
        << "static int g_outputColumn = 0;\n\n"
        ;
  } 

  if (m_funcsUsed.count("print_tab")) 
    strm << g_printTab;

  if (m_funcsUsed.count("print_newline")) 
    strm << g_printNewLine;

  if (m_funcsUsed.count("print_string")) 
    strm << g_printString;

  if (m_funcsUsed.count("print_int16")) 
    strm << g_printInt16;

  if (m_funcsUsed.count("print_single")) 
    strm << g_printSingle;
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
  int len = sprintf(buffer, "% lg ", value);
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