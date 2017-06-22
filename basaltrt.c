#include <unistd.h>
#include <string.h>

#define   STDOUT_FD     0


int basalt_init()
{
  printf("Basalt init\n");
  return 0;
}

int basalt_puts(const char * str)
{
  write(STDOUT_FD, str, strlen(str));
  return 0;
}