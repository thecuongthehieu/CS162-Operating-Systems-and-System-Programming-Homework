#ifndef RAM_H
#define RAM_H

#include <stddef.h>
#include <stdint.h>

typedef uint32_t vaddr_ptr;
typedef uint64_t paddr_ptr;

void ram_init(void);

void ram_store(paddr_ptr addr, void *buf, size_t len);
void ram_fetch(paddr_ptr addr, void *buf, size_t len);

void ram_destroy(void);

// Note ram should be initialized before EITHER of these is called
size_t ram_save(char *path);
size_t ram_load(char *path);

#endif
