#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>
#include <filesystem>
#include <algorithm>
#include "signatures.h"

namespace fs = std::filesystem;

struct FoundFile {
    std::string type;
    size_t offset;
};

void print_help() {
    std::cout << "Usage:\n"
        "  scan -i <input_file> scan|extract [options]\n"
        "Modes:\n"
        "  scan            list possible embedded files and simple integrity checks\n"
        "  extract         extract detected possible files (default smart mode)\n"
        "Common options:\n"
        "  -i <input>      input file absolute path (required)\n"
        "  -o <output>     output folder absolute path (default ./out_scan)\n"
        "  -all            consider all supported types\n"
        "  -png -jpg -zip  pick specific types (can repeat)\n"
        "  -smart          smart extraction (default)\n"
        "  -offset         extract by offset: requires -s and -e or -s and -length\n"
        "  -s <start>      start offset (decimal or 0xhex)\n"
        "  -e <end>        end offset (decimal or 0xhex, inclusive)\n"
        "  -length <len>   length in bytes (eg 1024 or 4K/2M)\n"
        "  -help           show this help\n\n";
}

std::vector<uint8_t> read_file(const std::string& path) {
    std::ifstream ifs(path, std::ios::binary);
    if (!ifs) throw std::runtime_error("Cannot open input file");
    return std::vector<uint8_t>((std::istreambuf_iterator<char>(ifs)),
        std::istreambuf_iterator<char>());
}

bool match_at(const std::vector<uint8_t>& data, size_t pos, const Signature& sig) {
    if (pos + sig.bytes.size() > data.size()) return false;
    for (size_t i = 0; i < sig.bytes.size(); i++) {
        if (data[pos + i] != sig.bytes[i])
            return false;
    }
    return true;
}

std::vector<FoundFile> scan_file(const std::vector<uint8_t>& buf,
    const std::vector<std::string>& want_types)
{
    std::vector<FoundFile> out;

    for (auto& kv : signatures) {
        const std::string& type = kv.first;
        const Signature& sig = kv.second;

        if (!want_types.empty() &&
            std::find(want_types.begin(), want_types.end(), type) == want_types.end())
            continue;

        for (size_t i = 0; i < buf.size(); i++) {
            if (match_at(buf, i, sig)) {
                out.push_back({ type, i });
            }
        }
    }
    return out;
}

void extract_files(const std::vector<uint8_t>& buf,
    const std::vector<FoundFile>& found,
    const std::string& outdir)
{
    fs::create_directories(outdir);

    std::map<std::string, int> counter;

    for (auto& f : found) {
        counter[f.type]++;
        std::string name = f.type + std::to_string(counter[f.type]);

        std::string outfile = outdir + "/" + name;

        std::ofstream ofs(outfile, std::ios::binary);
        if (!ofs) {
            std::cout << name << " 0x" << std::hex << f.offset << " output failed\n";
            continue;
        }

        // Extract until EOF
        for (size_t i = f.offset; i < buf.size(); i++) {
            ofs.put(buf[i]);
        }

        std::cout << name << " 0x" << std::hex << f.offset << " output succeed\n";
    }
}

int main(int argc, char* argv[])
{
    if (argc < 2) {
        print_help();
        return 0;
    }

    std::string mode = argv[1];
    if (mode == "help") {
        print_help();
        return 0;
    }

    std::string inpath, outpath;
    bool scan_mode = false;
    bool extract_mode = false;
    bool want_all = false;

    std::vector<std::string> want_types;

    for (int i = 2; i < argc; i++) {
        std::string p = argv[i];
        if (p == "-i" && i + 1 < argc) inpath = argv[++i];
        else if (p == "-o" && i + 1 < argc) outpath = argv[++i];
        else if (p == "-all") want_all = true;
        else if (p[0] == '-') want_types.push_back(p.substr(1));
    }

    if (mode == "scan") scan_mode = true;
    if (mode == "file") extract_mode = true;

    if (inpath.empty()) {
        std::cout << "Missing -i input file.\n";
        return 0;
    }

    if (extract_mode && outpath.empty()) {
        std::cout << "Missing -o output directory.\n";
        return 0;
    }

    if (want_all) want_types.clear();

    auto buf = read_file(inpath);
    auto found = scan_file(buf, want_types);

    if (scan_mode) {
        for (auto& f : found) {
            std::cout << std::left << std::setw(6) << f.type
                << " 0x" << std::hex << f.offset << "\n";
        }
    }

    if (extract_mode) {
        extract_files(buf, found, outpath);
    }

    return 0;
}
