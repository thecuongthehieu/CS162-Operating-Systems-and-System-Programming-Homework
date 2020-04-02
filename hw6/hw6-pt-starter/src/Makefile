CC?=gcc
IDIR = ../include
CFLAGS?=-g -Wall -I $(IDIR)  
BINARIES=mmu  

mmu: mmu.c ram.c 
	$(CC) $(CFLAGS) -o $@ $^	

clean:
	rm -f $(BINARIES)
