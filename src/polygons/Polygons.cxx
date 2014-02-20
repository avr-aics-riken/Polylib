/*
* Polylib - Polygon Management Library
*
* Copyright (c) 2010-2011 VCAD System Research Program, RIKEN.
* All rights reserved.
*
* Copyright (c) 2012-2014 Advanced Institute for Computational Science, RIKEN.
* All rights reserved.
*
*/

#include "polygons/Polygons.h"

#include "polygons/VTree.h"
#include "polygons/Vertex.h"
#include "polygons/DVertex.h"
#include "polygons/VertexList.h"
#include "polygons/Triangle.h"
#include "polygons/PrivateTriangle.h"
#include "polygons/DVertexTriangle.h"

#include "common/tt.h"
#include "common/axis.h"

#include "common/BBox.h"


namespace PolylibNS {

///
/// コンストラクタ。
///
Polygons::Polygons()
{

}

///
/// デストラクタ。
///
Polygons::~Polygons()
{

}

/// Vertex -> DVertex  へのリプレース
///
/// @param[in] nscalar スカラーデータ数
/// @param[in] nvector ベクトルデータ数

POLYLIB_STAT Polygons::replace_DVertex(int nscalar,int nvector){
	return PLSTAT_OK;
};

/// Vertex -> DVertex  への準備
///
/// @param[in] nscalar スカラーデータ数
/// @param[in] nvector ベクトルデータ数

POLYLIB_STAT Polygons::prepare_DVertex(int nscalar,int nvector){
	return PLSTAT_OK;
};

//
/// DVertex 追加作成用
/// 
/// @param[in] v 頂点座標（３点）
///  @return    polygonへのpointer
///

DVertexTriangle* Polygons::add_DVertex_Triangle(Vec3<REAL_TYPE>* v){


#ifdef DEBUG
	PL_DBGOSH << "Polygons::"<< __func__
		<<" v0 "<<v[0]
	<<" v1 "<<v[1]
	<<" v2 "<<v[2]
	<< std::endl;
#endif

	return NULL;
}

//
/// DVertex 追加作成後の重複頂点削除
/// 
///

void Polygons::finalize_DVertex(){
	//do nothing?
}





//=======================================================================
// Setter/Getter
//=======================================================================
///
/// 三角形ポリゴンのリストを取得。
///
/// @return 三角形ポリゴンのリスト。
///
std::vector<PrivateTriangle*>* Polygons::get_tri_list() const {
	return m_tri_list;
}

/// VertexListを取得。
///
/// @return VertexList 頂点リストクラス
///
VertexList* Polygons::get_vtx_list() const {
	return m_vertex_list;
}




/// print_vertex
/// test function for Vertex Class
///
void Polygons::print_vertex(){

	//  std::cout << __func__ << " begin"<<std::endl;
	/// codes for VertexList
	const std::vector<Vertex*>* vl_ptr=m_vertex_list->get_vertex_lists();

	for(int i=0;i<vl_ptr->size();++i){
		//	  std::cout << __func__ << " begin"<<std::endl;
		Vertex test=*((*vl_ptr)[i]);
		//	  std::cout << __func__ << " begin"<<std::endl;
		PL_DBGOS << __func__ <<" VertexList "<<i 
			<< " "<<test[AXIS_X] 
		<< " "<<test[AXIS_Y] 
		<< " "<<test[AXIS_Z] <<std::endl;;
	}

}





}// end of namespace
