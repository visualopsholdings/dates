/*
  stringutils.hpp
  
  Author: Paul Hamilton (paul@visualops.com)
  Date: 24-Sep-2026
    
  Utilities for strings.
  
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/visualopsholdings/dates
*/

#ifndef H_stringutils
#define H_stringutils

#include <string>
#include <vector>

namespace vops {

class StringUtils {

public:
  
  static void splitString(const std::string &s, char token, std::string *s1, std::string *s2);
  static void splitString(const std::string &s, char token, std::vector<std::string> *parts);
  static std::string stringBefore(const std::string &s, char token);
  static std::string stringAfter(const std::string &s, char token);
  static std::string trim(const std::string& str);

};

} // vops

#endif // H_stringutils
