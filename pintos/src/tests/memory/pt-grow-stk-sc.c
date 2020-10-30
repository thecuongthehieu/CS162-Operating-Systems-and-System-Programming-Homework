/* This test checks that the stack is properly extended even if
   the first access to a stack location occurs inside a system
   call.

   From Godmar Back. Simplified for Homework 5. */

#include <string.h>
#include <syscall.h>
#include "tests/vm/sample.inc"
#include "tests/lib.h"
#include "tests/main.h"

void
test_main (void)
{
  int handle;
  int slen = strlen (sample);
  char buf2[65536];

  /* Read back via read(). */
  CHECK ((handle = open ("sample.txt")) > 1, "open \"sample.txt\"");
  CHECK (read (handle, buf2 + 32768, slen) == slen, "read \"sample.txt\"");

  CHECK (!memcmp (sample, buf2 + 32768, slen), "compare read data against reference");
  close (handle);
}

int
main (int argc UNUSED, char *argv[] UNUSED)
{
  test_name = "pt-grow-stk-sc";
  msg ("begin");
  test_main();
  msg ("end");
  return 0;
}
