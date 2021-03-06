###################################################################################
#
# Polylib - Polygon Management Library
#
# Copyright (c) 2010-2011 VCAD System Research Program, RIKEN.
# All rights reserved.
#
# Copyright (c) 2012-2015 Advanced Institute for Computational Science (AICS), RIKEN.
# All rights reserved.
#
# Copyright (c) 2016-2018 Research Institute for Information Technology (RIIT), Kyushu University.
# All rights reserved.
#
###################################################################################
#
#-*- CMAKE -*-#
# Process this file with cmake to produce a configure script.

# Following parameters must be given at cmake command line

# Build options, see Readme.md for details.
#
# -D INSTALL_DIR=install_directory
#
# -D real_type={float|double}
#
# -D with_MPI={yes|no}
#
# -D with_example={no|yes}
#
# -D with_TP=Installed_directory
#

cmake_minimum_required(VERSION 2.6)


#######
# Build options
#######

option (real_type "Type of floating point" "OFF")
option (with_MPI "Enable MPI" "ON")
option (with_example "Compiling examples" "OFF")



#######
# Project setting
#######

project(Polylib C CXX)

if(INSTALL_DIR STREQUAL "")
  set(INSTALL_DIR "/usr/local/Polylib")
endif()

# project()でCMAKE_INSTALL_PREFIX=/usr/localとなるので、再定義
set (CMAKE_INSTALL_PREFIX "${INSTALL_DIR}")

#set(CMAKE_BUILD_TYPE Release)


#######
# Policy setting
#######

cmake_policy(SET CMP0012 NEW)


#######
set(PROJECT_VERSION "3.7.2")
set(LIB_REVISION "20180925_1830")
#######



#######
# Cmake module
#######

set(CMAKE_MODULE_PATH  ${PROJECT_SOURCE_DIR}/cmake)
include(${PROJECT_SOURCE_DIR}/cmake/CompileOptionSelector.cmake)
include(${PROJECT_SOURCE_DIR}/cmake/poly_package.cmake)



#######
# Options
#######

AddOptimizeOption()

precision()



#######
# Display options
#######

message(" ")
message( STATUS "Destination PATH: "               ${CMAKE_INSTALL_PREFIX})
message( STATUS "CMAKE_C_COMPILER: "               ${CMAKE_C_COMPILER})
message( STATUS "CMAKE_C_FLAGS: "                  ${CMAKE_C_FLAGS})
message( STATUS "CMAKE_CXX_COMPILER: "             ${CMAKE_CXX_COMPILER})
message( STATUS "CMAKE_CXX_FLAGS: "                ${CMAKE_CXX_FLAGS})

message(" ")
message( STATUS "Type of floating point : "    ${real_type})
message( STATUS "MPI support            : "    ${with_MPI})
message( STATUS "TextParser support     : "    ${with_TP})
message( STATUS "Example                : "    ${with_example})
message(" ")

if(CMAKE_CXX_COMPILER MATCHES ".*FCCpx$")
else()
  if(with_MPI)
    include(FindMPI)

    if(MPI_CXX_FOUND)
      message("MPI-CXX-CMP  : ${MPI_CXX_COMPILER}")
      message("MPI-CXX-FLAGS: ${MPI_CXX_COMPILE_FLAGS}")
      message("MPI-CXX-INC  : ${MPI_CXX_INCLUDE_PATH}")
      message("MPI-LNK-FLAGS: ${MPI_CXX_LINK_FLAGS}")
      message("MPI-LIBS     : ${MPI_CXX_LIBRARIES}")
    endif(MPI_CXX_FOUND)
  endif()
endif()


if(USE_F_TCS STREQUAL "YES")
else()
  if(with_MPI)
    include(FindMPI)

    if(MPI_CXX_FOUND)
      message("MPI-CXX-CMP  : ${MPI_CXX_COMPILER}")
      message("MPI-CXX-FLAGS: ${MPI_CXX_COMPILE_FLAGS}")
      message("MPI-CXX-INC  : ${MPI_CXX_INCLUDE_PATH}")
      message("MPI-LNK-FLAGS: ${MPI_CXX_LINK_FLAGS}")
      message("MPI-LIBS     : ${MPI_CXX_LIBRARIES}")
    endif(MPI_CXX_FOUND)
  endif()
endif()


message(" ")
message("PROJECT : Polylib " ${PROJECT_VERSION} " : " ${LIB_REVISION})
message(" ")



#######
# Find Libraries to depend on
#######

# TextParser
IF(with_TP)
  SET(TP_DIR "${with_TP}")
  SET(TP_INC "${TP_DIR}/include")
  SET(TP_LIB "${TP_DIR}/lib")
ELSE()
  MESSAGE("Error: can not find Text Parser")
ENDIF()



#######
# Example
#######

if (with_example)
  enable_testing()
  message("Code test will be performed")
else()
  message("Code test is skipped")
endif()



#######
# Sub Directories
#######

add_subdirectory(src)

add_subdirectory(doc)

if(with_example)
  add_subdirectory(examples)
endif()


#######
# configure files
#######

configure_file( include/polyVersion.h.in ${PROJECT_BINARY_DIR}/include/polyVersion.h @ONLY)


#######
# Install files
#######

install(FILES  ChangeLog.md
               Readme.md
               License.txt
               ${PROJECT_BINARY_DIR}/CMakeCache.txt
        DESTINATION share)


#######
# Installer
#######

polylib_package()
