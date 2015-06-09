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

AUTOMAKE_OPTIONS = subdir-objects

if SERIALTARGET
#  lib_LTLIBRARIES = libPOLY.la
#  libPOLY_la_CXXFLAGS = @TP_CFLAGS@ @REAL_OPT@ -I$(top_srcdir)/include
#  libPOLY_la_LDFLAGS = @LT_STATIC@
#  libPOLY_la_LDFLAGS = 
#  libPOLY_la_SOURCES =
lib_LIBRARIES = libPOLY.a
libPOLY_a_CXXFLAGS = @TP_CFLAGS@ @REAL_OPT@ -I$(top_srcdir)/include
libPOLY_a_SOURCES = \
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
#  lib_LTLIBRARIES = libPOLYmpi.la
#  libPOLYmpi_la_CXXFLAGS = @MPI_CFLAGS@ @TP_CFLAGS@ @REAL_OPT@ -I$(top_srcdir)/include
#  libPOLYmpi_la_LDFLAGS = @LT_STATIC@
#  libPOLYmpi_la_SOURCES =
lib_LIBRARIES = libPOLYmpi.a
libPOLYmpi_a_CXXFLAGS = @MPI_CFLAGS@ @TP_CFLAGS@ @REAL_OPT@ -I$(top_srcdir)/include
libPOLYmpi_a_SOURCES = \
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
  $(top_srcdir)/include/MPIPolylib.h \
  $(top_srcdir)/include/Polylib.h \
  $(top_srcdir)/include/c_lang/CMPIPolylib.h \
  $(top_srcdir)/include/c_lang/CPolylib.h \
  $(top_srcdir)/include/common/BBox.h \
  $(top_srcdir)/include/common/PolylibCommon.h \
  $(top_srcdir)/include/common/PolylibDefine.h \
  $(top_srcdir)/include/common/PolylibStat.h \
  $(top_srcdir)/include/common/tt.h \
  $(top_srcdir)/include/common/Vec2.h \
  $(top_srcdir)/include/common/Vec3.h \
  $(top_srcdir)/include/file_io/stl.h \
  $(top_srcdir)/include/file_io/obj.h \
  $(top_srcdir)/include/file_io/vtk.h \
  $(top_srcdir)/include/file_io/triangle_id.h \
  $(top_srcdir)/include/file_io/TriMeshIO.h \
  $(top_srcdir)/include/groups/PolygonGroup.h \
  $(top_srcdir)/include/groups/PolygonGroupFactory.h \
  $(top_srcdir)/include/polygons/DVertex.h \
  $(top_srcdir)/include/polygons/DVertexManager.h \
  $(top_srcdir)/include/polygons/DVertexTriangle.h \
  $(top_srcdir)/include/polygons/Polygons.h \
  $(top_srcdir)/include/polygons/PrivateTriangle.h \
  $(top_srcdir)/include/polygons/Triangle.h \
  $(top_srcdir)/include/polygons/TriMesh.h \
  $(top_srcdir)/include/polygons/VElement.h \
  $(top_srcdir)/include/polygons/Vertex.h \
  $(top_srcdir)/include/polygons/VertexList.h \
  $(top_srcdir)/include/polygons/VertKDT.h \
  $(top_srcdir)/include/polygons/VertKDTElem.h \
  $(top_srcdir)/include/polygons/VertKDTNode.h \
  $(top_srcdir)/include/polygons/VNode.h \
  $(top_srcdir)/include/polygons/VTree.h \
  $(top_srcdir)/include/util/time.h \
  $(top_srcdir)/include/polyVersion.h

EXTRA_DIST = Makefile_hand depend.inc
