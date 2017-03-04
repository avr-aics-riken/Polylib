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

# polyVersion.hはPROJECT_BINARY_DIR
include_directories(
       ${PROJECT_BINARY_DIR}    # config.h
       ${PROJECT_BINARY_DIR}/include
       ${PROJECT_SOURCE_DIR}/include
       ${TP_INC}
)


link_directories(
       ${TP_LIB}
)


set(poly_files
    Polylib.cxx
    c_lang/CPolylib.cxx
    common/BBox.cxx
    file_io/stl.cxx
    file_io/obj.cxx
    file_io/vtk.cxx
    file_io/triangle_id.cxx
    file_io/TriMeshIO.cxx
    groups/PolygonGroup.cxx
    groups/PolygonGroupFactory.cxx
    polygons/DVertex.cxx
    polygons/DVertexManager.cxx
    polygons/DVertexTriangle.cxx
    polygons/Polygons.cxx
    polygons/PrivateTriangle.cxx
    polygons/Triangle.cxx
    polygons/TriMesh.cxx
    polygons/VElement.cxx
    polygons/Vertex.cxx
    polygons/VertexList.cxx
    polygons/VertKDT.cxx
    polygons/VertKDTElem.cxx
    polygons/VertKDTNode.cxx
    polygons/VNode.cxx
    polygons/VTree.cxx
    util/poly_time.cxx
)

set(poly_mpi_files
    MPIPolylib.cxx
    c_lang/CMPIPolylib.cxx
)


if(NOT with_MPI)
  add_library(POLY STATIC ${poly_files})
  target_link_libraries(POLY -lTP)
  install(TARGETS POLY DESTINATION lib)

else()
  add_library(POLYmpi STATIC ${poly_files} ${poly_mpi_files})
  target_link_libraries(POLYmpi -lTPmpi)
  install(TARGETS POLYmpi DESTINATION lib)

endif()



install(FILES
        ${PROJECT_SOURCE_DIR}/include/MPIPolylib.h
        ${PROJECT_SOURCE_DIR}/include/Polylib.h
        ${PROJECT_BINARY_DIR}/include/polyVersion.h
        DESTINATION include
)

install(FILES
        ${PROJECT_SOURCE_DIR}/include/c_lang/CMPIPolylib.h
        ${PROJECT_SOURCE_DIR}/include/c_lang/CPolylib.h
        DESTINATION include/c_lang
)

install(FILES
        ${PROJECT_SOURCE_DIR}/include/common/BBox.h
        ${PROJECT_SOURCE_DIR}/include/common/PolylibCommon.h
        ${PROJECT_SOURCE_DIR}/include/common/PolylibDefine.h
        ${PROJECT_SOURCE_DIR}/include/common/PolylibStat.h
        ${PROJECT_SOURCE_DIR}/include/common/tt.h
        ${PROJECT_SOURCE_DIR}/include/common/Vec2.h
        ${PROJECT_SOURCE_DIR}/include/common/Vec3.h
        DESTINATION include/common
)

install(FILES
        ${PROJECT_SOURCE_DIR}/include/file_io/stl.h
        ${PROJECT_SOURCE_DIR}/include/file_io/obj.h
        ${PROJECT_SOURCE_DIR}/include/file_io/vtk.h
        ${PROJECT_SOURCE_DIR}/include/file_io/triangle_id.h
        ${PROJECT_SOURCE_DIR}/include/file_io/TriMeshIO.h
        DESTINATION include/file_io
)

install(FILES
        ${PROJECT_SOURCE_DIR}/include/groups/PolygonGroup.h
        ${PROJECT_SOURCE_DIR}/include/groups/PolygonGroupFactory.h
        DESTINATION include/groups
)

install(FILES
        ${PROJECT_SOURCE_DIR}/include/polygons/DVertex.h
        ${PROJECT_SOURCE_DIR}/include/polygons/DVertexManager.h
        ${PROJECT_SOURCE_DIR}/include/polygons/DVertexTriangle.h
        ${PROJECT_SOURCE_DIR}/include/polygons/Polygons.h
        ${PROJECT_SOURCE_DIR}/include/polygons/PrivateTriangle.h
        ${PROJECT_SOURCE_DIR}/include/polygons/Triangle.h
        ${PROJECT_SOURCE_DIR}/include/polygons/TriMesh.h
        ${PROJECT_SOURCE_DIR}/include/polygons/VElement.h
        ${PROJECT_SOURCE_DIR}/include/polygons/Vertex.h
        ${PROJECT_SOURCE_DIR}/include/polygons/VertexList.h
        ${PROJECT_SOURCE_DIR}/include/polygons/VertKDT.h
        ${PROJECT_SOURCE_DIR}/include/polygons/VertKDTElem.h
        ${PROJECT_SOURCE_DIR}/include/polygons/VertKDTNode.h
        ${PROJECT_SOURCE_DIR}/include/polygons/VNode.h
        ${PROJECT_SOURCE_DIR}/include/polygons/VTree.h
        DESTINATION include/polygons
)

install(FILES
        ${PROJECT_SOURCE_DIR}/include/util/poly_time.h
        DESTINATION include/util
)
