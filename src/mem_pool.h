#ifndef PSYCHE_MEM_POOL_H
#define PSYCHE_MEM_POOL_H

struct mem_pool {
  long size;
  long nblocks;
  long blocksize;
  long freeblocks;

  void* firstptr;
  void* freeptr;
  void* blockptr;

  struct mem_pool* prev;
  struct mem_pool* next;
};

void*
mem_pool_create(long size, long blocksize);

void
mem_pool_destroy(struct mem_pool* mpool);

void*
mem_pool_alloc(struct mem_pool* mpool);

void
mem_pool_free(struct mem_pool* mpool, void* ptr);

#endif  // PSYCHE_MEM_POOL_H