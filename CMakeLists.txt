# Catch2 v3 wired in the way its docs/cmake-integration.md teaches:
# FetchContent pulls a pinned release, Catch2::Catch2WithMain supplies main(),
# and catch_discover_tests() registers every TEST_CASE with CTest.
cmake_minimum_required(VERSION 3.16)

project(calc VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
  Catch2
  # A release tarball rather than the docs' GIT_REPOSITORY/GIT_TAG: no git is
  # needed to build, and the download is pinned by hash.
  URL https://github.com/catchorg/Catch2/archive/refs/tags/v3.8.1.tar.gz
  URL_HASH SHA256=18b3f70ac80fccc340d8c6ff0f339b2ae64944782f8d2fca2bd705cf47cadb79
  DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
FetchContent_MakeAvailable(Catch2)

# --- the library under test ---------------------------------------------------
add_library(calc src/calc.cpp)
target_include_directories(calc PUBLIC include)

# --- its tests ------------------------------------------------------------------
add_executable(tests tests/calc_test.cpp)
target_link_libraries(tests PRIVATE calc Catch2::Catch2WithMain)

list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
include(CTest)
include(Catch)
catch_discover_tests(tests)
