#ifndef BLOCK_MANAGER_H
#define BLOCK_MANAGER_H

#include "filesystem.h"

typedef struct {
    int block_used[FS_NUM_BLOCKS];
} BlockManager;

/* Starts the Block Manager */
void   bm_init(BlockManager *bm);

/* Counts free blocks */
size_t bm_count_free(const BlockManager *bm);

/* Allocates 'count' blocks and places the indices in out_blocks */
int    bm_allocate(BlockManager *bm, size_t count, int *out_blocks);

/* Frees 'count' blocks that are in the blocks[] array */
void   bm_free(BlockManager *bm, const int *blocks, size_t count);

#endif 
