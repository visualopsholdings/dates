/*
  testdate.cpp
  
  Author: Paul Hamilton (paul@visualops.com)
  Date: 24-Sep-2026
  
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/visualopsholdings/dates
*/

#include <iostream>

#include "date.hpp"

#define BOOST_AUTO_TEST_MAIN
#include <boost/test/unit_test.hpp>

using namespace std;
using namespace vops;

BOOST_AUTO_TEST_CASE( now )
{
  cout << "=== now ===" << endl;
  
  cout << Date::now() << endl;
  
}

BOOST_AUTO_TEST_CASE( toISODate )
{
  cout << "=== toISODate ===" << endl;
  
  BOOST_CHECK_EQUAL(Date(true).toISODate(1721890479599), "2024-07-25T06:54:39.599+00:00");
  
}

BOOST_AUTO_TEST_CASE( fromISODate )
{
  cout << "=== fromISODate ===" << endl;
  
  BOOST_CHECK_EQUAL(Date(true).fromISODate("2024-07-25T06:54:39.599+00:00"), 1721890479599);
  
}

BOOST_AUTO_TEST_CASE( fromISODateShortMS )
{
  cout << "=== fromISODateShortMS ===" << endl;
  
  BOOST_CHECK_EQUAL(Date(true).fromISODate("2024-09-18T11:11:30.2+00:00"), 1726657890002);
  
}

BOOST_AUTO_TEST_CASE( inPast )
{
  cout << "=== inPast ===" << endl;
  
  BOOST_CHECK(Date(true).fromISODate("2024-07-01T06:54:39.599+00:00") < Date::now());
  BOOST_CHECK(Date(true).fromISODate("2027-07-01T06:54:39.599+00:00") > Date::now());
  
}

BOOST_AUTO_TEST_CASE( getFutureTimeMorning )
{
  cout << "=== getFutureTimeMorning ===" << endl;
  
  // 2024-07-25T06:54:39.599+00:00
  BOOST_CHECK_EQUAL(Date(true).getFutureTime(1721890479599, 4), "2024-07-25T10:54:39.599+00:00");
  
}

BOOST_AUTO_TEST_CASE( getFutureTimeAfterLunch )
{
  cout << "=== getFutureTimeAfterLunch ===" << endl;
  
  // 2024-09-18T11:11:30.2+00:00
  BOOST_CHECK_EQUAL(Date(true).getFutureTime(1726657890002, 4), "2024-09-18T15:11:30.2+00:00");
  
}

BOOST_AUTO_TEST_CASE( getFutureTimeEndOfMonth )
{
  cout << "=== getFutureTimeEndOfMonth ===" << endl;
  
//  cout << Date(true).fromISODate("2024-09-30T22:11:30.2+00:00") << endl;
  
  // 2024-09-30T22:11:30.2+00:00
  BOOST_CHECK_EQUAL(Date(true).getFutureTime(1727734290002, 4), "2024-10-01T02:11:30.2+00:00");
  
}

BOOST_AUTO_TEST_CASE( rubyDate )
{
  cout << "=== rubyDate ===" << endl;
  
  // 2022-01-01 00:00:00 +1100
  BOOST_CHECK_EQUAL(Date(true).toISODate(Date(true).fromISODate("2022-01-01 01:20:32 +1100")), "2022-01-01T12:20:32.0+00:00");
  BOOST_CHECK_EQUAL(Date(true).toISODate(Date(true).fromISODate("2022-01-01 01:20:32 +0000")), "2022-01-01T01:20:32.0+00:00");
  
}

BOOST_AUTO_TEST_CASE( NodeJSDate )
{
  cout << "=== NodeJSDate ===" << endl;
  
  BOOST_CHECK_EQUAL(Date(true).fromISODate("2024-05-10T14:51:11.255Z"), 1715352671255);
  
}

BOOST_AUTO_TEST_CASE( getYear )
{
  cout << "=== getYear ===" << endl;
  
  BOOST_CHECK_EQUAL(Date(true).getYear("2024-05-10T14:51:11.255Z"), "2024");
  
}

