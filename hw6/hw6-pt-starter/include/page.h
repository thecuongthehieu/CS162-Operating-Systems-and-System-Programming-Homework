/*
This code is all loosely based off of the Linux Kernel. It is adapted to be slightly more readable for educational purposes.

In particular, to see a "real" version of an x86 PAE software paget able walk, see 
x86/include/asm/pgtable.h
arch/x86/include/asm/pgtable-3level_types.h
/arch/x86/include/asm/pgtable-3level.h

It may also help to read Chapter 3 of Mel Gorman's "Understanding the Linux Virtual Memory Manage" which can be found here: https://www.kernel.org/doc/gorman/html/understand/understand006.html

*/
#include <stdbool.h>
#include <stdint.h>

#define NO_ALIGN __attribute__((__packed__))

/* See Table 4-11: "Format of a PAE Page-Table Entry that Maps a 4-KByte Page" of the Intel IA32 manual Vol 3a */
typedef struct NO_ALIGN page_table_entry {
  bool present : 1;
  bool writeable : 1;
  bool supervisor : 1;
  bool pwt : 1; // Page-level write-through
  bool pcd : 1; // Page-level cache disable
  bool accessed : 1;
  bool dirty : 1;
  bool pat : 1; // Page attribute table enable
  bool global : 1;
  int ignored : 3; // These bits are ignored by the processor
  uint64_t pfn : 40;
  int zero : 11; // These bits are reserved
  bool nxe : 1; // IA32 Extended Feature Enable Register No-Execute Enable (IA32_EFER.NXE)
} pte_t;

/* See Table 4-10: "Format of a PAE Page-Table Entry that Maps a 4-KByte Page" of the Intel IA32 manual Vol 3a */
typedef struct NO_ALIGN page_directory_entry {

} pmd_t;

/* See Table 4-8: "Format of a PAE Page-Directory-Pointer-Table Entry (PDPTE)" of the Intel IA32 manual Vol 3a */
typedef struct NO_ALIGN page_directory_pointer_table_entry {

} pgd_t;

#ifndef PG_SIZE
#define PG_SIZE 4096
#endif

typedef uint8_t page_t[PG_SIZE];
