##############################################################################
#
# Polylib - Polygon Management Library
#
# Copyright (c) 2010-2011 VCAD System Research Program, RIKEN. 
# All rights reserved.
# 
# Copyright (c) 2012-2014 Advanced Institute for Computational Science, RIKEN.
# All rights reserved.
#
##############################################################################

if SERIALTARGET
  noinst_PROGRAMS = test test2
else
  noinst_PROGRAMS = test_mpi test_mpi2 test_mpi3
endif

DISTCLEANFILES=*~
CLEANFILES=data/*.log


test_SOURCES  = \
  test.cxx \
  blade1.stl \
  blade2.stl \
  blade3.stl \
  car.stl \
  car.stla \
  polylib_config-multi.tpp \
  polylib_config.tpp \
  polylib_config2.tpp \
  README \
  sphere.stl \
  tower.stl

test_CXXFLAGS = -I$(top_builddir)/include @TP_CFLAGS@

test2_SOURCES  = test2.cxx
test2_CXXFLAGS = -I$(top_builddir)/include @TP_CFLAGS@

test_mpi_SOURCES  = test_mpi.cxx
test_mpi_CXXFLAGS = -I$(top_builddir)/include @TP_CFLAGS@ @MPI_CFLAGS@

test_mpi2_SOURCES  = test_mpi2.cxx
test_mpi2_CXXFLAGS = -I$(top_builddir)/include @TP_CFLAGS@ @MPI_CFLAGS@

test_mpi3_SOURCES  = \
  test_mpi3.cxx \
  CarGroup.cxx \
  MyGroupFactory.cxx \
  CarGroup.h \
  MyGroupFactory.h
test_mpi3_CXXFLAGS = -I$(top_builddir)/include @TP_CFLAGS@ @MPI_CFLAGS@

test_LDADD = \
    -L$(top_builddir)/src -lPOLY \
    @MPI_LDFLAGS@ \
    @MPI_LIBS@ \
    @TP_LDFLAGS@ -lstdc++

test2_LDADD = \
    -L$(top_builddir)/src -lPOLY \
    @MPI_LDFLAGS@ \
    @MPI_LIBS@ \
    @TP_LDFLAGS@ -lstdc++

test_mpi_LDADD = \
    -L$(top_builddir)/src -lMPIPOLY \
    @MPI_LDFLAGS@ \
    @MPI_LIBS@ \
    @TP_LDFLAGS@ -lstdc++

test_mpi2_LDADD = \
    -L$(top_builddir)/src -lMPIPOLY \
    @MPI_LDFLAGS@ \
    @MPI_LIBS@ \
    @TP_LDFLAGS@ -lstdc++

test_mpi3_LDADD = \
    -L$(top_builddir)/src -lMPIPOLY \
    @MPI_LDFLAGS@ \
    @MPI_LIBS@ \
    @TP_LDFLAGS@ -lstdc++

dist_noinst_DATA=

.PHONY:$(dist_noinst_DATA)


