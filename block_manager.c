#include "block_manager.h"

void bm_init(BlockManager *bm) {
    if (!bm) return;
    for (int i = 0; i < FS_NUM_BLOCKS; ++i) {
        bm->block_used[i] = 0;
    }
}

size_t bm_count_free(const BlockManager *bm) {
    if (!bm) return 0;

    size_t free_count = 0;
    for (int i = 0; i < FS_NUM_BLOCKS; ++i) {
        if (!bm->block_used[i]) {
            ++free_count;
        }
    }
    return free_count;
}

int bm_allocate(BlockManager *bm, size_t count, int *out_blocks) {
    if (!bm || !out_blocks) return FS_ERR_INVALID_ARGUMENT;
    if (count == 0) return FS_OK;

    size_t free_count = bm_count_free(bm);
    if (free_count < count) {
        return FS_ERR_NO_SPACE;
    }

    size_t assigned = 0;
    for (int i = 0; i < FS_NUM_BLOCKS && assigned < count; ++i) {
        if (!bm->block_used[i]) {
            bm->block_used[i] = 1;
            out_blocks[assigned++] = i;
        }
    }

    return FS_OK;
}

void bm_free(BlockManager *bm, const int *blocks, size_t count) {
    if (!bm || !blocks) return;

    for (size_t i = 0; i < count; ++i) {
        int idx = blocks[i];
        if (idx >= 0 && idx < FS_NUM_BLOCKS) {
            bm->block_used[idx] = 0;
        }
    }
}
