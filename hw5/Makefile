CFLAGS=-g -Wall -std=c99 -D_POSIX_SOURCE -D_BSD_SOURCE -D_XOPEN_SOURCE=700 -fPIC
TEST_CFLAGS=-Wl,-rpath=.
TEST_LDFLAGS=-ldl

all: hw3lib.so mm_test

hw3lib.so: mm_alloc.o
	gcc -shared -o $@ $^

mm_alloc.o: mm_alloc.c
	gcc $(CFLAGS) -c -o $@ $^

mm_test: mm_test.c
	gcc $(CFLAGS) $(TEST_CFLAGS) -o $@ $^ $(TEST_LDFLAGS)

clean:
	rm -rf hw3lib.so mm_alloc.o mm_test
