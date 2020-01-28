#include <stdio.h>

void print_array (int *arr, int size) {
    int *endpoint = arr + size;
    while (arr < endpoint) {
        printf ("%d\n", *arr);
        arr += 1;
    }
}

int main () {
    int a[] = {1, 2, 0, 5};
    print_array (a, 4);
}
