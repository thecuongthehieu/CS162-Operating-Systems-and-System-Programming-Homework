#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char global[] = {'h', 'e', 'l', 'l', '0'};

char *f1 () {
    return "hello";
}

char *f2 () {
    return global;
}

char *f3 () {
    char hello[] = "hello";
    return hello;
}
char *f4 () {
    return malloc (strlen ("hello") + 1);
}

char *f5 () { 
    char *arr = calloc (strlen ("hello") + 1, sizeof (char));
    for (int i = 0; i < 5; i++) {
        arr[i] = "hello"[i];
    }
    return arr;
}

int main () {
    // State which of the following print statements will always succeed.
    // For those that are undefined explain why.
    printf ("%s\n", f1());
    printf ("%s\n", f2());
    printf ("%s\n", f3());
    printf ("%s\n", f4());
    printf ("%s\n", f5());
}
