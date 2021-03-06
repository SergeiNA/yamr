#define BOOST_TEST_MODULE min_prefix

#include "utils.hpp"
#include <iostream>
#include <sstream>
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(min_prefix)

BOOST_AUTO_TEST_CASE(test_class_MinPrefix)
{
    std::vector<std::string> input {"a","a","a",
                                 "ab","ab",
                                 "aab","aab",
                                 "aabc",
                                 "aabcd"
                                 };

    yamr::ReduceList out {"1","2","3"};
    MinPrefix min_pref;
    yamr::ReduceList rList;
    for(auto& s: input){
        auto tredl = min_pref(s);
        if(!tredl.empty())
            rList.emplace_back(tredl.at(0));
    }

    BOOST_CHECK_EQUAL(rList.size(), 3);
    BOOST_CHECK_EQUAL_COLLECTIONS(rList.begin(),rList.end(),out.begin(),out.end());

}

BOOST_AUTO_TEST_CASE(WordPrefixSplitter)
{
    PrefixSplitter wPref_split;
    std::string input1 ("aabbcc");
    std::string input2 ("aaaa");
    std::string input3 ("abcd");

    yamr::MappedWord out1 {"a","aa","aab","aabb","aabbc","aabbcc"};
    yamr::MappedWord out2 {"a","aa","aaa","aaaa"};
    yamr::MappedWord out3 {"a","ab","abc","abcd"};

    yamr::MappedWord w1 = wPref_split(input1);
    yamr::MappedWord w2 = wPref_split(input2);
    yamr::MappedWord w3 = wPref_split(input3);

    BOOST_CHECK_EQUAL_COLLECTIONS(w1.begin(),w1.end(),out1.begin(),out1.end());
    BOOST_CHECK_EQUAL_COLLECTIONS(w2.begin(),w2.end(),out2.begin(),out2.end());
    BOOST_CHECK_EQUAL_COLLECTIONS(w3.begin(),w3.end(),out3.begin(),out3.end());
}

BOOST_AUTO_TEST_SUITE_END()