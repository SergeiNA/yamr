#pragma once


#include <set>
#include <map>
#include <vector>

namespace yamr{

    using MappedList        = std::multiset<std::string>;
    using MLists            = std::vector<MappedList>;

    using SuffleList        = std::multiset<std::string>;
    using SLists            = std::vector<SuffleList>;

    using ReduceList        = std::map<std::string, std::size_t>;
    using Rlists            = std::vector<ReduceList>;

    using MapFileList       = std::vector<std::string>;
    using ShuffFileList     = std::vector<std::string>;
    using RedFileList       = std::vector<std::string>;

}