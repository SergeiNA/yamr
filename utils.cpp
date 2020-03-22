#include "utils.hpp"

#include <filesystem>
#include <exception>
#include <algorithm>
#include <string>

#include <iostream>

std::size_t align_by_endl(std::istream& is, std::size_t pos){
    is.seekg(pos = (pos == 0) ? pos: pos - 1, std::ios_base::beg);
    char c;
    while (is.get(c)) {
        ++pos;
        if (c == '\n') break;
    }
    return pos;
}

bool checkPath(const std::string& file){
    namespace fs =  std::filesystem;
    return fs::exists(file);
}


std::size_t fileSize(const std::string& fname){
    namespace fs =  std::filesystem;
    return fs::file_size(fs::path(fname));
    return 0;
}

FileRanges splitFile(const std::string& fname, std::size_t chunk_sz){
    std::ifstream ifs(fname);
    if(!ifs)
        throw "Cannot open input file";
    
    return splitStream(ifs, chunk_sz, fileSize(fname));
}

FileRanges splitStream(std::istream& is, std::size_t chunk_num, std::size_t fsize){
    FileRanges fileRanges(chunk_num);
    std::size_t init_chunck_sz = std::max(fsize/chunk_num, 1ul);
    for (auto i = 0ul; i < fileRanges.size() - 1; ++i){
        fileRanges[i].end = align_by_endl(is,(i+1)*init_chunck_sz);
        fileRanges[i+1].begin = fileRanges[i].end;
    }
    fileRanges.back().end = fsize;
    return fileRanges;
}

std::size_t getHash(const std::string& str){
    std::hash<std::string> hash_fn;
    return hash_fn(str);
}

std::vector<std::string> str2prefixes(std::string str){
    std::vector<std::string> wordPrefixes;
    wordPrefixes.reserve(str.size());
    for(auto i =0ul; i < str.size(); ++i)
        wordPrefixes.emplace_back(str.substr(0,i+1));
    return wordPrefixes;
}


yamr::MappedWord PrefixSplitter::operator()(std::string str){
    return str2prefixes(std::move(str));
}


yamr::ReduceList MinPrefix::operator()(std::string_view sv){
    yamr::ReduceList reList;
    if(prevs.empty())
        reList.push_back(std::to_string(0));
    if (sv == prevs && sv.size() > min_pref){
        min_pref = sv.size();
        reList.push_back(std::to_string(min_pref + 1));
    }
    prevs = sv;
    return reList;
}
