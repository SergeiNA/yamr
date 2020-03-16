#define BOOST_TEST_MODULE async_single_thread

#include "min_pref_handler.hpp"
#include <sstream>
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(bulk_suite)

BOOST_AUTO_TEST_CASE(test_regular)
{


    BOOST_CHECK_EQUAL(1, 1);
}

BOOST_AUTO_TEST_SUITE_END()