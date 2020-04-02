/* Tests multiple pages of sbrk'd data. */

#include <string.h>
#include "tests/lib.h"
#include "tests/main.h"

#define MEBI (1 << 20)

void
test_main (void)
{
  unsigned char* heap = sbrk(MEBI);
  memset(heap, 162, MEBI);
  for (int i = 0; i != MEBI; i++) {
      ASSERT(heap[i] == 162);
  }
}

int
main (int argc UNUSED, char *argv[] UNUSED)
{
  test_name = "sbrk-mebi";
  msg ("begin");
  test_main();
  msg ("end");
  return 0;
}
