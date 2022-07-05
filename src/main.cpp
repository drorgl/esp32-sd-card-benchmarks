#include <runner.h>

#include <log.h>

#define NELEMS(x) (sizeof(x) / sizeof((x)[0]))

#ifdef ESP32
#include "sd_mount.h"
#endif

#include <benchmark_tester.h>
#include <benchmark_printout.h>

#include <unbuffered_filesystem.h>
#include <buffered_filesystem.h>

uint32_t block_size[] = {512, 1024, 2 * 1024, 4 * 1024, 8 * 1024, 16 * 1024, 32 * 1024, 64 * 1024};
// bool syncs[] = {false, true};
bool syncs[] = {false};

void execute_full_benchmark()
{
    ConsoleTable table{"Block", "R/W", "Rnd/Seq", "Sync", "Aligned", "time", "iops", "bytes", "bps"};
    table.setPadding(2);
    table.setStyle(1);

    // write sequential
    for (uint32_t s = 0; s < NELEMS(syncs); s++)
    {
        for (uint32_t i = 0; i < NELEMS(block_size); i++)
        {
            struct statistics stats;
            test_write_sequential(syncs[s], block_size[i], &stats);
            print(stats);
            addTable(table, stats);
            printf("\r\n");
            test_read_sequential(syncs[s], block_size[i], &stats);
            print(stats);
            addTable(table, stats);
            printf("\r\n");
        }
    }
    // write random
    for (uint32_t s = 0; s < NELEMS(syncs); s++)
    {
        for (uint32_t i = 0; i < NELEMS(block_size); i++)
        {
            struct statistics stats;
            test_write_random(syncs[s], block_size[i], &stats);
            print(stats);
            addTable(table, stats);
            printf("\r\n");
            test_read_random(syncs[s], block_size[i], &stats);
            print(stats);
            addTable(table, stats);
            printf("\r\n");
        }
    }

    for (uint32_t s = 0; s < NELEMS(syncs); s++)
    {
        for (uint32_t i = 0; i < NELEMS(block_size); i++)
        {
            struct statistics stats;
            test_write_random_aligned(syncs[s], block_size[i], &stats);
            print(stats);
            addTable(table, stats);
            printf("\r\n");
            test_read_random_aligned(syncs[s], block_size[i], &stats);
            print(stats);
            addTable(table, stats);
            printf("\r\n");
        }
    }

    std::cout << table;
}

void execute_filesystem_tests()
{
    printf("Executing default newlib buffer size buffered test...\n");
    set_override_buffer_size(128);
    set_filesystem(&buffered_filesystem);
    execute_full_benchmark();

    printf("Executing 2048 buffer size buffered test...\n");
    set_override_buffer_size(2048);
    set_filesystem(&buffered_filesystem);
    execute_full_benchmark();

    printf("Executing 4096 buffer size buffered test...\n");
    set_override_buffer_size(4096);
    set_filesystem(&buffered_filesystem);
    execute_full_benchmark();

    printf("Executing unbuffered test...\n");
    set_filesystem(&unbuffered_filesystem);
    execute_full_benchmark();
}

MAIN()
{
    printf("Starting...\r\n");

#ifdef ESP32
    printf("Testing using SPI...\n");
    mount_sd_spi();
    execute_filesystem_tests();
    unmound_sd();

    printf("Testing using SDMMC...\n");
    mount_sd_sdmmc();
    execute_filesystem_tests();
    unmound_sd();
#else
    execute_filesystem_tests();
#endif
    return 0;
}