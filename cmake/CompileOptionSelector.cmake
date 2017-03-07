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
# Copyright (c) 2016-2017 Research Institute for Information Technology (RIIT), Kyushu University.
# All rights reserved.
#
###################################################################################

##
## Compile option selector
##

macro (AddOptimizeOption)
  if (TARGET_ARCH STREQUAL "FX10")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Kfast,ocl,preex,simd=2,array_private,parallel,optmsg=2 -V -Nsrc -x0 -Xg")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Kfast,ocl,preex,simd=2,array_private,parallel,optmsg=2 -V -Nsrc -x0 -Xg")
    # -Xg   : gcc compatible flag to suppress -rdynamic

  elseif (TARGET_ARCH STREQUAL "FX100")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Kfast,ocl,preex,simd=2,array_private,parallel,optmsg=2 -V -Nsrc -x0 -Xg")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Kfast,ocl,preex,simd=2,array_private,parallel,optmsg=2 -V -Nsrc -x0 -Xg")

  elseif (TARGET_ARCH STREQUAL "K")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Kfast,ocl,preex,simd=2,array_private,parallel,optmsg=2 -V -Nsrc -x0 -Xg")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Kfast,ocl,preex,simd=2,array_private,parallel,optmsg=2 -V -Nsrc -x0 -Xg")

  elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -Wall -fno-strict-aliasing")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3 -Wall -fno-strict-aliasing")

  elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Intel")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -fno-strict-aliasing")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3 -fno-strict-aliasing")

  elseif(CMAKE_CXX_COMPILER_ID STREQUAL "PGI")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fastsse")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fastsse")

  else()
    message("using default option")
  endif()
endmacro()

macro (AddSSE)
  if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    if(CMAKE_SYSTEM_NAME MATCHES "Darwin")
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -msse3")
    else()
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native")
    endif()
  elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Intel")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -xHost")
  elseif(CMAKE_CXX_COMPILER_ID STREQUAL "PGI")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fastsse")
  endif()
endmacro()


macro(C99)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99")
endmacro()


macro(CPP11)
  include(CheckCXXCompilerFlag)
  CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
  CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
  if(COMPILER_SUPPORTS_CXX11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
  elseif(COMPILER_SUPPORTS_CXX0X)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
  endif()
endmacro()


macro(checkOpenMP)
  if(enable_OPENMP)
    if(CMAKE_CXX_COMPILER MATCHES ".*FCCpx$")
      set(OpenMP_C_FLAGS "-Kopenmp")
      set(OpenMP_CXX_FLAGS "-Kopenmp")
    else()
      find_package(OpenMP REQUIRED)
    endif()

    # OpenMP_*_FLAGSにはfind_package(OpenMP REQUIRED)でオプションフラグが設定される
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
  endif()
endmacro()


macro(precision)
  if(real_type STREQUAL "float")
  else()
    ADD_DEFINITIONS(-D_REAL_IS_DOUBLE_)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_REAL_IS_DOUBLE_")
  ENDIF()
endmacro()
