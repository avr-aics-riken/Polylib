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

AUTOMAKE_OPTIONS = subdir-objects

if SERIALTARGET
#  lib_LIBRARIES = libPOLY.a
#  libPOLY_a_CXXFLAGS = @TP_CFLAGS@ -I$(top_builddir)/include
#  libPOLY_a_SOURCES =  
  lib_LTLIBRARIES = libPOLY.la
  libPOLY_la_CXXFLAGS = @TP_CFLAGS@ @REAL_OPT@ -I$(top_builddir)/include
#  libPOLY_la_LDFLAGS = @LT_STATIC@
#  libPOLY_la_LDFLAGS = 
   libPOLY_la_SOURCES = \
     Polylib.cxx \
     c_lang/CPolylib.cxx \
     common/BBox.cxx \
     file_io/stl.cxx \
     file_io/obj.cxx \
     file_io/vtk.cxx \
     file_io/triangle_id.cxx \
     file_io/TriMeshIO.cxx \
     groups/PolygonGroup.cxx \
     groups/PolygonGroupFactory.cxx \
     polygons/DVertex.cxx \
     polygons/DVertexManager.cxx \
     polygons/DVertexTriangle.cxx \
     polygons/Polygons.cxx \
     polygons/PrivateTriangle.cxx \
     polygons/Triangle.cxx \
     polygons/TriMesh.cxx \
     polygons/VElement.cxx \
     polygons/Vertex.cxx \
     polygons/VertexList.cxx \
     polygons/VertKDT.cxx \
     polygons/VertKDTElem.cxx \
     polygons/VertKDTNode.cxx \
     polygons/VNode.cxx \
     polygons/VTree.cxx \
     util/time.cxx
else
  # lib_LIBRARIES = libMPIPOLY.a
  # libMPIPOLY_a_CXXFLAGS = @MPI_CFLAGS@ @TP_CFLAGS@ -I$(top_builddir)/include
  # libMPIPOLY_a_SOURCES = 
  lib_LTLIBRARIES = libMPIPOLY.la
  libMPIPOLY_la_CXXFLAGS = @MPI_CFLAGS@ @TP_CFLAGS@ @REAL_OPT@ -I$(top_builddir)/include
#  libMPIPOLY_la_LDFLAGS = @LT_STATIC@
  libMPIPOLY_la_SOURCES = \
     MPIPolylib.cxx \
     Polylib.cxx \
     c_lang/CMPIPolylib.cxx \
     c_lang/CPolylib.cxx \
     common/BBox.cxx \
     file_io/stl.cxx \
     file_io/obj.cxx \
     file_io/vtk.cxx \
     file_io/triangle_id.cxx \
     file_io/TriMeshIO.cxx \
     groups/PolygonGroup.cxx \
     groups/PolygonGroupFactory.cxx \
     polygons/DVertex.cxx \
     polygons/DVertexManager.cxx \
     polygons/DVertexTriangle.cxx \
     polygons/Polygons.cxx \
     polygons/PrivateTriangle.cxx \
     polygons/Triangle.cxx \
     polygons/TriMesh.cxx \
     polygons/VElement.cxx \
     polygons/Vertex.cxx \
     polygons/VertexList.cxx \
     polygons/VertKDT.cxx \
     polygons/VertKDTElem.cxx \
     polygons/VertKDTNode.cxx \
     polygons/VNode.cxx \
     polygons/VTree.cxx \
     util/time.cxx
endif


nobase_include_HEADERS = \
  $(top_builddir)/include/MPIPolylib.h \
  $(top_builddir)/include/Polylib.h \
  $(top_builddir)/include/c_lang/CMPIPolylib.h \
  $(top_builddir)/include/c_lang/CPolylib.h \
  $(top_builddir)/include/common/axis.h \
  $(top_builddir)/include/common/BBox.h \
  $(top_builddir)/include/common/PolylibCommon.h \
  $(top_builddir)/include/common/PolylibDefine.h \
  $(top_builddir)/include/common/PolylibStat.h \
  $(top_builddir)/include/common/tt.h \
  $(top_builddir)/include/common/Vec2.h \
  $(top_builddir)/include/common/vec3_func.h \
  $(top_builddir)/include/common/Vec3.h \
  $(top_builddir)/include/common/vec3f_func.h \
  $(top_builddir)/include/file_io/stl.h \
  $(top_builddir)/include/file_io/obj.h \
  $(top_builddir)/include/file_io/vtk.h \
  $(top_builddir)/include/file_io/triangle_id.h \
  $(top_builddir)/include/file_io/TriMeshIO.h \
  $(top_builddir)/include/groups/PolygonGroup.h \
  $(top_builddir)/include/groups/PolygonGroupFactory.h \
  $(top_builddir)/include/polygons/DVertex.h \
  $(top_builddir)/include/polygons/DVertexManager.h \
  $(top_builddir)/include/polygons/DVertexTriangle.h \
  $(top_builddir)/include/polygons/Polygons.h \
  $(top_builddir)/include/polygons/PrivateTriangle.h \
  $(top_builddir)/include/polygons/Triangle.h \
  $(top_builddir)/include/polygons/TriMesh.h \
  $(top_builddir)/include/polygons/VElement.h \
  $(top_builddir)/include/polygons/Vertex.h \
  $(top_builddir)/include/polygons/VertexList.h \
  $(top_builddir)/include/polygons/VertKDT.h \
  $(top_builddir)/include/polygons/VertKDTElem.h \
  $(top_builddir)/include/polygons/VertKDTNode.h \
  $(top_builddir)/include/polygons/VNode.h \
  $(top_builddir)/include/polygons/VTree.h \
  $(top_builddir)/include/util/time.h \
  $(top_builddir)/include/Version.h

EXTRA_DIST = Makefile_hand depend.inc

