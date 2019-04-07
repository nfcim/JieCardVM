cmake_minimum_required(VERSION 2.8)

add_library(${CMAKE_PROJECT_NAME} STATIC
        ../include/vm.h
        types.h
        rtdata.h
        instruction.h
        interpreter.h
        vm.c
        rtdata.c
        instructions.c
        interpreter.c
)
