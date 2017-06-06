#pragma once

#include <stdint.h>

extern "C" {
uint32_t crc32_zlib(const void* data, size_t length, uint32_t previousCrc32);
}