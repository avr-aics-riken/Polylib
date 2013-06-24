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
AC_CONFIG_HEADERS([config.h])


#
# revision No.
#
PL_REVISION="20130624_2300"
AC_SUBST(PL_REVISION)


#
# platform
#
PL_DEVICE="$($SHELL ./polylib-uname)"
AC_SUBST(PL_DEVICE)

#
# compiler libs
#
AC_ARG_WITH(comp, [AC_HELP_STRING([--with-comp=(INTEL|FJ)],[Specify Compiler type])], , with_comp=none)
AC_SUBST(COMPILER_LIBS)
case "$with_comp" in
"INTEL")
  COMPILER_LIBS="-lifport -lifcore"
  ;;
"FJ")
  COMPILER_LIBS="--linkfortran"
  ;;
esac

enable_mpi="none"


#
# MPICH environment
#
AC_SUBST(MPICH_DIR)
AC_SUBST(MPICH_CFLAGS)
AC_SUBST(MPICH_LDFLAGS)
AC_SUBST(MPICH_LIBS)
AC_SUBST(MPICH_FCLIBS)
AC_ARG_WITH(mpich, [AC_HELP_STRING([--with-mpich=dir],[Specify MPICH install directory])], , with_mpich=none)
if test "$with_mpich" != "none" ; then
  MPICH_DIR=$with_mpich;
  MPICH_CFLAGS=-I$MPICH_DIR/include
  MPICH_LDFLAGS=-L$MPICH_DIR/lib
  enable_mpi="mpich"
fi

#
# OpenMPI environment
#
if test "$with_mpich" = "none" ; then
  AC_ARG_WITH(ompi, [AC_HELP_STRING([--with-ompi=dir],[Specify OpenMPI install directory])], , with_ompi=none)
  if test "$with_ompi" != "none" ; then
    MPICH_DIR=$with_ompi;
    MPICH_CFLAGS=-I$MPICH_DIR/include
    MPICH_LDFLAGS=-L$MPICH_DIR/lib
    enable_mpi="ompi"
  fi
fi


#
# MPI Library
#
if test "$with_comp" = "FJ" ; then
  MPICH_LIBS=""
  case "$host" in
  sparc*)
    MPICH_FCLIBS="$MPICH_LIBS"" -lmpi_f77 -lmpi_f90"
    ;;
  esac
elif test "$enable_mpi" = "mpich" ; then
  MPICH_LIBS="-lmpich"
  MPICH_FCLIBS="$MPICH_LIBS"" -lfmpich"
  case "$CPM_DEVICE" in
  "Lion"|"Snow_Leopard"|"Leopard"|"Mountain_Lion")
#    MPICH_LIBS="$MPICH_LIBS"" -lpmpich"
    ;;
  esac
elif test "$enable_mpi" = "ompi" ; then
  MPICH_LIBS="-lmpi"
  MPICH_FCLIBS="$MPICH_LIBS"" -lmpi_f77 -lmpi_f90"
  case "$CPM_DEVICE" in
  "Lion"|"Snow_Leopard"|"Leopard"|"Mountain_Lion")
#    MPICH_LIBS="$MPICH_LIBS"" -lmpi_cxx"
    ;;
  esac
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
AC_SUBST(TP_LIBS)
if test "$with_parser" != "none" ; then
  TP_DIR=$with_parser;
  TP_CFLAGS=-I$TP_DIR/include
  TP_LDFLAGS=-L$TP_DIR/lib
  TP_LIBS=$TP_DIR/lib/libTextParser.a
else
  echo "  Error: TextParser library not specified."
  echo "         set --with-parser option"
  exit 1
fi


#
# example
#
AC_ARG_WITH(example, [AC_HELP_STRING([--with-example=(yes|no)],[make example [yes]])], , with_example=yes)


#
# Polylib special flags
#
PL_CFLAGS="-I$PL_INST_DIR/include"
AC_SUBST(PL_CFLAGS)

PL_LDFLAGS="-L$PL_INST_DIR/lib"
AC_SUBST(PL_LDFLAGS)

PL_LIBS="libMPIPOLY.a libPOLY.a"
AC_SUBST(PL_LIBS)

PL_CXX="$CXX"
AC_SUBST(PL_CXX)

PL_FC="$FC"
AC_SUBST(PL_FC)

PL_FC_LD="$FC"
if test "$with_comp" = "FJ"; then
  PL_FC_LD="$CXX"" --linkfortran"
else
  PL_LIBS="$PL_LIBS"" -lstdc++"
fi
AC_SUBST(PL_FC_LD)



#
# MAKE_SUB_DIRS
#
AC_SUBST(MAKE_SUB_DIRS)
MAKE_SUB_DIRS="src docs"

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
AC_CHECK_LIB([TextParser], [TextParser::TextParser()])


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
                 docs/Makefile \
                 src/Makefile \
                 examples/Makefile \
                 polylib-config ])
AC_OUTPUT

chmod +x ./polylib-config
