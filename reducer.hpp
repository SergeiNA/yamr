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
    
    Rlists run();

    RedFileList run(const std::string& outfname);

    void setInput(SLists slists);
    void setInput(ShuffFileList sflists);
private:

    Rlists reduce_form_data();

    Rlists reduce_form_files();
    void clear();
private:
    ReduceFunc reduceFunc;
    SLists m_slists;
    ShuffFileList m_sflists;
    std::string m_outputfile;
    std::size_t m_rthreads;
};

template<class ReduceFunc>
Rlists Reducer<ReduceFunc>::reduce_form_data(){
        Rlists rlists(m_rthreads);
        std::vector<std::future<void>> tasks;
        tasks.reserve(m_rthreads);
        for(auto i = 0ul; i < m_rthreads; ++i){
            tasks.emplace_back(
                std::async(
                    std::launch::async,
                    [](ReduceList& redlist, SuffleList& slist, ReduceFunc reduce){
                        std::size_t min_pref =0;
                        for(auto& str: slist){
                            min_pref = reduce(std::move(str));
                        }
                        redlist["min_prefix"] = min_pref;
                    },
                    std::ref(rlists[i]),
                    std::ref(m_slists[i]),
                    reduceFunc
                )
            );
        }
    for(auto& t:tasks)
        t.get();
    return rlists;
    }

    template<class ReduceFunc>
    Rlists Reducer<ReduceFunc>::reduce_form_files(){
        Rlists rlists(m_rthreads);
        std::vector<std::future<void>> tasks;
        tasks.reserve(m_rthreads);
        for(auto i = 0ul; i < m_rthreads; ++i){
            tasks.emplace_back(
                std::async(
                    std::launch::async,
                    [](ReduceList& redlist, const std::string& ifile, ReduceFunc reduce){
                        std::ifstream ifs(ifile);
                        if(!ifs) throw std::invalid_argument("Reducer::reduce_form_files can not open the file");
                        std::size_t min_pref =0;
                        for(std::string str; std::getline(ifs,str);){
                            min_pref = reduce(std::move(str));
                        }
                        redlist["min_prefix"] = min_pref;
                    },
                    std::ref(rlists[i]),
                    std::ref(m_sflists[i]),
                    reduceFunc
                )
            );
        }
        for(auto& t:tasks)
            t.get();
        return rlists;
    }

    template<class ReduceFunc>
    Rlists Reducer<ReduceFunc>::run(){
        Rlists rlists;
        if(empty(m_sflists) && m_slists.empty() )
                throw std::invalid_argument("No data for process");
        else if(m_sflists.empty())
            rlists = reduce_form_data();
        else
            rlists = reduce_form_files();
        return rlists;
    }

    template<class ReduceFunc>
    RedFileList Reducer<ReduceFunc>::run(const std::string& outfname){
        Rlists rlists (run());
        RedFileList reduceFileList;
        reduceFileList.reserve(m_rthreads);
        for(auto i=0ul; i < m_rthreads; ++i){
            reduceFileList.emplace_back(outfname + std::to_string(i+1));
            std::ofstream ofs(reduceFileList.back());
            for(auto& [first, second]: rlists[i]){
                ofs << first << ' ' << second << '\n';
            }
        }
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