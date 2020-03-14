#pragma once

#include "common.hpp"

#include <map>
#include <vector>
#include <functional>

namespace yamr{

class Reducer{
public:
    Reducer(std::string ofile, std::size_t rthreads);
    void run(std::function<void(std::string_view)> analizer);
    void assign(SLists slists);
    void assign(ShuffFileList sflist);
private:
    Rlists rlists;
    std::string m_outputfile;
    std::size_t m_rthreads;
};

    
}