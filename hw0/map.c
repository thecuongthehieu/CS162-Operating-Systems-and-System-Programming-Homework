#include <stdio.h>
#include <stdlib.h>

/* A statically allocated variable */
int foo;

/* UNCOMMENT THIS LINE for 3.4.3*/
extern int recur(int i);


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
