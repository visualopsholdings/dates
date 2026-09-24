/*
  date.hpp
  
  Author: Paul Hamilton (paul@visualops.com)
  Date: 18-Sep-2024
    
  Date utilies for dealing with ISO string dates, and out unique "time_t" like
  int64 based date. It's time_t * 1000 and then the ms added.
  
  ISO dates look likt this:
    2024-07-25T06:54:39.599+00:00
    2024-07-25T06:54:39.599Z
  Our internal format is this as a int64_t, just like time_t but in milliseconds not seconds.
  
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/visualopsholdings/dates
*/

#ifndef H_date
#define H_date

#include <string>

namespace vops {

class Date {
 
public:
  Date(bool gm=false): _gm(gm) {}
  
  static int64_t now();

  static std::string tzMsFix(const std::string &d);

  std::string getFutureTime(int64_t now, int hours);
  int64_t fromRubyDate(const std::string &d);
  
  std::string toISODate(std::tm *tm, int ms) const;
  std::string toISODate(int64_t y) const;
  int64_t fromISODate(const std::string &d);
  
  int getMSNum(int64_t t);
  int64_t addMS(int64_t t, int count=1);

  int64_t getStartOfSecond(int64_t t);
  int64_t addSecond(int64_t t, int count=1);
  int64_t prevSecond(int64_t t, int count=1);
  std::string getSeconds(int64_t t);
  
  int64_t getStartOfMinute(int64_t t);
  int64_t addMinute(int64_t t, int count=1);
  int64_t prevMinute(int64_t t, int count=1);
  std::string getMinutes(int64_t t);

  int64_t getStartOfHour(int64_t t);
  int64_t addHour(int64_t t, int count=1);
  int64_t prevHour(int64_t t, int count=1);
  int getHourNum(int64_t t);

  std::string getTime(int64_t t);

  int64_t getStartOfDay(int64_t t);
  int64_t addDay(int64_t t, int count=1);
  int64_t prevDay(int64_t t, int count=1);
  int getDayNum(int64_t t);

  int64_t getStartOfWeek(int64_t t);
  int64_t addWeek(int64_t t, int count=1);
  int64_t prevWeek(int64_t t, int count=1);
  int getWeekDayNum(int64_t t);

  int64_t getStartOfMonth(int64_t t);
  int64_t addMonth(int64_t t, int count=1);
  int64_t prevMonth(int64_t t, int count=1);
  int getMonthNum(int64_t t);

  int64_t getStartOfYear(int64_t t);
  int64_t addYear(int64_t t, int count=1);
  int64_t prevYear(int64_t t, int count=1);
  int getYearNum(int64_t t);
  std::string getYear(const std::string &d);
  
  int64_t getStartOfDecade(int64_t t);

  int64_t getStartOfCentury(int64_t t);
  int64_t addCentury(int64_t t, int count=1);
  int64_t prevCentury(int64_t t, int count=1);
  int getCenturyNum(int64_t t);

  int64_t getStartOfMillenia(int64_t t);
  int64_t addMillenia(int64_t t, int count=1);
  int64_t prevMillenia(int64_t t, int count=1);
  int getMilleniaNum(int64_t t);
  
private:
  bool _gm;
  std::tm getTM(int64_t t, int *ms=nullptr) const;
  int64_t putTM(std::tm *tm, int *ms=nullptr);
    
};

} // vops

#endif // H_date
