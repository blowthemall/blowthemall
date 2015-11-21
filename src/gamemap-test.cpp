#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include "gamemap.h"

BOOST_AUTO_TEST_CASE(resolve_dots_or_throw_not_found) {
  bta::GameMap<int> map(4, 4);
  BOOST_CHECK_EQUAL(map.width(), 4);
}
