/* Tests the practice syscall */

#include "tests/lib.h"
#include "tests/main.h"
#include <stdio.h>

void
test_main (void)
{
  int i = practice (5);
  if (i != 6)
    {
      fail("Practice syscall failed because i was not 6 as expected\n");
    }
  i = practice (10);
  if (i != 11)
    {
      fail("Practice syscall failed because i was not 11 as expected\n");
    }
  printf ("practice\n");
}
