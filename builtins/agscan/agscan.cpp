#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>
#include <filesystem>
#include <algorithm>
#include <map>
#include "signatures.h"
#include "fix.h"

namespace fs = std::filesystem;

struct FoundFile {
    std::string type;
    size_t offset;
    FileFixInfo fix_info;
};

void print_help() {
    std::cout << "Usage:\n"
        "  scan -i <input_file> scan|extract [options]\n"
        "Modes:\n"
        "  scan            List embedded files with integrity checks\n"
        "  extract         Extract detected files\n"
        "Common options:\n"
        "  -i <input>      Input file absolute path (required)\n"
        "  -o <output>     Output folder absolute path (default ./out_scan)\n"
        "  -all            Show all possible files (including false positives)\n"
        "  -png -jpg -zip  Pick specific types (can repeat)\n"
        "  -help           Show this help\n\n";

    std::cout << "Only files with valid structure are shown.\n"
        "Use -all to see all possible matches (may include false or too many positives).\n\n";
}

std::vector<uint8_t> read_file(const std::string& path)
{
    std::ifstream ifs(path, std::ios::binary);
    if (!ifs)
        throw std::runtime_error("Cannot open input file");
    return std::vector<uint8_t>((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
}

bool match_at(const std::vector<uint8_t>& data, size_t pos, const Signature& sig) {
    if (pos + sig.bytes.size() > data.size())
        return false;

    for (size_t i = 0; i < sig.bytes.size(); i++)
    {
        if (data[pos + i] != sig.bytes[i])
            return false;
    }

    return true;
}

std::vector<FoundFile> scan_file(const std::vector<uint8_t>& buf, const std::vector<std::string>& want_types, bool show_all = false)
{
    std::vector<FoundFile> out;
    std::vector<std::pair<std::string, const Signature*>> signatures_to_scan;

    for (auto& kv : signatures)
    {
        const std::string& type = kv.first;
        if (!want_types.empty() && std::find(want_types.begin(), want_types.end(), type) == want_types.end())
            continue;
        signatures_to_scan.push_back({ type, &kv.second });
    }

    std::sort(signatures_to_scan.begin(), signatures_to_scan.end(),
        [](const auto& a, const auto& b) {return a.second->bytes.size() > b.second->bytes.size(); });

    for (size_t i = 0; i < buf.size(); i++)
    {
        for (auto& [type, sig_ptr] : signatures_to_scan)
        {
            // 在 scan_file 函数中的匹配逻辑修改为：
            if (match_at(buf, i, *sig_ptr))
            {
                // 新增：在扫描阶段就进行文件验证
                if (validate_file_signature(buf, i, type)) {
                    FoundFile found_file;
                    found_file.type = type;
                    found_file.offset = i;
                    found_file.fix_info = analyze_and_fix(buf, i, type);

                    // all会显示所有可能的文件类型，一般情况不建议添加（真的会输出很多）
                    if (show_all || found_file.fix_info.warnings.empty()) {
                        out.push_back(found_file);
                    }
                }

                // 跳过已匹配的区域
                i += sig_ptr->bytes.size() - 1;
                break;
            }
        }
    }
    return out;
}

void extract_files(const std::vector<uint8_t>& buf, const std::vector<FoundFile>& found, const std::string& outdir)
{
    fs::create_directories(outdir);
    std::map<std::string, int> counter;

    for (auto& f : found)
    {
        counter[f.type]++;
        std::string name = f.type + std::to_string(counter[f.type]);
        std::string outfile = outdir + "/" + name;
        std::ofstream ofs(outfile, std::ios::binary);

        if (!ofs)
        {
            std::cout << name << " 0x" << std::hex << f.offset << " output failed\n";
            continue;
        }

        // Extract until EOF
        for (size_t i = f.offset; i < buf.size(); i++)
        {
            ofs.put(buf[i]);
        }

        std::cout << name << " 0x" << std::hex << f.offset << " output succeed\n";
    }
}

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        print_help();
        return 0;
    }

    std::string mode = argv[1];
    if (mode == "help")
    {
        print_help();
        return 0;
    }

    std::string inpath, outpath;
    bool scan_mode = false;
    bool extract_mode = false;
    bool want_all = false;

    std::vector<std::string> want_types;

    for (int i = 2; i < argc; i++)
    {
        std::string p = argv[i];
        if (p == "-i" && i + 1 < argc) inpath = argv[++i];
        else if (p == "-o" && i + 1 < argc) outpath = argv[++i];
        else if (p == "-all") want_all = true;
        else if (p[0] == '-') want_types.push_back(p.substr(1));
    }

    if (mode == "scan") scan_mode = true;
    if (mode == "extract") extract_mode = true;  // 修正：应该是 "extract" 不是 "file"

    if (inpath.empty())
    {
        std::cout << "Missing -i input file.\n";
        return 0;
    }

    if (extract_mode && outpath.empty())
    {
        std::cout << "Missing -o output directory.\n";
        return 0;
    }

    if (want_all) want_types.clear();

    auto buf = read_file(inpath);
    auto found = scan_file(buf, want_types, want_all);  // 修正：添加 want_all 参数

    if (scan_mode) {
        for (auto& f : found) {
            std::cout << std::left << std::setw(6) << f.type
                << " 0x" << std::hex << f.offset << std::dec;

            // 元数据
            if (!f.fix_info.metadata.empty()) {
                std::cout << " [";
                for (size_t i = 0; i < f.fix_info.metadata.size(); i++) {
                    if (i > 0) std::cout << " ";
                    std::cout << f.fix_info.metadata[i];
                }
                std::cout << "]";
            }

            if (!f.fix_info.warnings.empty()) {
                std::cout << " (warning)";
            }

            std::cout << "\n";

            if (!f.fix_info.warnings.empty()) {
                for (const auto& warning : f.fix_info.warnings) {
                    std::cout << "      " << warning << "\n";
                }
            }
        }
    }

    if (extract_mode)
    {
        extract_files(buf, found, outpath);
    }

    return 0;
}