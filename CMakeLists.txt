cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_POSITION_INDEPENDENT_CODE On)

if(NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_BUILD_TYPE "Release")
    message("Building release")
else()
    message("Building debug")
endif()

project(SPS_SABRE_EventBuilder)

find_package(ROOT REQUIRED COMPONENTS Gui)

set(EVB_BINARY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/bin)
set(EVB_LIBRARY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib)

add_subdirectory(vendor/yaml-cpp)
add_subdirectory(src)