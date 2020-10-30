/*
 * mm_alloc.h
 *
 * Exports a clone of the interface documented in "man 3 malloc".
 */

#pragma once

#ifndef _malloc_H_
#define _malloc_H_

/* Define the block size since the sizeof will be wrong */
#define BLOCK_SIZE 40

#include <stdlib.h>

void* mm_malloc(size_t size);
void* mm_realloc(void* ptr, size_t size);
void mm_free(void* ptr);

//TODO: Add any implementation details you might need to this file

typedef struct s_block *s_block_ptr;
extern s_block_ptr HeadPtr;

/* block struct */
typedef struct s_block {
    size_t size; //8 bytes
    struct s_block *next; //8bytes
    struct s_block *prev; //8bytes
    int free; //4 bytes
    void *block; //8bytes
    
    //There will padding here of 4 bytes (on my 64 bit machine), but I like this method even though it wastes 4
    //bytes because it makes the 
    //program more cross platform since sizeof can be used.
    //void *ptr;
    /* A pointer to the allocated block */
    //char data[0];
    
 }s_block;


/* Split block according to size, b must exist */
void split_block (s_block_ptr b, size_t s);

/* Try fusing block with neighbors */
s_block_ptr fusion(s_block_ptr b);

/* Get the block from addr */
s_block_ptr get_block (void *p);

/* Add a new block at the of heap,
 * return NULL if things go wrong
 */
s_block_ptr extend_heap (s_block_ptr last , size_t s);

/* Copy info and contents of block from one location to another.
 * returns ptr to new block if successful or NULL otherwise
 */
void* mem_copy(s_block_ptr oldB, s_block_ptr newB);


#endif
