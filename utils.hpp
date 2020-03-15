#pragma once

#include "common.hpp"

#include <fstream>
#include <vector>

struct SplitRange{
    std::size_t begin=0;
    std::size_t end=0;
};

using FileRanges = std::vector<SplitRange>;

std::size_t fileSize(const std::string& fname);

std::size_t align_by_endl(std::istream& is, std::size_t rough_pos);

FileRanges splitFile(const std::string& fname, std::size_t chunk_sz);

FileRanges splitStream(std::istream& is, std::size_t chunk_num, std::size_t fsize);

std::size_t getHash(const std::string& str);

class CollapseDups{
public:
    void operator()(std::string);
    yamr::ReduceList get();
private:
    yamr::ReduceList rlist;
};


