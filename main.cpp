#include "mapper.hpp"
#include "hashSorter.hpp"
#include "reducer.hpp"
#include "min_pref_handler.hpp"
#include "utils.hpp"

#include <iostream>
#include <algorithm>
#include <set>

int main(int argc, char* argv[]) {
    if (argc < 4) {
        std::cout << "Wrong args: yamr <fname> <M> <R>\n"
             << "fname - path to input file\n"
             << "M - threads in mapper\n"
             << "R - threads in reducer"
             << std::endl;
        return 1;
    }

    if(!checkPath(argv[1]) ){
        std::cerr << "File path are incorrect" << std::endl;
        return 1;
    }
    std::string mThreads(argv[2]);
    std::string rThreads(argv[3]);

    if (!(std::all_of(mThreads.begin(), mThreads.end(), ::isdigit)
        && std::all_of(rThreads.begin(), rThreads.end(), ::isdigit))) {

        std::cerr << "M or R are incorrect" << std::endl;
        return 1;
    }
    try{
        std::size_t m_threads = std::stoul(mThreads);
        std::size_t r_threads = std::stoul(rThreads);

        // mapper
        yamr::Mapper mapper(argv[1],m_threads);
        auto mappedData = mapper.Run();

        // shuffle
        yamr::HashSorter hsorter(r_threads);
        hsorter.setInput(mappedData);
        auto shuffledata = hsorter.shuffle();

        // reduser
        yamr::Reducer reducer(r_threads);
        reducer.setInput(shuffledata);
        auto fileReduced = reducer.run(CollapseDups(), "outReducerd");
        
        // find min prefix
        std::cout<<"Min prefix: " << getMinPrefix(fileReduced) <<std::endl;

    }catch(std::exception& e){
        std::cout<<e.what()<<std::endl;
    }

    return 0;
}