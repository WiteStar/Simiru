#include "fix.h"
#include "crc_bruteforce.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>
#include <algorithm>

// 文件签名验证函数
bool validate_png_signature(const std::vector<uint8_t>& buf, size_t offset) {
    if (offset + 8 > buf.size()) return false;
    // PNG文件头
    return (buf[offset] == 0x89 && buf[offset + 1] == 0x50 && buf[offset + 2] == 0x4E &&
        buf[offset + 3] == 0x47 && buf[offset + 4] == 0x0D && buf[offset + 5] == 0x0A &&
        buf[offset + 6] == 0x1A && buf[offset + 7] == 0x0A);
}

bool validate_bmp_signature(const std::vector<uint8_t>& buf, size_t offset) {
    if (offset + 2 > buf.size()) return false;
    // BMP文件头
    if (buf[offset] != 'B' || buf[offset + 1] != 'M') return false;

    // 检查文件大小是否合理
    if (offset + 6 < buf.size()) {
        uint32_t file_size = (buf[offset + 2] << 0) | (buf[offset + 3] << 8) |
            (buf[offset + 4] << 16) | (buf[offset + 5] << 24);
        if (file_size < 14 || file_size > 100 * 1024 * 1024) return false;
    }

    return true;
}

bool validate_jpg_signature(const std::vector<uint8_t>& buf, size_t offset) {
    if (offset + 2 > buf.size()) return false;
    // JPEG文件头
    if (buf[offset] != 0xFF || buf[offset + 1] != 0xD8) return false;

    // 检查下一个标记是否合理
    if (offset + 4 < buf.size()) {
        if (buf[offset + 2] == 0xFF) {
            uint8_t marker = buf[offset + 3];
            // 合理的JPEG标记
            return (marker >= 0xC0 && marker <= 0xFE) || marker == 0xE0 || marker == 0xE1;
        }
    }

    return false;
}

bool validate_gif_signature(const std::vector<uint8_t>& buf, size_t offset) {
    if (offset + 6 > buf.size()) return false;
    // GIF文件头
    return (buf[offset] == 'G' && buf[offset + 1] == 'I' && buf[offset + 2] == 'F' &&
        buf[offset + 3] == '8' && (buf[offset + 4] == '7' || buf[offset + 4] == '9') &&
        buf[offset + 5] == 'a');
}

bool validate_zip_signature(const std::vector<uint8_t>& buf, size_t offset) {
    if (offset + 4 > buf.size()) return false;
    // ZIP文件头
    if (buf[offset] != 0x50 || buf[offset + 1] != 0x4B || buf[offset + 2] != 0x03 || buf[offset + 3] != 0x04)
        return false;

    // 检查版本号是否合理
    if (offset + 6 < buf.size()) {
        uint8_t version = buf[offset + 4];
        if (version > 63) return false; // 版本号应该不会太大
    }

    return true;
}

bool validate_pdf_signature(const std::vector<uint8_t>& buf, size_t offset) {
    if (offset + 8 > buf.size()) return false;
    // PDF文件头
    std::string header(reinterpret_cast<const char*>(&buf[offset]), 8);
    if (header.find("%PDF-") != 0) return false;

    // 检查版本号是否合理
    char version_char = header[5];
    return (version_char >= '0' && version_char <= '9');
}

bool validate_exe_signature(const std::vector<uint8_t>& buf, size_t offset) {
    if (offset + 2 > buf.size()) return false;
    // EXE/DLL文件头 (MZ签名)
    if (buf[offset] != 0x4D || buf[offset + 1] != 0x5A) return false;

    // 检查PE头偏移是否合理
    if (offset + 0x3C < buf.size()) {
        uint32_t pe_offset = (buf[offset + 0x3C] << 0) | (buf[offset + 0x3D] << 8) |
            (buf[offset + 0x3C + 2] << 16) | (buf[offset + 0x3C + 3] << 24);
        if (pe_offset < 64 || pe_offset > 4096) return false;
    }

    return true;
}

bool validate_generic_signature(const std::vector<uint8_t>& buf, size_t offset, const std::string& type) {
    // 对于没有专用验证的类型，检查文件是否有合理的大小
    // 避免匹配到文件内部的随机字节
    return (offset + 100 < buf.size());
}

