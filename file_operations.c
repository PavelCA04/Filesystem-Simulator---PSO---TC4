#include "file_operations.h"

static size_t blocks_for_size(size_t size) {
    if (size == 0) return 0;
    return (size + FS_BLOCK_SIZE - 1) / FS_BLOCK_SIZE;
}

int file_create(Directory *dir,
                BlockManager *bm,
                Storage *st,
                const char *name,
                size_t size) {
    (void)st; 

    if (!dir || !bm || !name) return FS_ERR_INVALID_ARGUMENT;

    int entry_index = -1;
    int rc = dir_add(dir, name, size, &entry_index);
    if (rc != FS_OK) {
        return rc;
    }

    FileEntry *e = dir_get(dir, entry_index);
    if (!e) {
        return FS_ERR_INVALID_ARGUMENT;
    }

    size_t required_blocks = blocks_for_size(size);
    if (required_blocks == 0) {
        e->block_count = 0;
        return FS_OK;
    }

    rc = bm_allocate(bm, required_blocks, e->blocks);
    if (rc != FS_OK) {
        /* revert entry in case of error */
        dir_remove(dir, name);
        return rc;
    }

    e->block_count = (int)required_blocks;
    return FS_OK;
}

int file_write(Directory *dir,
               BlockManager *bm,
               Storage *st,
               const char *name,
               size_t offset,
               const char *data,
               size_t data_len,
               size_t *bytes_written) {
    (void)bm; 

    if (bytes_written) *bytes_written = 0;

    if (!dir || !st || !name || !data) return FS_ERR_INVALID_ARGUMENT;

    int idx = dir_find(dir, name);
    if (idx == -1) return FS_ERR_FILE_NOT_FOUND;

    FileEntry *f = dir_get(dir, idx);
    if (!f) return FS_ERR_FILE_NOT_FOUND;

    if (offset > f->size) {
        return FS_ERR_INVALID_OFFSET;
    }
    if (data_len == 0) {
        return FS_OK;
    }
    if (offset + data_len > f->size) {
        return FS_ERR_OUT_OF_BOUNDS;
    }

    for (size_t i = 0; i < data_len; ++i) {
        size_t pos = offset + i;
        size_t block_index = pos / FS_BLOCK_SIZE;
        size_t block_offset = pos % FS_BLOCK_SIZE;

        if ((int)block_index >= f->block_count) {
            return FS_ERR_OUT_OF_BOUNDS;
        }

        int disk_block = f->blocks[block_index];
        int rc = storage_write_byte(st, disk_block, block_offset,
                                    (unsigned char)data[i]);
        if (rc != FS_OK) {
            return rc;
        }
    }

    if (bytes_written) {
        *bytes_written = data_len;
    }

    return FS_OK;
}

int file_read(Directory *dir,
              Storage *st,
              const char *name,
              size_t offset,
              size_t size,
              char *out_buffer,
              size_t *out_bytes_read) {
    if (out_bytes_read) *out_bytes_read = 0;

    if (!dir || !st || !name || !out_buffer) return FS_ERR_INVALID_ARGUMENT;

    int idx = dir_find(dir, name);
    if (idx == -1) return FS_ERR_FILE_NOT_FOUND;

    FileEntry *f = dir_get(dir, idx);
    if (!f) return FS_ERR_FILE_NOT_FOUND;

    if (offset > f->size) {
        return FS_ERR_INVALID_OFFSET;
    }
    if (size == 0) {
        return FS_OK;
    }
    if (offset + size > f->size) {
        return FS_ERR_OUT_OF_BOUNDS;
    }

    for (size_t i = 0; i < size; ++i) {
        size_t pos = offset + i;
        size_t block_index = pos / FS_BLOCK_SIZE;
        size_t block_offset = pos % FS_BLOCK_SIZE;

        if ((int)block_index >= f->block_count) {
            return FS_ERR_OUT_OF_BOUNDS;
        }

        int disk_block = f->blocks[block_index];
        unsigned char value = 0;
        int rc = storage_read_byte(st, disk_block, block_offset, &value);
        if (rc != FS_OK) {
            return rc;
        }
        out_buffer[i] = (char)value;
    }

    if (out_bytes_read) {
        *out_bytes_read = size;
    }

    return FS_OK;
}

int file_delete(Directory *dir,
                BlockManager *bm,
                const char *name) {
    if (!dir || !bm || !name) return FS_ERR_INVALID_ARGUMENT;

    int idx = dir_find(dir, name);
    if (idx == -1) {
        return FS_ERR_FILE_NOT_FOUND;
    }

    FileEntry *f = dir_get(dir, idx);
    if (!f) {
        return FS_ERR_FILE_NOT_FOUND;
    }

    if (f->block_count > 0) {
        bm_free(bm, f->blocks, (size_t)f->block_count);
    }

    return dir_remove(dir, name);
}
