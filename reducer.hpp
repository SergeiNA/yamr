#pragma once

#include "common.hpp"

#include <map>
#include <vector>
#include <future>
#include <fstream>
#include <exception>
namespace yamr{

template <class ReduceFunc>
class Reducer{
public:
    Reducer(std::size_t rthreads);
    
    RedFileList run(const std::string& outfname);

    void setInput(SLists slists);
    void setInput(ShuffFileList sflists);
private:

    void reduce_form_data(const RedFileList& flist);

    void reduce_form_files(const RedFileList& flist);
    void clear();
private:
    ReduceFunc reduceFunc;
    SLists m_slists;
    ShuffFileList m_sflists;
    std::string m_outputfile;
    std::size_t m_rthreads;
};

template<class ReduceFunc>
void Reducer<ReduceFunc>::reduce_form_data(const RedFileList& flist){
    std::vector<std::future<void>> tasks;
    tasks.reserve(m_rthreads);
    for(auto i = 0ul; i < m_rthreads; ++i){
        tasks.emplace_back(
            std::async(
                std::launch::async,
                [](const std::string& ofile, SuffleList& slist, ReduceFunc reduce){
                    std::ofstream ofs(ofile);
                    if(!ofs)
                        throw "Reducer::reduce_form_data can not create a file";
                    for(auto& str: slist){
                        auto max_dub_pref = reduce(std::move(str));
                        if(!max_dub_pref.empty())
                            for(auto& s:max_dub_pref)
                                ofs<<s<<'\n';
                    }
                },
                std::cref(flist[i]),
                std::ref(m_slists[i]),
                reduceFunc
            )
        );
    }
for(auto& t:tasks)
    t.get();
}

template<class ReduceFunc>
void Reducer<ReduceFunc>::reduce_form_files(const RedFileList& flist){
    std::vector<std::future<void>> tasks;
    tasks.reserve(m_rthreads);
    for(auto i = 0ul; i < m_rthreads; ++i){
        tasks.emplace_back(
            std::async(
                std::launch::async,
                [](const std::string& ofile, const std::string& ifile, ReduceFunc reduce){
                    std::ifstream ifs(ifile);
                    if(!ifs) throw std::invalid_argument("Reducer::reduce_form_files can not open the file");
                    std::ofstream ofs(ofile);
                    if(!ofs)
                        throw "Reducer::reduce_form_data can not create a file";
                    for(std::string str; std::getline(ifs,str);){
                        auto max_dub_pref = reduce(std::move(str));
                        if(!max_dub_pref.empty())
                            for(auto& s:max_dub_pref)
                                ofs<<s<<'\n';
                    }
                },
                std::ref(flist[i]),
                std::ref(m_sflists[i]),
                reduceFunc
            )
        );
    }
    for(auto& t:tasks)
        t.get();
}
  
template<class ReduceFunc>
RedFileList Reducer<ReduceFunc>::run(const std::string& outfname){
    RedFileList reduceFileList;
    reduceFileList.reserve(m_rthreads);
    for(auto i=0ul; i < m_rthreads; ++i)
        reduceFileList.emplace_back(outfname + std::to_string(i+1));

    if(empty(m_sflists) && m_slists.empty() )
            throw std::invalid_argument("No data for process");
    else if(m_sflists.empty())
        reduce_form_data(reduceFileList);
    else
        reduce_form_files(reduceFileList);
    return reduceFileList;
}

template<class ReduceFunc>
Reducer<ReduceFunc>::Reducer(std::size_t rthreads):
    m_rthreads(rthreads){
}

template<class ReduceFunc>
void Reducer<ReduceFunc>::setInput(SLists slists){
    if(slists.size()!= m_rthreads)
        throw std::invalid_argument("SLists size does not match");
    clear();
    m_slists = std::move(slists);
}
template<class ReduceFunc>
void Reducer<ReduceFunc>::setInput(ShuffFileList sflists){
    if(m_sflists.size()!= m_rthreads)
        throw std::invalid_argument("ShuffFileList size does not match");
    clear();
    m_sflists = std::move(sflists);
}

template<class ReduceFunc>
void Reducer<ReduceFunc>::clear(){
    m_slists.clear();
    m_sflists.clear();
}
}