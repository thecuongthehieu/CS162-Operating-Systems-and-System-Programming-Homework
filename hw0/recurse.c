#include <stdio.h>

int recur(int i) {
    /* A stack allocated variable within a recursive function */
    int j = i;
    printf("%i\n", i);

    if (i > 0) {
        return recur(i - 1);
    }

    return 0;
}
