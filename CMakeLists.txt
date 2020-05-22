cmake_minimum_required(VERSION 2.8)
project(jiecardvm LANGUAGES C CXX)

option(ENABLE_DEBUG_OUTPUT "Print debug messages" ON)


set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 11)

set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -Wall -g")
add_compile_options("-fsanitize=address")
link_libraries("-fsanitize=address")

if (ENABLE_DEBUG_OUTPUT)
    add_definitions(-DDEBUG_OUTPUT)
endif()

# public headers
include_directories("include" "littlefs" "lib")

add_subdirectory(lib)
add_subdirectory(test)
