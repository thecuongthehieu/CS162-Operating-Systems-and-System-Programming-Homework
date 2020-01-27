#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// FIX ME! I'm a broken implementation of strcpy!
char *strcpy_staff(char *dest, const char *src) {
    if (dest == NULL) {
        return NULL;
    }
    while (*src != '\0') {
        *dest = *src;
        dest++;
        src++;
    }
    return dest;
}


int main() {
    char *str = "hello world";
    char *str1 = malloc(strlen(str) + 1);
    char *str2 = strcpy_staff(str1, str);
    printf("%s\n",str1);
    printf("%s\n",str2);
    return 0;
}
