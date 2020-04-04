/* Test that free'd memory is reused. */

#include <stdlib.h>
#include <string.h>
#include "tests/lib.h"
#include "tests/main.h"

#define NUM_INTS (1 << 12)

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

void
test_main (void)
{
  int* buffer = calloc(NUM_INTS, sizeof(int));
  check_zero(buffer, NUM_INTS);
  fill(buffer, NUM_INTS);
  free(buffer);

  int* new_buffer = calloc(NUM_INTS, sizeof(int));
  ASSERT(buffer == new_buffer);
  check_zero(new_buffer, NUM_INTS);
}

int
main (int argc UNUSED, char *argv[] UNUSED)
{
  test_name = "malloc-free";
  msg ("begin");
  test_main();
  msg ("end");
  return 0;
}
