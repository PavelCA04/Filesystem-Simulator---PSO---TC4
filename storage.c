#include "storage.h"
#include <string.h>

void storage_init(Storage *s) {
    if (!s) return;
    memset(s->data, 0, sizeof(s->data));
}

int storage_write_byte(Storage *s,
                       int block_index,
                       size_t block_offset,
                       unsigned char value) {
    if (!s) return FS_ERR_INVALID_ARGUMENT;

    if (block_index < 0 || block_index >= FS_NUM_BLOCKS) {
        return FS_ERR_OUT_OF_BOUNDS;
    }
    if (block_offset >= FS_BLOCK_SIZE) {
        return FS_ERR_OUT_OF_BOUNDS;
    }

    size_t pos = (size_t)block_index * FS_BLOCK_SIZE + block_offset;
    s->data[pos] = value;
    return FS_OK;
}

int storage_read_byte(Storage *s,
                      int block_index,
                      size_t block_offset,
                      unsigned char *out_value) {
    if (!s || !out_value) return FS_ERR_INVALID_ARGUMENT;

    if (block_index < 0 || block_index >= FS_NUM_BLOCKS) {
        return FS_ERR_OUT_OF_BOUNDS;
    }
    if (block_offset >= FS_BLOCK_SIZE) {
        return FS_ERR_OUT_OF_BOUNDS;
    }

    size_t pos = (size_t)block_index * FS_BLOCK_SIZE + block_offset;
    *out_value = s->data[pos];
    return FS_OK;
}
