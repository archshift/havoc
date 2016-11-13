cmake_minimum_required(VERSION 3.0)
include(ExternalProject)
project(havoc)

if (NOT MSVC)
    find_package(PkgConfig REQUIRED)
    pkg_search_module(LIBUSB REQUIRED libusb-1.0)
    pkg_search_module(TCLAP REQUIRED tclap)

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
else()
    set(LIBUSB_DIR "externals/libusb")
	set(LIBUSB_OUTPUT_PREFIX "${CMAKE_BINARY_DIR}/libusb-prefix/")
	externalproject_add(libusb
		PREFIX ${LIBUSB_OUTPUT_PREFIX}
		SOURCE_DIR ${CMAKE_SOURCE_DIR}/${LIBUSB_DIR}
		CONFIGURE_COMMAND ""
		BUILD_COMMAND msbuild /p:Configuration=Release /p:Platform=x64
			/p:OutDir=${LIBUSB_OUTPUT_PREFIX} /p:IntDir=${LIBUSB_OUTPUT_PREFIX}/build/
			/m ${CMAKE_SOURCE_DIR}/${LIBUSB_DIR}/msvc/libusb_dll_2015.vcxproj
		COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_SOURCE_DIR}/${LIBUSB_DIR}/libusb/libusb.h" "${LIBUSB_OUTPUT_PREFIX}/include/libusb-1.0/libusb.h"
		INSTALL_COMMAND ""
		)
	
	set(LIBUSB_INCLUDE_DIRS ${LIBUSB_OUTPUT_PREFIX}/include)
	set(LIBUSB_LIBRARIES ${LIBUSB_OUTPUT_PREFIX}/libusb-1.0.lib)

	set(TCLAP_DIR "externals/tclap")
	set(TCLAP_INCLUDE_DIRS "${CMAKE_SOURCE_DIR}/${TCLAP_DIR}/include")
endif()

include_directories(${TCLAP_INCLUDE_DIRS})
include_directories(${LIBUSB_INCLUDE_DIRS})

add_subdirectory("src")
