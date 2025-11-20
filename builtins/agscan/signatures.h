#pragma once
#include <string>
#include <vector>
#include <map>

struct Signature {
    std::string ext;                 // e.g. "png"
    std::vector<uint8_t> bytes;      // magic bytes
};

extern std::map<std::string, Signature> signatures;
