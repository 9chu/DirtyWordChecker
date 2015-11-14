cmake_minimum_required(VERSION 2.6)
project(DirtyWordChecker)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

set(SOURCE_FILES
    DirtyWordChecker.cpp
    main.cpp)

add_executable(DirtyWordChecker ${SOURCE_FILES})
