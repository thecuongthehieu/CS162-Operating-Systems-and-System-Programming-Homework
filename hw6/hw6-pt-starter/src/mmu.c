#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "constants.h"
#include "page.h"
#include "ram.h"


/* These macros may or may not be useful.
 * */


//#define PMD_PFN_MASK
//#define PTE_PFN_MASK
//#define PAGE_OFFSET_MASK
//
//#define vaddr_pgd(vaddr)
//#define vaddr_pmd(vaddr)
//#define vaddr_pte(vaddr)
//#define vaddr_off(vaddr)
//
//#define pfn_to_addr(pfn) (pfn << PAGE_SHIFT)


/* Translates the virtual address vaddr and stores the physical address in paddr.
 * If a page fault occurs, return a non-zero value, otherwise return 0 on a successful translation.
 * */

int virt_to_phys(vaddr_ptr vaddr, paddr_ptr cr3, paddr_ptr *paddr) {
  /* TODO */

  return 1;
}

char *str_from_virt(vaddr_ptr vaddr, paddr_ptr cr3) {
  size_t buf_len = 1;
  char *buf = malloc(buf_len);
  char c = ' ';
  paddr_ptr paddr;

  for (int i=0; c; i++) {
    if(virt_to_phys(vaddr + i, cr3, &paddr)){
      printf("Page fault occured at address %p\n", (void *) vaddr + i);
      return (void *) 0;
    }

    ram_fetch(paddr, &c, 1);
    buf[i] = c;
    if (i + 1 >= buf_len) {
      buf_len <<= 1;
      buf = realloc(buf, buf_len);
    }
    buf[i + 1] = '\0';
  }
  return buf;
}

int main(int argc, char **argv) {

  if (argc != 4) {
    printf("Usage: ./mmu <mem_file> <cr3> <vaddr>\n");
    return 1;
  }

  paddr_ptr translated;

  ram_init();
  ram_load(argv[1]);

  paddr_ptr cr3 = strtol(argv[2], NULL, 0);
  vaddr_ptr vaddr = strtol(argv[3], NULL, 0);


  if(virt_to_phys(vaddr, cr3, &translated)){
    printf("Page fault occured at address %p\n", vaddr);
    exit(1);
  }

  char *str = str_from_virt(vaddr, cr3);
  printf("Virtual address %p translated to physical address %p\n", vaddr, translated);
  printf("String representation of data at virtual address %p: %s\n", vaddr, str);

  return 0;
}
