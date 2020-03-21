#pragma once

#include "common.hpp"

#include <fstream>
#include <vector>
#include <string_view>
#include <string>
#include <algorithm>


struct SplitRange{
    std::size_t begin=0;
    std::size_t end=0;
};

using FileRanges = std::vector<SplitRange>;

bool checkPath(const std::string& file);

std::size_t fileSize(const std::string& fname);

std::size_t align_by_endl(std::istream& is, std::size_t rough_pos);

FileRanges splitFile(const std::string& fname, std::size_t chunk_sz);

FileRanges splitStream(std::istream& is, std::size_t chunk_num, std::size_t fsize);

std::size_t getHash(const std::string& str);


std::vector<std::string> str2prefixes(std::string);

class PrefixSplitter{
public:
    yamr::MappedWord operator()(std::string str);
};

class MinPrefix{
public:
    std::size_t operator()(std::string_view sv);
private:
    std::size_t min_pref=0;
    std::string_view prevs;
};

