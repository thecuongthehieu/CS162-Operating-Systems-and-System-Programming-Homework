#include <stdio.h>
#include <stdlib.h>
#include <string.h>

long int strlen_staff(const char *src) {
    long int count = 0;
    while(*src != '\0') {
        count++;
        src += 1;
    }
    return count;
}


int main() {
    char *str = "hello world";
    printf("The length of string '%s' is: %ld\n", str, strlen(str));
    return 0;
}
