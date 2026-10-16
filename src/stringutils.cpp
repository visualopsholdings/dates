/*
  stringutils.cpp
  
  Author: Paul Hamilton (paul@visualops.com)
  Date: 24-Sep-2026
    
  Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
 
  https://github.com/visualopsholdings/dates
*/

#include "stringutils.hpp"

#include "log.hpp"

#include <iostream>

#if TARGET_OS_OSX
#include <ranges>
namespace r = std::ranges;
#else
// for GCC 11, we use range-v3
#include <range/v3/all.hpp> 
namespace r = ranges;
#endif

namespace vops {

std::string StringUtils::trim(const std::string& str) {

  const std::string whitespace = " \t\n\r\f\v";
  const auto start = str.find_first_not_of(whitespace);
  if (start == std::string::npos) {
    return ""; // String is all whitespace
  }
  
  const auto end = str.find_last_not_of(whitespace);
  const auto range = end - start + 1;
  
  return str.substr(start, range);
}

void StringUtils::splitString(const std::string &s, char token, std::vector<std::string> *parts) {

  if (!parts) {
    return;
  }

  std::string::size_type start = 0;
  std::string::size_type end;

  while ((end = s.find(token, start)) != std::string::npos) {
      parts->push_back(s.substr(start, end - start));
      start = end + 1;
  }
  parts->push_back(s.substr(start));
  
}

void StringUtils::splitString(const std::string &s, char token, std::string *s1, std::string *s2) {

  auto words = s | r::views::split(token);
  auto i = words.begin();
  if (i == words.end()) {
    L_ERROR("splitString no string");
  }
  *s1 = r::to<std::string>(*i);
  i++;
  if (i == words.end()) {
    L_ERROR("splitString no second string");
  }
  *s2 = r::to<std::string>(*i);

}

std::string StringUtils::stringBefore(const std::string &s, char token) {
  auto words = s | r::views::split(token);
  auto i = words.begin();
  if (i == words.end()) {
    L_ERROR("stringBefore no string");
  }
  return r::to<std::string>(*i);
}

std::string StringUtils::stringAfter(const std::string &s, char token) {
  auto words = s | r::views::split(token);
  auto i = words.begin();
  if (i == words.end()) {
    L_ERROR("stringAfter no string");
  }
  i++;
  if (i == words.end()) {
    L_ERROR("stringAfter no second string");
  }
  return r::to<std::string>(*i);
}

// SDL_Color StringUtils::hexToSDLColor(const std::string& hex, Uint8 alpha) {
// 
//   std::string h = hex;
//   
//   if (!h.empty() && h[0] == '#') {
//       h = h.substr(1);
//   }
//   
//   uint32_t value = static_cast<uint32_t>(std::strtoul(h.c_str(), nullptr, 16));
// 
//   SDL_Color color;
//   color.r = (value >> 16) & 0xFF;
//   color.g = (value >> 8) & 0xFF;
//   color.b = value & 0xFF;
//   color.a = alpha;
// 
//   return color;
//   
// }

} // vops
