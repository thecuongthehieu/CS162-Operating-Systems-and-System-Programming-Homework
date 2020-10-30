/*
This file contains constants specific to the paging structure for i386 PAE. It's a simplified subset of:
include/linux/types.h
arch/x86/include/asm/pgtable.h
arch/x86/include/asm/pgtable_types.h
arch/x86/include/asm/page.h
arch/x86/include/asm/page_types.h
arch/x86/include/asm/pgtable-3level.h
arch/x86/include/asm/pgtable-3level_types.h
arch/x86/include/asm/page_32.h
arch/x86/include/asm/page_32_types.h
*/

#ifndef CONSTANTS_H
#define CONSTANTS_H

#ifndef PG_SIZE
#define PG_SIZE 4096
#endif

/* ****************************************************************** */
/* Hint: These constants may be useful for traversing the page table */
/* ****************************************************************** */

/*
 * PGDIR_SHIFT determines what a top-level page table entry can map
 */
#define PGDIR_SHIFT	30
#define PTRS_PER_PGD	4

/*
 * PMD_SHIFT determines the size of the area a middle-level
 * page table can map
 */
#define PMD_SHIFT	21
#define PTRS_PER_PMD	512

/*
 * entries per page directory level
 */
#define PTRS_PER_PTE	512

/* PAGE_SHIFT determines the page size */
#define PAGE_SHIFT		12

#define MAX_POSSIBLE_PHYSMEM_BITS	36

#endif

