#pragma once

#include "common.hpp"

#include <map>
#include <vector>
#include <future>

#include <iostream>
namespace yamr{

class Reducer{
public:
    Reducer(std::size_t rthreads);
    
    template<class A>
    Rlists run(A analizer);

    template<class A>
    RedFileList run(A analizer,const std::string& outfname);

    void setInput(SLists slists);
    void setInput(ShuffFileList sflists);
private:
    template<class A>
    Rlists reduce_form_data(A analizer);

    template<class A>
    Rlists reduce_form_files(A analizer);
    void clear();
private:
    SLists m_slists;
    ShuffFileList m_sflists;
    std::string m_outputfile;
    std::size_t m_rthreads;
};

template<class A>
Rlists Reducer::reduce_form_data(A analizer){
        Rlists rlists(m_rthreads);
        std::vector<std::future<void>> tasks;
        tasks.reserve(m_rthreads);
        for(auto i = 0ul; i < m_rthreads; ++i){
            tasks.emplace_back(
                std::async(
                    std::launch::async,
                    [](ReduceList& redlist, SuffleList& slist, A analizer){
                        for(auto& str: slist){
                            analizer(std::move(str));
                        }
                        redlist = analizer.get();
                    },
                    std::ref(rlists[i]),
                    std::ref(m_slists[i]),
                    analizer
                )
            );
        }
    for(auto& t:tasks)
        t.get();
    return rlists;
    }

    template<class A>
    Rlists Reducer::reduce_form_files(A analizer){
        return {};
    }

    template<class A>
    Rlists Reducer::run(A analizer){
        Rlists rlists;
        if(empty(m_sflists) && m_slists.empty() )
                throw std::invalid_argument("No data for process");
        else if(m_sflists.empty())
            rlists = reduce_form_data(std::move(analizer));
        else
            rlists = reduce_form_files(std::move(analizer));
        return rlists;
    }

    template<class A>
    RedFileList Reducer::run(A analizer, const std::string& outfname){
        Rlists rlists (run(std::move(analizer)));
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

}