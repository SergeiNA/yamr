#include "min_pref_handler.hpp"

#include <fstream>
#include <algorithm>


void MinPrefix::operator()(std::string_view sv){
    if(prevs.empty()){
        prevs = sv;
        return;
    }
    std::size_t tmin_pref =0;
    auto min_length = std::min(prevs.size(),sv.size());
    for (auto i = 0ul; i < min_length; ++i){
        if(prevs[i]!=sv[i])
            break;
        ++tmin_pref;
    }
    min_pref = std::max(tmin_pref, min_pref);
    prevs = sv;
    }

std::size_t MinPrefix::get() const noexcept{
    return min_pref;
}


std::size_t getMinPrefix(yamr::Rlists& reducedData){
    auto resultReduce = reducedData[0];
    for(auto i =1ul; i < reducedData.size(); ++i)
            resultReduce.merge(reducedData[i]);
    
    MinPrefix min_pref;
    for(auto &[str,_]:resultReduce)
        min_pref(str);
    return min_pref.get();
}

std::size_t getMinPrefix(const yamr::RedFileList& rflist){
    std::set<std::string> strlist;
    for(const auto& file: rflist){
        std::ifstream ifs(file);
        if(!ifs) throw std::invalid_argument("getMinPrefix file doesn't exist");
        for(std::string str; std::getline(ifs,str);)
            strlist.emplace(std::move(str));
    }
    MinPrefix min_pref;
    for(auto &str:strlist)
        min_pref(str);
    return min_pref.get();
}