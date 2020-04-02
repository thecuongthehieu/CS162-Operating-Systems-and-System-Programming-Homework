/* Test realloc. */

#include <stdlib.h>
#include <string.h>
#include "tests/lib.h"
#include "tests/main.h"

#define NUM_INTS 3787

static void
check_zero(int* buffer, int len)
{
  for (int i = 0; i != len; i++) {
    ASSERT(buffer[i] == 0);
  }
}

static void
fill(int* buffer, int len)
{
  for (int i = 0; i != len; i++) {
    buffer[i] = 0x05158E57;
  }
}

static void
check_filled(int* buffer, int len) {
  for (int i = 0; i != len; i++) {
    ASSERT(buffer[i] == 0x05158E57);
  }
}

static void*
test_alloc(int num_ints)
{
  int* ptr = calloc(num_ints, sizeof(int));
  check_zero(ptr, num_ints);
  fill(ptr, num_ints);
  return ptr;
}

void
test_main (void)
{
  int* p = test_alloc(2 * NUM_INTS);
  int* q = realloc(p, 3 * NUM_INTS * sizeof(int));
  ASSERT(q != NULL);
  check_filled(q, 2 * NUM_INTS);
  free(q);
}

int
main (int argc UNUSED, char *argv[] UNUSED)
{
  test_name = "realloc-1";
  msg ("begin");
  test_main();
  msg ("end");
  return 0;
}
