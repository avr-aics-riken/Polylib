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
AC_INIT([Polylib], [2.6.4], [keno@riken.jp],[Polylib])
AM_INIT_AUTOMAKE()
AC_CONFIG_SRCDIR([src/MPIPolylib.cxx])
AC_CONFIG_HEADERS([config.h])


#
# revision No.
#
PL_REVISION="20130627_2359"
AC_SUBST(PL_REVISION)


#
# platform
#
PL_DEVICE="$($SHELL ./polylib-uname)"
AC_SUBST(PL_DEVICE)

#
# MPI environment
#
enable_mpi="none"

AC_SUBST(MPI_DIR)
AC_SUBST(MPI_CFLAGS)
AC_SUBST(MPI_LDFLAGS)
AC_SUBST(MPI_LIBS)


# mpich
AC_ARG_WITH(mpich, [AC_HELP_STRING([--with-mpich=dir],[Specify MPICH install directory])], , with_mpich=none)
if test "$with_mpich" != "none" ; then
  MPI_DIR=$with_mpich;
  MPI_CFLAGS=-I$MPI_DIR/include
  MPI_LDFLAGS=-L$MPI_DIR/lib
  MPI_LIBS=-lmpich
  enable_mpi="mpich"
fi


# openmpi
if test "$with_mpich" = "none" ; then
  AC_ARG_WITH( ompi, [AC_HELP_STRING([--with-ompi=dir],[Specify OpenMPI install directory])], , with_ompi=none)
  if test "$with_ompi" != "none" ; then
    MPI_DIR=$with_ompi;
    MPI_CFLAGS=-I$MPI_DIR/include
    MPI_LDFLAGS=-L$MPI_DIR/lib
    MPI_LIBS=-lmpi
    enable_mpi="ompi"
  fi
fi

if test "$enable_mpi" = "none" ; then
  echo "  Warning: Neither MPICH nor OpenMPI library is not specified."
  echo "           If compilation error occurs about MPI, specify --with-mpich or --with-ompi option."
fi



#
# default prefix setting
#
AC_PREFIX_DEFAULT([/usr/local/Polylib])
if test "$prefix" = "NONE" ; then
  PL_INST_DIR=/usr/local/Polylib
else
  PL_INST_DIR="$prefix"
fi


#
# TextParser environment
#
AC_ARG_WITH(parser, [AC_HELP_STRING([--with-parser=dir],[Specify TextParser install directory])], , with_parser=none)
AC_SUBST(TP_DIR)
AC_SUBST(TP_CFLAGS)
AC_SUBST(TP_LDFLAGS)

if test "$with_parser" != "none" ; then
  TP_DIR=$with_parser;
  TP_CFLAGS=`$TP_DIR/bin/tp-config --cflags`
  TP_LDFLAGS=`$TP_DIR/bin/tp-config --libs`
else
  echo "  Error: TextParser library is not specified."
  echo "         Specify --with-parser option."
  exit 1
fi


#
# example
#
AC_ARG_WITH(example, [AC_HELP_STRING([--with-example=(yes|no)],[make example [yes]])], , with_example=yes)


#
# Polylib special flags
#

PL_CXX="$CXX"
AC_SUBST(PL_CXX)

# if a wrapper compiler is used, "enable_mpi" is empty, but parallel.
# wrappers like mpicxx, mpicc, mpiCC, mpic++, mpiFCCpx are taking into consideration, if not add in case.

case "$PL_CXX" in
  mpicxx)   enable_mpi="mpi" ;;
  mpicc)    enable_mpi="mpi" ;;
  mpiCC)    enable_mpi="mpi" ;;
  mpic++)   enable_mpi="mpi" ;;
  mpiFCCpx) enable_mpi="mpi" ;;
esac


if test "$enable_mpi" = "none" ; then
  PL_LIBS="-lPOLY"
else
  PL_LIBS="-lMPIPOLY"
fi

AC_SUBST(PL_LIBS)

# SERIALTARGET = true, if serial
AM_CONDITIONAL([SERIALTARGET], [test "$enable_mpi" = "none"])


PL_CFLAGS="-I$PL_INST_DIR/include"
AC_SUBST(PL_CFLAGS)

PL_LDFLAGS="-L$PL_INST_DIR/lib"
AC_SUBST(PL_LDFLAGS)



#
# MAKE_SUB_DIRS
#
AC_SUBST(MAKE_SUB_DIRS)
MAKE_SUB_DIRS="src doc"

if test "$with_example" = "yes" ; then
  MAKE_SUB_DIRS="$MAKE_SUB_DIRS"" examples"
fi


#
# Checks for programs.
#
AC_PROG_CXX
AC_PROG_CC
AC_PROG_CPP
AC_PROG_MAKE_SET
AC_PROG_RANLIB


#
# Checks for libraries.
#
AC_CHECK_LIB([TP], [TextParser::TextParser()])


#
# Checks for header files.
#
AC_CHECK_HEADERS([stdlib.h string.h sys/time.h])


#
# Checks for typedefs, structures, and compiler characteristics.
#
AC_CHECK_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_SIZE_T


#
# Checks for library functions.
#
AC_FUNC_MALLOC
AC_CHECK_FUNCS([gettimeofday memset strchr strstr])



AC_CONFIG_FILES([Makefile \
                 doc/Makefile \
                 src/Makefile \
                 examples/Makefile \
                 polylib-config ])
AC_OUTPUT

chmod +x ./polylib-config
