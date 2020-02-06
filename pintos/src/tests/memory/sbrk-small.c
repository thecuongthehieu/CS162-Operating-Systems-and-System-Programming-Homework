#include "tests/lib.h"
#include "sbrk-simple.h"

int
main (int argc UNUSED, char *argv[] UNUSED)
{
  test_name = "sbrk-small";
  msg ("begin");
  test_sbrk(100);
  msg ("end");
  return 0;
}
