#include "benchmark_tester.h"
#include <virtual_directory.h>

#include <log.h>

#include <assert.h>
#include <time.h>
#include <sys/time.h>

#include <malloc.h>
#include <string.h>

#include <stdlib.h>

// static uint32_t max_file_size = 1024 * 1024 * 1;
static uint32_t max_file_size = 1024 * 512;

filesystem const *fs;

void set_filesystem(const filesystem *_filesystem)
{
    fs = _filesystem;
}

void randomize_bytes(uint8_t *bytes, size_t size)
{
    for (size_t i = 0; i < size; i++)
    {
        bytes[i] = rand();
    }
}

uint64_t randomValue(uint64_t lower, uint64_t upper)
{
    uint64_t num = (rand() %
                    (upper - lower + 1)) +
                   lower;
    return num;
}

void create_empty_file(const char *filename, uint64_t size)
{
    char fullpath[256];
    vd_cwd(fullpath, sizeof(fullpath));
    strncat(fullpath, filename, sizeof(fullpath) - 1);

    FILE *create_f = fs->fopen(fullpath, "wb", 4096);
    if (create_f == NULL)
    {
        log_error("fopen() failed");
    }

    if (fs->seek(create_f, size, SEEK_SET) == -1)
    {
        log_error("fseek() failed");
    }

    if (fs->write(create_f, "\0", 1) != 1)
    {
        log_error("fputc() failed");
    }
    fs->close(create_f);
}

void test_write_sequential(bool sync, uint32_t block_size, struct statistics *stats)
{
    const char *filename = "test_file1.tmp";
    char fullpath[256];
    vd_cwd(fullpath, sizeof(fullpath));
    strncat(fullpath, filename, sizeof(fullpath) - 1);

    FILE *f = fs->fopen(fullpath, "wb", block_size);
    if (f == NULL)
    {
        log_error("fopen() failed");
    }
    uint8_t *buf = (uint8_t *)malloc(block_size);
    assert(buf);
    randomize_bytes(buf, block_size);

    uint32_t iops = 0;
    uint32_t total_time = 0;
    uint32_t written = 0;
    struct timeval tv_start;
    gettimeofday(&tv_start, NULL);

    while (written < max_file_size)
    {
        if (fs->write(f, buf, block_size) !=block_size)
        {
            log_error("Error writing to backing store\n");
        }
        if (sync)
        {
            // fflush(f);
        }
        written += block_size;
        iops++;
    }

    struct timeval tv_end;
    gettimeofday(&tv_end, NULL);

    float t_s = tv_end.tv_sec - tv_start.tv_sec + 1e-6f * (tv_end.tv_usec - tv_start.tv_usec);
    total_time += (t_s * 1e3);

    stats->sync = sync;
    stats->rw = true;
    stats->random = false;
    stats->block_size = block_size;
    stats->aligned = 0;

    stats->time = total_time;
    stats->bytes = written;
    stats->iops = iops;

    free(buf);
    // close(f);
    fs->close(f);
}

