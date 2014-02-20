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
  noinst_PROGRAMS = test test2 test_vtx test_vtx_float test_obj test_xyzrgb_statuette_stl test_obj2 test_obj3 test_obj4 test_read test_DVertex
else
  noinst_PROGRAMS = test_mpi test_mpi2 test_mpi3 test_mpi_xyzrgb1 test_mpi_xyzrgb2 test_mpi_xyzrgb4 test_mpi_xyzrgb8 test_mpi_DVertex test_mpi_move
endif


DISTCLEANFILES=*~ test test2 test_vtx test_vtx_float test_obj test_xyzrgb_statuette_stl test_obj2 test_obj3 test_obj4 test_read test_DVertex test_mpi test_mpi2 test_mpi3 test_mpi_xyzrgb1 test_mpi_xyzrgb2 test_mpi_xyzrgb4 test_mpi_xyzrgb8 test_mpi_DVertex test_mpi_move
CLEANFILES=data/*.log

EXTRA_DIST=  blade1.stl \
  blade2.stl \
  blade3.stl \
  car.stl \
  car.stla \
  polylib_config-multi.tp \
  polylib_config.tp \
  polylib_config2.tp \
  polylib_config_move.tp \
  polylib_config_xyzrgb_statuette_stl.tp \
  polylib_config_objfile_bin.tp \
  polylib_config_vtxtest.tp \
  polylib_config_objfile.tp \
  README \
  sphere.stl \
  tower.stl \
  cube.obj \
square.obj \
 car_0_20130731174205.id \
 car_0_20130731174205.stla \
 car_1_20130731174205.id \
 car_1_20130731174205.stla \
 car_2_20130731174205.id \
 car_2_20130731174205.stla \
 car_3_20130731174205.id \
 car_3_20130731174205.stla \
 polylib_config_0_20130731174205.tpp \
 polylib_config_1_20130731174205.tpp \
 polylib_config_2_20130731174205.tpp \
 polylib_config_3_20130731174205.tpp \
 sphere_3_20130731174205.id \
 sphere_3_20130731174205.stla \
 windmill_blades_blade1_0_20130731174205.id \
 windmill_blades_blade1_0_20130731174205.stla \
 windmill_blades_blade1_1_20130731174205.id \
 windmill_blades_blade1_1_20130731174205.stla \
 windmill_blades_blade1_2_20130731174205.id \
 windmill_blades_blade1_2_20130731174205.stla \
 windmill_blades_blade1_3_20130731174205.id \
 windmill_blades_blade1_3_20130731174205.stla \
 windmill_blades_blade2_0_20130731174205.id \
 windmill_blades_blade2_0_20130731174205.stla \
 windmill_blades_blade2_1_20130731174205.id \
 windmill_blades_blade2_1_20130731174205.stla \
 windmill_blades_blade2_2_20130731174205.id \
 windmill_blades_blade2_2_20130731174205.stla \
 windmill_blades_blade2_3_20130731174205.id \
 windmill_blades_blade2_3_20130731174205.stla \
 windmill_blades_blade3_0_20130731174205.id \
 windmill_blades_blade3_0_20130731174205.stla \
 windmill_blades_blade3_1_20130731174205.id \
 windmill_blades_blade3_1_20130731174205.stla \
 windmill_blades_blade3_2_20130731174205.id \
 windmill_blades_blade3_2_20130731174205.stla \
 windmill_blades_blade3_3_20130731174205.id \
 windmill_blades_blade3_3_20130731174205.stla \
 windmill_tower_0_20130731174205.id \
 windmill_tower_0_20130731174205.stla \
 windmill_tower_1_20130731174205.id \
 windmill_tower_1_20130731174205.stla \
 windmill_tower_2_20130731174205.id \
 windmill_tower_2_20130731174205.stla \
 windmill_tower_3_20130731174205.id \
 windmill_tower_3_20130731174205.stla 



test_SOURCES  =  test.cxx 
test_CXXFLAGS = -I$(top_builddir)/include @TP_CFLAGS@ -DPL_REAL=@PL_REAL@

test2_SOURCES  = test2.cxx
test2_CXXFLAGS = -I$(top_builddir)/include @TP_CFLAGS@ -DPL_REAL=@PL_REAL@

test_vtx_SOURCES  = test_vtx.cxx
test_vtx_CXXFLAGS = -I$(top_builddir)/include @TP_CFLAGS@ -DPL_REAL=@PL_REAL@

test_obj_SOURCES  = test_obj.cxx
test_obj_CXXFLAGS = -I$(top_builddir)/include @TP_CFLAGS@ -DPL_REAL=@PL_REAL@
test_obj2_SOURCES  = test_obj2.cxx
test_obj2_CXXFLAGS = -I$(top_builddir)/include @TP_CFLAGS@ -DPL_REAL=@PL_REAL@
test_obj3_SOURCES  = test_obj3.cxx
test_obj3_CXXFLAGS = -I$(top_builddir)/include @TP_CFLAGS@ -DPL_REAL=@PL_REAL@
test_obj4_SOURCES  = test_obj4.cxx
test_obj4_CXXFLAGS = -I$(top_builddir)/include @TP_CFLAGS@ -DPL_REAL=@PL_REAL@

test_read_SOURCES  = test_read.cxx
test_read_CXXFLAGS = -I$(top_builddir)/include @TP_CFLAGS@ -DPL_REAL=@PL_REAL@

test_vtx_float_SOURCES  = test_vtx_float.cxx
test_vtx_float_CXXFLAGS = -I$(top_builddir)/include @TP_CFLAGS@ -DPL_REAL=@PL_REAL@

test_DVertex_SOURCES = test_DVertex.cxx
test_DVertex_CXXFLAGS = -I$(top_builddir)/include @TP_CFLAGS@ -DPL_REAL=@PL_REAL@


test_xyzrgb_statuette_stl_SOURCES  = test_xyzrgb_statuette_stl.cxx
test_xyzrgb_statuette_stl_CXXFLAGS = -I$(top_builddir)/include @TP_CFLAGS@ -DPL_REAL=@PL_REAL@


test_mpi_SOURCES  = test_mpi.cxx
test_mpi_CXXFLAGS = -I$(top_builddir)/include @TP_CFLAGS@ @MPI_CFLAGS@ -DPL_REAL=@PL_REAL@

test_mpi2_SOURCES  = test_mpi2.cxx
test_mpi2_CXXFLAGS = -I$(top_builddir)/include @TP_CFLAGS@ @MPI_CFLAGS@ -DPL_REAL=@PL_REAL@

test_mpi3_SOURCES  = \
  test_mpi3.cxx \
  CarGroup.cxx \
  MyGroupFactory.cxx \
  CarGroup.h \
  MyGroupFactory.h
test_mpi3_CXXFLAGS = -I$(top_builddir)/include @TP_CFLAGS@ @MPI_CFLAGS@ -DPL_REAL=@PL_REAL@


test_mpi_xyzrgb1_SOURCES=test_mpi_xyzrgb1.cxx
test_mpi_xyzrgb1_CXXFLAGS = -I$(top_builddir)/include @TP_CFLAGS@ @MPI_CFLAGS@ -DPL_REAL=@PL_REAL@

test_mpi_xyzrgb2_SOURCES=test_mpi_xyzrgb2.cxx
test_mpi_xyzrgb2_CXXFLAGS = -I$(top_builddir)/include @TP_CFLAGS@ @MPI_CFLAGS@ -DPL_REAL=@PL_REAL@

test_mpi_xyzrgb4_SOURCES=test_mpi_xyzrgb4.cxx
test_mpi_xyzrgb4_CXXFLAGS = -I$(top_builddir)/include @TP_CFLAGS@ @MPI_CFLAGS@ -DPL_REAL=@PL_REAL@

test_mpi_xyzrgb8_SOURCES=test_mpi_xyzrgb8.cxx
test_mpi_xyzrgb8_CXXFLAGS = -I$(top_builddir)/include @TP_CFLAGS@ @MPI_CFLAGS@ -DPL_REAL=@PL_REAL@



test_mpi_DVertex_SOURCES  = test_mpi_DVertex.cxx
test_mpi_DVertex_CXXFLAGS = -I$(top_builddir)/include @TP_CFLAGS@ @MPI_CFLAGS@ -DPL_REAL=@PL_REAL@


test_mpi_move_SOURCES  = \
  test_mpi_move.cxx \
  CarGroup.cxx \
  MyGroupFactory.cxx \
  CarGroup.h \
  MyGroupFactory.h
test_mpi_move_CXXFLAGS = -I$(top_builddir)/include @TP_CFLAGS@ @MPI_CFLAGS@ -DPL_REAL=@PL_REAL@


test_LDADD = \
    -L$(top_builddir)/src/.libs -lPOLY \
    @MPI_LDFLAGS@ \
    @MPI_LIBS@ \
    @TP_LDFLAGS@ -lstdc++

# test_LDADD = \
#     -L$(top_builddir)/src -lPOLY \
#     @MPI_LDFLAGS@ \
#     @MPI_LIBS@ \
#     @TP_LDFLAGS@

test2_LDADD = \
    -L$(top_builddir)/src/.libs -lPOLY \
    @MPI_LDFLAGS@ \
    @MPI_LIBS@ \
    @TP_LDFLAGS@ -lstdc++

test_vtx_LDADD = \
    -L$(top_builddir)/src/.libs -lPOLY \
    @MPI_LDFLAGS@ \
    @MPI_LIBS@ \
    @TP_LDFLAGS@ -lstdc++

test_obj_LDADD = \
    -L$(top_builddir)/src/.libs -lPOLY \
    @MPI_LDFLAGS@ \
    @MPI_LIBS@ \
    @TP_LDFLAGS@ -lstdc++
test_obj2_LDADD = \
    -L$(top_builddir)/src/.libs -lPOLY \
    @MPI_LDFLAGS@ \
    @MPI_LIBS@ \
    @TP_LDFLAGS@ -lstdc++
test_obj3_LDADD = \
    -L$(top_builddir)/src/.libs -lPOLY \
    @MPI_LDFLAGS@ \
    @MPI_LIBS@ \
    @TP_LDFLAGS@ -lstdc++
test_obj4_LDADD = \
    -L$(top_builddir)/src/.libs -lPOLY \
    @MPI_LDFLAGS@ \
    @MPI_LIBS@ \
    @TP_LDFLAGS@ -lstdc++
test_read_LDADD = \
    -L$(top_builddir)/src/.libs -lPOLY \
    @MPI_LDFLAGS@ \
    @MPI_LIBS@ \
    @TP_LDFLAGS@ -lstdc++

test_vtx_float_LDADD = \
    -L$(top_builddir)/src/.libs -lPOLY \
    @MPI_LDFLAGS@ \
    @MPI_LIBS@ \
    @TP_LDFLAGS@ -lstdc++

test_DVertex_LDADD = \
    -L$(top_builddir)/src/.libs -lPOLY \
    @MPI_LDFLAGS@ \
    @MPI_LIBS@ \
    @TP_LDFLAGS@ -lstdc++

test_xyzrgb_statuette_stl_LDADD = \
    -L$(top_builddir)/src/.libs -lPOLY \
    @MPI_LDFLAGS@ \
    @MPI_LIBS@ \
    @TP_LDFLAGS@ -lstdc++




test_mpi_LDADD = \
    -L$(top_builddir)/src/.libs -lMPIPOLY \
    @MPI_LDFLAGS@ \
    @MPI_LIBS@ \
    @TP_LDFLAGS@ -lstdc++

#    @TP_MPI_LDFLAGS@ -lstdc++

test_mpi2_LDADD = \
    -L$(top_builddir)/src/.libs -lMPIPOLY \
    @MPI_LDFLAGS@ \
    @MPI_LIBS@ \
    @TP_LDFLAGS@ -lstdc++

#    @TP_MPI_LDFLAGS@ -lstdc++

test_mpi3_LDADD = \
    -L$(top_builddir)/src/.libs -lMPIPOLY \
    @MPI_LDFLAGS@ \
    @MPI_LIBS@ \
    @TP_LDFLAGS@ -lstdc++

#    @TP_MPI_LDFLAGS@ -lstdc++


test_mpi_xyzrgb1_LDADD = \
    -L$(top_builddir)/src/.libs -lMPIPOLY \
    @MPI_LDFLAGS@ \
    @MPI_LIBS@ \
    @TP_LDFLAGS@ -lstdc++


test_mpi_xyzrgb2_LDADD = \
    -L$(top_builddir)/src/.libs -lMPIPOLY \
    @MPI_LDFLAGS@ \
    @MPI_LIBS@ \
    @TP_LDFLAGS@ -lstdc++


test_mpi_xyzrgb4_LDADD = \
    -L$(top_builddir)/src/.libs -lMPIPOLY \
    @MPI_LDFLAGS@ \
    @MPI_LIBS@ \
    @TP_LDFLAGS@ -lstdc++


test_mpi_xyzrgb8_LDADD = \
    -L$(top_builddir)/src/.libs -lMPIPOLY \
    @MPI_LDFLAGS@ \
    @MPI_LIBS@ \
    @TP_LDFLAGS@ -lstdc++



test_mpi_DVertex_LDADD = \
    -L$(top_builddir)/src/.libs -lMPIPOLY \
    @MPI_LDFLAGS@ \
    @MPI_LIBS@ \
    @TP_LDFLAGS@ -lstdc++


test_mpi_move_LDADD = \
    -L$(top_builddir)/src/.libs -lMPIPOLY \
    @MPI_LDFLAGS@ \
    @MPI_LIBS@ \
    @TP_LDFLAGS@ -lstdc++


dist_noinst_DATA=

.PHONY:$(dist_noinst_DATA)


