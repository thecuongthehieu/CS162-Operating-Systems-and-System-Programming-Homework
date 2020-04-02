/* This simulation of physical memory is really backed by a 1 level page table,
   but the "chunk" will be used to avoid confusion */
#include "ram.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define RAM_SIZE (1ull << 36)

#define CHUNK_SIZE (1ull << 22)
#define MAX_CHUNKS (1ull << 14)

typedef uint8_t chunk[CHUNK_SIZE];

int32_t chunk_table[MAX_CHUNKS];
chunk *chunks;
size_t num_chunks;

chunk *get_chunk(size_t idx) {
  int32_t chunk_table_entry = chunk_table[idx];
  if (chunk_table_entry < 0) {
    chunk_table_entry = num_chunks;
    chunk_table[idx] = chunk_table_entry;
    num_chunks++;
    chunks = realloc(chunks, CHUNK_SIZE * num_chunks);
  }
  return &chunks[chunk_table_entry];
}

void ram_init(void) {
  for (int i = 0; i < MAX_CHUNKS; i++) {
    chunk_table[i] = -1;
  }
  chunks = NULL;
  num_chunks = 0;
}

void ram_store(paddr_ptr addr, void *buf, size_t len) {
  while (len) {
    size_t chunk_idx = addr / CHUNK_SIZE;
    chunk *chunk = get_chunk(chunk_idx);
    size_t offset = addr % CHUNK_SIZE;
    void *in_chunk = &((*chunk)[offset]);
    size_t amount_in_chunk = CHUNK_SIZE - offset;
    if (len <= amount_in_chunk) {
      /* Everything fits in this chunk */
      memcpy(in_chunk, buf, len);
      break;
    } else {
      memcpy(in_chunk, buf, amount_in_chunk);
      len -= amount_in_chunk;
      buf += amount_in_chunk;
    }
  }
}

void ram_fetch(paddr_ptr addr, void *buf, size_t len) {
  while (len) {
    size_t chunk_idx = addr / CHUNK_SIZE;
    chunk *chunk = get_chunk(chunk_idx);
    size_t offset = addr % CHUNK_SIZE;
    void *in_chunk = &((*chunk)[offset]);
    size_t amount_in_chunk = CHUNK_SIZE - offset;
    if (len <= amount_in_chunk) {
      /* Everything fits in this chunk */
      memcpy(buf, in_chunk, len);
      break;
    } else {
      memcpy(buf, in_chunk, amount_in_chunk);
      len -= amount_in_chunk;
      buf += amount_in_chunk;
    }
  }
}

void ram_destroy(void) {
  free(chunks);
  ram_init();
}

size_t ram_save(char *path) {
  FILE *f = fopen(path, "wb");
  size_t written = fwrite(chunk_table, sizeof(chunk_table), 1, f) * sizeof(chunk_table);
  printf("Writing %lu chunks\n", num_chunks);
  written += fwrite(chunks, sizeof(chunk), num_chunks, f) * sizeof(chunk);
  printf("Wrote %lu bytes to file\n", written);
  fclose(f);
  return written;
}

size_t ram_load(char *path) {
  FILE *f = fopen(path, "rb");
  size_t read = fread(chunk_table, sizeof(chunk_table), 1, f);
  while (!feof(f)) {
    chunks = realloc(chunks, ++num_chunks * CHUNK_SIZE);
    read += fread(&chunks[num_chunks - 1], sizeof(chunk), 1, f);
  }
  printf("Read bytes: %lu from file\n", read);
  fclose(f);
  return read;
}
