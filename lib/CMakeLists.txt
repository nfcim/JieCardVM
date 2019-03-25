cmake_minimum_required(VERSION 2.8)

add_library(${CMAKE_PROJECT_NAME} STATIC
        instruction.h
        ../include/vm.h
        vm.c
)
