/* Try to read and write sbrk'd memory. */
#include "sbrk-simple.h"
#include <stdlib.h>
#include <string.h>
#include "tests/lib.h"

char global;

void
test_sbrk (int amount)
{
  char* bounce = sbrk(amount);
  ASSERT(bounce > &global);
  strlcpy(bounce, "Fall 2019 is the best semester to take CS 162", amount);
  msg("%s", bounce);
}
