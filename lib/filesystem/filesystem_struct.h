#pragma once

#include <stdio.h>

typedef struct
{
    FILE *(*fopen)(const char *filename, const char *mode, const size_t buffer_size);
    off_t (*seek)(FILE *file, off_t offset, int whence); // SEEK_SET / SEEK_CUR / SEEK_END
    ssize_t (*write)(FILE *file, const void *data, size_t len);
    ssize_t (*read)(FILE *file, void *data, size_t len);
    int (*close)(FILE *file);
} filesystem;