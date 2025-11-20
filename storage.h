#ifndef STORAGE_H
#define STORAGE_H

#include "filesystem.h"

/* Represents the storage of the filesystem */
typedef struct {
    unsigned char data[FS_TOTAL_SIZE];
} Storage;

/* Initializes the storage */
void storage_init(Storage *s);

/* Writes a byte to a specific block in the storage */
int  storage_write_byte(Storage *s,
                        int block_index,
                        size_t block_offset,
                        unsigned char value);

/* Reads a byte from a specific block in the storage */
int  storage_read_byte(Storage *s,
                       int block_index,
                       size_t block_offset,
                       unsigned char *out_value);

#endif 
