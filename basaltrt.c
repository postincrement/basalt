#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

static int g_tabLen       = 14;
static int g_outputColumn = 0;

int basalt_init()
{
  return 0;
}

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
  char buffer[64];
  int len = sprintf(buffer, "% .13lg ", value);
  write(STDOUT_FILENO, buffer, len);
  g_outputColumn += len;
  return 0;
}

void basalt_set_width(int width)
{
  if (width > 0)
    g_tabLen = width;
}

int basalt_print_tab()
{  
  int spaces = g_tabLen - (g_outputColumn % g_tabLen);
  int i;
  for (i = 0; i < spaces; ++i)
    write(STDOUT_FILENO, " ", 1);
  g_outputColumn += spaces;
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

void basalt_set_string(char ** out, const char * src)
{
  char * copy = src ? strdup(src) : NULL;
  if (out == NULL)
    return;
  if (*out)
    free(*out);
  *out = copy;
}

void basalt_clear_string(char ** out)
{
  if (out == NULL)
    return;
  if (*out)
    free(*out);
  *out = NULL;
}

static char g_inLine[512];
static char * g_inPtr;

static void basalt_pull_line(void)
{
  if (g_inPtr && *g_inPtr && *g_inPtr != '\n' && *g_inPtr != '\r')
    return;
  if (!fgets(g_inLine, (int)sizeof g_inLine, stdin)) {
    g_inLine[0] = 0;
    g_inPtr = g_inLine;
    return;
  }
  g_inPtr = g_inLine;
}

double basalt_read_number(void)
{
  char * end = 0;
  basalt_pull_line();
  double value = strtod(g_inPtr, &end);
  if (end == g_inPtr) {
    value = 0;
    while (*g_inPtr && *g_inPtr != ',' && *g_inPtr != '\n' && *g_inPtr != '\r')
      g_inPtr++;
  } else
    g_inPtr = end;
  while (*g_inPtr == ' ' || *g_inPtr == '\t')
    g_inPtr++;
  if (*g_inPtr == ',')
    g_inPtr++;
  return value;
}

static void basalt_store_input(char ** out, const char * start, int length)
{
  char * copy = (char *)malloc((size_t)length + 1);
  if (length > 0)
    memcpy(copy, start, (size_t)length);
  copy[length] = 0;
  if (*out)
    free(*out);
  *out = copy;
}

void basalt_input_string(char ** out)
{
  basalt_pull_line();
  const char * start = g_inPtr;
  int length = 0;
  while (start[length] && start[length] != ',' && start[length] != '\n' && start[length] != '\r')
    length++;
  basalt_store_input(out, start, length);
  g_inPtr = (char *)start + length;
  while (*g_inPtr == ' ' || *g_inPtr == '\t')
    g_inPtr++;
  if (*g_inPtr == ',')
    g_inPtr++;
}

void basalt_line_input(char ** out)
{
  basalt_pull_line();
  const char * start = g_inPtr;
  int length = 0;
  while (start[length] && start[length] != '\n' && start[length] != '\r')
    length++;
  basalt_store_input(out, start, length);
  g_inPtr = (char *)start + length;
  if (*g_inPtr == '\r') g_inPtr++;
  if (*g_inPtr == '\n') g_inPtr++;
}

static unsigned long g_rndSeed = 327680ul;

float basalt_rnd(float x)
{
  if (x < 0)
    g_rndSeed = (unsigned long)(-x * 1000.0f) | 1ul;
  if (x != 0.0f)
    g_rndSeed = g_rndSeed * 214013ul + 2531011ul;
  return (float)((g_rndSeed >> 16) & 32767ul) / 32768.0f;
}
