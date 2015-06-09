##############################################################################
#
# Polylib - Polygon Management Library
#
# Copyright (c) 2010-2011 VCAD System Research Program, RIKEN. 
# All rights reserved.
# 
# Copyright (c) 2012-2015 Advanced Institute for Computational Science, RIKEN.
# All rights reserved.
#
##############################################################################
#
#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.57])

AC_INIT([Polylib], [3.5.0], [keno@riken.jp],[Polylib])

echo "----------------------------------------"
echo "Running Polylib Configure Script"
echo "----------------------------------------"


AC_CONFIG_SRCDIR([src/MPIPolylib.cxx])


#
# Specify directory for auxillary build tools (e.g., install-sh, config.sub, config.guess) and M4 files.
#

AC_CONFIG_AUX_DIR(config)


#
# To prevent the source codes from breaking, we suppress to do configure in the source directory.
#

if test -e configure.ac ; then
echo "Please run configure from a separate build directory. It is not allowd to run configure in the source directory."
echo "Pleaes try ..."
echo " "
echo "$ cd BUILD_DIR"
echo "$ ../configure [options]"
exit
fi


#
# Configure should create config.h from config.h.in
#

AC_CONFIG_HEADERS([config.h])


#
# Use automake
#

AM_INIT_AUTOMAKE()




# add to switch libtool
#AC_CONFIG_MACRO_DIR([m4]) // SO

#
# revision No.
#
PL_REVISION="20150609_1500"
AC_SUBST(PL_REVISION)


PL_CXX="$CXX"
AC_SUBST(PL_CXX)

enable_mpi="none"

# if a wrapper compiler is used, "enable_mpi" is empty, but parallel.
# wrappers like mpicxx, mpicc, mpiCC, mpic++, mpiicc, mpiicpc, mpiFCCpx are taking into consideration, if not add in case.
# CC is for Hopper

case "$PL_CXX" in
  mpicxx)   enable_mpi="mpi" ;;
  mpicc)    enable_mpi="mpi" ;;
  mpiCC)    enable_mpi="mpi" ;;
  mpic++)   enable_mpi="mpi" ;;
  mpiicc)   enable_mpi="mpi" ;;
  mpiicpc)  enable_mpi="mpi" ;;
  mpiFCCpx) enable_mpi="mpi" ;;
  CC)       enable_mpi="mpi" ;;
esac


#
# MPI environment
#
AC_SUBST(MPI_DIR)
AC_SUBST(MPI_CFLAGS)
AC_SUBST(MPI_LDFLAGS)
AC_SUBST(MPI_LIBS)


# mpich
AC_ARG_WITH(mpich, [AC_HELP_STRING([--with-mpich=dir],[Specify MPICH install directory])], , with_mpich=none)
if test x"$with_mpich" != x"none" ; then
  MPI_DIR=$with_mpich;
  MPI_CFLAGS=-I$MPI_DIR/include
  MPI_LDFLAGS=-L$MPI_DIR/lib
  MPI_LIBS=-lmpich
  enable_mpi="mpich"
fi


# openmpi
if test x"$with_mpich" = x"none" ; then
  AC_ARG_WITH( ompi, [AC_HELP_STRING([--with-ompi=dir],[Specify OpenMPI install directory])], , with_ompi=none)
  if test x"$with_ompi" != x"none" ; then
    MPI_DIR=$with_ompi;
    MPI_CFLAGS=-I$MPI_DIR/include
    MPI_LDFLAGS=-L$MPI_DIR/lib
    MPI_LIBS=-lmpi
    enable_mpi="ompi"
  fi
fi

if test x"$enable_mpi" = x"none" ; then
  echo "  Warning: Neither MPICH nor OpenMPI library is not specified."
  echo "           If compilation error occurs about MPI, specify --with-mpich or --with-ompi option."
fi



#
# default prefix setting
#
AC_PREFIX_DEFAULT([/usr/local/Polylib])
if test x"$prefix" = x"NONE" ; then
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
AC_SUBST(TP_MPI_LDFLAGS)

if test x"$with_parser" != x"none" ; then
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
AC_ARG_WITH(example, [AC_HELP_STRING([--with-example=(no|yes)],[make example [no]])], , with_example=no)




#
# enable/disable shared library // SO
#
#dnl AC_ARG_ENABLE(shared, [AC_HELP_STRING([--enable-shared=(no|yes)],[make shared library [yes]])], , enable_shared=no)

#dnl if test "$enable_shared" != "yes" ; then
#dnl LT_STATIC="-static"
#dnl else
#dnl LT_STATIC=""
#dnl fi
#dnl AC_SUBST(LT_STATIC)



#
# Polylib special flags
#


if test x"$enable_mpi" = x"none" ; then
  PL_LIBS="-lPOLY"
else
  PL_LIBS="-lPOLYmpi"
fi

AC_SUBST(PL_LIBS)


# SERIALTARGET = true, if serial
AM_CONDITIONAL([SERIALTARGET], [test "$enable_mpi" = "none"])


PL_CFLAGS="-I$PL_INST_DIR/include"
AC_SUBST(PL_CFLAGS)

PL_LDFLAGS="-L$PL_INST_DIR/lib"
AC_SUBST(PL_LDFLAGS)



#
# Type of REAL
#
AC_ARG_WITH(real, [AC_HELP_STRING([--with-real=(float|double)],[Specify real type [float]])], , with_real=float)
AC_SUBST(REAL_OPT)

if test x"$with_real" = x"double" ; then
  REAL_OPT=-D_REAL_IS_DOUBLE_
else
  REAL_OPT=
fi



#
# MAKE_SUB_DIRS
#
AC_SUBST(MAKE_SUB_DIRS)
MAKE_SUB_DIRS="src doc"
if test x"$with_example" = x"yes" ; then
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

# add to switch libtool // SO
#AC_PROG_LIBTOOL
#AM_PROG_LIBTOOL
#AC_SUBST(LIBTOOL_DEPS)

#
# Checks for libraries.
#


#
# Checks for header files.
#
AC_CHECK_HEADERS([stdlib.h string.h sys/time.h])


#
# Checks for typedefs, structures, and compiler characteristics.
#
AC_C_INLINE
AC_TYPE_SIZE_T


#
# Checks for library functions.
#
AC_CHECK_FUNCS([gettimeofday memset strchr strstr strcpy])



AC_CONFIG_FILES([Makefile \
                 doc/Makefile \
                 src/Makefile \
                 examples/Makefile \
                 include/polyVersion.h \
                 polylib-config ])
AC_OUTPUT

chmod +x ./polylib-config

echo "---------------------------------------------"
echo "Finished Running Polylib Configure Script"
echo "---------------------------------------------"
