#include "filesystem.h"
#include "storage.h"
#include "block_manager.h"
#include "directory.h"
#include "file_operations.h"

#include <stdio.h>

/* Global structures */
static Storage      g_storage;
static BlockManager g_block_manager;
static Directory    g_directory;

void fs_init(void) {
    storage_init(&g_storage);
    bm_init(&g_block_manager);
    dir_init(&g_directory);
}

/* API's that delegate to file_operations */

int fs_create(const char *name, size_t size) {
    return file_create(&g_directory, &g_block_manager, &g_storage, name, size);
}

int fs_write(const char *name,
             size_t offset,
             const char *data,
             size_t data_len,
             size_t *bytes_written) {
    return file_write(&g_directory,
                      &g_block_manager,
                      &g_storage,
                      name,
                      offset,
                      data,
                      data_len,
                      bytes_written);
}

int fs_read(const char *name,
            size_t offset,
            size_t size,
            char *out_buffer,
            size_t *out_bytes_read) {
    return file_read(&g_directory,
                     &g_storage,
                     name,
                     offset,
                     size,
                     out_buffer,
                     out_bytes_read);
}

int fs_delete(const char *name) {
    return file_delete(&g_directory, &g_block_manager, name);
}

void fs_list(void) {
    dir_list(&g_directory);
}

size_t fs_get_free_space(void) {
    size_t free_blocks = bm_count_free(&g_block_manager);
    return free_blocks * (size_t)FS_BLOCK_SIZE;
}
