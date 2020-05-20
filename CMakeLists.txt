cmake_minimum_required(VERSION 2.8)
project(jiecardvm LANGUAGES C CXX)

set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 11)

set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -Wall -g")

# public headers
include_directories("include" "littlefs" "lib")

add_subdirectory(lib)
add_subdirectory(test)
