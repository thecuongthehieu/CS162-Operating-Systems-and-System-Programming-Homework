EXECUTABLES=pthread lwords pwords 
CC=gcc
CFLAGS=-g -pthread -Wall -std=gnu99
LDFLAGS=-pthread

.PHONY: all clean

all: $(EXECUTABLES)

pthread: pthread.o
lwords: lwords.o word_count_l.o word_helpers.o list.o debug.o
pwords: pwords.o word_count_p.o word_helpers.o list.o debug.o

$(EXECUTABLES):
	$(CC) $(LDFLAGS) $^ -o $@

lwords.o: words.c
pwords.o: pwords.c
word_count_p.o: word_count_p.c
word_count_l.o: word_count_l.c

lwords.o word_count_l.o:
	$(CC) $(CFLAGS) -DPINTOS_LIST -c $< -o $@

pwords.o word_count_p.o:
	$(CC) $(CFLAGS) -DPINTOS_LIST -DPTHREADS -c $< -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(EXECUTABLES) *.o
