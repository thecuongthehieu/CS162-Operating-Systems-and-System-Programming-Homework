#include "tests/lib.h"
#include "sbrk-oom.h"

int
main (int argc UNUSED, char *argv[] UNUSED)
{
  test_name = "sbrk-oom-1";
  msg ("begin");
  test_sbrk_oom(25, 7);
  msg ("end");
  return 0;
}
