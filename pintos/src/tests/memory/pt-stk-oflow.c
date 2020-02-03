/* Check that stack overflow fails in the expected way. */

#include <string.h>
#include "tests/arc4.h"
#include "tests/cksum.h"
#include "tests/lib.h"
#include "tests/main.h"

static int
recurse (int x)
{
  int y = recurse (x + 1);
  msg ("recurse (%d) returned", x + 1);
  return x + y;
}

void
test_main (void)
{
  recurse (0);
  msg ("recurse (0) returned");
}

int
main (int argc UNUSED, char *argv[] UNUSED)
{
  test_name = "pt-stk-oflow";
  msg ("begin");
  test_main();
  msg ("end");
  return 0;
}
