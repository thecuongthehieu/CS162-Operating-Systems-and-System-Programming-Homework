#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//You should try to understand why these 3 method works, but the intended bug is not here
char is_visible_char(char letter) {
    return ('!' <= letter) && (letter <= '~');
}

size_t snscan_word(FILE*file, char **output_buffer) {
    size_t max_size = 10;
    char *buf = malloc(sizeof(*buf) * max_size);
    char letter = 0;

    while (fread(&letter, 1, 1, stdin) && !is_visible_char(letter));

    if (!letter) {
        return 0;
    }
    buf[0] = letter;

    size_t i;
    for (i = 1; fread(&letter, 1, 1, stdin) && is_visible_char(letter); i++) {
        if (i+1 >= max_size) {
            max_size *= 2;
            buf = realloc(buf, max_size);
        }
        buf[i] = letter;
    }
    buf = realloc(buf, sizeof(*buf) * (i + 1));
    *output_buffer = buf;
    return i;
}

void read_strings(char ***arr_ptr, size_t *len) {
    size_t i;
    size_t max_size = 10;
    char **arr = malloc(max_size * sizeof(*arr));

    char *buf;
    for (i = 0; snscan_word(stdin, &buf) > 0; i++) { 
        if (i >= max_size) {
            max_size *= 2;
            arr = realloc(arr, max_size * sizeof(*arr));
        }
        arr[i] = buf;
    }
    *arr_ptr = arr;
    *len = i;
}

char **sort(char **arr, size_t len) {
    if (len <= 1) {
        return arr;
    }

    char **left = sort(arr, len/2);
    size_t left_size = len/2;

    char **right = sort(&arr[len/2], len - (len/2));
    size_t right_size = len - (len/2);

    char *temp[len];
    char **merged = (char**) temp;
    size_t left_index, right_index;
    left_index = right_index = 0;

    while (left_index < left_size && right_index < right_size) {
        int cmp = strcmp(left[left_index], right[right_index]);
        if (cmp < 0) {
            merged[left_index + right_index] = left[left_index];
            ++left_index;
        } else {
            merged[left_index + right_index] = right[right_index];
            ++right_index;
        }
    }

    while (left_index < left_size) {
        merged[left_index + right_index] = left[left_index];
        ++left_index;
    }

    while (right_index < right_size) {
        merged[left_index + right_index] = right[right_index];
        ++right_index;
    }
    return merged;
}

int main(int argc, char **argv) {

   char **words;
   size_t len;
   read_strings(&words, &len);

   words = sort(words, len);

   for (int i = 0; i < len; i++) {
       printf("%s\n", words[i]);
   }

}
