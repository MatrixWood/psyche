#ifndef PSYCHE_ARENA_H
#define PSYCHE_ARENA_H

struct arena {
  char* limit;
  char* avail;

  char** blocks;
  int iblocks;
  int nblocks;
};

struct arena*
arena_create(struct psyche* psyche);

void
arena_destroy(struct psyche* psyche, struct arena* arena);

void*
arena_alloc(struct psyche* psyche, struct arena* arena, long bytes);

#endif  // PSYCHE_ARENA_H