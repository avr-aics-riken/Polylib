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


message(" ")
message("Examples : ")
message(STATUS "with_MPI           = " ${with_MPI})
message(STATUS "CXX FLAGS          = " ${CMAKE_CXX_FLAGS})


include_directories(
      ${PROJECT_BINARY_DIR}/include
      ${PROJECT_SOURCE_DIR}/include
      ${PROJECT_SOURCE_DIR}/include/common
      ${PROJECT_SOURCE_DIR}/examples
      ${TP_INC}
)

link_directories(
      ${PROJECT_BINARY_DIR}/src
      ${TP_LIB}
)


if(NOT with_MPI)

#### Example1 : test

add_executable(test1 test.cxx)
target_link_libraries(test1 -lPOLY -lTP)
add_test(Example1 test1)


#### Example2 : test2

add_executable(test2 test2.cxx)
target_link_libraries(test2 -lPOLY -lTP)
add_test(Example2 test2)


### Example3 : test_vtx.cxx

add_executable(test_vtx test_vtx.cxx)
target_link_libraries(test_vtx -lPOLY -lTP)
add_test(Example3 test_vtx)


### Example4 : test_obj.cxx

add_executable(test_obj test_obj.cxx)
target_link_libraries(test_obj -lPOLY -lTP)
add_test(Example4 test_obj)


### Example5 : test_obj2.cxx

add_executable(test_obj2 test_obj2.cxx)
target_link_libraries(test_obj2 -lPOLY -lTP)
add_test(Example5 test_obj2)


### Example6 : test_obj3.cxx

add_executable(test_obj3 test_obj3.cxx)
target_link_libraries(test_obj3 -lPOLY -lTP)
add_test(Example6 test_obj3)


### Example7 : test_obj4.cxx

add_executable(test_obj4 test_obj4.cxx)
target_link_libraries(test_obj4 -lPOLY -lTP)
add_test(Example7 test_obj4)


### Example8 : test_read.cxx

add_executable(test_read test_read.cxx)
target_link_libraries(test_read -lPOLY -lTP)
add_test(Example8 test_read)


### Example9 : test_vtx_float.cxx

add_executable(test_vtx_float test_vtx_float.cxx)
target_link_libraries(test_vtx_float -lPOLY -lTP)
add_test(Example9 test_vtx_float)


### Example10 : test_DVertex.cxx

add_executable(test_DVertex test_DVertex.cxx)
target_link_libraries(test_DVertex -lPOLY -lTP)
add_test(Example10 test_DVertex)


### Example11 : test_xyzrgb_statuette_stl.cxx

add_executable(test_xyzrgb_statuette_stl test_xyzrgb_statuette_stl.cxx)
target_link_libraries(test_xyzrgb_statuette_stl -lPOLY -lTP)
add_test(Example11 test_xyzrgb_statuette_stl)


else()

### Example12 : test_mpi

add_executable(test_mpi test_mpi.cxx)
target_link_libraries(test_mpi -lPOLYmpi -lTPmpi)
set (test_parameters -np 4 "./test_mpi")
add_test(NAME Example12 COMMAND "mpirun" ${test_parameters})


### Example13 : test_mpi2

add_executable(test_mpi2 test_mpi2.cxx)
target_link_libraries(test_mpi2 -lPOLYmpi -lTPmpi)
set (test_parameters -np 4 "./test_mpi2")
add_test(NAME Example13 COMMAND "mpirun" ${test_parameters})


### Example14 : test_mpi3

add_executable(test_mpi3
               test_mpi3.cxx
               CarGroup.cxx
               MyGroupFactory.cxx
               )
target_link_libraries(test_mpi3 -lPOLYmpi -lTPmpi)
set (test_parameters -np 4 "./test_mpi3")
add_test(NAME Example14 COMMAND "mpirun" ${test_parameters})


### Example15 : test_mpi_xyzrgb1

add_executable(test_mpi_xyzrgb1 test_mpi_xyzrgb1.cxx)
target_link_libraries(test_mpi_xyzrgb1 -lPOLYmpi -lTPmpi)
set (test_parameters -np 1 "./test_mpi_xyzrgb1")
add_test(NAME Example15 COMMAND "mpirun" ${test_parameters})


### Example16 : test_mpi_xyzrgb2

add_executable(test_mpi_xyzrgb2 test_mpi_xyzrgb2.cxx)
target_link_libraries(test_mpi_xyzrgb2 -lPOLYmpi -lTPmpi)
set (test_parameters -np 2 "./test_mpi_xyzrgb2")
add_test(NAME Example16 COMMAND "mpirun" ${test_parameters})


### Example17 : test_mpi_xyzrgb4

add_executable(test_mpi_xyzrgb4 test_mpi_xyzrgb4.cxx)
target_link_libraries(test_mpi_xyzrgb4 -lPOLYmpi -lTPmpi)
set (test_parameters -np 4 "./test_mpi_xyzrgb4")
add_test(NAME Example17 COMMAND "mpirun" ${test_parameters})


### Example18 : test_mpi_xyzrgb8

add_executable(test_mpi_xyzrgb8 test_mpi_xyzrgb8.cxx)
target_link_libraries(test_mpi_xyzrgb8 -lPOLYmpi -lTPmpi)
set (test_parameters -np 8 "./test_mpi_xyzrgb8")
add_test(NAME Example18 COMMAND "mpirun" ${test_parameters})


### Example19 : test_mpi_Dvertex

add_executable(test_mpi_Dvertex test_mpi_Dvertex.cxx)
target_link_libraries(test_mpi_Dvertex -lPOLYmpi -lTPmpi)
set (test_parameters -np 2 "./test_mpi_Dvertex")
add_test(NAME Example19 COMMAND "mpirun" ${test_parameters})


### Example20 : test_mpi_Dvertex

#add_executable(test_mpi_move test_mpi_move.cxx)
#target_link_libraries(test_mpi_move -lPOLYmpi -lTPmpi)
#set (test_parameters -np 4 "./test_mpi_move")
#add_test(NAME Example20 COMMAND "mpirun" ${test_parameters})


endif()
