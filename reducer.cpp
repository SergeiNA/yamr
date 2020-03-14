#include "reducer.hpp"

#include <exception>

namespace yamr{

    Reducer::Reducer(std::string ofile, std::size_t rthreads):
        m_outputfile(ofile),m_rthreads(rthreads){
            rlists.reserve(rthreads);
    }


    void Reducer::run(std::function<void(std::string_view)> analizer){

    }
    void Reducer::assign(SLists slists){
        if(slists.size()!= m_rthreads)
            throw std::invalid_argument("size does not match");
    }
    void Reducer::assign(ShuffFileList sflist){
        if(sflist.size()!= m_rthreads)
            throw std::invalid_argument("size does not match");
    }

    
}