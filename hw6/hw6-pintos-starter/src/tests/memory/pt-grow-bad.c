/* Read from an address 4,096 bytes below the stack pointer.
   The process must be terminated with -1 exit code. */

#include <string.h>
#include "tests/arc4.h"
#include "tests/cksum.h"
#include "tests/lib.h"
#include "tests/main.h"

void
test_main (void)
{
  asm volatile ("movl -4096(%esp), %eax");
}

int
main (int argc UNUSED, char *argv[] UNUSED)
{
  test_name = "pt-grow-bad";
  msg ("begin");
  test_main();
  msg ("end");
  return 0;
}
