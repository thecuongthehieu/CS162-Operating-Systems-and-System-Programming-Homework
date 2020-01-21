#include <stdio.h>
#include <stdlib.h>

/* A statically allocated variable */
int foo;

/* UNCOMMENT THIS LINE for 3.4.3*/
//extern int recur(int i);

int recur(int i) {
    /* A stack allocated variable within a recursive function */
    int j = i;

    if (i > 0) {
        return recur(i - 1);
    }

    return 0;
}

/* A statically allocated, pre-initialized variable */
volatile int stuff = 7;

int main(int argc, char *argv[]) {
    /* A stack allocated variable */
    volatile int i = 0;

    /* Dynamically allocate some stuff */
    volatile char *buf1 = malloc(100);
    /* ... and some more stuff */
    volatile char *buf2 = malloc(100);

    recur(3);
    return 0;
}
