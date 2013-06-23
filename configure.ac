##############################################################################
#
# Polylib - Polygon Management Library
#
# Copyright (c) 2010-2011 VCAD System Research Program, RIKEN. 
# All rights reserved.
# 
# Copyright (c) 2012-2013 Advanced Institute for Computational Science, RIKEN.
# All rights reserved.
#
##############################################################################
#
#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([Polylib], [2.6], [keno@riken.jp],[Polylib])
AM_INIT_AUTOMAKE()
AC_CONFIG_SRCDIR([src/MPIPolylib.cxx])
AC_CONFIG_HEADERS([include/Polylib.h])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_CPP
AC_PROG_MAKE_SET
AC_PROG_RANLIB

# Checks for libraries.
# FIXME: Replace `main' with a function in `-lMPIPolylib':
AC_CHECK_LIB([MPIPolylib], [main])
# FIXME: Replace `main' with a function in `-lPolylib':
AC_CHECK_LIB([Polylib], [main])
# FIXME: Replace `main' with a function in `-lTextParser':
AC_CHECK_LIB([TextParser], [main])
# FIXME: Replace `main' with a function in `-lTextParser_mpi':
AC_CHECK_LIB([TextParser_mpi], [main])

# Checks for header files.
AC_CHECK_HEADERS([stdlib.h string.h sys/time.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_SIZE_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_CHECK_FUNCS([gettimeofday memset strchr strstr])

AC_CONFIG_FILES([Makefile
                 docs/Makefile
                 examples/Makefile
                 src/Makefile])
AC_OUTPUT
