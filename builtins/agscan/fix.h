#pragma once
#include <vector>
#include <string>
#include <cstdint>

struct FileFixInfo {
    std::string type;
    size_t offset;
    std::vector<std::string> suggestions;  // 修复建议
    std::vector<std::string> warnings;     // 警告信息
    std::vector<std::string> metadata;     // 元数据信息
    bool repairable = false;               // 是否可修复
};

// 文件签名验证函数声明
bool validate_file_signature(const std::vector<uint8_t>& buf, size_t offset, const std::string& type);

// 修复功能接口
FileFixInfo analyze_and_fix(const std::vector<uint8_t>& buf, size_t offset, const std::string& type);

// 各格式的专用分析函数
FileFixInfo analyze_png(const std::vector<uint8_t>& buf, size_t offset);
FileFixInfo analyze_zip(const std::vector<uint8_t>& buf, size_t offset);
FileFixInfo analyze_jpg(const std::vector<uint8_t>& buf, size_t offset);
FileFixInfo analyze_gif(const std::vector<uint8_t>& buf, size_t offset);
FileFixInfo analyze_bmp(const std::vector<uint8_t>& buf, size_t offset);
FileFixInfo analyze_pdf(const std::vector<uint8_t>& buf, size_t offset);

// 工具函数
bool check_zip_pseudo_encryption(const std::vector<uint8_t>& buf, size_t offset);