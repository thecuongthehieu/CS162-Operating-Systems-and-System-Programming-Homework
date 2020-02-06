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

  /* Check that nothing was actually mapped into the address space. */
  heap = sbrk(0);
  msg("Reading this byte should fail: %d", heap[162]);
}

int
main (int argc UNUSED, char *argv[] UNUSED)
{
  test_name = "sbrk-fail-2";
  msg ("begin");
  test_main();
  msg ("end");
  return 0;
}
