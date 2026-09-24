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

// std::optional<std::string> StringUtils::system(std::optional<std::string> cmd) {
// 
//   if (!cmd) {
//     return std::nullopt;
//   }
//   
//   FILE *pipe = popen(cmd->c_str(), "r");
//   if (!pipe) {
//     BOOST_LOG_TRIVIAL(error) << "Failed to run command " << *cmd;
//     return std::nullopt;
//   }
// 
//   char buffer[128];
//   std::ostringstream output;
//   while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
//     output << buffer;
//   }
//   pclose(pipe);
//   
//   return output.str();
// 
// }
// 
// std::string StringUtils::stitchBash(const std::vector<std::string> &lines) {
// 
//   std::string cmd;
//   int n = 0;
//   for (auto l: lines) {
//     if (!cmd.empty()) {
//       cmd += "\n";
//     }
//     cmd += "echo line " + std::to_string(n) + ":;" + l + " || exit 1;";
//     n++;
//   }
//   
//   return cmd;
//   
// }
// 
// void StringUtils::systemRun(std::optional<std::string> cmd, std::ostream& outfile, bool output) {
// 
//   if (!cmd) {
//     return;
//   }
//   
//   outfile << "$ " << *cmd << std::endl;
// 
//   FILE *pipe = popen(cmd->c_str(), "r");
//   if (!pipe) {
//     BOOST_LOG_TRIVIAL(error) << "Failed to run command " << *cmd;
//     return;
//   }
//   
//   char buffer[128];
//   while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
//     if (output) {
//       std::cout << buffer << std::flush;
//     }
//     else {
//       L_DEBUG(buffer);
//     }
//     outfile << buffer;
//   }
//   outfile << std::endl;
//   pclose(pipe);
// 
// }
// 
// std::string StringUtils::expandTilde(const std::string &path) {
// 
//   if (!path.empty() && path[0] == '~') {
//     std::string p = path;
//     const char* home = getenv("HOME");
//     if (home) {
//       p.replace(0, 1, home);
//     }
//     return p;
//   }
//   
//   return path;
//   
// }

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
