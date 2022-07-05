#include "unbuffered_filesystem.h"

#include <log.h>

#include <unistd.h>

static FILE *unbuffered_fopen(const char *filename, const char *mode, const size_t buffer_size)
{
    log_info("unuffered file open %s as %s", filename, mode);
    return fopen(filename, mode);
}

static off_t unbuffered_seek(FILE *file, off_t offset, int whence)
{
    return lseek(fileno(file), offset, whence);
}

static ssize_t unbuffered_write(FILE *file, const void *data, size_t len)
{
    if (write(fileno(file), data, len) == -1)
    {
        log_error("Error writing to backing store\n");
        return 0;
    }
    return len;
}

static ssize_t unbuffered_read(FILE *file, void *data, size_t len)
{
    if (read(fileno(file), data, len) != len)
    {
        log_error("Error reading from backing store\n");
        return 0;
    }
    return len;
}

static int unbuffered_close(FILE *file)
{
    return fclose(file);
}

const filesystem unbuffered_filesystem = {
    .fopen = unbuffered_fopen,
    .seek = unbuffered_seek,
    .write = unbuffered_write,
    .read = unbuffered_read,
    .close = unbuffered_close};