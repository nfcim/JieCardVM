cmake_minimum_required(VERSION 2.8)

add_library(${CMAKE_PROJECT_NAME} STATIC
        types.h
        rtdata.h
        instruction.h
        ../include/vm.h
        vm.c
        rtdata.c
        instructions.c
)
