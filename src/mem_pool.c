#include "mem_pool.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <inttypes.h>

#define ALIGN 8
#define ROUNDUP(s) (((s) + ALIGN - 1) / ALIGN * ALIGN)

void*
mem_pool_create(long size, long blocksize) {
  int i;
  struct mem_pool* mpool;

  if (size < blocksize) {
    printf("size must bigger than blocksize\n");

    return NULL;
  }

  if (size % blocksize != 0) {
    printf("size mod block must be 0\n");

    return NULL;
  }

  mpool = (struct mem_pool*)malloc(sizeof(*mpool));
  if (!mpool) {
    return NULL;
  }

  memset(mpool, 0, sizeof(*mpool));

  mpool->blockptr = malloc(ROUNDUP(size));
  if (!mpool->blockptr) {
    free(mpool);

    return NULL;
  }
  mpool->firstptr = (void*)ROUNDUP((uintptr_t)mpool->blockptr);
  memset(mpool->blockptr, 0, ROUNDUP(size));
  mpool->size = size;
  mpool->nblocks = size / blocksize;
  mpool->blocksize = blocksize;
  mpool->freeptr = NULL;
  mpool->freeblocks = 0;

  for (i = 0; i < mpool->nblocks; ++i) {
    mem_pool_free(mpool, (char*)mpool->firstptr + blocksize * i);
  }

  return mpool;
}

void
mem_pool_destroy(struct mem_pool* mpool) {
  free(mpool->blockptr);
  free(mpool);
}

void*
mem_pool_alloc(struct mem_pool* mpool) {
  void* ptr;
  void* freeptr;

  if (mpool->freeblocks > 0) {
    ptr = mpool->freeptr;
    memcpy(&freeptr, ptr, sizeof(void*));
    mpool->freeptr = freeptr;
    mpool->freeblocks -= 1;

    return ptr;
  }
  printf("error\n");

  return NULL;
}

void
mem_pool_free(struct mem_pool* mpool, void* ptr) {
  void* freeptr;
  if (ptr) {
    assert(ptr >= mpool->blockptr &&
           (char*)ptr < (char*)mpool->blockptr + mpool->size);
    
    assert((uintptr_t)((char *)ptr -
                       (char *)mpool->firstptr) %
            mpool->blocksize == 0);

    freeptr = mpool->freeptr;
    memcpy(ptr, &freeptr, sizeof(void *));
    mpool->freeptr = ptr;
    mpool->freeblocks += 1;
  }
}