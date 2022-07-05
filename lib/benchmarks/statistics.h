#pragma once

#include <stdint.h>
#include <stdbool.h>

struct statistics
{
    bool random;
    bool sync;
    uint32_t block_size;
    bool rw;
    uint32_t aligned;
    uint64_t time;
    uint64_t bytes;
    uint64_t iops;
};