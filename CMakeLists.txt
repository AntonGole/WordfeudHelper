cmake_minimum_required(VERSION 3.10)
project(WordfeudHelper LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


file(GLOB_RECURSE LIB_SOURCES src/*.cpp)
list(FILTER LIB_SOURCES EXCLUDE REGEX "main\\.cpp$")
add_library(WordfeudHelperLib ${LIB_SOURCES})
target_include_directories(WordfeudHelperLib PUBLIC include)

add_executable(WordfeudHelper main.cpp)
target_link_libraries(WordfeudHelper PRIVATE WordfeudHelperLib)


option(BUILD_TESTS "Build unit tests" OFF)

if(BUILD_TESTS)
  enable_testing()
  include(FetchContent)

  FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.13.0.zip
  )
  set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
  FetchContent_MakeAvailable(googletest)

  add_executable(WordfeudHelperTests
    tests/test_main.cpp
  )
  target_link_libraries(WordfeudHelperTests
    PRIVATE WordfeudHelperLib gtest_main
  )
  target_include_directories(WordfeudHelperTests PRIVATE include)

  include(GoogleTest)
  gtest_discover_tests(WordfeudHelperTests)
endif()
