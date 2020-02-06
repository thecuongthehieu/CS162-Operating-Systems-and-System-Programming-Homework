/* Simple test of malloc. */

#include <stdlib.h>
#include <string.h>
#include "tests/lib.h"
#include "tests/main.h"

void
test_main (void)
{
  char* buffer = malloc(100);
  strlcpy(buffer, "David Culler is the instructor for CS 162", 100);
  msg("%s", buffer);
}

int
main (int argc UNUSED, char *argv[] UNUSED)
{
  test_name = "malloc-simple";
  msg ("begin");
  test_main();
  msg ("end");
  return 0;
}
