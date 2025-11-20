#ifndef DIRECTORY_H
#define DIRECTORY_H

#include "filesystem.h"

/* Individual file entry in the root directory */
typedef struct {
    int    used;                                /* 0 = free, 1 = used */
    char   name[FS_MAX_FILENAME];               /* File name             */
    size_t size;                                /* Logical size (bytes)  */
    int    block_count;                         /* Number of blocks      */
    int    blocks[FS_MAX_BLOCKS_PER_FILE];      /* Block indices         */
} FileEntry;

/* Complete root directory */
typedef struct {
    FileEntry entries[FS_MAX_FILES];
} Directory;

/* Initializes the directory (no files) */
void      dir_init(Directory *dir);

/* Finds a file by name; returns index or -1 if not found */
int       dir_find(const Directory *dir, const char *name);

/* Adds an entry and returns the index in out_index */
int       dir_add(Directory *dir,
                  const char *name,
                  size_t size,
                  int *out_index);

/* Removes an entry by name */
int       dir_remove(Directory *dir, const char *name);

/* Gets pointer to entry given an index (or NULL) */
FileEntry *dir_get(Directory *dir, int index);

/* Lists files to stdout */
void      dir_list(const Directory *dir);

#endif