void test_write_random(bool sync, uint32_t block_size, struct statistics *stats)
{
    const char *test_filename = "test_file2.tmp";
    char fullpath[256];
    vd_cwd(fullpath, sizeof(fullpath));
    strncat(fullpath, test_filename, sizeof(fullpath) - 1);

    create_empty_file(test_filename, max_file_size);

    FILE *f = fs->fopen(fullpath, "r+b", block_size);
    if (f == NULL)
    {
        log_error("fopen() failed");
    }
    uint8_t *buf = (uint8_t *)malloc(block_size);
    assert(buf);
    randomize_bytes(buf, block_size);

    uint32_t iops = 0;
    uint32_t total_time = 0;
    uint32_t written = 0;
    srand(time(NULL));

    struct timeval tv_start;
    gettimeofday(&tv_start, NULL);

    while (written < max_file_size)
    {
        uint64_t location = randomValue(0, max_file_size - block_size);
        if (fs->seek(f, location, SEEK_SET) != location)
        {
            log_warn("Error seeking in backing store");
        }

        if (fs->write(f, buf, block_size) != block_size)
        {
            log_error("Error writing to backing store\n");
        }
        if (sync)
        {
            // fflush(f);
        }
        written += block_size;
        iops++;
    }

    struct timeval tv_end;
    gettimeofday(&tv_end, NULL);

    float t_s = tv_end.tv_sec - tv_start.tv_sec + 1e-6f * (tv_end.tv_usec - tv_start.tv_usec);
    total_time += (t_s * 1e3);

    stats->sync = sync;
    stats->rw = true;
    stats->random = true;
    stats->block_size = block_size;
    stats->aligned = 0;

    stats->time = total_time;
    stats->bytes = written;
    stats->iops = iops;

    free(buf);
    fs->close(f);
}
void test_write_random_aligned(bool sync, uint32_t block_size, struct statistics *stats)
{
    const char *test_filename = "test_file3.tmp";
    char fullpath[256];
    vd_cwd(fullpath, sizeof(fullpath));
    strncat(fullpath, test_filename, sizeof(fullpath) - 1);

    create_empty_file(test_filename, max_file_size);

    FILE *f = fs->fopen(fullpath, "r+b", block_size);
    if (f == NULL)
    {
        log_error("fopen() failed");
    }
    uint8_t *buf = (uint8_t *)malloc(block_size);
    assert(buf);
    randomize_bytes(buf, block_size);

    uint32_t iops = 0;
    uint32_t total_time = 0;
    uint32_t written = 0;
    srand(time(NULL));

    struct timeval tv_start;
    gettimeofday(&tv_start, NULL);

    while (written < max_file_size)
    {
        uint64_t location = randomValue(0, (max_file_size / block_size) - 1) * block_size;

        if (fs->seek(f, location, SEEK_SET) != location)
        {
            log_warn("Error seeking in backing store");
        }

        if (fs->write(f, buf, block_size) != block_size)
        {
            log_error("Error writing to backing store\n");
        }
        if (sync)
        {
            // fflush(f);
        }
        written += block_size;
        iops++;
    }

    struct timeval tv_end;
    gettimeofday(&tv_end, NULL);

    float t_s = tv_end.tv_sec - tv_start.tv_sec + 1e-6f * (tv_end.tv_usec - tv_start.tv_usec);
    total_time += (t_s * 1e3);

    stats->sync = sync;
    stats->rw = true;
    stats->random = true;
    stats->block_size = block_size;
    stats->aligned = block_size;

    stats->time = total_time;
    stats->bytes = written;
    stats->iops = iops;

    free(buf);
    fs->close(f);
}

