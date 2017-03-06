/*
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
*/


#include "mpi.h"
#include <iostream>
#include "Polylib.h"
#include "MPIPolylib.h"
#include "CarGroup.h"
//#include "BladeGroup.h"
#include "MyGroupFactory.h"

using namespace std;
using namespace PolylibNS;




struct MyParallelInfo {
	PL_REAL bpos[3]; //基準座標
	unsigned bbsize[3]; //number of voxel 計算領域
	unsigned gcsize[3]; //number of guidecell voxel
	PL_REAL dx[3]; //size of voxel
};

static MyParallelInfo myParaInfos[4] = {
	{{-1100, -1800,-1800,}, {18,18,18,}, {1, 1,1,}, {100,100,100} },
	{{-1100,     0,-1800,}, {18,18,18,}, {1, 1,1,}, {100,100,100} },
	{{-1100, -1800,    0,}, {18,18,18,}, {1, 1,1,}, {100,100,100} },
	{{-1100,     0,    0,}, {18,18,18,}, {1, 1,1,}, {100,100,100} }
};



#ifdef WIN32
int main_test_mpi3(int argc, char** argv ){
#else
int main(int argc, char** argv ){
#endif


	int rank;
	unsigned int step;
	POLYLIB_STAT stat;
	PolylibMoveParams params;

	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	cout << "Starting program on rank:"<<rank<<endl;

	MPIPolylib* p_polylib = MPIPolylib::get_instance();

	p_polylib->set_factory(new MyGroupFactory() );

	stat = p_polylib->init_parallel_info(MPI_COMM_WORLD,
		myParaInfos[rank].bpos,
		myParaInfos[rank].bbsize,
		myParaInfos[rank].gcsize,
		myParaInfos[rank].dx);

	if(stat !=PLSTAT_OK) return -1;

	stat=p_polylib->load_rank0("config/polylib_config2.tp");
	if(stat !=PLSTAT_OK) return -1;
	cout << "data loading finished on rank:"<<rank<<endl;
	p_polylib->show_group_hierarchy();

	string fname="";
	string stl="stl_a";
	string extend="";
	//  ID_FORMAT id_format=ID_BIN;
	ID_FORMAT id_format=ID_ASCII;
	//  p_polylib->save_rank0(&fname,stl,extend);
	p_polylib->save_parallel(&fname,stl,extend,id_format);

	MPI_Finalize();

	return 0;


}
