#include "buffered_filesystem.h"

#include <log.h>

static uint32_t override_buffer_size = 0;

void set_override_buffer_size(uint32_t override_size)
{
    override_buffer_size = override_size;
}

static FILE *buffered_fopen(const char *filename, const char *mode, const size_t buffer_size)
{
    log_info("Buffered file open %s as %s", filename, mode );
    FILE *file = fopen(filename, mode);

    if (override_buffer_size == 0)
    {
        log_info("setting buffer size %d", (int)buffer_size);
        if (setvbuf(file, NULL, _IOFBF, buffer_size) != 0)
        {
            log_error("setvbuf failed");
            perror ("The following error occurred");
        }
    }
    else
    {
        log_info("setting buffer size %d", (int)override_buffer_size);
        if (setvbuf(file, NULL, _IOFBF, override_buffer_size) != 0)
        {
            log_error("setvbuf failed");
            perror ("The following error occurred");
        }
    }
    log_info("File opened successfully as %d", fileno(file));
    return file;
}

static off_t buffered_seek(FILE *file, off_t offset, int whence)
{
    if (fseek(file, offset, whence) != 0)
    {
        log_error("fseek() failed");
        perror ("The following error occurred");
        return -1;
    }
    return offset;
}

static ssize_t buffered_write(FILE *file, const void *data, size_t len)
{
    size_t write_result = fwrite(data,len, 1, file);
    if (write_result != 1)
    {
        log_error("Error writing to backing store %d %d\n", (int)len, write_result);
        perror ("The following error occurred");
        return 0;
    }
    return len;
}

static ssize_t buffered_read(FILE *file, void *data, size_t len)
{
    size_t read_result = fread(data, len, 1, file);
    if (read_result != 1)
    {
        log_error("Error reading to backing store\n");
        perror ("The following error occurred");
        return 0;
    }
    return len;
}

static int buffered_close(FILE *file)
{
    return fclose(file);
}

const filesystem buffered_filesystem = {
    .fopen = buffered_fopen,
    .seek = buffered_seek,
    .write = buffered_write,
    .read = buffered_read,
    .close = buffered_close};