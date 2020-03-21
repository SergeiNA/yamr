#pragma once
#include <vector>
#include <set>
#include <future>
#include <iostream>

#include "utils.hpp"
#include "common.hpp"

namespace yamr{

template <class MapFunc>
class Mapper{
public:
    Mapper(const std::string& fname, std::size_t nthreads);
    MLists Run();
    MapFileList Run(const std::string& outfname);

private: 
    MapFunc mapper;
    std::string inputFile;
    std::size_t m_nthreads;
};

template <class MapFunc>
Mapper<MapFunc>::Mapper(const std::string& fname, std::size_t nthreads):
        inputFile(fname), m_nthreads(nthreads) {}

template <class MapFunc>
MLists Mapper<MapFunc>::Run(){
    MLists map_cont;
    map_cont.reserve(m_nthreads);
    auto fileRanges =  splitFile(inputFile, m_nthreads);
    std::vector<std::future<MappedList>> tasks;
    tasks.reserve(m_nthreads);
    for(auto i = 0ul; i< m_nthreads; ++i){
        tasks.emplace_back(std::async(
            std::launch::async,
            [](MapFunc mapper, const std::string& ifname, const SplitRange& range){
                        MappedList mlist;
                        std::ifstream ifs(ifname);
                        ifs.seekg(range.begin);
                        for (std::string str; std::getline(ifs, str); ) {
                            auto tmapped = mapper(std::move(str));
                            std::move(tmapped.begin(), tmapped.end(), std::back_inserter(mlist) );
                            if ((std::size_t)ifs.tellg() >= range.end) break;
                        }
                        ifs.close();
                        return mlist;
                    },
                mapper,
                std::cref(inputFile),
                std::cref(fileRanges[i])
            )
        );
    }
    for(auto& t:tasks)
        map_cont.emplace_back(t.get());
    return map_cont;

}

template <class MapFunc>
MapFileList Mapper<MapFunc>::Run(const std::string& outfname){
    MLists map_cont(Run());
    MapFileList mappedFileList;
    mappedFileList.reserve(m_nthreads);
    for(auto i=0ul; i < m_nthreads; ++i){
        mappedFileList.emplace_back(outfname + std::to_string(i+1));
        std::ofstream ofs(mappedFileList.back());
        for(auto& itr: map_cont[i]){
            ofs<<itr<<'\n';
        }
    }
    return mappedFileList;
}

}