/* C generator by basalt */
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <string.h>

extern int basalt_init();
extern int basalt_print_tab();
extern int basalt_print_newline();
extern int basalt_print_string(const char *);
extern int basalt_print_int16(int16_t);
extern int basalt_print_int32(int32_t);
extern int basalt_print_single(float);
extern int basalt_print_double(double);
extern int basalt_strlen(const char *);
extern char * basalt_strdup(const char *);
extern char * basalt_str_single(float);
extern char * basalt_str_double(double);
extern char * basalt_str_int16(int16_t);
extern char * basalt_str_int32(int32_t);

/* Vars */
int16_t USER_b_int16 = 0; /* b */

/* declare structure used for GOTO, GOSUB etc */
struct BlockFunction {
  struct BlockFunction (* m_func)();
};

/* forward declare each block of code */
struct BlockFunction Block_1();
struct BlockFunction Block_for_0();

struct BlockFunction Block_1()
{
  struct BlockFunction nextBlock;

  /* 1 print "--START--" */
  basalt_print_string("--START--");
  basalt_print_newline();

  /* 20 for b = 0 to 5 */
  USER_b_int16 = 0;
  nextBlock.m_func = &Block_for_0;
  return nextBlock;
}

struct BlockFunction Block_for_0()
{
  struct BlockFunction nextBlock;
  nextBlock.m_func = 0;

  /* 30 rem  for c = 10 to 15 */

  /* 40     print b : rem c */
  basalt_print_int16(USER_b_int16);
  basalt_print_newline();

  /* 50 rem  next c */

  /* 60 next b */
  USER_b_int16 += 1;
  if (USER_b_int16 <= 5)
  {
    nextBlock.m_func = &Block_for_0;
    return nextBlock;
  }

  /* 80 print b , "finished" */
  basalt_print_int16(USER_b_int16);
  basalt_print_tab();
  basalt_print_string("finished");
  basalt_print_newline();

  /* 9999 system */
  return nextBlock;
}

int main(int argc, char * argv[])
{
  basalt_init();
  struct BlockFunction block;
  block.m_func = &Block_1;
  while (block.m_func != 0) {
    block = (*block.m_func)();
  }
  exit(0);
}
