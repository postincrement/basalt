#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>

#define   STDOUT_FD     0

static int g_tabLen       = 14;
static int g_outputColumn = 0;

int basalt_init()
{
  return 0;
}

int basalt_print_string(const char * str)
{
  int len = strlen(str);
  write(STDOUT_FD, str, len);
  g_outputColumn += len;
  return 0;
}

int basalt_print_integer(uint16_t value)
{
  char buffer[10];
  int len = sprintf(buffer, "%i", value);
  write(STDOUT_FD, buffer, len);
  g_outputColumn += len;
  return 0;
}

int basalt_print_single(float value)
{
  char buffer[20];
  int len = sprintf(buffer, "%f", value);
  write(STDOUT_FD, buffer, len);
  g_outputColumn += len;
  return 0;
}

int basalt_print_double(double value)
{
  char buffer[20];
  int len = sprintf(buffer, "%lf", value);
  write(STDOUT_FD, buffer, len);
  g_outputColumn += len;
  return 0;
}

int basalt_print_tab()
{  
  int spaces = g_tabLen - (g_outputColumn % g_tabLen);
  int i;
  for (i = 0; i < spaces; ++i)
    write(STDOUT_FD, " ", 1);
  g_outputColumn += spaces;
  return 0;
}

int basalt_print_eol()
{
  write(STDOUT_FD, "\n", 1);
  g_outputColumn = 0;
  return 0;
}


