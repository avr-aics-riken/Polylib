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
# Copyright (c) 2016-2018 Research Institute for Information Technology (RIIT), Kyushu University.
# All rights reserved.
#
###################################################################################
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
	PL_REAL bpos[3]; //基準座標
	unsigned bbsize[3]; //number of voxel 計算領域
	unsigned gcsize[3]; //number of guidecell voxel
	PL_REAL dx[3]; //size of voxel
};

static MyParallelInfo myParaInfos[2] = {
	{{-220, -220,-220,}, {22,44,44,}, {1, 1,1,}, {10,10,10} },
	{{0, -220,-220,}, {22,44,44,}, {1, 1,1,}, {10,10,10} },
};


#ifdef WIN32
int main_test_mpi_DVertex(int argc, char** argv ){
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

	MPIPolylib* pl_instance = MPIPolylib::get_instance();

	cout << "program at 1 on rank :"<<rank<<endl;



	//  pl_instance->set_factory(new MyGroupFactory() );

	stat = pl_instance->init_parallel_info(MPI_COMM_WORLD,
		myParaInfos[rank].bpos,
		myParaInfos[rank].bbsize,
		myParaInfos[rank].gcsize,
		myParaInfos[rank].dx);

	cout << "program at 2 on rank :"<<rank<<endl;


	std::string pgname="DV/byhand";
	pl_instance->make_DVertex_PolygonGroup(pgname,4,3);

	pl_instance->show_group_hierarchy();


	Vec3<PL_REAL> vlist[3];
	double value=10.0*rank;

	for (int itri=0;itri<2;itri++){
		if(itri==0){
			vlist[0].assign(20*(rank-1)+1,0.,0.);
			vlist[1].assign(20*(rank-1)+1,1.,0.);
			vlist[2].assign(20*(rank-1)+1,0.,1.);
		} else {

			vlist[0].assign(20*(rank-1)+2,1.,0.);
			vlist[1].assign(20*(rank-1)+1.,1.,0.);
			vlist[2].assign(20*(rank-1)+1.,0.,1.);

		}

		PL_DBGOSH << "vlist[0]=(" <<vlist[0]<<")"<<std::endl;
		PL_DBGOSH << "vlist[1]=(" <<vlist[1]<<")"<<std::endl;
		PL_DBGOSH << "vlist[2]=(" <<vlist[2]<<")"<<std::endl;

		DVertexTriangle* tri=
			pl_instance->add_DVertex_Triangle(pgname,vlist);
		tri->set_id(itri+rank*10);

		DVertex** vec_list= tri->get_DVertex();
		PL_DBGOSH << "back from get_DVertex " <<vec_list <<std::endl;
		DVertex* DVp=NULL;
		for(int ii=0;ii<3;++ii){
			DVp=vec_list[ii];

			DVp->set_scalar(0,value);
			value+=1.5;
			DVp->set_scalar(1,value);
			value+=1.5;
			DVp->set_scalar(2,value);
			value+=1.5;
			DVp->set_scalar(3,value);
			value+=1.5;
			PL_DBGOSH <<pgname<<std::endl;
			PL_DBGOSH<<  "0 "
				<< DVp->get_scalar(0)<<" "
				<<  "1 "
				<< DVp->get_scalar(1)<<" "
				<<  "2 "
				<< DVp->get_scalar(2)<<" "
				<<  "3 "
				<< DVp->get_scalar(3)<<std::endl;

			PL_DBGOSH << "set_vector" <<std::endl;

			Vec3<PL_REAL> a(10,20+ii,30+ii+rank*100);
			Vec3<PL_REAL> b(13,23+ii,33+ii+rank*100);
			Vec3<PL_REAL> c(14,24+ii,34+ii+rank*100);

			DVp->set_vector(0,a);
			DVp->set_vector(1,b);
			DVp->set_vector(2,c);

			Vec3<PL_REAL> d,e,f;
			DVp->get_vector(0,&d);
			DVp->get_vector(1,&e);
			DVp->get_vector(2,&f);

			PL_DBGOSH<<  "vector 0 a "<<a<<  " d "<< d  << std::endl;
			PL_DBGOSH<<  "vector 1 b "<<b<<  " e "<< e  << std::endl;
			PL_DBGOSH<<  "vector 2 c "<<c<<  " f "<< f  << std::endl;

		}

	}


	pl_instance->finalize_DVertex(pgname);

	string fname="";
	string stl="vtk_a";
	string extend="";
	ID_FORMAT id_format=ID_ASCII;

	cout << "program at save_rank0 on rank :"<<rank<<endl;
	pl_instance->save_rank0(&fname,stl,extend);
	cout << "program at end save_rank0 on rank :"<<rank<<endl;

	cout << "program at 5 on rank :"<<rank<<endl;
	MPI_Finalize();
	cout << "program at 6 on rank :"<<rank<<endl;
	return 0;
}
