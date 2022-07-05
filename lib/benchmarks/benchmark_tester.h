#pragma once

#include <filesystem_struct.h>

#include "statistics.h"
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

void set_filesystem(const filesystem *_filesystem);

void create_empty_file(const char *filename, uint64_t size);

void test_write_sequential(bool sync, uint32_t block_size, struct statistics *stats);
void test_write_random(bool sync, uint32_t block_size, struct statistics *stats);
void test_write_random_aligned(bool sync, uint32_t block_size, struct statistics *stats);

void test_read_sequential(bool sync, uint32_t block_size, struct statistics *stats);
void test_read_random(bool sync, uint32_t block_size, struct statistics *stats);
void test_read_random_aligned(bool sync, uint32_t block_size, struct statistics *stats);

#ifdef __cplusplus
}
#endif