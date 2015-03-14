/*
* Polylib - Polygon Management Library
*
* Copyright (c) 2010-2011 VCAD System Research Program, RIKEN.
* All rights reserved.
*
* Copyright (c) 2012-2015 Advanced Institute for Computational Science, RIKEN.
* All rights reserved.
*
*/


#include "polygons/TriMesh.h"


#include <algorithm>
#include "file_io/TriMeshIO.h"
#include "polygons/VertKDT.h"
#include "polygons/DVertexManager.h"
#include "polygons/DVertex.h"
#include "polygons/Triangle.h"
#include "polygons/DVertexTriangle.h"
#include "polygons/VTree.h"


#define M_MAX_ELEMENTS 15	/// VTreeのノードが持つ最大要素数

namespace PolylibNS {


// public /////////////////////////////////////////////////////////////////////
// std::sort用ファンクタ

struct PrivTriaLess{
	bool operator()( const PrivateTriangle *l, const PrivateTriangle *r ) const
	{
		return l->get_id() < r->get_id();
	}
};
// std::equal用ファンクタ

struct PrivTriaEqual{
	bool operator()( const PrivateTriangle *l, const PrivateTriangle *r ) const
	{
		return l->get_id() == r->get_id();
	}
};

// public /////////////////////////////////////////////////////////////////////

TriMesh::TriMesh()
{
	m_vtree = NULL;
	this->m_tri_list = NULL;
	//this->m_vtx_list = NULL;
	this->m_vertKDT = NULL;
	//std::cout << __func__ << " VertKDT is deleting "<<this-> m_vertKDT<< std::endl;
	this->m_vertex_list = NULL;
	m_max_elements = M_MAX_ELEMENTS;
	m_tolerance = 1.0e-10; // tempolary
	m_DVM_ptr=NULL;
}
// public /////////////////////////////////////////////////////////////////////

TriMesh::TriMesh(PL_REAL tolerance)
{
	m_vtree = NULL;
	this->m_tri_list = NULL;
	//this->m_vtx_list = NULL;
	this->m_vertex_list = NULL;
	m_vertKDT = NULL;
	//std::cout << __func__ << " VertKDT is deleting "<<this-> m_vertKDT<< std::endl;
	m_max_elements = M_MAX_ELEMENTS;
	m_tolerance = tolerance;
	m_DVM_ptr=NULL;
}

// public /////////////////////////////////////////////////////////////////////

TriMesh::~TriMesh()
{
	delete m_vtree;
	if (this->m_tri_list != NULL) {
		std::vector<PrivateTriangle*>::iterator itr;
		for (itr = this->m_tri_list->begin(); itr != this->m_tri_list->end(); itr++) {
			delete *itr;
		}
		this->m_tri_list->clear();
	}

	delete this->m_tri_list;

	// if (this->m_vtx_list != NULL) {
	//   std::vector<Vertex*>::iterator itr;
	// 	for (itr = this->m_vtx_list->begin(); itr != this->m_vtx_list->end(); itr++) {
	// 		delete *itr;
	// 	}
	// 	this->m_vtx_list->clear();
	// }

	// delete this->m_vtx_list;

	if (this->m_vertex_list != NULL) {
		delete this->m_vertex_list;
	}

	if(m_DVM_ptr!=NULL){
		delete m_DVM_ptr;
	}

}


// public /////////////////////////////////////////////////////////////////////

void TriMesh::init(const PL_REAL* vertlist,
	const int* idlist,
	const int* exidlist,
	const int n_start_tri,
	const int n_start_id,
	const int n_start_exid,
	const unsigned int n_tri)
{
	//#define DEBUG
#ifdef DEBUG
	PL_DBGOSH << "TriMesh::"<<__func__<< " in."<<std::endl;
#endif


	init_tri_list();
	init_vertex_list();
#ifdef DEBUG
	PL_DBGOSH << "TriMesh::"<<__func__<< " end of init tri_list and vert_list"<<std::endl;
	PL_DBGOSH <<"test "<<idlist[n_start_id]<<" "<< idlist[n_start_id+1]<<std::endl;
#endif

	for(int i=0;i<n_tri;++i) {
		int id=n_start_tri+i*9;
		Vertex* vtx_tri[3];
		for(int j=0;j<3;++j){
			Vertex* v=new Vertex(vertlist[id+j*3],vertlist[id+j*3+1],vertlist[id+j*3+2]);

			// vtx_tri[j]=this->m_vertex_list->vtx_add_KDT(v);
			//if(vtx_tri[j]!=v) delete v;
			//はじめに、重複を許して作成する。
			this->m_vertex_list->vtx_add_nocheck(v);
			vtx_tri[j]=v;
		}
		//int id2=n_start_id+i;
		//PrivateTriangle* tri=new PrivateTriangle(vtx_tri,idlist[id2]);
		PrivateTriangle* tri=new PrivateTriangle(vtx_tri,idlist[n_start_id+i],exidlist[n_start_exid+i]);
		this->m_tri_list->push_back(tri);
	}

	//最後に。頂点重複をチェックして、ポリゴン情報内のポインタも付け替える。
	//  POLYLIB_STAT status = TriMeshIO::vtx_compaction(this->m_vertex_list,
	//this->m_tri_list);

	//  POLYLIB_STAT status = vtx_compaction();

	vtx_compaction();

#ifdef DEBUG
	PL_DBGOSH << "TriMesh::"<<__func__<< " end of vtx_comaction"<<std::endl;
#endif
	//#undef DEBUG
}

// public /////////////////////////////////////////////////////////////////////

void TriMesh::init_dvertex(const PL_REAL* vertlist,
	const int* idlist,
	const int* exidlist,
	const PL_REAL* scalarlist,
	const PL_REAL* vectorlist,
	const int n_start_tri,
	const int n_start_id,
	const int n_start_exid,
	const int n_start_scalar,
	const int n_start_vector,
	const unsigned int n_tri,
	const int n_scalar,
	const int n_vector
	)

{
	//#define DEBUG
#ifdef DEBUG
	PL_DBGOSH << "TriMesh::"<<__func__<< " in."<<std::endl;
#endif

	int scalarindex=0;
	int vectorindex=0;


	init_tri_list();
	init_vertex_list();
#ifdef DEBUG
	PL_DBGOSH << "TriMesh::"<<__func__<< " end of init tri_list and vert_list"<<std::endl;
	PL_DBGOSH <<"test "<<idlist[n_start_id]<<" "<< idlist[n_start_id+1]<<std::endl;
#endif

	for(int i=0;i<n_tri;++i) {
		int id=n_start_tri+i*9;
		DVertex* vtx_tri[3];
		for(int j=0;j<3;++j){
			DVertex* dv=new DVertex(m_DVM_ptr);
			Vec3<PL_REAL> vec(vertlist[id+j*3],vertlist[id+j*3+1],vertlist[id+j*3+2]);

			Vertex* v=dv;

			*v=vec;
			for(int iscalar=0;iscalar<m_DVM_ptr->nscalar();iscalar++){
				dv->set_scalar(iscalar,
					scalarlist[n_start_scalar+scalarindex]);
				scalarindex++;
			}
			for(int ivector=0;ivector<m_DVM_ptr->nvector();ivector++){
				Vec3<PL_REAL> data_vec(vectorlist[n_start_scalar+scalarindex],
					vectorlist[n_start_scalar+scalarindex+1],
					vectorlist[n_start_scalar+scalarindex+2]);
				dv->set_vector(ivector,data_vec);
				vectorindex+=3;
			}

			// vtx_tri[j]=this->m_vertex_list->vtx_add_KDT(v);
			//if(vtx_tri[j]!=v) delete v;
			//はじめに、重複を許して作成する。
			this->m_vertex_list->vtx_add_nocheck(v);
			vtx_tri[j]=dv;
		}
		//int id2=n_start_id+i;
		//PrivateTriangle* tri=new PrivateTriangle(vtx_tri,idlist[id2]);
		DVertexTriangle* dtri=new DVertexTriangle(vtx_tri,idlist[n_start_id+i],exidlist[n_start_exid+i]);
		PrivateTriangle* tri=dtri;
		this->m_tri_list->push_back(tri);
	}

	//最後に。頂点重複をチェックして、ポリゴン情報内のポインタも付け替える。
	//  POLYLIB_STAT status = TriMeshIO::vtx_compaction(this->m_vertex_list,
	//this->m_tri_list);

	//  POLYLIB_STAT status = vtx_compaction();

	vtx_compaction();

#ifdef DEBUG
	PL_DBGOSH << "TriMesh::"<<__func__<< " end of vtx_comaction"<<std::endl;
#endif

}



// public /////////////////////////////////////////////////////////////////////

void TriMesh::add_dvertex(const PL_REAL* vertlist,
	const int* idlist,
	const int* exidlist,
	const PL_REAL* scalarlist,
	const PL_REAL* vectorlist,
	const int n_start_tri,
	const int n_start_id,
	const int n_start_exid,
	const int n_start_scalar,
	const int n_start_vector,
	const unsigned int n_tri,
	const int n_scalar,
	const int n_vector
	)
{

	//#define DEBUG
#ifdef DEBUG
	PL_DBGOSH << "TriMesh::"<<__func__<< " in."<<this<<std::endl;
	PL_DBGOSH << "TriMesh::"<<__func__<< " in."<<this->m_tri_list<<std::endl;
	PL_DBGOSH << "TriMesh::"<<__func__<< " in."<<this->m_vertex_list<<std::endl;
#endif
	int scalarindex=0;
	int vectorindex=0;

	if(n_tri==0) return;// PLSTAT_OK; // do nothing

	if (this->m_tri_list == NULL) {
		PL_DBGOSH << "TriMesh::"<<__func__<< " create vector<PrivateTrianle> ? right?"<<std::endl;
		this->m_tri_list = new std::vector<PrivateTriangle*>;
	}
	if (this->m_vertex_list == NULL) {
		PL_DBGOSH << "TriMesh::"<<__func__<< " create VertexList ? right?"<<std::endl;
		this->m_vertex_list = new VertexList;
	}

#ifdef DEBUG
	PL_DBGOSH << "TriMesh::"<<__func__<< " create vertex and triangle nvtx_before "<< this->m_vertex_list->size()<<std::endl;
#endif

	for(int i=0;i<n_tri;++i) {
		int id=n_start_tri+i*9;
		DVertex* vtx_tri[3];
		for(int j=0;j<3;++j){
			DVertex* dv=new DVertex(m_DVM_ptr);
			Vec3<PL_REAL> vec(vertlist[id+j*3],
				vertlist[id+j*3+1],
				vertlist[id+j*3+2]);

			Vertex * v=dv;
			*v=vec;
#ifdef DEBUG
			PL_DBGOSH << "TriMesh::"<<__func__<< " going to add vertex ("<< vec <<")"<<std::endl;
#endif


			for(int iscalar=0;iscalar<m_DVM_ptr->nscalar();iscalar++){
				dv->set_scalar(iscalar,
					scalarlist[n_start_scalar+scalarindex]);
				scalarindex++;
			}
			for(int ivector=0;ivector<m_DVM_ptr->nvector();ivector++){
				Vec3<PL_REAL> data_vec(vectorlist[n_start_scalar+vectorindex],
					vectorlist[n_start_scalar+vectorindex+1],
					vectorlist[n_start_scalar+vectorindex+2]);

#ifdef DEBUG
				PL_DBGOSH << "TriMesh::"<<__func__
					<< " " << i
					<< " " << j
					<< " " << ivector
					<<" ("<< data_vec <<")"<<std::endl;
#endif

				dv->set_vector(ivector,data_vec);
				vectorindex+=3;
			}



			//vtx_tri[j]=this->m_vertex_list->vtx_add_KDT(v);
			//if(vtx_tri[j]!=v) delete v;

			this->m_vertex_list->vtx_add_nocheck((Vertex*)dv);
			vtx_tri[j]=dv;
		}
		int id2=n_start_id+i;
		int id3=n_start_exid+i;
		DVertexTriangle* tri=new DVertexTriangle(vtx_tri,idlist[id2],exidlist[id3]);
		//      PrivateTriangle* tri=new PrivateTriangle(vtx_tri,idlist[id2]);
		this->m_tri_list->push_back((PrivateTriangle*)tri);
	}


#ifdef DEBUG
	PL_DBGOSH << "TriMesh::"<<__func__<< " create vertex and triangle nvtx_after "<< this->m_vertex_list->size()<<std::endl;
	for(int i =0 ;i<this->m_vertex_list->size();++i){
		PL_DBGOSH << "TriMesh::"<<__func__<< " vec "<<i<<" "<< *(this->m_vertex_list->get_vertex_lists()->at(i))<<std::endl;
	}
	for(int i =0 ;i<this->m_tri_list->size();++i){
		PL_DBGOSH << "TriMesh::"<<__func__<< " tri_id "<<i<<" "<< this->m_tri_list->at(i)->get_id()<<std::endl;
	}
#endif




#ifdef DEBUG
	PL_DBGOSH << "TriMesh::"<<__func__<< " sort"<<std::endl;
#endif

	// 三角形リストをID順にソート
	std::sort( this->m_tri_list->begin(),
		this->m_tri_list->end(),
		PrivTriaLess() );

#ifdef DEBUG
	PL_DBGOSH << "TriMesh::"<<__func__<< " erase"<<std::endl;
#endif


	//DVertexの場合は行わない。
	// ID重複ぶんを削除
	// this->m_tri_list->erase(
	// 			    std::unique(this->m_tri_list->begin(), this->m_tri_list->end(), PrivTriaEqual()),
	// 			    this->m_tri_list->end());



	// #ifdef DEBUG
	//     PL_DBGOSH << "TriMesh::"<<__func__<< " create vertex and triangle nvtx_after "<< this->m_vertex_list->size()<<std::endl;
	//     for(int i =0 ;i<this->m_vertex_list->size();++i){
	//       PL_DBGOSH << "TriMesh::"<<__func__<< " vec "<<i<<" "<< *(this->m_vertex_list->get_vertex_lists()->at(i))<<std::endl;
	//     }
	//     for(int i =0 ;i<this->m_tri_list->size();++i){
	//       PL_DBGOSH << "TriMesh::"<<__func__<< " tri_id "<<i<<" "<< this->m_tri_list->at(i)->get_id()<<std::endl;
	//     }
	// #endif


#ifdef DEBUG
	PL_DBGOSH << "TriMesh::"<<__func__<< " end."<<std::endl;
#endif
	//#undef DEBUG
}






// public /////////////////////////////////////////////////////////////////////

void TriMesh::init(const std::vector<PrivateTriangle*>* trias)
{
	//#define DEBUG
#ifdef DEBUG
	PL_DBGOSH<< "TriMesh::" << __func__ <<std::endl;
#endif
	init_tri_list();
#ifdef DEBUG
	PL_DBGOSH<< "TriMesh::" << __func__ << " init_tri_list end"<<std::endl;
#endif

	VertKDT* new_vertKDT=new VertKDT(this->m_max_elements);
	VertexList* new_vertex_list= new VertexList(new_vertKDT,this->m_tolerance);
	//  std::map<Vertex*,int> vmap;

	std::vector<PrivateTriangle*>::const_iterator itr;

	//first correct vertices
	for (itr = trias->begin(); itr != trias->end(); itr++) {
		try{
			Vertex** tmpvert=(*itr)->get_vertex();
			if(tmpvert==0) throw "tmpvert";
			//      PL_DBGOSH<< "TriMesh::" << __func__ << " get_vertex "<< tmpvert <<std::endl;
			Vertex* tmpvert_in[3];

			// make deep copy!
			Vertex* tmp = new Vertex(*(tmpvert[0]));
			//      PL_DBGOSH<< "TriMesh::" << __func__ << " make new Vertex 0 "<< tmp <<std::endl;
			if(tmp==0) throw "tmp";
			new_vertex_list->vtx_add_nocheck(tmp);
			tmpvert_in[0]=tmp;
			Vertex* tmp1 = new Vertex(*(tmpvert[1]));
			//PL_DBGOSH<< "TriMesh::" << __func__ << " make new Vertex 1 "<< tmp1 <<std::endl;
			if(tmp1==0) throw "tmp1";
			new_vertex_list->vtx_add_nocheck(tmp1);
			tmpvert_in[1]=tmp1;
			Vertex* tmp2 = new Vertex(*(tmpvert[2]));
			//PL_DBGOSH<< "TriMesh::" << __func__ << " make new Vertex 2 "<< tmp2 <<std::endl;
			if(tmp2==0) throw "tmp2";
			new_vertex_list->vtx_add_nocheck(tmp2);
			tmpvert_in[2]=tmp2;

			// new_vertex_list->vtx_add_nocheck(tmpvert[0]);
			// new_vertex_list->vtx_add_nocheck(tmpvert[1]);
			// new_vertex_list->vtx_add_nocheck(tmpvert[2]);
			/* vmap[tmpvert[0]]=1; */
			/* vmap[tmpvert[1]]=1; */
			/* vmap[tmpvert[2]]=1; */
			//    PrivateTriangle* tri=new PrivateTriangle( (*itr)->get_vertex(),
			PrivateTriangle* tri=new PrivateTriangle( tmpvert_in,
				(*itr)->get_normal(), 
				(*itr)->get_area(),
				(*itr)->get_id()
				);
			tri->set_exid((*itr)->get_exid());
			this->m_tri_list->push_back(tri);
		}
		catch (char* str) {
			std::cerr << "TriMesh::"<<__func__<<" error in "<<str<<std::endl;
		}
	}

#ifdef DEBUG
	PL_DBGOSH<< "TriMesh::" << __func__ << " create tempolary VertexList and tri_list"<<std::endl;
#endif


	/* init_vertex_list(); */
	/* m_vertKDT=new_vertKDT; */
	/* this->m_vertex_list=new_vertex_list; */
	replace_vertex_list(new_vertex_list);



	//  POLYLIB_STAT status = TriMeshIO::vtx_compaction(new_vertex_list, this->m_tri_list);
	//  POLYLIB_STAT status = vtx_compaction();
	vtx_compaction();
	//#undef DEBUG
}




// public /////////////////////////////////////////////////////////////////////

void TriMesh::add(const PL_REAL* vertlist,
	const int* idlist,
	const int* exidlist,
	const int n_start_tri,
	const int n_start_id,
	const int n_start_exid,
	const unsigned int n_tri)
{
#ifdef DEBUG
	PL_DBGOSH << "TriMesh::add start"<<std::endl;
#endif
	if (this->m_tri_list == NULL) {
		this->m_tri_list = new std::vector<PrivateTriangle*>;
	}
	if (this->m_vertex_list == NULL) {
		this->m_vertex_list = new VertexList;
	}
#ifdef DEBUG
	PL_DBGOSH << "TriMesh::add VertexList and PrivateTriangle is ready."<<std::endl;
#endif

	for(int i=0;i<n_tri;++i) {
		int id=n_start_tri+i*9;
		Vertex* vtx_tri[3];
		for(int j=0;j<3;++j){
			Vertex* v=new Vertex(vertlist[id+j*3],vertlist[id+j*3+1],vertlist[id+j*3+2]);


			//vtx_tri[j]=this->m_vertex_list->vtx_add_KDT(v);
			//if(vtx_tri[j]!=v) delete v;
			this->m_vertex_list->vtx_add_nocheck(v);
			/* PL_DBGOSH << "TriMesh::add Triangle. vtx "<< j << " " <<v<<std::endl; */
			/* PL_DBGOSH << "TriMesh::add Triangle. vtx "<< j << " " <<*v<<std::endl; */
			//   なくなっていた１行
			vtx_tri[j]=v;
			//   なくなっていた１行

		}
		int id2=n_start_id+i;
		int id3=n_start_exid+i;

		if(vtx_tri[0]!=NULL &&vtx_tri[1]!=NULL &&vtx_tri[2]!=NULL){
			//PL_DBGOSH << __func__  << " Vertex pointer is checked."<<std::endl;
		} else {
			PL_ERROSH << __func__ 
				<< " NULL pointer "<< vtx_tri[0]
			<<" "<< vtx_tri[1]
			<<" "<< vtx_tri[2]<<std::endl;
		} 

		PrivateTriangle* tri=new PrivateTriangle(vtx_tri,idlist[id2],exidlist[id3]);

		/* PL_DBGOSH << "TriMesh::add Triangle. triangle "<< i  */
		/* 		 << " id "<<idlist[id2]<<std::endl; */

		this->m_tri_list->push_back(tri);

		/* PL_DBGOSH << "TriMesh::add Triangle. triangle "<< i <<std::endl; */
		/* // */
	}

	//PL_DBGOSH << "TriMesh::add Triangle. add vtx"<<std::endl;

	// 三角形リストをID順にソート
	std::sort( this->m_tri_list->begin(), this->m_tri_list->end(), PrivTriaLess() );
#ifdef DEBUG
	PL_DBGOSH << "TriMesh::add Triangle. sort"<<std::endl;
#endif
	// Comment
	// Vertex および PrivateTriangle が delete されていない。
	// 2013-Jan.-10th T. Suzuki

#ifdef DEBUG     
	for(int i=0;i<this->m_tri_list->size();++i){
		PL_DBGOSH << " before id "<<(*this->m_tri_list)[i]->get_id() <<std::endl;
	}
#endif

	// ID重複ぶんを削除
	this->m_tri_list->erase(
		std::unique(this->m_tri_list->begin(), 
		this->m_tri_list->end(), 
		PrivTriaEqual()),
		this->m_tri_list->end());

#ifdef DEBUG
	for(int i=0;i<this->m_tri_list->size();++i){
		PL_DBGOSH << " after id "<<(*this->m_tri_list)[i]->get_id() <<std::endl;
	}
	PL_DBGOSH << "TriMesh::add end"<<std::endl;
#endif

}


// public /////////////////////////////////////////////////////////////////////

void
	TriMesh::add(
	const std::vector<PrivateTriangle*> *trias
	)
{
	//#define DEBUG
#ifdef DEBUG
	PL_DBGOSH << "TriMesh::add_triangles() in." << std::endl;
#endif
	unsigned int i;

	if (this->m_tri_list == NULL) {
		this->m_tri_list = new std::vector<PrivateTriangle*>;
	}
	if (this->m_vertex_list == NULL) {
		this->m_vertex_list = new VertexList;
	}

	// ひとまず全部追加
	for( i=0; i<trias->size(); i++ ) {
		this->m_tri_list->push_back( new PrivateTriangle( *(trias->at(i)) ) );
	}

	// 三角形リストをID順にソート
	std::sort( this->m_tri_list->begin(), this->m_tri_list->end(), PrivTriaLess() );

	// ID重複ぶんを削除
	this->m_tri_list->erase(
		std::unique(this->m_tri_list->begin(), this->m_tri_list->end(), PrivTriaEqual()),
		this->m_tri_list->end());
#ifdef DEBUG
	PL_DBGOSH << "TriMesh<T>::add_triangles() in." << std::endl;
#endif

#undef DEBUG
}

// public /////////////////////////////////////////////////////////////////////

POLYLIB_STAT TriMesh::import(const std::map<std::string, std::string> fmap, PL_REAL scale)
{

	//      PL_DBGOSH << __func__ << " scale "<< scale <<std::endl;

	init_tri_list();
	//PL_DBGOSH << __func__ << " scale 1 "<< scale <<std::endl;
	init_vertex_list();
	//PL_DBGOSH << __func__ << " scale 2 "<< scale <<std::endl;

	POLYLIB_STAT ret = TriMeshIO::load(this->m_vertex_list,
		this->m_tri_list, fmap, scale);
	if(ret!=PLSTAT_OK) return ret;

	vtx_compaction();		

	return ret;

	//return TriMeshIO::load(this->m_tri_list, fmap, scale);
}


// public /////////////////////////////////////////////////////////////////////

POLYLIB_STAT TriMesh::build()
{
	//#define DEBUG
#ifdef DEBUG
	PL_DBGOSH << "TriMesh::build() start" << std::endl;
#endif // DEBUG

	BBox bbox;

	//	BBox bbox=this->m_vertex_list->get_bbox();

	// now bbox is taken from VertKDT.


	// #ifdef DEBUG
	//   PL_DBGOSH << "TriMesh::build() test" << std::endl;
	//   BBox vtx_bbox=this->m_vertex_list->get_bbox();
	//   PL_DBGOSH << "TriMesh::build() vertex_list bbox min"
	// 	    << vtx_bbox.min<<"max"<< vtx_bbox.max <<std::endl;

	//   PL_DBGOSH << "TriMesh::build() vertex_list 0 1 2 "
	// 	    << this->m_vertex_list->ith(0)<<" "
	// 	    << this->m_vertex_list->ith(1)<<" "
	// 	    << this->m_vertex_list->ith(2)<<std::endl;
	// #endif // DEBUG
	int counter=0;
	std::vector<PrivateTriangle*>::iterator itr;
	/// TriMeshクラスに含まれる全三角形ポリゴンを外包するBoundingBoxを計算
	bbox.init();
	for (itr = this->m_tri_list->begin(); itr != this->m_tri_list->end(); itr++) {
		counter++;
#ifdef DEBUG
		PL_DBGOSH << "TriMesh::build() polygon loop " << counter << std::endl;
#endif // DEBUG


		//		const Vec3<PL_REAL>* vtx_arr = (*itr)->get_vertex();

		Vertex** vtx_arr= (*itr)->get_vertex();
#ifdef DEBUG
		PL_DBGOSH << "TriMesh::build() polygon get_vertex "
			<< vtx_arr[0]<<" "
			<< vtx_arr[1]<<" "
			<< vtx_arr[2]<< std::endl;
#endif // DEBUG

		for (int i = 0; i < 3; i++) {
#ifdef DEBUG
			PL_DBGOSH << "TriMesh::build() vertex normal " << (*(vtx_arr[i]))[0] <<std::endl;
			PL_DBGOSH << "TriMesh::build() vertex normal " << (*(vtx_arr[i]))[1] <<std::endl;
			PL_DBGOSH << "TriMesh::build() vertex normal " << (*(vtx_arr[i]))[2] <<std::endl;
#endif // DEBUG

			//   if( ( (*(vtx_arr[i]))[0] > 1000) || ( (*(vtx_arr[i]))[1] > 1000) ||( (*(vtx_arr[i]))[2] > 1000) ){
			// PL_DBGOSH << "TriMesh::build() vertex error " << (*(vtx_arr[i]))[0] <<std::endl;
			// PL_DBGOSH << "TriMesh::build() vertex error " << (*(vtx_arr[i]))[1] <<std::endl;
			// PL_DBGOSH << "TriMesh::build() vertex error " << (*(vtx_arr[i]))[2] <<std::endl;
			//     }


			bbox.add(  (Vec3<PL_REAL>) (*(vtx_arr[i])) );
		}

	}

	m_bbox = bbox;
	//#define DEBUG
#ifdef DEBUG
	Vec3<PL_REAL> min = m_bbox.getPoint(0);
	Vec3<PL_REAL> max = m_bbox.getPoint(7);
	PL_DBGOSH << "TriMesh::build:min=(" <<min<< "),max=(" <<max<< ")" << std::endl;
#endif
	//PL_DBGOSH << "TriMesh::build:min=(" <<m_bbox.min<< "),max=(" <<m_bbox.max<< ")" << std::endl;

	//#undef DEBUG
	// 木構造作成
	if (m_vtree != NULL) delete m_vtree;
	m_vtree = new VTree(m_max_elements, m_bbox, this->m_tri_list);
	// if (m_vertKDT!=NULL) delete m_vertKDT;
	// m_vertKDT = new VertKDT(m_max_elements, m_bbox, this->m_vertex_list);
#ifdef DEBUG
	PL_DBGOSH << "TriMesh::build: end of vtree " << std::endl;
#endif

	return PLSTAT_OK;
	//#undef DEBUG
}

// public /////////////////////////////////////////////////////////////////////

int TriMesh::triangles_num() {
	if (this->m_tri_list == NULL)		return 0;
	else						return this->m_tri_list->size();
}

// public /////////////////////////////////////////////////////////////////////

const std::vector<PrivateTriangle*> *TriMesh::search(
	BBox	*bbox, 
	bool	every
	) const {
		//#define DEBUG
#ifdef DEBUG
		Vec3<PL_REAL> min = bbox->getPoint(0);
		Vec3<PL_REAL> max = bbox->getPoint(7);
		PL_DBGOSH << "TriMesh::search:min=(" <<min<< "),max=(" <<max<< ")" << std::endl;
#endif

		return m_vtree->search(bbox, every);
		//#undef DEBUG
}

// public /////////////////////////////////////////////////////////////////////

POLYLIB_STAT TriMesh::search(
	BBox						*bbox, 
	bool						every, 
	std::vector<PrivateTriangle*>	*tri_list
	) const {
		return m_vtree->search(bbox, every, tri_list);
}

// public /////////////////////////////////////////////////////////////////////

const std::vector<PrivateTriangle*>* TriMesh::linear_search(
	BBox	*q_bbox, 
	bool	every
	) const {
		std::vector<PrivateTriangle*>		   *tri_list = new std::vector<PrivateTriangle*>;
		std::vector<PrivateTriangle*>::iterator itr;

		for (itr = this->m_tri_list->begin(); itr != this->m_tri_list->end(); itr++) {
			BBox bbox;
			bbox.init();
			//const Vec3<PL_REAL>* vtx_arr = (*itr)->get_vertex();
			Vertex** vtx_arr = (*itr)->get_vertex();
			for (int i = 0; i < 3; i++) {
				bbox.add( (Vec3<PL_REAL>) *vtx_arr[i]);
			}
			if (every == true) {
				if (q_bbox->contain( (Vec3<PL_REAL>) *vtx_arr[0]) == true && 
					q_bbox->contain( (Vec3<PL_REAL>) *vtx_arr[1]) == true &&
					q_bbox->contain( (Vec3<PL_REAL>) *vtx_arr[2]) == true)
				{
					tri_list->push_back(*itr);
				}
			}
			else {
#ifdef OLD_DEF
				if (bbox.crossed(*q_bbox) == true				||
					bbox.contain(q_bbox->getPoint(0)) == true	||
					q_bbox->crossed(bbox) == true				||
					q_bbox->contain(bbox.getPoint(0)) == true) {
#else
				if (bbox.crossed(*q_bbox) == true) {
#endif
					tri_list->push_back(*itr);
				}
			}
		}
		return tri_list;
}

// public /////////////////////////////////////////////////////////////////////

POLYLIB_STAT TriMesh::linear_search(
	BBox						*q_bbox, 
	bool						every, 
	std::vector<PrivateTriangle*>	*tri_list
	) const {
		if (tri_list == NULL) return PLSTAT_ARGUMENT_NULL;

		std::vector<PrivateTriangle*>::iterator itr;

		for (itr = this->m_tri_list->begin(); itr != this->m_tri_list->end(); itr++) {
			BBox bbox;
			bbox.init();
			//const Vec3<PL_REAL>* vtx_arr = (*itr)->get_vertex();
			Vertex** vtx_arr = (*itr)->get_vertex();

			for (int i = 0; i < 3; i++) {
				bbox.add( (Vec3<PL_REAL>) *vtx_arr[i]);
			}
			if (every == true) {
				if (q_bbox->contain( (Vec3<PL_REAL>) *vtx_arr[0]) == true	&&
					q_bbox->contain( (Vec3<PL_REAL>) *vtx_arr[1]) == true	&&
					q_bbox->contain( (Vec3<PL_REAL>) *vtx_arr[2]) == true) {
						tri_list->push_back(*itr);
#ifdef DEBUG
						PL_DBGOSH << "TriMesh::linear_search:IN TRUE" << std::endl;
						PL_DBGOSH << "     vertex 0:" << (Vec3<PL_REAL>) *vtx_arr[0] << std::endl;
						PL_DBGOSH << "     vertex 1:" << (Vec3<PL_REAL>) *vtx_arr[1] << std::endl;
						PL_DBGOSH << "     vertex 2:" << (Vec3<PL_REAL>) *vtx_arr[2] << std::endl;
#endif
				}
			}
			else {
#ifdef OLD_DEF
				if (bbox.crossed(*q_bbox) == true				||
					q_bbox->crossed(bbox) == true				||
					bbox.contain(q_bbox->getPoint(0)) == true	||
					q_bbox->contain(bbox.getPoint(0)) == true) {
#else
				if (bbox.crossed(*q_bbox) == true) {
#endif
					tri_list->push_back(*itr);
#ifdef DEBUG
					PL_DBGOSH << "TriMesh::linear_search:IN FALSE" << std::endl;
#endif
				}
			}
#ifdef DEBUG
			for (int i=0; i<8; i++) {
				PL_DBGOSH << "TriMesh::linear_search:q_box[" << i << "]:" 
					<< q_bbox->getPoint(i) << std::endl;
			}
			PL_DBGOSH << "TriMesh::linear_searc:" << " id:" << (*itr)->get_id()
				<< ",v(" << vtx_arr << ")" << std::endl;
#endif
		}
		return PLSTAT_OK;
}

// public /////////////////////////////////////////////////////////////////////

const PrivateTriangle* TriMesh::search_nearest(
	const Vec3<PL_REAL>&    pos
	) const {
		return m_vtree->search_nearest(pos);
}

// public /////////////////////////////////////////////////////////////////////

POLYLIB_STAT TriMesh::set_all_exid(
	const int    id
	) const {
		// 全ポリゴンのm_exidをidで上書き
		std::vector<PrivateTriangle*>::iterator itr;
		for (itr = this->m_tri_list->begin(); itr != this->m_tri_list->end(); itr++) {
			(*itr)->set_exid( id );
		}
		return PLSTAT_OK;
}

// private ////////////////////////////////////////////////////////////////////

void TriMesh::init_tri_list()
{
	if (this->m_tri_list == NULL) {
		this->m_tri_list = new std::vector<PrivateTriangle*>;


	}
	else {
		std::vector<PrivateTriangle*>::iterator itr;
		for (itr = this->m_tri_list->begin();
			itr != this->m_tri_list->end();
			itr++) {
				delete *itr;
		}

		this->m_tri_list->clear();

	}
	if(m_vtree!=NULL) {
		delete m_vtree;
		m_vtree=NULL;
	}

}

// private ////////////////////////////////////////////////////////////////////

void TriMesh::init_vertex_list()
{
	//#define DEBUG
#ifdef DEBUG
	PL_DBGOSH << __func__ << " in " <<std::endl;
#endif
	if (this->m_vertex_list == NULL) {
		//std::cout << __func__ << " VertexList is null"<< std::endl;
	} else {
		//std::cout << __func__ << " VertexList is deleting"<< std::endl;
		this->m_vertex_list->vtx_clear();
		delete this->m_vertex_list;

	}
#ifdef DEBUG
	PL_DBGOSH << __func__ << " clear vertex " <<std::endl;
#endif
	if (this->m_vertKDT == NULL) {
		//std::cout << __func__ << " VertKDT is null"<< std::endl;
	} else {
		//std::cout << __func__ << " VertKDT is deleting "<<this-> m_vertKDT<< std::endl;
		delete this->m_vertKDT;
	}
#ifdef DEBUG
	PL_DBGOSH << __func__ << " clear KDT " <<std::endl;
#endif

	this->m_vertKDT = new VertKDT(m_max_elements);
	//std::cout << __func__ << " VertKDT end"<< std::endl;
	this->m_vertex_list = new VertexList(this->m_vertKDT,this->m_tolerance);
	//std::cout << __func__ << " vertex end"<< std::endl;
	//this->m_vertex_list = new VertexList;
#ifdef DEBUG
	PL_DBGOSH << __func__ << " end " <<std::endl;
#endif
	//#undef DEBUG
}

// private ////////////////////////////////////////////////////////////////////

void TriMesh::replace_vertex_list(VertexList* vlist)
{
	//#define DEBUG
#ifdef DEBUG
	PL_DBGOSH << __func__ << " in " <<std::endl;
#endif
	if (this->m_vertex_list == NULL) {
		//std::cout << __func__ << " VertexList is null"<< std::endl;
	} else {
		//std::cout << __func__ << " VertexList is deleting"<< std::endl;
		this->m_vertex_list->vtx_clear();
		delete this->m_vertex_list;
	}
#ifdef DEBUG
	PL_DBGOSH << __func__ << " clear vertex " <<std::endl;
#endif
	if (this->m_vertKDT == NULL) {
		//std::cout << __func__ << " VertKDT is null"<< std::endl;
	} else {
		//std::cout << __func__ << " VertKDT is deleting "<<this-> m_vertKDT<< std::endl;
		delete this->m_vertKDT;
	}
#ifdef DEBUG
	PL_DBGOSH << __func__ << " clear KDT " <<std::endl;
#endif

	//    m_vertKDT = new VertKDT(m_max_elements);
	//std::cout << __func__ << " VertKDT end"<< std::endl;
	//    vlist->setKDT(m_vertKDT);
	this->m_vertex_list = vlist;
	m_vertKDT = vlist->getKDT();
	//    this->m_vertex_list = new VertexList(this->m_vertKDT,this->m_tolerance);
	//std::cout << __func__ << " vertex end"<< std::endl;
	//this->m_vertex_list = new VertexList;


#ifdef DEBUG
	PL_DBGOSH << __func__ << " end " <<std::endl;
#endif
	//#undef DEBUG
}



// private ////////////////////////////////////////////////////////////////////

void TriMesh::vtx_compaction()
{
	//#define DEBUG

#ifdef DEBUG
	PL_DBGOSH << "vtx_compaction" <<std::endl;
#endif

	std::map<Vertex*,Vertex*>* vtx_map=new  std::map<Vertex*,Vertex*>;
	;
	this->m_vertex_list->vertex_compaction(vtx_map);
	std::vector<PrivateTriangle*>::iterator itr;

#ifdef DEBUG
	int counter =0;
#endif
	for(itr=this->m_tri_list->begin();
		itr!=this->m_tri_list->end();
		itr++){

			Vertex**  tmp_list=  (*itr)->get_vertex();
			bool vertex_replace=false;
#ifdef DEBUG
			PL_DBGOSH << "pointer "<<tmp_list[0]<< " "<<tmp_list[1]<<" "<<tmp_list[2]<<std::endl;
#endif
			for(int i=0;i<3;i++){
#ifdef DEBUG
				if(counter<10)
					PL_DBGOSH << "pointer map old "<<tmp_list[i]<< " new "<<(*vtx_map)[tmp_list[i]]<<" "<<*(tmp_list[i])<<std::endl;
#endif
				if((*vtx_map)[tmp_list[i]]==tmp_list[i]){
					// do nothing
				} else { // substitute 
#ifdef DEBUG
					if(counter<10)
						PL_DBGOSH << "pointer swaped old "<<tmp_list[i]<< " new "<<(*vtx_map)[tmp_list[i]]<<std::endl;
#endif
					tmp_list[i]=(*vtx_map)[tmp_list[i]];
					vertex_replace=true;
				}

			}
			if(vertex_replace){ //vertex replaced
#ifdef DEBUG
				PL_DBGOSH << "vertex replaced "<< *itr <<std::endl;
#endif

				(*itr)->set_vertexes(tmp_list,true,true);

				if((*itr)->get_area()==0.0){ //zero area check
					PL_DBGOSH << __func__ 
						<< " Warning : polygon contains a triangle that its area is zero." << std::endl;
					PL_DBGOSH <<  "vertex0 ("<< *(tmp_list[0]) <<")"<<std::endl;
					PL_DBGOSH <<  "vertex1 ("<< *(tmp_list[1]) <<")"<<std::endl;
					PL_DBGOSH <<  "vertex2 ("<< *(tmp_list[2]) <<")"<<std::endl;

				}
			} else {
				// vertex not replaced
				(*itr)->set_vertexes(tmp_list,false,false);
			}

#ifdef DEBUG
			counter++;
#endif
	}

	//#undef DEBUG
}
//// public ///////////////////////////////

POLYLIB_STAT TriMesh::replace_DVertex(int nscalar,int nvector){
	//#define DEBUG
#ifdef DEBUG
	PL_DBGOSH << __func__ << std::endl;
#endif

	m_DVM_ptr = new DVertexManager(nscalar,nvector);
	std::map<Vertex*,Vertex*> ptr_map;
#ifdef DEBUG
	PL_DBGOSH << __func__ << " 1"<<std::endl;
#endif

	VertexList* new_dv=new VertexList(this->m_tolerance);

#ifdef DEBUG
	PL_DBGOSH << __func__ << " 2"<<std::endl;
#endif

	const std::vector<Vertex*>* old_vlist=this->m_vertex_list->get_vertex_lists();

#ifdef DEBUG
	PL_DBGOSH << __func__ << " 3"<<std::endl;
#endif


	for(std::vector<Vertex*>::const_iterator itr = old_vlist->begin();

		itr != old_vlist->end();
	itr++){

#ifdef DEBUG
		PL_DBGOSH << __func__ << "3 1"<<std::endl;
#endif

		Vec3<PL_REAL> pos((*(*itr))[0],(*(*itr))[1],(*(*itr))[2]);

#ifdef DEBUG
		PL_DBGOSH << __func__ << "3 2"<< pos <<std::endl;
#endif

		Vertex* dv;
		DVertex* tmp = new DVertex(m_DVM_ptr) ;
		dv=tmp;

#ifdef DEBUG
		PL_DBGOSH << __func__ << " 3 3"<< std::endl;
#endif

		*dv=pos;

#ifdef DEBUG
		PL_DBGOSH << __func__ << "3 4"<<std::endl;
#endif

		new_dv->vtx_add_nocheck( dv);

#ifdef DEBUG
		PL_DBGOSH << __func__ << "3 5"<< std::endl;
#endif

		ptr_map.insert(std::pair<Vertex*,Vertex*>( (*itr) , dv) );

#ifdef DEBUG
		PL_DBGOSH << __func__ << "3 6"<<std::endl;
#endif

	}

#ifdef DEBUG
	PL_DBGOSH << __func__ << " 4"<<std::endl;
#endif

	std::vector<PrivateTriangle*>* 
		tmp_dvertex_tri_list=new std::vector<PrivateTriangle*>;

	//  三角形リストの頂点ポインタ付け替え
	if (this->m_tri_list != NULL) {
#ifdef DEBUG
		PL_DBGOSH << __func__ << " 4 1"<<std::endl;
#endif


		std::vector<PrivateTriangle*>::iterator itr;
		for (itr = this->m_tri_list->begin(); itr != this->m_tri_list->end(); itr++) {

#ifdef DEBUG
			PL_DBGOSH << __func__ << " 4 2"<<std::endl;
#endif

			Vertex** tmpvert=(*itr)->get_vertex();
			DVertex* tmpdvert[3];

#ifdef DEBUG
			PL_DBGOSH << __func__ << " 4 2 1"<<std::endl;
#endif

			tmpvert[0]=ptr_map[tmpvert[0]];
			tmpvert[1]=ptr_map[tmpvert[1]];
			tmpvert[2]=ptr_map[tmpvert[2]];

#ifdef DEBUG
			PL_DBGOSH << __func__ << " 4 2 2"<<std::endl;
#endif

			tmpdvert[0]=dynamic_cast<DVertex*>(tmpvert[0]);
			tmpdvert[1]=dynamic_cast<DVertex*>(tmpvert[1]);
			tmpdvert[2]=dynamic_cast<DVertex*>(tmpvert[2]);

			// (*itr)->set_vertexes(tmpvert,true,true);
			//make new DVertexTriangle

#ifdef DEBUG
			PL_DBGOSH << __func__ << " 4 3"<<std::endl;
#endif

			DVertexTriangle* new_dv_tri= new DVertexTriangle(tmpdvert,(*itr)->get_id());
#ifdef DEBUG
			PL_DBGOSH << __func__ << " 4 4"<<std::endl;
#endif

			tmp_dvertex_tri_list->push_back((PrivateTriangle*)new_dv_tri);

		}


	}
#ifdef DEBUG
	PL_DBGOSH << __func__ << " 5"<<std::endl;
#endif



	init_vertex_list();
	this->m_vertex_list=new_dv;


	init_tri_list();
	this->m_tri_list= tmp_dvertex_tri_list;

	build();

#ifdef DEBUG
	PL_DBGOSH << __func__ << " end"<<std::endl;
#endif
	//#undef DEBUG
	return PLSTAT_OK;
}
/////////////////////////


POLYLIB_STAT TriMesh::prepare_DVertex(int nscalar,int nvector){
	//#define DEBUG
#ifdef DEBUG
	PL_DBGOSH << __func__ << std::endl;
#endif
	init_tri_list();
	init_vertex_list();
	m_DVM_ptr = new DVertexManager(nscalar,nvector);
#ifdef DEBUG
	PL_DBGOSH << __func__ 
		<< " nscalar="<<m_DVM_ptr->nscalar()
		<< " nvector="<<m_DVM_ptr->nvector()
		//<< " size="<<m_DVM_ptr->size() 
		<< " this " <<this
		<< std::endl;
#endif
	//#undef DEBUG   



	return PLSTAT_OK;
}
/////////////////////////


DVertexTriangle* 
	TriMesh::add_DVertex_Triangle(Vec3<PL_REAL>* v)
{

	//#define DEBUG
#ifdef DEBUG
	PL_DBGOSH << "TriMesh::"<< __func__<<" "<<this<<std::endl;
	PL_DBGOSH << "TriMesh::"<< __func__
		<<" v0 "<<v[0]
	<<" v1 "<<v[1]
	<<" v2 "<<v[2]
	<< std::endl;
#endif

	//   Vertex* vtx_list[3];   
	DVertex* vtx_list[3];   
	//PL_DBGOSH << "TriMesh::"<< __func__<< " 1" << " this "<<this <<std::endl;
	for(int i=0;i<3;i++){
		Vertex* dv;

#ifdef DEBUG
		if(m_DVM_ptr==NULL){
			PL_DBGOSH << __func__  << "m_DVM_ptr is NULL"<<std::endl;
		}else {
			PL_DBGOSH << __func__ 
				<< " nscalar="<<m_DVM_ptr->nscalar()
				<< " nvector="<<m_DVM_ptr->nvector()
				//<< " size="<<m_DVM_ptr->size()
				<< std::endl;
		}
#endif


		//     PL_DBGOSH << "TriMesh::"<< __func__<< " 2 1 "<< m_DVM_ptr<<std::endl;
		DVertex* tmp = new DVertex(m_DVM_ptr) ;
		//PL_DBGOSH << "TriMesh::"<< __func__<< " 2 1 "<<*tmp<<std::endl;
		//PL_DBGOSH << "TriMesh::"<< __func__<< " 2 1 "<<tmp<<std::endl;
		dv=tmp;

		//   if(m_DVM_ptr->size()==4){
		// PL_DBGOSH << "TriMesh::"<< __func__<< " 2 1 1"<<std::endl;
		//     DVertex<PL_REAL,float>* tmp = new DVertex<PL_REAL,float>(m_DVM_ptr) ;
		// PL_DBGOSH << "TriMesh::"<< __func__<< " 2 1 1"<<std::endl;
		//   }   else if(m_DVM_ptr->size()==8){
		// PL_DBGOSH << "TriMesh::"<< __func__<< " 2 1 2"<<std::endl;
		//     DVertex<PL_REAL,double>* tmp = new  DVertex<PL_REAL,double>(m_DVM_ptr);
		// PL_DBGOSH << "TriMesh::"<< __func__<< " 2 1 2"<<std::endl;
		//     dv=tmp;
		//   }
		//PL_DBGOSH << "TriMesh::"<< __func__<< " 2 2 "<<std::endl;
		*dv=v[i];
		//PL_DBGOSH << "TriMesh::"<< __func__<< " 2 3 "<<std::endl;
		//   *tmp=*dv;
		//vtx_list[i]=dv;
		vtx_list[i]=tmp;

		//PL_DBGOSH << "TriMesh::"<< __func__<< " 2 4 "<<*tmp<<std::endl;
		if(this->m_vertex_list!=NULL)  this->m_vertex_list->vtx_add_nocheck((Vertex*)tmp);
		else    PL_DBGOSH << "TriMesh::"<< __func__<< " null VertexList"<<std::endl;
	}
	//  PL_DBGOSH << "TriMesh::"<< __func__<< " VertexList ptr"<<this->m_vertex_list;
	//PL_DBGOSH << "TriMesh::"<< __func__<< "add tringle"<<std::endl;

	int n_tri=this->m_tri_list->size();

	DVertexTriangle* ret=new DVertexTriangle(vtx_list,n_tri);

	this->m_tri_list->push_back( (PrivateTriangle*) ret);

	return ret;
}


void TriMesh::finalize_DVertex()
{
	if(this->m_vertex_list!=NULL && this->m_tri_list!=NULL ){
		if(this->m_vertKDT!=NULL)      delete this->m_vertKDT;
		this->m_vertKDT = new VertKDT(m_max_elements);
		this->m_vertex_list->setKDT(this->m_vertKDT);

		// for(int i =0 ;i<this->m_vertex_list->size();++i){
		// 	 PL_DBGOSH << "TriMesh::"<<__func__<< " before vec "<<i<<" "<< *(this->m_vertex_list->get_vertex_lists()->at(i))<<std::endl;
		// }
		// for(int i =0 ;i<this->m_tri_list->size();++i){
		// 	 PL_DBGOSH << "TriMesh::"<<__func__<< " before tri_id "<<i<<" "<< this->m_tri_list->at(i)->get_id()<<std::endl;
		// }

		vtx_compaction();
		// for(int i =0 ;i<this->m_vertex_list->size();++i){
		// 	 PL_DBGOSH << "TriMesh::"<<__func__<< " after vec "<<i<<" "<< *(this->m_vertex_list->get_vertex_lists()->at(i))<<std::endl;
		// }
		// for(int i =0 ;i<this->m_tri_list->size();++i){
		// 	 PL_DBGOSH << "TriMesh::"<<__func__<< " after tri_id "<<i<<" "<< this->m_tri_list->at(i)->get_id()<<std::endl;
		// }


		build();
	} else {
#ifdef WIN32
		PL_DBGOSH<<__FUNCTION__<<" not performed!!!"<<std::endl;
#else
		PL_DBGOSH<<__func__<<" not performed!!!"<<std::endl;
#endif
	}
}




//=======================================================================
// Setter/Getter
//=======================================================================
///
/// TriMeshクラスが管理しているBoundingBoxを返す。
///
BBox TriMesh::get_bbox() const {
	return m_bbox;
}

///
/// KD木クラスを取得。
///
/// @return KD木クラス。
///
VertKDT *TriMesh::get_vertkdt() const {
	return m_vertKDT;
}

///
/// KD木クラスを取得。
///
/// @return KD木クラス。
///
VTree *TriMesh::get_vtree() const {
	return m_vtree;
}
///
/// DVertexManager
///
/// @return KD木クラス。
///
DVertexManager* TriMesh::DVM() const {
	return m_DVM_ptr;
}
///
/// hasDVertex
///
/// @return KD木クラス。
///
bool TriMesh::hasDVertex() const 
{
	if(m_DVM_ptr==NULL) return false;
	return true;
}

void TriMesh::print_memory_size() const 
{
	unsigned int memsize_vtree=m_vtree->memory_size();
	unsigned int memsize_vkdt=m_vertKDT->memory_size();
	unsigned int memsize_pt_list=
		(sizeof(PrivateTriangle)+sizeof(PrivateTriangle*))
		*(this->m_tri_list->size());
	unsigned int memsize_vt_list=(sizeof(Vertex)+sizeof(Vertex*))
		*(this->m_vertex_list->size());
	unsigned int memsize_all=
		memsize_vt_list+memsize_pt_list+memsize_vkdt+memsize_vtree;
#ifdef WIN32
	PL_DBGOSH<< "TriMesh::"<<__FUNCTION__<<std::endl;
#else
	PL_DBGOSH<< "TriMesh::"<<__func__<<std::endl;
#endif      
	PL_DBGOSH<< "size of VertexList      "<< memsize_vt_list<<std::endl;
	PL_DBGOSH<< "size of VertKDT         "<<memsize_vkdt<<std::endl;
	PL_DBGOSH<< "size of PrivateTriangle "<<memsize_pt_list<<std::endl;
	PL_DBGOSH<< "size of VTree           "<<memsize_vtree<<std::endl;
	PL_DBGOSH<< "---------------------------------------------"<<std::endl;
	PL_DBGOSH<< "total size                 "<<memsize_all<<std::endl;
}


} //namespace PolylibNS


