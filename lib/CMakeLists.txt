cmake_minimum_required(VERSION 2.8)

add_library(${CMAKE_PROJECT_NAME} STATIC
        ../include/vm.h
        types.h
        rtda.h
        instruction.h
        ../littlefs/lfs.h
        ../littlefs/lfs_util.h
        context.h
        vm.c
        rtda.c
        instructions.c
        ../littlefs/lfs.c
        ../littlefs/lfs_util.c
        context.c
        )
