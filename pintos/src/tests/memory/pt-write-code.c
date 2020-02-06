/* Try to write to the code segment.
   The process must be terminated with -1 exit code. */

#include "tests/lib.h"
#include "tests/main.h"

void
test_main (void)
{
  *(int *) test_main = 0;
  fail ("writing the code segment succeeded");
}

int
main (int argc UNUSED, char *argv[] UNUSED)
{
  test_name = "pt-write-code";
  msg ("begin");
  test_main();
  msg ("end");
  return 0;
}
