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
#include "CarGroup.h"
//#include "BladeGroup.h"
#include "MyGroupFactory.h"

using namespace std;
using namespace PolylibNS;

struct MyParallelInfo {
	REAL_TYPE bpos[3]; //基準座標
	unsigned bbsize[3]; //number of voxel 計算領域
	unsigned gcsize[3]; //number of guidecell voxel
	REAL_TYPE dx[3]; //size of voxel
};

static MyParallelInfo myParaInfos[4] = {
	{{  0,   0, -10,}, {10,10,20,}, {1, 1, 1,}, { 1, 1, 1,} },
	{{  0, -10, -10,}, {10,10,20,}, {1, 1, 1,}, { 1, 1, 1,} },
	{{-10, -10, -10,}, {10,10,20,}, {1, 1, 1,}, { 1, 1, 1,} },
	{{-10,   0, -10,}, {10,10,20,}, {1, 1, 1,}, { 1, 1, 1,} },
};

#ifdef WIN32
int main_test_move(int argc, char** argv ){
#else
int main(int argc, char** argv ){
#endif

	int rank;
	unsigned int step;
	POLYLIB_STAT stat;
	PolylibMoveParams params;

	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	PL_DBGOSH << "Starting program on rank:"<<rank<<endl;

	MPIPolylib* p_polylib = MPIPolylib::get_instance();

	p_polylib->set_factory(new MyGroupFactory() );

	stat = p_polylib->init_parallel_info(MPI_COMM_WORLD,
		myParaInfos[rank].bpos,
		myParaInfos[rank].bbsize,
		myParaInfos[rank].gcsize,
		myParaInfos[rank].dx);

	if(stat !=PLSTAT_OK) return -1;

	stat=p_polylib->load_rank0("./polylib_config_move.tp");
	if(stat !=PLSTAT_OK) return -1;
	//  PL_DBGOSH << "data loading finished on rank:"<<rank<<endl;
	//  p_polylib->show_group_hierarchy();



	PolygonGroup* pg=p_polylib->get_group("Square");

	if(pg) {
		PL_DBGOSH << "found" << std::endl;
	} else {
		PL_DBGOSH << "not found" << std::endl;
	}




	for(int step=0;step<2;++step){
		PL_DBGOSH << "starting step "<<step << std::endl;
		// 次計算ステップへ進むためにポリゴン情報を更新
		// move パラメタセットを設定
		PolylibMoveParams params;
		params.m_current_step = step;
		params.m_next_step     = step+1;
		params.m_delta_t       = 1.0;
		// move 実行
		stat = p_polylib->move(params);
		if( stat != PLSTAT_OK ) return -1;
		// migrate 実行
		stat = p_polylib->migrate();
		if( stat != PLSTAT_OK ) return -1;


		string fname="";
		string stl="obj_a";
		std::stringstream ss;
		ss << step;
		string extend="";
		ss >>extend;
		//  ID_FORMAT id_format=ID_BIN;
		ID_FORMAT id_format=ID_ASCII;
		//  p_polylib->save_rank0(&fname,stl,extend);
		p_polylib->save_parallel(&fname,stl,extend,id_format);

		PL_DBGOSH << "ending step "<<step << " " << extend <<std::endl;

	}



	// string fname="";
	// string stl="stl_a";
	// string extend="";
	// //  ID_FORMAT id_format=ID_BIN;
	// ID_FORMAT id_format=ID_ASCII;
	// //  p_polylib->save_rank0(&fname,stl,extend);
	// p_polylib->save_parallel(&fname,stl,extend,id_format);

	MPI_Finalize();

	return 0;


}
