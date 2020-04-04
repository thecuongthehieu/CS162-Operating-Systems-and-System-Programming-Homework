#include "tests/lib.h"
#include "sbrk-simple.h"

int
main (int argc UNUSED, char *argv[] UNUSED)
{
  test_name = "sbrk-multi";
  msg ("begin");
  test_sbrk(10000);
  msg ("end");
  return 0;
}
