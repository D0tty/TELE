cmake_minimum_required(VERSION 3.17)
project(tele LANGUAGES CXX)
enable_language(CXX)
set(CMAKE_CXX_STANDARD 17)
set(THREADS_PREFER_PTHREAD_FLAG TRUE)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
include(FindPkgConfig)

file(CREATE_LINK ../images ./images SYMBOLIC)

find_package(Boost REQUIRED COMPONENTS system filesystem)
find_package(Threads REQUIRED)
find_package(Qt5Widgets REQUIRED)
pkg_search_module(SDL2 REQUIRED sdl2)
pkg_search_module(SDL2IMAGE REQUIRED SDL2_image)
include_directories(${SDL2_INCLUDE_DIRS} ${SDL2IMAGE_INCLUDE_DIRS} ${Qt5Widgets_INCLUDE_DIRS})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")
message(STATUS "Boost version: ${Boost_VERSION}")

add_executable(${PROJECT_NAME}
        src/main.cc

        include/csv.hpp

        src/misc/misc.cc
        src/misc/misc.hh
        src/misc/Singleton.cc
        src/misc/Singleton.hh

        src/data/Coordinate.cc
        src/data/Coordinate.hh

        src/metadata/GeoTaggedImage.cc
        src/metadata/GeoTaggedImage.hh
        src/metadata/GeoTaggedImageList.cc
        src/metadata/GeoTaggedImageList.hh

        src/gui/Imageviewer.cc
        src/gui/Imageviewer.hh

        src/maths/Convert.cc
        src/maths/Convert.hh

        src/data/Export.cc
        src/data/Export.hh)

add_compile_options(
        -Wall
        -Wextra
        -Werror
        -O0
        -g3)

target_include_directories(${PROJECT_NAME} PUBLIC
        src
        include
        ${Qt5Widgets_INCLUDE_DIRS}
        ${Boost_INCLUDE_DIRS})

target_link_libraries(${PROJECT_NAME}
        pthread
        ${Qt5Widgets_LIBRARIES}
        ${Boost_LIBRARIES}
        ${SDL2_LIBRARIES}
        ${SDL2IMAGE_LIBRARIES})