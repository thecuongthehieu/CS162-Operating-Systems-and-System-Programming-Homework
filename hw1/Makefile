EXECUTABLES=words lwords
CC=gcc
CFLAGS=-g -Wall -std=gnu99

.PHONY: all clean

all: $(EXECUTABLES)

words: words.o word_helpers.o word_count.o
lwords: lwords.o word_count_l.o word_helpers.o list.o debug.o

$(EXECUTABLES):
	$(CC) $(LDFLAGS) $^ -o $@

lwords.o: words.c
word_count_l.o: word_count_l.c

lwords.o word_count_l.o:
	$(CC) $(CFLAGS) -DPINTOS_LIST -c $< -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(EXECUTABLES) *.o
