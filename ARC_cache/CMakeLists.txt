cmake_minimum_required(VERSION 3.11)
project("ARC-cache")

SET(SRCS src/test.cpp; src/main.cpp)

add_library(cache_lib STATIC ${SRCS})
target_include_directories(cache_lib PUBLIC "include")

add_executable(a src/main.cpp)
target_link_libraries(a cache_lib)
