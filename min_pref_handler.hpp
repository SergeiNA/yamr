#pragma once

#include <string_view>
#include <map>

class MinPrefix{
public:
    void operator()(std::string_view);
    std::size_t get_min_pref() const noexcept;
private:
    std::size_t min_pref=0;
    std::string_view prevs;
};
