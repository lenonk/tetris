cmake_minimum_required(VERSION 4.1)
project(tetris)

set(CMAKE_CXX_STANDARD 26)

# Release build optimizations
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG -march=native -flto")
    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
endif()

set(HDRS
        include/Colors.h
        include/Grid.h
        include/Game.h
        include/Block.h
        include/Position.h
)

set(SRCS
        src/main.cpp
        src/Colors.cpp
        src/Grid.cpp
        src/Game.cpp
        src/Block.cpp
)

add_executable(tetris ${SRCS})

target_link_libraries(tetris raylib)
target_include_directories(tetris PRIVATE include)
