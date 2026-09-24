#   CMakeLists.txt
#
#   Author: Paul Hamilton (paul@visualops.com)
#   Date: 24-Sep-2026
#
#   Input file for Dates libray.
#
#   Licensed under [version 3 of the GNU General Public License] contained in LICENSE.
#   
#   https://github.com/visualopsholdings/dates

cmake_minimum_required(VERSION 3.5)
cmake_policy(SET CMP0167 NEW) # Boost
cmake_policy(SET CMP0169 OLD) # FetchContent

set(USE_MONGO ON)

project (dates)
  find_package(Boost 1.90.0 COMPONENTS unit_test_framework chrono log_setup log REQUIRED)
  add_definitions(-DBOOST_ALL_DYN_LINK) 
  enable_testing()
  
set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (UNIX AND NOT APPLE)
  add_definitions(-funwind-tables) 
endif ()

include_directories(include)

add_library(DatesLib STATIC
  src/date.cpp 
  src/stringutils.cpp 
  src/log.cpp)

# Tests

add_executable(TestDate test/testdate.cpp)
  target_link_libraries(TestDate DatesLib ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY} ${Boost_LOG_LIBRARY})

add_test(TestDate TestDate)

add_executable(TestStringUtils test/teststringutils.cpp src/stringutils.cpp)
  target_link_libraries(TestStringUtils ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY} ${Boost_LOG_LIBRARY})

add_test(TestStringUtils TestStringUtils)

