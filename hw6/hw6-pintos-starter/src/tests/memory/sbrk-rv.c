/* Tests that the return value of sbrk is correct. */

#include "tests/lib.h"
#include "tests/main.h"

#define LARGE_AMOUNT 10000

void
test_main (void)
{
  unsigned char* heap = sbrk(LARGE_AMOUNT);
  unsigned char* brk = sbrk(0);
  ASSERT(LARGE_AMOUNT == (int) (brk - heap));
}

int
main (int argc UNUSED, char *argv[] UNUSED)
{
  test_name = "sbrk-rv";
  msg ("begin");
  test_main();
  msg ("end");
  return 0;
}
