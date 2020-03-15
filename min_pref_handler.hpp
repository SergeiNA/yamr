#pragma once

#include <string_view>
#include <string>
#include <map>

class MinPrefix{
public:
    void operator()(std::string_view);
    std::size_t get() const noexcept;
    std::string info() const noexcept;
    bool exist() const noexcept;
private:
    std::size_t min_pref=0;
    std::string_view prevs;
};

