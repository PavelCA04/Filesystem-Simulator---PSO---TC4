#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "filesystem.h"

#define MAX_LINE 2048

/* Transforms a string to uppercase. */
static void str_to_upper(char *s) {
    if (!s) return;
    for (; *s; ++s) {
        *s = (char)toupper((unsigned char)*s);
    }
}

/* Trims leading and trailing whitespace. */
static void trim_whitespace(char *s) {
    if (!s || *s == '\0') return;

    char *start = s;
    while (*start && isspace((unsigned char)*start)) {
        start++;
    }

    if (start != s) {
        memmove(s, start, strlen(start) + 1);
    }

    size_t len = strlen(s);
    if (len == 0) return;

    char *end = s + len - 1;
    while (end >= s && isspace((unsigned char)*end)) {
        *end = '\0';
        if (end == s) break;
        end--;
    }
}

/* If the string starts and ends with double quotes, removes them. */
static void strip_quotes(char *s) {
    if (!s) return;

    trim_whitespace(s);
    size_t len = strlen(s);
    if (len >= 2 && s[0] == '"' && s[len - 1] == '"') {
        memmove(s, s + 1, len - 2);
        s[len - 2] = '\0';
    }
}

static void print_fs_error(int code) {
    switch (code) {
        case FS_OK:
            break;
        case FS_ERR_NO_SPACE:
            printf("Error: no sufficient space in the filesystem.\n");
            break;
        case FS_ERR_FILE_EXISTS:
            printf("Error: file already exists.\n");
            break;
        case FS_ERR_FILE_NOT_FOUND:
            printf("Error: file not found.\n");
            break;
        case FS_ERR_INVALID_OFFSET:
            printf("Error: invalid offset.\n");
            break;
        case FS_ERR_OUT_OF_BOUNDS:
            printf("Error: operation exceeds file bounds.\n");
            break;
        case FS_ERR_INVALID_ARGUMENT:
            printf("Error: invalid argument.\n");
            break;
        default:
            printf("Unknown error (%d).\n", code);
            break;
    }
}

static void print_help(void) {
    printf("Supported commands:\n");
    printf("  CREATE <filename> <size_bytes>\n");
    printf("  WRITE  <filename> <offset> <data>\n");
    printf("  READ   <filename> <offset> <size>\n");
    printf("  DELETE <filename>\n");
    printf("  LIST\n");
    printf("  EXIT\n");
}

int main(void) {
    fs_init();

    printf("Filesystem Simulator\n");
    printf("Total space: %zu bytes, block: %d bytes, max files: %d\n",
           (size_t)FS_TOTAL_SIZE, FS_BLOCK_SIZE, FS_MAX_FILES);
    printf("Type 'HELP' to see the commands.\n\n");

    char line[MAX_LINE];

    while (1) {
        printf("> ");
        if (!fgets(line, sizeof(line), stdin)) {
            break; 
        }

        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }

        if (line[0] == '\0') {
            continue;
        }

        char command[16] = {0};
        if (sscanf(line, "%15s", command) != 1) {
            continue;
        }

        str_to_upper(command);

        if (strcmp(command, "HELP") == 0) {
            print_help();
            continue;
        }

        if (strcmp(command, "EXIT") == 0 || strcmp(command, "QUIT") == 0) {
            break;
        }

        if (strcmp(command, "CREATE") == 0) {
            char name[FS_MAX_FILENAME];
            size_t size_bytes;
            int scanned = sscanf(line, "%*s %63s %zu", name, &size_bytes);
            if (scanned != 2) {
                printf("Usage: CREATE <filename> <size_bytes>\n");
                        continue;
            }

            int rc = fs_create(name, size_bytes);
            if (rc != FS_OK) {
                print_fs_error(rc);
            } else {
                printf("File '%s' created (%zu bytes).\n", name, size_bytes);
            }
            continue;
        }

        if (strcmp(command, "DELETE") == 0) {
            char name[FS_MAX_FILENAME];
            int scanned = sscanf(line, "%*s %63s", name);
            if (scanned != 1) {
                printf("Usage: DELETE <filename>\n");
                continue;
            }

            int rc = fs_delete(name);
            if (rc != FS_OK) {
                print_fs_error(rc);
            } else {
                printf("File '%s' deleted.\n", name);
            }
            continue;
        }

        if (strcmp(command, "LIST") == 0) {
            fs_list();
            printf("Free space: %zu bytes.\n", fs_get_free_space());
            continue;
        }

        if (strcmp(command, "WRITE") == 0) {
            char name[FS_MAX_FILENAME];
            size_t offset;
            char data[MAX_LINE];

            int scanned = sscanf(line, "%*s %63s %zu %[^\n]", name, &offset, data);
            if (scanned < 3) {
                printf("Usage: WRITE <filename> <offset> <data>\n");
                continue;
            }

            trim_whitespace(data);
            strip_quotes(data);

            size_t bytes_written = 0;
            int rc = fs_write(name, offset, data, strlen(data), &bytes_written);
            if (rc != FS_OK) {
                print_fs_error(rc);
            } else {
                printf("Wrote %zu bytes to '%s'.\n", bytes_written, name);
            }
            continue;
        }

        if (strcmp(command, "READ") == 0) {
            char name[FS_MAX_FILENAME];
            size_t offset;
            size_t size_bytes;

            int scanned = sscanf(line, "%*s %63s %zu %zu",
                                 name, &offset, &size_bytes);
            if (scanned != 3) {
                printf("Usage: READ <filename> <offset> <size_bytes>\n");
                continue;
            }

            char *buffer = (char *)malloc(size_bytes + 1);
            if (!buffer) {
                printf("Error: could not allocate memory.\n");
                continue;
            }

            size_t bytes_read = 0;
            int rc = fs_read(name, offset, size_bytes, buffer, &bytes_read);
            if (rc != FS_OK) {
                print_fs_error(rc);
                free(buffer);
                continue;
            }

            buffer[bytes_read] = '\0';
            printf("%s\n", buffer);

            free(buffer);
            continue;
        }

        printf("Unknown command: %s\n", command);
        printf("Type 'HELP' to see the list of commands.\n");
    }

    printf("Exiting the simulator.\n");
    return 0;
}
