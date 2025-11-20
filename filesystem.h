#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <stddef.h>

/* --- Parameters of the filesystem --- */

#define FS_TOTAL_SIZE          (1024 * 1024)   /* 1 MB total storage */
#define FS_BLOCK_SIZE          512             /* Block size in bytes */
#define FS_MAX_FILES           100             /* Maximum number of files */
#define FS_MAX_FILENAME        64              /* Maximum filename length */

#define FS_NUM_BLOCKS          (FS_TOTAL_SIZE / FS_BLOCK_SIZE)
#define FS_MAX_BLOCKS_PER_FILE FS_NUM_BLOCKS

/* --- Error codes --- */

#define FS_OK                    0
#define FS_ERR_NO_SPACE         -1
#define FS_ERR_FILE_EXISTS      -2
#define FS_ERR_FILE_NOT_FOUND   -3
#define FS_ERR_INVALID_OFFSET   -4
#define FS_ERR_OUT_OF_BOUNDS    -5
#define FS_ERR_INVALID_ARGUMENT -6

/* API */

/* Initializes the filesystem */
void   fs_init(void);

/* Creates a file */
int    fs_create(const char *name, size_t size);

/* Writes data_len bytes starting at offset in a file */
int    fs_write(const char *name,
                size_t offset,
                const char *data,
                size_t data_len,
                size_t *bytes_written);

/* Reads size bytes starting at offset into buffer */
int    fs_read(const char *name,
               size_t offset,
               size_t size,
               char *out_buffer,
               size_t *out_bytes_read);

/* Deletes a file */
int    fs_delete(const char *name);

/* Lists files to stdout */
void   fs_list(void);

/* Returns total free space (in bytes) */
size_t fs_get_free_space(void);

#endif
