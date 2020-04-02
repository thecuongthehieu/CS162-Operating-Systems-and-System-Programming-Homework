/* Tests out-of-memory conditions with sbrk. */

#include "sbrk-oom.h"
#include <stdlib.h>
#include <string.h>
#include "tests/lib.h"

static int
fill_memory(int step_size)
{
    int count = 0;
    while (sbrk(step_size) != (void*) -1) {
        count++;
    }
    return count;
}

static void
empty_memory(int count, int step_size)
{
  int offset = step_size >> 1;
  unsigned char* brk = sbrk(-offset);
  ASSERT(brk != (void*) -1);
  for (int i = 0; i < count - 1; i++) {
    brk = sbrk(-step_size);
    ASSERT(brk != (void*) -1);
  }
  brk = sbrk(-step_size + offset);
  ASSERT(brk != (void*) -1)
}

static int
oom_iteration(int step_size)
{
  unsigned char* start = sbrk(0);
  int depth = fill_memory(step_size);
  memset(start, 162, depth * step_size);
  for (int i = 0; i != depth * step_size; i++) {
    ASSERT(start[i] == 162);
  }
  empty_memory(depth, step_size);
  unsigned char* end = sbrk(0);
  ASSERT(start == end);
  return depth;
}

void
test_sbrk_oom (int num_iterations, int step_size)
{
  int first_depth = oom_iteration(1);
  int expected_depth = oom_iteration(step_size);
  for (int i = 0; i != num_iterations - 1; i++) {
    int reached_depth = oom_iteration(step_size);
    if (reached_depth != expected_depth) {
      fail("after run %d/%d, expected depth %d, actual depth %d.",
            i + 2, num_iterations, expected_depth, reached_depth);
    }
  }
  msg("Completed all %d iterations", num_iterations);
  int final_depth = oom_iteration(1);
  if (first_depth != final_depth) {
    fail("On final run, expected sbrk'd bytes is %d but actual is %d.", first_depth, final_depth);
  }
  msg("Now checking that memory is actually unmapped");
  msg("This access should fail: %d", *((int*) sbrk(0)));
}
