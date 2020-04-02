/* Tests multiple pages of sbrk'd data. */

#include <stdint.h>
#include <string.h>
#include "tests/lib.h"
#include "tests/main.h"

#define GIBI (1 << 30)
#define MEBI (1 << 20)

void
test_main (void)
{
  unsigned char* heap = sbrk(INTPTR_MAX);
  ASSERT(heap == (void*) -1);

  heap = sbrk(GIBI);
  ASSERT(heap == (void*) -1);

  /* Allocating a mebibyte should still succeed. */
  heap = sbrk(MEBI);
  memset(heap, 162, MEBI);
  for (int i = 0; i != MEBI; i++) {
      ASSERT(heap[i] == 162);
  }
}

int
main (int argc UNUSED, char *argv[] UNUSED)
{
  test_name = "sbrk-fail-1";
  msg ("begin");
  test_main();
  msg ("end");
  return 0;
}
