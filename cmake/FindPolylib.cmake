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

# - Try to find Polylib
# Once done, this will define
#
#  PL_FOUND - system has Polylib
#  PL_INCLUDE_DIRS - Polylib include directories
#  PL_LIBRARIES - link these to use Polylib

include(LibFindMacros)

# Use pkg-config to get hints about paths
libfind_pkg_check_modules(PL_PKGCONF PL)

if(CMAKE_PREFIX_PATH)
  set(PL_CANDIDATE_PATH ${CMAKE_PREFIX_PATH})
  file(GLOB tmp "${CMAKE_PREFIX_PATH}/[Jj][Hh][Pp][Cc][Nn][Dd][Ff]*/")
  list(APPEND PL_CANDIDATE_PATH ${tmp})
endif()

# Include dir
find_path(PL_INCLUDE_DIR
  NAMES Polylib.h
  PATHS ${PL_ROOT} ${PL_PKGCONF_INCLUDE_DIRS} ${PL_CANDIDATE_PATH}
  PATH_SUFFIXES include
)

# Finally the library itself
find_library(PL_LIBRARY
  NAMES PL
  PATHS ${PL_ROOT} ${PL_PKGCONF_LIBRARY_DIRS} ${PL_CANDIDATE_PATH}
  PATH_SUFFIXES lib
)

# Set the include dir variables and the libraries and let libfind_process do the rest.
# NOTE: Singular variables for this library, plural for libraries this this lib depends on.
set(PL_PROCESS_INCLUDES PL_INCLUDE_DIR)
set(PL_PROCESS_LIBS PL_LIBRARY)
libfind_process(PL)
