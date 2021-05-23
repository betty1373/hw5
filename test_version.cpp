#define BOOST_TEST_MODULE test_version

#include "lib.h"

#include <boost/test/unit_test.hpp>
using namespace libSrc;
BOOST_AUTO_TEST_SUITE(test_version)

BOOST_AUTO_TEST_CASE(test_valid_version)
{
    BOOST_CHECK(libSrc::version() > 0);

    
}

}
