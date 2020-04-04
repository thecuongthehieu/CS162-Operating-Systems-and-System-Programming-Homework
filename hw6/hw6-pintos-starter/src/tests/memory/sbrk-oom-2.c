#include "tests/lib.h"
#include "sbrk-oom.h"

int
main (int argc UNUSED, char *argv[] UNUSED)
{
  test_name = "sbrk-oom-2";
  msg ("begin");
  test_sbrk_oom(1000, 35069);
  msg ("end");
  return 0;
}