BOOST_AUTO_TEST_CASE( getStartOfDay )
{
  cout << "=== getStartOfDay ===" << endl;
  
  BOOST_CHECK_EQUAL(Date(true).toISODate(Date(true).getStartOfDay(Date(true).fromISODate("2022-04-05T12:20:32.45+00:00"))), "2022-04-05T00:00:00.0+00:00");
  
}

BOOST_AUTO_TEST_CASE( getStartOfMonth )
{
  cout << "=== getStartOfMonth ===" << endl;
  
  BOOST_CHECK_EQUAL(Date(true).toISODate(Date(true).getStartOfMonth(Date(true).fromISODate("2022-04-05T12:20:32.45+00:00"))), "2022-04-01T00:00:00.0+00:00");
  
}

BOOST_AUTO_TEST_CASE( addMonth )
{
  cout << "=== addMonth ===" << endl;
  
  BOOST_CHECK_EQUAL(Date(true).toISODate(Date(true).addMonth(Date(true).fromISODate("2022-04-01T00:00:00.0+00:00"))), "2022-05-01T00:00:00.0+00:00");
  
}

BOOST_AUTO_TEST_CASE( getMonthNum )
{
  cout << "=== getMonthNum ===" << endl;
  
  BOOST_CHECK_EQUAL(Date(true).getMonthNum(Date(true).fromISODate("2022-04-01T00:00:00.0+00:00")), 3);
  BOOST_CHECK_EQUAL(Date(true).getMonthNum(Date(true).fromISODate("2026-07-25T00:00:00.0+00:00")), 6);
  
}

BOOST_AUTO_TEST_CASE( getDayNum )
{
  cout << "=== getDayNum ===" << endl;
  
  BOOST_CHECK_EQUAL(Date(true).getDayNum(Date(true).fromISODate("2022-04-01T00:00:00.0+00:00")), 1);
  BOOST_CHECK_EQUAL(Date(true).getDayNum(Date(true).fromISODate("2026-07-25T00:00:00.0+00:00")), 25);
  
}

BOOST_AUTO_TEST_CASE( getWeekDayNum )
{
  cout << "=== getWeekDayNum ===" << endl;
  
  BOOST_CHECK_EQUAL(Date(true).getWeekDayNum(Date(true).fromISODate("2022-04-01T00:00:00.0+00:00")), 5);
  BOOST_CHECK_EQUAL(Date(true).getWeekDayNum(Date(true).fromISODate("2026-07-25T00:00:00.0+00:00")), 6);
  
}

BOOST_AUTO_TEST_CASE( getStartOfYear )
{
  cout << "=== getStartOfYear ===" << endl;
  
  BOOST_CHECK_EQUAL(Date(true).toISODate(Date(true).getStartOfYear(Date(true).fromISODate("2022-04-05T12:20:32.45+00:00"))), "2022-01-01T00:00:00.0+00:00");
  
}

BOOST_AUTO_TEST_CASE( addYear )
{
  cout << "=== addYear ===" << endl;
  
  BOOST_CHECK_EQUAL(Date(true).toISODate(Date(true).addYear(Date(true).fromISODate("2022-04-01T00:00:00.0+00:00"))), "2023-04-01T00:00:00.0+00:00");
  
}

BOOST_AUTO_TEST_CASE( getYearNum )
{
  cout << "=== getYearNum ===" << endl;
  
  BOOST_CHECK_EQUAL(Date(true).getYearNum(Date(true).fromISODate("2022-04-01T00:00:00.0+00:00")), 2022);
  BOOST_CHECK_EQUAL(Date(true).getYearNum(Date(true).fromISODate("2026-07-25T00:00:00.0+00:00")), 2026);
  
}

BOOST_AUTO_TEST_CASE( getHourNum )
{
  cout << "=== getHourNum ===" << endl;
  
  BOOST_CHECK_EQUAL(Date(true).getHourNum(Date(true).fromISODate("2022-04-01T12:00:00.0+00:00")), 12);
  BOOST_CHECK_EQUAL(Date(true).getHourNum(Date(true).fromISODate("2026-07-25T14:00:00.0+00:00")), 14);
  
}

