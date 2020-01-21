CC?=gcc
CFLAGS?=-Wall
BINARIES=wc

all: default
default: $(BINARIES)

%: %.c
	gcc $(CFLAGS) -o $@ $^

clean:
	rm -f $(BINARIES)
