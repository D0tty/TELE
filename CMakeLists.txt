cmake_minimum_required(VERSION 3.17)
project(tele LANGUAGES CXX)
enable_language(CXX)
set(CMAKE_CXX_STANDARD 17)
set(THREADS_PREFER_PTHREAD_FLAG TRUE)

include(FindPkgConfig)

find_package(Boost REQUIRED)
find_package(Threads REQUIRED)
pkg_search_module(SDL2 REQUIRED sdl2)
pkg_search_module(SDL2IMAGE REQUIRED SDL2_image)
include_directories(${SDL2_INCLUDE_DIRS} ${SDL2IMAGE_INCLUDE_DIRS})

include_directories(${Boost_INCLUDE_DIRS})

add_executable(${PROJECT_NAME}
        src/main.cpp
        include/csv.hpp src/misc/misc.cc src/misc/misc.hh)

add_compile_options(
        -Wall
        -Wextra
        -Werror
        -O0
        -g3)

target_include_directories(${PROJECT_NAME}
        PUBLIC include)

target_link_libraries(${PROJECT_NAME}
        pthread
        ${Boost_LIBRARIES}
        ${SDL2_LIBRARIES}
        ${SDL2IMAGE_LIBRARIES})