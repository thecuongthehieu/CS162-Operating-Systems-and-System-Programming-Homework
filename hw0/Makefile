CC?=gcc
CFLAGS?=-g
BINARIES=map

map:
	$(CC) $(CFLAGS) map.c recurse.c -o map

default: map

clean:
	rm -f $(BINARIES)