void test_read_sequential(bool sync, uint32_t block_size, struct statistics *stats)
{
    const char *test_filename = "test_file.tmp";
    char fullpath[256];
    vd_cwd(fullpath, sizeof(fullpath));
    strncat(fullpath, test_filename, sizeof(fullpath) - 1);

    create_empty_file(test_filename, max_file_size);

    // printf("opening %s\r\n", fullpath);
    FILE *f = fs->fopen(fullpath, "r+b", block_size);
    // int f = open(fullpath, O_RDONLY,0644);
    if (f == NULL)
    {
        log_error("fopen() failed");
    }
    uint8_t *buf = (uint8_t *)malloc(block_size);
    assert(buf);
    randomize_bytes(buf, block_size);

    uint32_t iops = 0;
    uint32_t total_time = 0;
    uint32_t written = 0;
    srand(time(NULL));

    struct timeval tv_start;
    gettimeofday(&tv_start, NULL);

    while (written < max_file_size)
    {
        if (fs->read(f, buf, block_size) != block_size)
        {
            log_error("Error reading from backing store\n");
        }
        if (sync)
        {
            // fflush(f);
        }
        written += block_size;
        iops++;
    }

    struct timeval tv_end;
    gettimeofday(&tv_end, NULL);

    float t_s = tv_end.tv_sec - tv_start.tv_sec + 1e-6f * (tv_end.tv_usec - tv_start.tv_usec);
    total_time += (t_s * 1e3);

    stats->sync = sync;
    stats->rw = false;
    stats->random = false;
    stats->block_size = block_size;
    stats->aligned = 0;

    stats->time = total_time;
    stats->bytes = written;
    stats->iops = iops;

    free(buf);
    // close(f);
    fs->close(f);
}
void test_read_random(bool sync, uint32_t block_size, struct statistics *stats)
{
    const char *test_filename = "test_file.tmp";
    char fullpath[256];
    vd_cwd(fullpath, sizeof(fullpath));
    strncat(fullpath, test_filename, sizeof(fullpath) - 1);

    create_empty_file(test_filename, max_file_size);

    FILE *f = fs->fopen(fullpath, "r+b", block_size);
    if (f == NULL)
    {
        log_error("fopen() failed");
    }
    uint8_t *buf = (uint8_t *)malloc(block_size);
    assert(buf);
    randomize_bytes(buf, block_size);

    uint32_t iops = 0;
    uint32_t total_time = 0;
    uint32_t written = 0;
    srand(time(NULL));

    struct timeval tv_start;
    gettimeofday(&tv_start, NULL);

    while (written < max_file_size)
    {
        uint64_t location = randomValue(0, max_file_size - block_size);
        if (fs->seek(f, location, SEEK_SET) != location)
        {
            log_warn("Error seeking in backing store");
        }

        if (fs->read(f, buf, block_size) != block_size)
        {
            log_error("Error reading from backing store\n");
        }
        if (sync)
        {
            // fflush(f);
        }
        written += block_size;
        iops++;
    }

    struct timeval tv_end;
    gettimeofday(&tv_end, NULL);

    float t_s = tv_end.tv_sec - tv_start.tv_sec + 1e-6f * (tv_end.tv_usec - tv_start.tv_usec);
    total_time += (t_s * 1e3);

    stats->sync = sync;
    stats->rw = false;
    stats->random = true;
    stats->block_size = block_size;
    stats->aligned = 0;

    stats->time = total_time;
    stats->bytes = written;
    stats->iops = iops;

    free(buf);
    // close(f);
    fs->close(f);
}
void test_read_random_aligned(bool sync, uint32_t block_size, struct statistics *stats)
{
    const char *test_filename = "test_file.tmp";
    char fullpath[256];
    vd_cwd(fullpath, sizeof(fullpath));
    strncat(fullpath, test_filename, sizeof(fullpath) - 1);

    create_empty_file(test_filename, max_file_size);

    // printf("opening %s\r\n", fullpath);
    FILE *f = fs->fopen(fullpath, "r+b", block_size);
    if (f == NULL)
    {
        log_error("fopen() failed");
    }
    uint8_t *buf = (uint8_t *)malloc(block_size);
    assert(buf);
    randomize_bytes(buf, block_size);

    uint32_t iops = 0;
    uint32_t total_time = 0;
    uint32_t written = 0;
    srand(time(NULL));

    struct timeval tv_start;
    gettimeofday(&tv_start, NULL);

    while (written < max_file_size)
    {
        uint64_t location = randomValue(0, (max_file_size / block_size) - 1) * block_size;
        if (fs->seek(f, location, SEEK_SET) != location)
        {
            log_warn("Error seeking in backing store");
        }

        if (fs->read(f, buf, block_size) != block_size)
        {
            log_error("Error reading from backing store\n");
        }
        if (sync)
        {
            // fflush(f);
        }
        written += block_size;
        iops++;
    }

    struct timeval tv_end;
    gettimeofday(&tv_end, NULL);

    float t_s = tv_end.tv_sec - tv_start.tv_sec + 1e-6f * (tv_end.tv_usec - tv_start.tv_usec);
    total_time += (t_s * 1e3);

    stats->sync = sync;
    stats->rw = false;
    stats->random = true;
    stats->block_size = block_size;
    stats->aligned = block_size;

    stats->time = total_time;
    stats->bytes = written;
    stats->iops = iops;

    free(buf);
    // close(f);
    fs->close(f);
}
