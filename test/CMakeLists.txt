cmake_minimum_required(VERSION 2.8)

set(TEST_EXE ${CMAKE_PROJECT_NAME}_test)

add_executable(${TEST_EXE}
        main.c
)

target_link_libraries(${TEST_EXE} ${CMAKE_PROJECT_NAME})
