#pragma once
#include <vector>
#include <set>

#include "utils.hpp"
#include "common.hpp"

namespace yamr{

class Mapper{
public:
    Mapper(const std::string& fname, std::size_t nthreads);
    MLists Run();
    MapFileList Run(const std::string& outfname);

private:
    std::string inputFile;
    std::size_t m_nthreads;
};


}