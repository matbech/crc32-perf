#pragma once

#include <stdint.h>

extern "C" {
uint32_t crc32_16bytes(const void* data, size_t length, uint32_t previousCrc32);
uint32_t crc32_8bytes(const void* data, size_t length, uint32_t previousCrc32);
uint32_t crc32_4bytes(const void* data, size_t length, uint32_t previousCrc32);
}