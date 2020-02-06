/*
 * Checks that the stack is aligned properly in userspace.
 * This program is used for all of the stack-align-* tests.
 */

#include "tests/lib.h"
#include "tests/main.h"
#include <stdint.h>

void NO_INLINE check_pointer_alignment (void *candidate);
void NO_INLINE check_stack_alignment (void);

void NO_INLINE
check_pointer_alignment (void *candidate)
{
  uintptr_t address = (uintptr_t) candidate;
  uintptr_t address_last_nybble = address & 0xf;
  msg ("last nybble of candidate is 0x%x", address_last_nybble);
  if (address_last_nybble != 0x0)
    {
      fail ("candidate is not aligned to a 16-byte boundary");
    }
}

void NO_INLINE
check_stack_alignment (void)
{
  /*
   * __attribute__ ((aligned(16))) tells the compiler to align buffer, which is
   * allocated on the stack, to a 16-byte boundary.
   */
  char buffer[32] __attribute__ ((aligned(16)));
  check_pointer_alignment (&buffer[0]);
}

int
main (int argc UNUSED, char *argv[] UNUSED)
{
  test_name = "stack-align";
  msg("begin");
  check_stack_alignment();
  msg("end");
  return 0;
}
