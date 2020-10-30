CC?=gcc
CFLAGS?=-g
BINARIES=map limits

default:
	make map
	make limits

map:
	$(CC) $(CFLAGS) map.c recurse.c -o map

limits:
	$(CC) $(CFLAGS) limits.c -o limits

clean:
	rm -f $(BINARIES)
