CC?=gcc
CFLAGS?=-g
BINARIES=map limits

all: default
default: $(BINARIES)

%: %.c
	gcc $(CFLAGS) -o $@ $^

clean:
	rm -f $(BINARIES)
