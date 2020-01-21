.DEFAULT_GOAL := default

CC?=gcc
CFLAGS?=-Wall
SOURCES=main.c word_count.c
# comment the following out if you are providing your own sort_words
LIBRARIES=wc_sort.o
BINARIES=words

%: %.c
	$(CC) $(CFLAGS) $(LIBRARIES) -o $@ $^

clean:
	rm -f $(BINARIES)

executable:
	$(CC) $(CFLAGS) $(SOURCES) $(LIBRARIES) -o $(BINARIES)

default: executable