bool validate_file_signature(const std::vector<uint8_t>& buf, size_t offset, const std::string& type) {
    if (type == "png") {
        return validate_png_signature(buf, offset);
    }
    else if (type == "bmp") {
        return validate_bmp_signature(buf, offset);
    }
    else if (type == "jpg" || type == "jpeg") {
        return validate_jpg_signature(buf, offset);
    }
    else if (type == "gif") {
        return validate_gif_signature(buf, offset);
    }
    else if (type == "zip") {
        return validate_zip_signature(buf, offset);
    }
    else if (type == "pdf" || type == "ai") {
        return validate_pdf_signature(buf, offset);
    }
    else if (type == "exe" || type == "dll") {
        return validate_exe_signature(buf, offset);
    }

    // 对于其他类型，使用基本验证
    return validate_generic_signature(buf, offset, type);
}

FileFixInfo analyze_and_fix(const std::vector<uint8_t>& buf, size_t offset, const std::string& type)
{
    FileFixInfo info;
    info.type = type;
    info.offset = offset;

    if (type == "png")
    {
        return analyze_png(buf, offset);
    }
    else if (type == "zip" || type == "docx" || type == "xlsx" || type == "pptx")
    {
        return analyze_zip(buf, offset);
    }
    else if (type == "jpg" || type == "jpeg")
    {
        return analyze_jpg(buf, offset);
    }
    else if (type == "pdf" || type == "ai")
    {
        return analyze_pdf(buf, offset);
    }
    else if (type == "gif")
    {
        return analyze_gif(buf, offset);
    }
    else if (type == "bmp")
    {
        return analyze_bmp(buf, offset);
    }

    return info;
}

FileFixInfo analyze_png(const std::vector<uint8_t>& buf, size_t offset)
{
    FileFixInfo info;

    // 检查文件头完整性
    if (offset + 8 > buf.size())
    {
        info.warnings.push_back("PNG header incomplete");
        return info;
    }

    uint32_t actual_width = 0, actual_height = 0;
    bool has_valid_ihdr = false;

    // 获取实际分辨率（从IHDR块读取）
    for (size_t i = offset + 8; i < buf.size() - 20; i++)
    {
        if (buf[i] == 'I' && buf[i + 1] == 'H' && buf[i + 2] == 'D' && buf[i + 3] == 'R')
        {
            if (i + 16 < buf.size())
            {
                actual_width = (buf[i + 4] << 24) | (buf[i + 5] << 16) | (buf[i + 6] << 8) | buf[i + 7];
                actual_height = (buf[i + 8] << 24) | (buf[i + 9] << 16) | (buf[i + 10] << 8) | buf[i + 11];

                info.metadata.push_back("actual:" + std::to_string(actual_width) + "x" + std::to_string(actual_height));
                info.metadata.push_back("bit:" + std::to_string(buf[i + 12]));
                has_valid_ihdr = true;
            }
            break;
        }
    }

    // CRC爆破获取可能的分辨率
    if (has_valid_ihdr) {
        auto possible_dims = brute_force_png_crc(buf, offset);
        if (!possible_dims.empty()) {
            std::string crc_info = "crc_fix:";
            for (size_t i = 0; i < possible_dims.size(); i++) {
                if (i > 0) crc_info += "/";
                crc_info += std::to_string(possible_dims[i].first) + "x" + std::to_string(possible_dims[i].second);

                // 标记与实际分辨率不同的结果
                if (possible_dims[i].first != static_cast<int>(actual_width) ||
                    possible_dims[i].second != static_cast<int>(actual_height)) {
                    crc_info += "*";  // 星号表示与实际不同
                }
            }
            info.metadata.push_back(crc_info);
        }
    }

    // 检查IEND块
    bool has_iend = false;
    for (size_t i = offset; i < buf.size() - 3; i++)
    {
        if (buf[i] == 'I' && buf[i + 1] == 'E' && buf[i + 2] == 'N' && buf[i + 3] == 'D')
        {
            has_iend = true;
            break;
        }
    }

    if (!has_iend)
    {
        info.warnings.push_back("Missing IEND chunk");
    }

    return info;
}

FileFixInfo analyze_zip(const std::vector<uint8_t>& buf, size_t offset)
{
    FileFixInfo info;

    if (check_zip_pseudo_encryption(buf, offset))
    {
        info.warnings.push_back("Pseudo-encryption detected");
    }

    return info;
}

FileFixInfo analyze_jpg(const std::vector<uint8_t>& buf, size_t offset)
{
    FileFixInfo info;

    // 检查文件头
    if (buf[offset] != 0xFF || buf[offset + 1] != 0xD8)
    {
        info.warnings.push_back("JPEG header corrupted");
    }

    // 检查EOI标记
    bool has_eoi = false;
    for (size_t i = offset + 2; i < buf.size() - 1; i++)
    {
        if (buf[i] == 0xFF && buf[i + 1] == 0xD9)
        {
            has_eoi = true;
            break;
        }
    }

    if (!has_eoi)
    {
        info.warnings.push_back("Missing EOI marker");
    }

    return info;
}

