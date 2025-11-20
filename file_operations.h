#ifndef FILE_OPERATIONS_H
#define FILE_OPERATIONS_H

#include "filesystem.h"
#include "directory.h"
#include "block_manager.h"
#include "storage.h"

/* Creates a file in the directory using the block manager */
int file_create(Directory *dir,
                BlockManager *bm,
                Storage *st,          /* st is here for symmetry; not used much */
                const char *name,
                size_t size);

/* Writes data_len bytes starting at offset in a file */
int file_write(Directory *dir,
               BlockManager *bm,
               Storage *st,
               const char *name,
               size_t offset,
               const char *data,
               size_t data_len,
               size_t *bytes_written);

/* Reads size bytes starting at offset into buffer */
int file_read(Directory *dir,
              Storage *st,
              const char *name,
              size_t offset,
              size_t size,
              char *out_buffer,
              size_t *out_bytes_read);

/* Deletes a file */
int file_delete(Directory *dir,
                BlockManager *bm,
                const char *name);

#endif
