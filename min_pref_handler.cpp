#include "min_pref_handler.hpp"
#include <iostream>
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
