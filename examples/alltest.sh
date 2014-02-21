#!/bin/bash

PREFIX=/home/hsim/GitData/Polylib_3.2_fjqs
PARSER=/home/hsim/opt/TextParser
MPI=/home/hsim/opt/openmpi

func() {

dname=$1
echo $dname

cd ..

make clean

./configure --prefix=$PREFIX --with-parser=$PARSER --with-ompi=$MPI --with-example=yes CXX=mpicxx CXXFLAGS="-O3 -Wall -fno-strict-aliasing" --with-real=$dname

make

cd examples
mpirun -np 4 ./test_mpi >../log/test_mpi.$dname.log 2>&1
mpirun -np 4 ./test_mpi2 >../log/test_mpi2.$dname.log 2>&1
mpirun -np 4 ./test_mpi3 >../log/test_mpi3.$dname.log 2>&1
mpirun -np 1 ./test_mpi_xyzrgb1 >../log/test_mpi_xyzrgb1.$dname.log 2>&1
mpirun -np 2 ./test_mpi_xyzrgb2 >../log/test_mpi_xyzrgb2.$dname.log 2>&1
mpirun -np 4 ./test_mpi_xyzrgb4 >../log/test_mpi_xyzrgb4.$dname.log 2>&1
mpirun -np 8 ./test_mpi_xyzrgb8 >../log/test_mpi_xyzrgb8.$dname.log 2>&1
mpirun -np 2 ./test_mpi_DVertex >../log/test_mpi_DVertex.$dname.log 2>&1
mpirun -np 4 ./test_mpi_move >../log/test_mpi_move.$dname.log 2>&1

}

mkdir ../log
rm -rf ../log/*.log

array="float double"

for var in ${array}
do
    echo $var
    func $var 
done


