/*
  date.cpp
  
  Author: Paul Hamilton (paul@visualops.com)
  Date: 24-Sep-2026
    
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/visualopsholdings/dates
*/

#include "date.hpp"
#include "log.hpp"
#include "stringutils.hpp"

#include <sstream>
#include <ctime>
#include <iomanip>
#include <cstdlib>

using namespace std;

#define TIME_FORMAT "%Y-%m-%dT%H:%M:%S"
#define RUBY_TIME_FORMAT "%Y-%m-%d %H:%M:%S"

namespace vops {

int64_t Date::now() {

  auto now = chrono::system_clock::now();
  auto duration = now.time_since_epoch();
  auto milliseconds
        = chrono::duration_cast<chrono::milliseconds>(
              duration)
              .count();

  return milliseconds;

}

string Date::getFutureTime(int64_t now, int hours) {

  time_t tnum = now / 1000;

  auto tp = chrono::system_clock::from_time_t(tnum);
  auto hrs = chrono::hours(hours);
  long t = chrono::system_clock::to_time_t(tp + hrs);
  
  t *= 1000;

  int ms = now - (tnum * 1000);
  t += ms;
  
  return toISODate(t);

}

tm Date::getTM(int64_t t, int *ms) const {

  time_t tnum = t / 1000;
  if (ms) {
    *ms = t - (tnum * 1000);
  }
  return _gm ? *gmtime(&tnum) : *localtime(&tnum);
  
}

int64_t Date::putTM(tm *tm, int *ms) {

  auto t = _gm ? timegm(tm) : timelocal(tm);
//  L_TRACE("t: " << t);
  
  return (t * 1000) + (ms ? *ms : 0);

}

string Date::toISODate(tm *tm, int ms) const {

//   L_TRACE("tm_sec " << tm.tm_sec);
//   L_TRACE("tm_min " << tm.tm_min);
//   L_TRACE("tm_hour " << tm.tm_hour);
//   L_TRACE("tm_mday " << tm.tm_mday);
//   L_TRACE("tm_mon " << tm.tm_mon);
//   L_TRACE("tm_year " << tm.tm_year);
//   L_TRACE("tm_wday " << tm.tm_wday);
//   L_TRACE("tm_yday " << tm.tm_yday);
//   L_TRACE("tm_isdst " << tm.tm_isdst);

  stringstream ss;
  // 2024-07-01T06:54:39
  ss << put_time(tm, TIME_FORMAT);
  ss << ".";
  ss << ms;
  ss << "+00:00";

  return ss.str();

}

string Date::toISODate(int64_t t) const {

  int ms;
  auto tm = getTM(t, &ms);
  return toISODate(&tm, ms);
  
}

int64_t Date::fromISODate(const string &d) {

//  L_TRACE(d);

  // no T, try a ruby date.
  if (d.rfind("T") == string::npos) {
    return fromRubyDate(d);
  }
  
  // handle the TZ offset at the end.
  int hrs = 0;
  int mins = 0;
  auto plus = d.rfind("+");
  string rest;
  if (plus != string::npos) {
    auto offs = d.substr(plus+1);
    rest = d.substr(0, plus);
    if (offs.size() != 5) {
      L_ERROR("wierd offset " << offs);
      return 0;
    }
    hrs =  atol(offs.substr(0, 2).c_str());
    mins =  atol(offs.substr(3, 2).c_str());
//    L_TRACE(hrs << " " << mins); 
  }
  else {
    // check for Z at end.
    if (d.back() != 'Z') {
      L_ERROR("missing Z or offset");
      return 0;
    }
    rest = d.substr(0, d.size()-1);
  }
//  L_TRACE("after TZ: " << rest);
  
  // if there is a DOT, find the Z or + for the milliseconds.
  long ms = 0;
  auto dot = rest.rfind(".");
  if (dot != string::npos) {
    string rem = rest.substr(dot+1);
//    L_TRACE(rem);
    rest = rest.substr(0, dot);
    ms = atol(rem.c_str());
//    L_TRACE("ms: " << ms);
  }
//  L_TRACE("after MS: " << rest);

  tm tm = {};
  istringstream ss(rest);
  // 2024-07-01T06:54:39
  ss >> get_time(&tm, TIME_FORMAT);
   
  // make sure to account for the offset.
  tm.tm_hour -= hrs;
  tm.tm_min -= mins;

//   L_TRACE("tm_sec " << tm.tm_sec);
//   L_TRACE("tm_min " << tm.tm_min);
//   L_TRACE("tm_hour " << tm.tm_hour);
//   L_TRACE("tm_mday " << tm.tm_mday);
//   L_TRACE("tm_mon " << tm.tm_mon);
//   L_TRACE("tm_year " << tm.tm_year);
//   L_TRACE("tm_wday " << tm.tm_wday);
//   L_TRACE("tm_yday " << tm.tm_yday);
//   L_TRACE("tm_isdst " << tm.tm_isdst);

  auto t = timegm(&tm);
//  L_TRACE("t: " << t);
  
  return (t * 1000) + ms;
  
}

int64_t Date::fromRubyDate(const string &d) {

  auto spc = d.rfind(" ");
  if (spc == string::npos) {
    L_ERROR("no space in " << d);
    return 0;
  }
  
  string start = d.substr(0, spc);
//  L_TRACE(start);
  
  tm tm = {};
  istringstream ss(start);
  // 2024-07-01 06:54:39
  ss >> get_time(&tm, RUBY_TIME_FORMAT);
   
//   L_TRACE("tm_sec " << tm.tm_sec);
//   L_TRACE("tm_min " << tm.tm_min);
//   L_TRACE("tm_hour " << tm.tm_hour);
//   L_TRACE("tm_mday " << tm.tm_mday);
//   L_TRACE("tm_mon " << tm.tm_mon);
//   L_TRACE("tm_year " << tm.tm_year);
//   L_TRACE("tm_wday " << tm.tm_wday);
//   L_TRACE("tm_yday " << tm.tm_yday);
//   L_TRACE("tm_isdst " << tm.tm_isdst);

  auto t = _gm ? timegm(&tm) : timelocal(&tm);
//  L_TRACE("t: " << t);
  
  string rem = d.substr(spc+1);
//  L_TRACE(rem);
  auto plus = rem.rfind("+");
  if (plus == string::npos) {
    L_ERROR("no plus in " << rem);
    return 0;
  }

  long hrs = atol(rem.substr(1).c_str()) / 100;
//  L_TRACE(hrs);
  long offs = hrs * 60 * 60;
  if (rem[0] == '-') {
    offs *= -1;
  }
//  L_TRACE(offs);

  return (t + offs) * 1000;
  
}

string Date::getYear(const string &d) {

  return StringUtils::stringBefore(d, '-');
  
}

int64_t Date::getStartOfDay(int64_t t) {

  int ms;
  auto tm = getTM(t, &ms);
  tm.tm_hour = 0;
  tm.tm_min = 0;
  tm.tm_sec = 0;
  ms = 0;
  return putTM(&tm, &ms);
  
}

int64_t Date::getStartOfSecond(int64_t t) {

  // just wipe out the seconds.
  return (t / 1000) * 1000;
  
}

int Date::getMSNum(int64_t t) {

  time_t tnum = t / 1000;
  return t - (tnum * 1000);
  
}

int64_t Date::getStartOfMinute(int64_t t) {

  int ms;
  auto tm = getTM(t, &ms);
  tm.tm_sec = 0;
  ms = 0;
  return putTM(&tm, &ms);
  
}

int64_t Date::getStartOfHour(int64_t t) {

  int ms;
  auto tm = getTM(t, &ms);
  tm.tm_min = 0;
  tm.tm_sec = 0;
  ms = 0;
  return putTM(&tm, &ms);
  
}

int64_t Date::getStartOfMonth(int64_t t) {

  int ms;
  auto tm = getTM(t, &ms);
  tm.tm_hour = 0;
  tm.tm_min = 0;
  tm.tm_sec = 0;
  tm.tm_mday = 1;
  ms = 0;
  return putTM(&tm, &ms);
  
}

int64_t Date::addMonth(int64_t t, int count) {

  int ms;
  auto tm = getTM(t, &ms);
  tm.tm_mon += count;
  return putTM(&tm, &ms);
  
}

int64_t Date::prevMonth(int64_t t, int count) {

  int ms;
  auto tm = getTM(t, &ms);
  tm.tm_mon -= count;
  return putTM(&tm, &ms);
  
}

int Date::getMonthNum(int64_t t) {

  int ms;
  auto tm = getTM(t, &ms);
  return tm.tm_mon;
  
}

int Date::getDayNum(int64_t t) {

  int ms;
  auto tm = getTM(t, &ms);
  return tm.tm_mday;
  
}

int Date::getWeekDayNum(int64_t t) {

  int ms;
  auto tm = getTM(t, &ms);
  return tm.tm_wday;
  
}

int64_t Date::getStartOfYear(int64_t t) {

  int ms;
  auto tm = getTM(t, &ms);
  tm.tm_hour = 0;
  tm.tm_min = 0;
  tm.tm_sec = 0;
  tm.tm_mon = 0;
  tm.tm_mday = 1;
  ms = 0;
  return putTM(&tm, &ms);
  
}

int64_t Date::addYear(int64_t t, int count) {

  int ms;
  auto tm = getTM(t, &ms);
  tm.tm_year += count;
  return putTM(&tm, &ms);
  
}

int64_t Date::prevYear(int64_t t, int count) {

  int ms;
  auto tm = getTM(t, &ms);
  tm.tm_year -= count;
  return putTM(&tm, &ms);
  
}

int Date::getYearNum(int64_t t) {

  int ms;
  auto tm = getTM(t, &ms);
  return tm.tm_year + 1900;
  
}

int64_t Date::addDay(int64_t t, int count) {

  int ms;
  auto tm = getTM(t, &ms);
  tm.tm_mday += count;
  return putTM(&tm, &ms);
  
}

int64_t Date::prevDay(int64_t t, int count) {

  int ms;
  auto tm = getTM(t, &ms);
  tm.tm_mday -= count;
  return putTM(&tm, &ms);
  
}

int64_t Date::addHour(int64_t t, int count) {

  int ms;
  auto tm = getTM(t, &ms);
  tm.tm_hour += count;
  return putTM(&tm, &ms);
  
}

int64_t Date::prevHour(int64_t t, int count) {

  int ms;
  auto tm = getTM(t, &ms);
  tm.tm_hour -= count;
  return putTM(&tm, &ms);
  
}

int Date::getHourNum(int64_t t) {

  int ms;
  auto tm = getTM(t, &ms);
  return tm.tm_hour;
  
}

int64_t Date::getStartOfCentury(int64_t t) {

  int ms;
  auto tm = getTM(t, &ms);
  
  int full_year = tm.tm_year + 1900;   // tm_year is years since 1900
  int century_start_year = (full_year / 100) * 100;  // round down to nearest 100
  tm.tm_year = century_start_year - 1900;
  tm.tm_mon  = 0;
  tm.tm_mday = 1;
  tm.tm_hour = 0;
  tm.tm_min  = 0;
  tm.tm_sec  = 0;
  
  ms = 0;
  return putTM(&tm, &ms);
  
}

int64_t Date::addCentury(int64_t t, int count) {

  int ms;
  auto tm = getTM(t, &ms);
  tm.tm_year += 100 * count;
  return putTM(&tm, &ms);
  
}

int64_t Date::prevCentury(int64_t t, int count) {

  int ms;
  auto tm = getTM(t, &ms);
  tm.tm_year -= 100 * count;
  return putTM(&tm, &ms);
  
}

int Date::getCenturyNum(int64_t t) {

  int ms;
  auto tm = getTM(t, &ms);
  int full_year = tm.tm_year + 1900;
  return (full_year - 1) / 100 + 1;
  
}

int64_t Date::getStartOfMillenia(int64_t t) {

  int ms;
  auto tm = getTM(t, &ms);
  
  int full_year = tm.tm_year + 1900;
  int millennium_start_year = (full_year / 1000) * 1000;  // round down to nearest 1000

  tm.tm_year = millennium_start_year - 1900;
  tm.tm_mon  = 0;
  tm.tm_mday = 1;
  tm.tm_hour = 0;
  tm.tm_min  = 0;
  tm.tm_sec  = 0;
    
  ms = 0;
  return putTM(&tm, &ms);
  
}

int64_t Date::addMillenia(int64_t t, int count) {

  int ms;
  auto tm = getTM(t, &ms);
  tm.tm_year += 1000 * count;
  return putTM(&tm, &ms);
  
}

int64_t Date::prevMillenia(int64_t t, int count) {

  int ms;
  auto tm = getTM(t, &ms);
  tm.tm_year -= 1000 * count;
  return putTM(&tm, &ms);
  
}

int Date::getMilleniaNum(int64_t t) {

  int ms;
  auto tm = getTM(t, &ms);
  int full_year = tm.tm_year + 1900;
  return (full_year - 1) / 1000 + 1; ;
  
}

int64_t Date::getStartOfWeek(int64_t t) {

  int ms;
  auto tm = getTM(t, &ms);
  tm.tm_mday -= tm.tm_wday;   // tm_wday: 0 = Sunday, ..., 6 = Saturday
  tm.tm_hour = 0;
  tm.tm_min  = 0;
  tm.tm_sec  = 0;
  ms = 0;
  return putTM(&tm, &ms);
  
}

int64_t Date::addWeek(int64_t t, int count) {

  int ms;
  auto tm = getTM(t, &ms);
  tm.tm_mday += 7 * count;
  return putTM(&tm, &ms);
  
}

int64_t Date::prevWeek(int64_t t, int count) {

  int ms;
  auto tm = getTM(t, &ms);
  tm.tm_mday -= 7 * count;
  return putTM(&tm, &ms);
  
}

string Date::getTime(int64_t t) {

  int ms;
  auto tm = getTM(t, &ms);
  stringstream ss;
  ss << setfill('0') << setw(2) << tm.tm_hour << ":" <<
    setfill('0') << setw(2) << tm.tm_min;
  
  return ss.str();
  
}

string Date::getSeconds(int64_t t) {

  int ms;
  auto tm = getTM(t, &ms);
  stringstream ss;
  ss << setfill('0') << setw(2) << tm.tm_sec;
  
  return ss.str();
  
}

string Date::getMinutes(int64_t t) {

  int ms;
  auto tm = getTM(t, &ms);
  stringstream ss;
  ss << setfill('0') << setw(2) << tm.tm_min;
  
  return ss.str();
  
}

int64_t Date::getStartOfDecade(int64_t t) {

  int ms;
  auto tm = getTM(t, &ms);
  
  int full_year = tm.tm_year + 1900;
  int decade_start_year = (full_year / 10) * 10;   // round down to nearest 10

  tm.tm_year = decade_start_year - 1900;
  tm.tm_mon  = 0;
  tm.tm_mday = 1;
  tm.tm_hour = 0;
  tm.tm_min  = 0;
  tm.tm_sec  = 0;
    
  ms = 0;
  return putTM(&tm, &ms);
  
}

int64_t Date::addMinute(int64_t t, int count) {

  int ms;
  auto tm = getTM(t, &ms);
  tm.tm_min += count;
  return putTM(&tm, &ms);

}

int64_t Date::prevMinute(int64_t t, int count) {

  int ms;
  auto tm = getTM(t, &ms);
  tm.tm_min -= count;
  return putTM(&tm, &ms);

}

int64_t Date::addSecond(int64_t t, int count) {

  int ms;
  auto tm = getTM(t, &ms);
  tm.tm_sec += count;
  return putTM(&tm, &ms);

}

int64_t Date::prevSecond(int64_t t, int count) {

  int ms;
  auto tm = getTM(t, &ms);
  tm.tm_sec -= count;
  return putTM(&tm, &ms);

}

int64_t Date::addMS(int64_t t, int count) {

  int ms;
  auto tm = getTM(t, &ms);
  ms += count;
  return putTM(&tm, &ms);

}

std::string Date::tzMsFix(const std::string &d) {

//   getting:  2026-07-25T00:00:00+NN:00
//   required: 2026-07-12T00:00:00.0+00:00
  string d2, tz;
  StringUtils::splitString(d, '+', &d2, &tz);
  if (tz == "00:00") {
    if (d2.find('.') == string::npos) {
      d2 += ".0";
    }
    return d2 + "+00:00";
  }

  Date date(true);
  auto d3 = date.fromISODate(d2 + ".0+00:00");
  string h, m;
  StringUtils::splitString(tz, ':', &h, &m);
  auto offset = (stoi(h) * 60 + stoi(m)) * 60 * 1000;
  d3 -= offset;
  return date.toISODate(d3);
  
}

} // vops
