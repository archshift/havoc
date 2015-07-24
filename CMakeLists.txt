cmake_minimum_required(VERSION 2.8.4)
project(havoc)

find_package(PkgConfig REQUIRED)
pkg_search_module(LIBUSB REQUIRED libusb-1.0)
pkg_search_module(TCLAP REQUIRED tclap)
include_directories(${LIBUSB_INCLUDE_DIRS})
include_directories(${TCLAP_INCLUDE_DIRS})

if (NOT MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
endif()

add_subdirectory("src")
