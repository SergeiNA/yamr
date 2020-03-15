#include "hashSorter.hpp"

#include <future>
#include <exception>

namespace yamr{

HashSorter::HashSorter(std::size_t shuffsize): 
    m_shuffsize(shuffsize),
    th_save_slists(shuffsize)
    {}

void HashSorter::setInput(MLists mlists){
    clear();
    m_mappedLists = std::move(mlists);
}

void HashSorter::setInput(MapFileList mapfLists){
    clear();
    m_mappedFileLists = std::move(mapfLists);
}

void HashSorter::clear(){
    m_mappedLists.clear();
    m_mappedFileLists.clear();
}

SLists HashSorter::shuffle(){
    SLists slists;
    if(m_mappedLists.empty() && m_mappedFileLists.empty() )
        throw std::invalid_argument("No data for process");
    else if(m_mappedFileLists.empty())
        slists = shuffle_form_data();
    else
        slists = shuffle_form_files();
    return slists;
}

SLists HashSorter::shuffle_form_data(){
    SLists slists(m_shuffsize);
    std::vector<std::future<void>> tasks;
    auto mapper_threads = m_mappedLists.size();
    tasks.reserve(mapper_threads);
    for(auto i = 0ul; i< mapper_threads; ++i){
            tasks.emplace_back(std::async(
                std::launch::async,
                [&slists]( const MappedList& mList, ThreadSafe_SLists& th_safe){
                        for(auto& str : mList){
                            auto nchunck = getHash(str)%th_safe.m_nthreads;
                            th_safe.write(slists,std::move(str), nchunck);
                        }
                    },
                    std::cref(m_mappedLists[i]),
                    std::ref(th_save_slists)
                )
            );
        }
    for(auto& t:tasks)
       t.get();
    return slists;
}

SLists HashSorter::shuffle_form_files(){

    return {};
}

ShuffFileList HashSorter::shuffle(const std::string& outfname){
    SLists slists(shuffle());
    ShuffFileList shuffleFileList;
    shuffleFileList.reserve(m_shuffsize);
    for(auto i=0ul; i < m_shuffsize; ++i){
        shuffleFileList.emplace_back(outfname + std::to_string(i+1));
        std::ofstream ofs(shuffleFileList.back());
        for(auto& itr: slists[i]){
            ofs<<itr<<'\n';
        }
    }
    return shuffleFileList;
}

 HashSorter::ThreadSafe_SLists::ThreadSafe_SLists(std::size_t nthreads): m_nthreads(nthreads){
    std::vector<std::mutex> tlocks(nthreads);
    locks = std::move(tlocks);
 }

void HashSorter::ThreadSafe_SLists::write(SLists& slists, std::string str, std::size_t nthread){
    std::lock_guard<std::mutex> lk(locks[nthread]);
    slists[nthread].emplace(std::move(str));
}

}