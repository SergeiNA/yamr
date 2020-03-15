#include "mapper.hpp"
#include "hashSorter.hpp"
#include "reducer.hpp"
#include "min_pref_handler.hpp"
#include "utils.hpp"

#include <iostream>
#include <set>

int main(int argc, char* argv[]) {
    // if (argc < 4) {
    //     std::cout << "Wrong args: yamr <fname> <M> <R>\n"
    //          << "fname - path to input file\n"
    //          << "M - threads in mapper\n"
    //          << "R - threads in reducer"
    //          << std::endl;
    //     return 1;
    // }
    try{
        yamr::Mapper mapper("tMapper.txt",4);
        auto mappedData = mapper.Run();
        auto filesSMapped =mapper.Run("otMapper");
        yamr::HashSorter hsorter(3);
        hsorter.setInput(mappedData);
        auto shuffledata = hsorter.shuffle();
        auto filesShuffeled =  hsorter.shuffle("otShuffel");
        yamr::Reducer reducer(3);
        reducer.setInput(shuffledata);
        auto reducedData = reducer.run(CollapseDups());
        auto fileReduced1 = reducer.run(CollapseDups(), "otReducerd");

        auto resultReduce = reducedData[0];
        for(auto i =1ul; i<reducedData.size(); ++i){
            resultReduce.merge(reducedData[i]);
        }
        MinPrefix min_pref;
        for(auto &[str,_]:resultReduce)
            min_pref(str);
        std::cout<<"Min prefix: " << min_pref.get() <<std::endl;

    }catch(std::exception& e){
        std::cout<<e.what();
    }

    
    // test split
    // FileRanges frange1 = splitFile("tsplit.dat",2);
    // for(const auto& r: frange1){
    //     std::cout<< r.begin<<" : "<<r.end<<std::endl;
    //     std::ifstream ifs("tsplit.dat");
    //     ifs.seekg(r.begin);
    //     std::string str; 
    //     std::getline(ifs, str);
    //     std::cout<<str<<std::endl;
    // }
    // FileRanges frange2 = splitFile("tsplit.dat",3);
    // for(const auto& r: frange2){
    //     std::cout<< r.begin<<" : "<<r.end<<std::endl;
    //     std::ifstream ifs("tsplit.dat");
    //     ifs.seekg(r.begin);
    //     std::string str; 
    //     std::getline(ifs, str);
    //     std::cout<<str<<std::endl;
    // }
    // FileRanges frange3 = splitFile("tsplit.dat",4);
    // for(const auto& r: frange3){
    //     std::cout<< r.begin<<" : "<<r.end<<std::endl;
    //     std::ifstream ifs("tsplit.dat");
    //     ifs.seekg(r.begin);
    //     std::string str; 
    //     std::getline(ifs, str);
    //     std::cout<<str<<std::endl;
    // }
    

    return 0;
}