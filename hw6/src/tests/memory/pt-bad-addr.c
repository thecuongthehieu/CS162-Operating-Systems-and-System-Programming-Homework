/* Accesses a bad address.
   The process must be terminated with -1 exit code. */

#include "tests/lib.h"
#include "tests/main.h"

void
test_main (void)
{
  fail ("bad addr read as %d", *(int *) 0x04000000);
}

int
main (int argc UNUSED, char *argv[] UNUSED)
{
  test_name = "pt-bad-addr";
  msg ("begin");
  test_main();
  msg ("end");
  return 0;
}
