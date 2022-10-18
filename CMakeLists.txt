cmake_minimum_required(VERSION 3.16)
project(CFG)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

set(CFG_lib
        CFG.cpp
        CFG.h
        Components.cpp
        Components.h
        Tree.cpp
        Tree.h
        )

add_executable(CFG_testFile1
        CFG_testfile.cpp
        ${CFG_lib}
        )

add_executable(CFG_testfile2
        CFG_testfile2.cpp
        ${CFG_lib}
        )

add_executable(CNF_testfile
        CNF_testfile.cpp
        ${CFG_lib}
        )
