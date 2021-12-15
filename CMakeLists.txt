cmake_minimum_required(VERSION 3.13)
project(SPS_SABRE_EventBuilder)
set(CMAKE_CXX_STANDARD 11)

find_package(ROOT 6.22 CONFIG REQUIRED)

if(${ROOT_VERSION} VERSION_LESS "6.20")
	include("${ROOT_DIR}/modules/RootNewMacros.cmake")
	# For ROOT versions than 6.16, things break 
	# if nothing is in the global include list!
	if (${ROOT_VERSION} VERSION_LESS "6.16")
	   include_directories(ROOT_NONEXISTENT_DIRECTORY_HACK)
	endif()
endif()

if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
	set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_SOURCE_DIR} CACHE PATH "..." FORCE)
endif(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)

set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")

set(PCH_DIR ${CMAKE_CURRENT_LIST_DIR}/src/)

add_subdirectory(src)