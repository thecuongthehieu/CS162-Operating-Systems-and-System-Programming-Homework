#include "tests/lib.h"
#include "sbrk-simple.h"

int
main (int argc UNUSED, char *argv[] UNUSED)
{
  test_name = "sbrk-none";
  msg ("begin");
  test_sbrk(0);
  msg ("end");
  return 0;
}
