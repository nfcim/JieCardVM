cmake_minimum_required(VERSION 2.8)

add_library(${CMAKE_PROJECT_NAME} STATIC
        ../include/vm.h
        rtda.h
        instructions.h
        library.h
        ../littlefs/lfs.h
        ../littlefs/lfs_util.h
        context.h
        utils.h
        globals.h
        vm.c
        rtda.c
        instructions.c
        library.c
        ../littlefs/lfs.c
        ../littlefs/lfs_util.c
        context.c
        utils.c
        globals.c
        )
