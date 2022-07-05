#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

// void mount_sd();
void unmound_sd();

void mount_sd_spi();
void mount_sd_sdmmc();

#ifdef __cplusplus
}
#endif