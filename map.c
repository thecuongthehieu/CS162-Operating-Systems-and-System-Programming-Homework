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
int stuff = 7;

int main(int argc, char *argv[]) {
    /* A stack allocated variable */
    int i;

    /* Dynamically allocate some stuff */
    char *buf1 = malloc(100);
    /* ... and some more stuff */
    char *buf2 = malloc(100);

    recur(3);
    return 0;
}
