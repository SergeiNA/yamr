#include "utils.hpp"

#include <filesystem>
#include <exception>
#include <algorithm>
#include <string>

std::size_t align_by_endl(std::istream& is, std::size_t pos){
    is.seekg(pos = (pos == 0) ? pos: pos - 1, std::ios_base::beg);
    char c;
    while (is.get(c)) {
        ++pos;
        if (c == '\n') break;
    }
    return pos;
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