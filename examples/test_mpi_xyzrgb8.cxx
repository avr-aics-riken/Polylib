/*
* Polylib - Polygon Management Library
*
* Copyright (c) 2010-2011 VCAD System Research Program, RIKEN.
* All rights reserved.
*
* Copyright (c) 2012-2013 Advanced Institute for Computational Science, RIKEN.
* All rights reserved.
*
*/

#include "mpi.h"
#include <iostream>
#include "Polylib.h"
#include "MPIPolylib.h"
//#include "CarGroup.h"
//#include "BladeGroup.h"
//#include "MyGroupFactory.h"

using namespace std;
using namespace PolylibNS;



struct MyParallelInfo {
	REAL_TYPE bpos[3]; //基準座標
	unsigned bbsize[3]; //number of voxel 計算領域
	unsigned gcsize[3]; //number of guidecell voxel
	REAL_TYPE dx[3]; //size of voxel
};


static MyParallelInfo myParaInfos[8] = {
	{{-220, -220,-220,}, {22,22,22,}, {1, 1,1,}, {10,10,10} },
	{{-220, -220,-0,}, {22,22,22,}, {1, 1,1,}, {10,10,10} },
	{{-220, 0,-220,}, {22,22,22,}, {1, 1,1,}, {10,10,10} },
	{{-220, 0,0,}, {22,22,22,}, {1, 1,1,}, {10,10,10} },
	{{0, -220,-220,}, {22,22,22,}, {1, 1,1,}, {10,10,10} },
	{{0, -220,0,}, {22,22,22,}, {1, 1,1,}, {10,10,10} },
	{{0, 0,-220,}, {22,22,22,}, {1, 1,1,}, {10,10,10} },
	{{0, 0,0,}, {22,22,22,}, {1, 1,1,}, {10,10,10} },
};


#ifdef WIN32
int main_test_mpi_xyzrgb8(int argc, char** argv ){
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

	cout << "program at 1 on rank :"<<rank<<endl;



	//  p_polylib->set_factory(new MyGroupFactory() );

	stat = p_polylib->init_parallel_info(MPI_COMM_WORLD,
		myParaInfos[rank].bpos,
		myParaInfos[rank].bbsize,
		myParaInfos[rank].gcsize,
		myParaInfos[rank].dx);

	cout << "program at 2 on rank :"<<rank<<endl;

	if(stat !=PLSTAT_OK) return -1;
	stat=p_polylib->load_rank0("polylib_config_xyzrgb_statuette_stl.tp");

	cout << "program at 3 on rank :"<<rank<<endl;
	// stat=p_polylib->load_rank0("./polylib_config2.tpp");
	if(stat !=PLSTAT_OK) return -1;
	cout << "data loading finished on rank:"<<rank<<endl;
	p_polylib->show_group_hierarchy();
	cout << "program at 4 on rank :"<<rank<<endl;
	string fname="";
	//  string stl="obj_b";
	string stl="stl_b";
	string extend="";
	ID_FORMAT id_format=ID_BIN;
	//  ID_FORMAT id_format=ID_ASCII;
	//  p_polylib->save_rank0(&fname,stl,extend);
	p_polylib->save_parallel(&fname,stl,extend,id_format);

	MPI_Finalize();

	return 0;
}
