#include <unistd.h>
#include <string.h>

#define   STDOUT_FD     0


static int g_tabLen       = 14;
static int g_outputColumn = 0;

int basalt_init()
{
  return 0;
}

int basalt_puts_string(const char * str)
{
  int len = strlen(str);
  write(STDOUT_FD, str, len);
  g_outputColumn += len;
  return 0;
}

int basalt_puts_tab()
{  
  int spaces = g_tabLen - (g_outputColumn % g_tabLen);
  int i;
  for (i = 0; i < spaces; ++i)
    write(STDOUT_FD, " ", 1);
  g_outputColumn += spaces;
  return 0;
}

int basalt_puts_eol()
{
  write(STDOUT_FD, "\n", 1);
  g_outputColumn = 0;
  return 0;
}


