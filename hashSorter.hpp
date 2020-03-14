#pragma once

#include "utils.hpp"
#include "common.hpp"

#include <mutex>
#include <memory>

namespace yamr{

class HashSorter{
    struct ThreadSafe_SLists{
        ThreadSafe_SLists(std::size_t nthreads);
        void write(SLists& slists, std::string str, std::size_t nth);
        std::vector<std::mutex> locks;
        std::size_t m_nthreads;
    };

public:
    HashSorter(std::size_t shuffsize);
    void setInput(MLists mlists);
    void setInput(MapFileList mapfLists);
    SLists shuffle();
    ShuffFileList shuffle(const std::string& outfname);
private:
    SLists shuffle_form_data();
    SLists shuffle_form_files();
    void clear();
private:
    MLists m_mappedLists;
    MapFileList m_mappedFileLists;
    std::size_t m_shuffsize;
    ThreadSafe_SLists th_save_slists;
};

}