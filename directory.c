#include "directory.h"
#include <stdio.h>
#include <string.h>

void dir_init(Directory *dir) {
    if (!dir) return;

    for (int i = 0; i < FS_MAX_FILES; ++i) {
        dir->entries[i].used = 0;
        dir->entries[i].name[0] = '\0';
        dir->entries[i].size = 0;
        dir->entries[i].block_count = 0;
        for (int j = 0; j < FS_MAX_BLOCKS_PER_FILE; ++j) {
            dir->entries[i].blocks[j] = -1;
        }
    }
}

int dir_find(const Directory *dir, const char *name) {
    if (!dir || !name) return -1;

    for (int i = 0; i < FS_MAX_FILES; ++i) {
        if (dir->entries[i].used &&
            strncmp(dir->entries[i].name, name, FS_MAX_FILENAME) == 0) {
            return i;
        }
    }
    return -1;
}

int dir_add(Directory *dir, const char *name, size_t size, int *out_index) {
    if (!dir || !name) return FS_ERR_INVALID_ARGUMENT;

    size_t len = strlen(name);
    if (len == 0 || len >= FS_MAX_FILENAME) {
        return FS_ERR_INVALID_ARGUMENT;
    }

    /* Check if file already exists */
    if (dir_find(dir, name) != -1) {
        return FS_ERR_FILE_EXISTS;
    }

    int free_index = -1;
    for (int i = 0; i < FS_MAX_FILES; ++i) {
        if (!dir->entries[i].used) {
            free_index = i;
            break;
        }
    }

    if (free_index == -1) {
        return FS_ERR_NO_SPACE; /* No space in the directory */
    }

    FileEntry *e = &dir->entries[free_index];
    e->used = 1;
    strncpy(e->name, name, FS_MAX_FILENAME - 1);
    e->name[FS_MAX_FILENAME - 1] = '\0';
    e->size = size;
    e->block_count = 0;
    for (int j = 0; j < FS_MAX_BLOCKS_PER_FILE; ++j) {
        e->blocks[j] = -1;
    }

    if (out_index) {
        *out_index = free_index;
    }

    return FS_OK;
}

int dir_remove(Directory *dir, const char *name) {
    if (!dir || !name) return FS_ERR_INVALID_ARGUMENT;

    int idx = dir_find(dir, name);
    if (idx == -1) {
        return FS_ERR_FILE_NOT_FOUND;
    }

    FileEntry *e = &dir->entries[idx];
    e->used = 0;
    e->name[0] = '\0';
    e->size = 0;
    e->block_count = 0;
    for (int j = 0; j < FS_MAX_BLOCKS_PER_FILE; ++j) {
        e->blocks[j] = -1;
    }

    return FS_OK;
}

FileEntry *dir_get(Directory *dir, int index) {
    if (!dir) return NULL;
    if (index < 0 || index >= FS_MAX_FILES) return NULL;
    if (!dir->entries[index].used) return NULL;
    return &dir->entries[index];
}

void dir_list(const Directory *dir) {
    if (!dir) return;

    int any = 0;
    for (int i = 0; i < FS_MAX_FILES; ++i) {
        if (dir->entries[i].used) {
            printf("%s - %zu bytes\n",
                   dir->entries[i].name,
                   dir->entries[i].size);
            any = 1;
        }
    }

    if (!any) {
        printf("(no files)\n");
    }
}
