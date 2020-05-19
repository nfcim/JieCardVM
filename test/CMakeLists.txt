cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(TEST_EXE ${CMAKE_PROJECT_NAME}_test)

add_executable(${TEST_EXE}
        main.cpp
        instructions.cpp
        context.cpp
        ../littlefs/bd/lfs_filebd.c
)

target_link_libraries(${TEST_EXE} ${CMAKE_PROJECT_NAME})
target_include_directories(${TEST_EXE} PRIVATE "")
