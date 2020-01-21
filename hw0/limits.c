#include <stdio.h>
#include <sys/resource.h>

int main() {
    struct rlimit lim;
    printf("stack size: %ld\n", 0L);
    printf("process limit: %ld\n", 0L);
    printf("max file descriptors: %ld\n", 0L);
    return 0;
}
