.DEFAULT_GOAL := default

CC?=gcc
CFLAGS?=-Wall
SOURCES=main.c word_count.c
# The skeleton would include the library, unless student does the extra credit
LIBRARIES=wc_sort.o
BINARIES=words

%: %.c
	$(CC) $(CFLAGS) $(LIBRARIES) -o $@ $^

clean:
	rm -f $(BINARIES)

executable:
	$(CC) $(CFLAGS) $(SOURCES) $(LIBRARIES) -o $(BINARIES)

default: executable
