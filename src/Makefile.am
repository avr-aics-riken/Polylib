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
  ../include/MPIPolylib.h \
  ../include/Polylib.h \
  ../include/c_lang/CMPIPolylib.h \
  ../include/c_lang/CPolylib.h \
  ../include/common/BBox.h \
  ../include/common/PolylibCommon.h \
  ../include/common/PolylibDefine.h \
  ../include/common/PolylibStat.h \
  ../include/common/tt.h \
  ../include/common/Vec2.h \
  ../include/common/Vec3.h \
  ../include/file_io/stl.h \
  ../include/file_io/obj.h \
  ../include/file_io/vtk.h \
  ../include/file_io/triangle_id.h \
  ../include/file_io/TriMeshIO.h \
  ../include/groups/PolygonGroup.h \
  ../include/groups/PolygonGroupFactory.h \
  ../include/polygons/DVertex.h \
  ../include/polygons/DVertexManager.h \
  ../include/polygons/DVertexTriangle.h \
  ../include/polygons/Polygons.h \
  ../include/polygons/PrivateTriangle.h \
  ../include/polygons/Triangle.h \
  ../include/polygons/TriMesh.h \
  ../include/polygons/VElement.h \
  ../include/polygons/Vertex.h \
  ../include/polygons/VertexList.h \
  ../include/polygons/VertKDT.h \
  ../include/polygons/VertKDTElem.h \
  ../include/polygons/VertKDTNode.h \
  ../include/polygons/VNode.h \
  ../include/polygons/VTree.h \
  ../include/util/time.h \
  ../include/polyVersion.h

EXTRA_DIST = Makefile_hand depend.inc
