/* Tests deallocation using sbrk. */

#include <string.h>
#include "tests/lib.h"
#include "tests/main.h"

#define PAGE_MULTIPLE (4096 * 3)
#define EXTRA 712

void
test_main (void)
{
  unsigned char* heap = sbrk(PAGE_MULTIPLE + EXTRA);
  msg("Allocated %d bytes", PAGE_MULTIPLE + EXTRA);
  for (int i = 0; i != PAGE_MULTIPLE + EXTRA; i++) {
      ASSERT(heap[i] == 0);
  }
  msg("Accessed %d bytes", PAGE_MULTIPLE + EXTRA);
  unsigned char* rv = sbrk(-EXTRA);
  ASSERT(PAGE_MULTIPLE + EXTRA == (int) (rv - heap));
  msg("Deallocated %d bytes", EXTRA);
  for (int i = 0; i != PAGE_MULTIPLE; i++) {
      ASSERT(heap[i] == 0);
  }
  msg("Accessed %d bytes", PAGE_MULTIPLE);
  msg("Accessing extra byte (should fail): %d", (int) heap[PAGE_MULTIPLE]);
}

int
main (int argc UNUSED, char *argv[] UNUSED)
{
  test_name = "sbrk-dealloc";
  msg ("begin");
  test_main();
  msg ("end");
  return 0;
}
