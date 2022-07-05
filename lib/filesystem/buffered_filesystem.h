# pragma once

#include "filesystem_struct.h"
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif
extern const filesystem buffered_filesystem;
void set_override_buffer_size(uint32_t override_size);
#ifdef __cplusplus
}
#endif