BOOST_AUTO_TEST_CASE( addDay )
{
  cout << "=== addDay ===" << endl;
  
  BOOST_CHECK_EQUAL(Date(true).toISODate(Date(true).addDay(Date(true).fromISODate("2022-04-01T00:00:00.0+00:00"))), "2022-04-02T00:00:00.0+00:00");
  
}

BOOST_AUTO_TEST_CASE( getStartOfCentury )
{
  cout << "=== getStartOfCentury ===" << endl;
  
  BOOST_CHECK_EQUAL(Date(true).toISODate(Date(true).getStartOfCentury(Date(true).fromISODate("2022-04-05T12:20:32.45+00:00"))), "2000-01-01T00:00:00.0+00:00");
  
}

BOOST_AUTO_TEST_CASE( addCentury )
{
  cout << "=== addCentury ===" << endl;
  
  BOOST_CHECK_EQUAL(Date(true).toISODate(Date(true).addCentury(Date(true).fromISODate("2022-04-01T00:00:00.0+00:00"))), "2122-04-01T00:00:00.0+00:00");
  
}

BOOST_AUTO_TEST_CASE( prevCentury )
{
  cout << "=== prevCentury ===" << endl;
  
  BOOST_CHECK_EQUAL(Date(true).toISODate(Date(true).prevCentury(Date(true).fromISODate("2022-04-01T00:00:00.0+00:00"))), "1922-04-01T00:00:00.0+00:00");
  
}

BOOST_AUTO_TEST_CASE( getTime )
{
  cout << "=== getTime ===" << endl;
  
  BOOST_CHECK_EQUAL(Date(true).getTime(Date(true).fromISODate("2022-04-05T12:20:32.45+00:00")), "12:20");
  BOOST_CHECK_EQUAL(Date(true).getTime(Date(true).fromISODate("2022-04-05T02:03:32.45+00:00")), "02:03");
  BOOST_CHECK_EQUAL(Date(true).getTime(Date(true).fromISODate("2022-04-05T22:03:32.45+00:00")), "22:03");
  
}

BOOST_AUTO_TEST_CASE( getSeconds )
{
  cout << "=== getSeconds ===" << endl;
  
  BOOST_CHECK_EQUAL(Date(true).getSeconds(Date(true).fromISODate("2022-04-05T22:03:32.45+00:00")), "32");
  
}

BOOST_AUTO_TEST_CASE( getMinutes )
{
  cout << "=== getMinutes ===" << endl;
  
  BOOST_CHECK_EQUAL(Date(true).getMinutes(Date(true).fromISODate("2022-04-05T12:20:32.45+00:00")), "20");
  BOOST_CHECK_EQUAL(Date(true).getMinutes(Date(true).fromISODate("2022-04-05T02:03:32.45+00:00")), "03");
  
}

BOOST_AUTO_TEST_CASE( getStartOfMinute )
{
  cout << "=== getStartOfMinute ===" << endl;
  
  Date d(true);
  BOOST_CHECK_EQUAL(d.toISODate(d.getStartOfMinute(d.fromISODate("2022-04-05T12:20:32.45+00:00"))), "2022-04-05T12:20:00.0+00:00");
  
}

BOOST_AUTO_TEST_CASE( tzMsFix )
{
  cout << "=== tzMsFix ===" << endl;
  
  BOOST_CHECK_EQUAL(Date::tzMsFix("2026-07-25T00:00:00+00:00"), "2026-07-25T00:00:00.0+00:00");
  BOOST_CHECK_EQUAL(Date::tzMsFix("2026-07-25T00:00:00+10:00"), "2026-07-24T14:00:00.0+00:00");
  BOOST_CHECK_EQUAL(Date::tzMsFix("2026-07-25T00:00:00.0+00:00"), "2026-07-25T00:00:00.0+00:00");
  BOOST_CHECK_EQUAL(Date::tzMsFix("2026-07-25T00:00:00.0+10:00"), "2026-07-24T14:00:00.0+00:00");
  
}

