/*
  teststringutils.cpp
  
  Author: Paul Hamilton (paul@visualops.com)
  Date: 124-Sep-2026
  
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/visualopsholdings/dates
*/

#include "stringutils.hpp"

#define BOOST_AUTO_TEST_MAIN
#include <boost/test/unit_test.hpp>

using namespace std;
using namespace vops;

BOOST_AUTO_TEST_CASE( split )
{
  cout << "=== split ===" << endl;
  
  string s1;
  string s2;
  StringUtils::splitString("aaaa aaa:xxx-", ' ', &s1, &s2);
  
  BOOST_CHECK_EQUAL(StringUtils::stringBefore(StringUtils::stringAfter(s2, ':'), '-'), "xxx");
  
}
