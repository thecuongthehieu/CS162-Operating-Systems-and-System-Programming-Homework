/* Reads from a file into a bad address.
   The process must be terminated with -1 exit code. */

#include <syscall.h>
#include "tests/lib.h"
#include "tests/main.h"

void
test_main (void)
{
  int handle;

  CHECK ((handle = open ("sample.txt")) > 1, "open \"sample.txt\"");
  read (handle, (char *) &handle - 4096, 1);
  fail ("survived reading data into bad address");
}

int
main (int argc UNUSED, char *argv[] UNUSED)
{
  test_name = "pt-bad-read";
  msg ("begin");
  test_main();
  msg ("end");
  return 0;
}
