#include "reducer.hpp"

#include <exception>
#include <fstream>

namespace yamr{

    Reducer::Reducer(std::size_t rthreads):
        m_rthreads(rthreads){
    }

    void Reducer::setInput(SLists slists){
        if(slists.size()!= m_rthreads)
            throw std::invalid_argument("SLists size does not match");
        clear();
        m_slists = std::move(slists);
    }

    void Reducer::setInput(ShuffFileList sflists){
        if(m_sflists.size()!= m_rthreads)
            throw std::invalid_argument("ShuffFileList size does not match");
        clear();
        m_sflists = std::move(sflists);
    }

    void Reducer::clear(){
        m_slists.clear();
        m_sflists.clear();
    }
    
}