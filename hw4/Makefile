CC=gcc
CFLAGS=-g -ggdb3 -Wall -std=gnu99
LDFLAGS=-pthread
EXECUTABLES=httpserver forkserver threadserver poolserver
SOURCE=httpserver.c libhttp.c wq.c

all: $(EXECUTABLES)

httpserver: $(SOURCE)
	$(CC) $(CFLAGS) $(LDFLAGS) -D BASICSERVER $(SOURCE) -o $@
forkserver: $(SOURCE)
	$(CC) $(CFLAGS) $(LDFLAGS) -D FORKSERVER $(SOURCE) -o $@
threadserver: $(SOURCE)
	$(CC) $(CFLAGS) $(LDFLAGS) -D THREADSERVER $(SOURCE) -o $@
poolserver: $(SOURCE)
	$(CC) $(CFLAGS) $(LDFLAGS) -D POOLSERVER $(SOURCE) -o $@

clean:
	rm -f $(EXECUTABLES)
