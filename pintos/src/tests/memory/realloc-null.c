/* Edge-case tests for realloc. */

#include <stdlib.h>
#include <string.h>
#include "tests/lib.h"
#include "tests/main.h"

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
  int* p = realloc(NULL, 8191 * sizeof(int));
  fill(p, 8191);

  int* q = realloc(NULL, 0);
  free(q);

  int* r = realloc(p, 0);
  free(r);
}

int
main (int argc UNUSED, char *argv[] UNUSED)
{
  test_name = "realloc-null";
  msg ("begin");
  test_main();
  msg ("end");
  return 0;
}
