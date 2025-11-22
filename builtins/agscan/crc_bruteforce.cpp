#include "crc_bruteforce.h"
#include <iostream>
#include <vector>
#include <cstdint>
#include <utility>

// 使用与Python binascii.crc32兼容的CRC32实现
uint32_t calculate_crc32(const uint8_t* data, size_t length) {
    uint32_t crc = 0xFFFFFFFF;

    for (size_t i = 0; i < length; i++) {
        crc ^= data[i];
        for (int j = 0; j < 8; j++) {
            if (crc & 1) {
                crc = (crc >> 1) ^ 0xEDB88320;
            }
            else {
                crc >>= 1;
            }
        }
    }

    return crc ^ 0xFFFFFFFF;
}

std::vector<std::pair<int, int>> brute_force_png_crc(const std::vector<uint8_t>& buf, size_t offset) {
    std::vector<std::pair<int, int>> results;

    // 查找IHDR块位置
    size_t ihdr_pos = 0;
    for (size_t i = offset + 8; i < buf.size() - 20; i++) {
        if (buf[i] == 'I' && buf[i + 1] == 'H' && buf[i + 2] == 'D' && buf[i + 3] == 'R') {
            ihdr_pos = i;
            break;
        }
    }

    if (ihdr_pos == 0) return results;

    // 提取CRC值
    size_t crc_pos = ihdr_pos + 4 + 13; // 跳过"IHDR"和13字节数据
    if (crc_pos + 3 >= buf.size()) return results;

    uint32_t actual_crc = (buf[crc_pos] << 24) | (buf[crc_pos + 1] << 16) |
        (buf[crc_pos + 2] << 8) | buf[crc_pos + 3];

    // 提取当前宽度和高度
    size_t width_pos = ihdr_pos + 4;
    uint32_t current_width = (buf[width_pos] << 24) | (buf[width_pos + 1] << 16) |
        (buf[width_pos + 2] << 8) | buf[width_pos + 3];
    uint32_t current_height = (buf[width_pos + 4] << 24) | (buf[width_pos + 5] << 16) |
        (buf[width_pos + 6] << 8) | buf[width_pos + 7];

    // 提取IHDR数据块中的其他参数
    std::vector<uint8_t> other_params;
    size_t other_params_pos = width_pos + 8;
    for (int i = 0; i < 5; i++) {
        if (other_params_pos + i < buf.size()) {
            other_params.push_back(buf[other_params_pos + i]);
        }
        else {
            return results;
        }
    }

    // 常见分辨率列表
    std::vector<std::pair<int, int>> common_resolutions = {
        {3456, 2600}, {3456, 2160},  // 您遇到的实际尺寸
        {1920, 1080}, {1280, 720}, {1366, 768}, {1600, 900},
        {2560, 1440}, {3840, 2160}, {1920, 1200}, {1680, 1050},
        {1440, 900}, {1280, 800}, {1024, 768}, {800, 600},
        {3456, 2304}, {3456, 1944}, {3456, 2234}, {3456, 2260},
        {3456, 2180}, {3456, 2140}, {3456, 2200}, {3456, 2080},
        {3000, 2000}, {4000, 3000}, {2592, 1944}, {3264, 2448}
    };

    // 检查常见分辨率
    for (const auto& res : common_resolutions) {
        // 构建IHDR数据块数据
        std::vector<uint8_t> chunk_data;

        // 添加"IHDR"类型
        chunk_data.push_back('I');
        chunk_data.push_back('H');
        chunk_data.push_back('D');
        chunk_data.push_back('R');

        // 添加宽度（大端）
        chunk_data.push_back((res.first >> 24) & 0xFF);
        chunk_data.push_back((res.first >> 16) & 0xFF);
        chunk_data.push_back((res.first >> 8) & 0xFF);
        chunk_data.push_back(res.first & 0xFF);

        // 添加高度（大端）
        chunk_data.push_back((res.second >> 24) & 0xFF);
        chunk_data.push_back((res.second >> 16) & 0xFF);
        chunk_data.push_back((res.second >> 8) & 0xFF);
        chunk_data.push_back(res.second & 0xFF);

        // 添加其他参数
        for (uint8_t param : other_params) {
            chunk_data.push_back(param);
        }

        // 计算CRC32
        uint32_t calculated_crc = calculate_crc32(chunk_data.data(), chunk_data.size());

        if (calculated_crc == actual_crc) {
            results.push_back(res);
        }
    }

    return results;
}