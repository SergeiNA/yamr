#define BOOST_TEST_MODULE min_prefix

#include "min_pref_handler.hpp"
#include <sstream>
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(min_prefix)

BOOST_AUTO_TEST_CASE(test_class_MinPrefix)
{
    std::set<std::string> input {"aabbcc",
                                 "aabbcd",
                                 "aabdcc",
                                 "aaaccc",
                                 "aaabcc"
                                 };
    MinPrefix min_pref;
    for(auto& s: input)
        min_pref(s);

    BOOST_CHECK_EQUAL(min_pref.get(), 5);
}

BOOST_AUTO_TEST_CASE(test_get_min_prefix)
{
    std::map<std::string, std::size_t> input1 {{"aabbcc",1},
                                  {"aabbcd",1},
                                  {"aabdcc",1},
                                  {"aaaccc",1},
                                  {"aaabcc",1}
                                 };

    std::map<std::string, std::size_t> input2 {{"aadbcc",1},
                                  {"acbbcd",1},
                                  {"aasdcc",1},
                                  {"affccc",1},
                                  {"aaaacc",1}
                                 };

    std::map<std::string, std::size_t> input3 {{"aabfcc",1},
                                  {"aafbcd",1},
                                  {"aabdfc",1},
                                  {"aaaggc",1},
                                  {"aaabhc",1}
                                 };

    std::vector<std::map<std::string, std::size_t>> Input {input1, input2, input3};

    BOOST_CHECK_EQUAL(getMinPrefix(Input), 5);
}

BOOST_AUTO_TEST_SUITE_END()