FileFixInfo analyze_pdf(const std::vector<uint8_t>& buf, size_t offset)
{
    FileFixInfo info;

    // 检查文件头
    if (offset + 8 > buf.size())
    {
        info.warnings.push_back("PDF header incomplete");
        return info;
    }

    // 获取版本信息
    std::string version(reinterpret_cast<const char*>(&buf[offset]), 8);
    info.metadata.push_back(version);

    // 检查EOF标记
    bool has_eof = false;
    for (size_t i = offset; i < buf.size() - 4; i++)
    {
        if (buf[i] == '%' && buf[i + 1] == '%' && buf[i + 2] == 'E' && buf[i + 3] == 'O' && buf[i + 4] == 'F')
        {
            has_eof = true;
            break;
        }
    }

    if (!has_eof)
    {
        info.warnings.push_back("Missing EOF marker");
    }

    return info;
}

FileFixInfo analyze_gif(const std::vector<uint8_t>& buf, size_t offset)
{
    FileFixInfo info;

    // 检查文件头
    if (offset + 10 > buf.size())
    {
        info.warnings.push_back("GIF header incomplete");
        return info;
    }

    // 验证GIF签名
    if (buf[offset] != 'G' || buf[offset + 1] != 'I' || buf[offset + 2] != 'F') {
        info.warnings.push_back("Invalid GIF signature");
        return info;
    }

    // 获取GIF版本和尺寸
    if (offset + 13 < buf.size())
    {
        uint16_t width = (buf[offset + 6] << 8) | buf[offset + 7];  // GIF使用大端序
        uint16_t height = (buf[offset + 8] << 8) | buf[offset + 9]; // GIF使用大端序

        // 只显示合理的尺寸
        if (width >= 1 && width <= 10000 && height >= 1 && height <= 10000) {
            info.metadata.push_back(std::to_string(width) + "x" + std::to_string(height));

            // 获取版本信息
            std::string version(reinterpret_cast<const char*>(&buf[offset + 4]), 3);
            info.metadata.push_back("ver:" + version);
        }
    }

    // 检查结束符
    bool has_end = false;
    for (size_t i = offset; i < buf.size(); i++)
    {
        if (buf[i] == 0x3B)
        {
            has_end = true;
            break;
        }
    }

    if (!has_end)
    {
        info.warnings.push_back("Missing GIF terminator");
    }

    return info;
}

FileFixInfo analyze_bmp(const std::vector<uint8_t>& buf, size_t offset)
{
    FileFixInfo info;

    // 检查文件头完整性
    if (offset + 18 > buf.size())
    {
        info.warnings.push_back("BMP header incomplete");
        return info;
    }

    // 验证BMP文件头签名
    if (buf[offset] != 'B' || buf[offset + 1] != 'M') {
        info.warnings.push_back("Invalid BMP signature");
        return info;
    }

    // 获取BMP尺寸 - BMP使用小端序！
    if (offset + 26 < buf.size())
    {
        // 宽度（小端序）
        uint32_t width = (buf[offset + 18] << 0) | (buf[offset + 19] << 8) |
            (buf[offset + 20] << 16) | (buf[offset + 21] << 24);

        // 高度（小端序）
        uint32_t height = (buf[offset + 22] << 0) | (buf[offset + 23] << 8) |
            (buf[offset + 24] << 16) | (buf[offset + 25] << 24);

        // 严格的尺寸验证
        if (width >= 1 && width <= 10000 && height >= 1 && height <= 10000) {
            info.metadata.push_back(std::to_string(width) + "x" + std::to_string(height));

            // 获取位深度（小端序）
            if (offset + 30 < buf.size()) {
                uint16_t bit_count = (buf[offset + 28] << 0) | (buf[offset + 29] << 8);
                if (bit_count == 1 || bit_count == 4 || bit_count == 8 ||
                    bit_count == 16 || bit_count == 24 || bit_count == 32) {
                    info.metadata.push_back("bit:" + std::to_string(bit_count));
                }
                else {
                    info.warnings.push_back("Invalid bit depth");
                }
            }
        }
        else {
            info.warnings.push_back("Invalid dimensions");
            // 不显示无效的尺寸数据
        }
    }

    return info;
}

bool check_zip_pseudo_encryption(const std::vector<uint8_t>& buf, size_t offset)
{
    if (offset + 6 >= buf.size()) return false;
    uint16_t flags = (buf[offset + 6] << 8) | buf[offset + 7];
    return (flags & 0x0001) != 0;
}