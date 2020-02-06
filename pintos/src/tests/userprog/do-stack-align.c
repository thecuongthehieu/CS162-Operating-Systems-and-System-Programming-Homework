/* Does absolutely nothing. */

#include "tests/lib.h"

int
main (int argc UNUSED, char *argv[] UNUSED)
{
  register unsigned int esp asm("esp");
  return esp % 16;
}
