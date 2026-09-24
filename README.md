# dates

Library dealing with dates in UTC and 64 bit integer format.

# to build

```
mkdir build
cd build
cmake ..
make -j4
make test
```

In your CMakeLists.txt you can use like this

```
FetchContent_Declare(
  dates
  GIT_REPOSITORY https://github.com/visualopsholdings/dates.git
  GIT_TAG main
)
FetchContent_MakeAvailable(dates)

include_directories(${dates_SOURCE_DIR}/include)
```

Then use "DatesLib" in your target libraries.
