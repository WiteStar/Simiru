#pragma once
#include <vector>
#include <cstdint>
#include <utility>

// CRC32¼ÆËãº¯Êý
uint32_t calculate_crc32(const uint8_t* data, size_t length);

// PNG CRC±¬ÆÆº¯Êý
std::vector<std::pair<int, int>> brute_force_png_crc(const std::vector<uint8_t>& buf, size_t offset);