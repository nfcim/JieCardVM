cmake_minimum_required(VERSION 2.8)
project(jiecardvm LANGUAGES C CXX)

set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 11)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -g -D_GNU_SOURCE")

# public headers
include_directories("include" "littlefs")

add_subdirectory(lib)
add_subdirectory(test)
