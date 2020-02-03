/* Edge-case tests for malloc. */

#include <stdlib.h>
#include <string.h>
#include "tests/lib.h"
#include "tests/main.h"

void
test_main (void)
{
  char* p = malloc(0);
  free(p);
  free(NULL);
}

int
main (int argc UNUSED, char *argv[] UNUSED)
{
  test_name = "malloc-null";
  msg ("begin");
  test_main();
  msg ("end");
  return 0;
}
