#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>

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
  char buffer[20];
  int len = sprintf(buffer, "% lg ", value);
  write(STDOUT_FILENO, buffer, len);
  g_outputColumn += len;
  return 0;
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
