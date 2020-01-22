#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char **argv) {

    char *buf = malloc(sizeof(char) * 64); 

    int res = scanf("%s", buf);
    if (res < 1) {
        return 0;
    }
    printf("%s\n", buf);

    size_t len = strlen(buf);

    char *prev_word = malloc(len * sizeof(char));
    strcpy(prev_word, buf);

    while (scanf("%s", buf) > 0) {
        if (strcmp(buf, prev_word)) {
            printf("%s\n", buf);

            free(prev_word);

            len = strlen(buf);
            prev_word = malloc(len * sizeof(char));
            strcpy(prev_word, buf);
        }
    }
    return 0;
}
