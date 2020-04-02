/* Tests that sbrk'd memory is zeroed out. */

#include <string.h>
#include "tests/lib.h"
#include "tests/main.h"

#define LARGE_AMOUNT 10000

void
test_main (void)
{
  unsigned char* heap = sbrk(LARGE_AMOUNT);
  for (int i = 0; i != LARGE_AMOUNT; i++) {
      ASSERT(heap[i] == 0);
  }
}

int
main (int argc UNUSED, char *argv[] UNUSED)
{
  test_name = "sbrk-zero";
  msg ("begin");
  test_main();
  msg ("end");
  return 0;
}
