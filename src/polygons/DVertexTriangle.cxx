// -*- Mode: c++-mode -*-
////////////////////////////////////////////////////////////////////////////
///
/// クラス:DVertexTriangleクラス
/// DVertexを頂点に持つTriagnleクラス
///
////////////////////////////////////////////////////////////////////////////

#include "polygons/DVertexTriangle.h"
#include "polygons/DVertex.h"


namespace PolylibNS{



///
/// コンストラクタ。
///
/// @param[in] vertex_ptr	ポリゴンの頂点(DVertex)へのポインタ。
/// @param[in] id		三角形ポリゴンID。
///
DVertexTriangle::DVertexTriangle(
	DVertex*	vertex_ptr[3] ,
	int		id
	) : PrivateTriangle( (Vertex**)vertex_ptr,id) {
		//			) : PrivateTriangle( vertex_ptr,id) {
		m_dvertex_ptr[0]=vertex_ptr[0];
		m_dvertex_ptr[1]=vertex_ptr[1];
		m_dvertex_ptr[2]=vertex_ptr[2];
}

///
/// コンストラクタ。
///
/// @param[in] vertex_ptr	ポリゴンの頂点へのポインタ。
/// @param[in] normal	法線。
/// @param[in] id		三角形ポリゴンID。
///
DVertexTriangle::DVertexTriangle(
	DVertex*	vertex_ptr[3], 
	Vec3<PL_REAL>	normal, 
	int		id
	) : PrivateTriangle((Vertex**)vertex_ptr, normal,id) {

		for(int i=0;i<3;i++){
			m_dvertex_ptr[i]=vertex_ptr[i];
		}

}

///
/// コンストラクタ。
///
/// @param[in] vertex_ptr	ポリゴンの頂点へのポインタ。
/// @param[in] normal	法線。
/// @param[in] area		ポリゴンの面積。
/// @param[in] id		三角形ポリゴンID。
///
DVertexTriangle::DVertexTriangle(
	DVertex*	vertex_ptr[3], 
	Vec3<PL_REAL>	normal, 
	PL_REAL	area, 
	int		id

	) : PrivateTriangle((Vertex**)vertex_ptr, normal, area,id) {

		for(int i=0;i<3;i++){
			m_dvertex_ptr[i]=vertex_ptr[i];
		}
}

///
/// コンストラクタ。
///
/// @param[in] tri		ポリゴン。
/// @param[in] id		三角形ポリゴンID。
///
DVertexTriangle::DVertexTriangle(
	PrivateTriangle	tri, 
	int			id
	) : PrivateTriangle(tri.get_vertex(), tri.get_normal(),id) {

}

///
/// コンストラクタ。
///
/// @param[in] tri		ポリゴン。
/// 
///
DVertexTriangle::DVertexTriangle(
	const DVertexTriangle &tri 
	) : PrivateTriangle(tri.get_vertex(), tri.get_normal(),tri.get_id()) {

}

///
/// コンストラクタ。
///
/// @param[in] dim		ポリゴン頂点座標配列。
/// @param[in] id		三角形ポリゴンID。
///
DVertexTriangle::DVertexTriangle(
	PL_REAL		*dim,
	int			id
	){
		for( int i=0; i<3; i++ ) {
			this->m_vertex_ptr[i]->t[0] = *dim++;
			this->m_vertex_ptr[i]->t[1] = *dim++;
			this->m_vertex_ptr[i]->t[2] = *dim++;
		}
		this->m_id = id;
		this->calc_normal();
		this->calc_area();

		for( int i=0; i<3; i++ ) {
			m_dvertex_ptr[i]=dynamic_cast<DVertex*>(m_vertex_ptr[i]);
		}
}


//  getter
DVertex** DVertexTriangle::get_DVertex() {

	//	  std::cout <<__func__<<" "<<m_dvertex_ptr <<std::endl;
	m_dvertex_ptr[0]=dynamic_cast<DVertex*>(this->m_vertex_ptr[0]);
	m_dvertex_ptr[1]=dynamic_cast<DVertex*>(this->m_vertex_ptr[1]);
	m_dvertex_ptr[2]=dynamic_cast<DVertex*>(this->m_vertex_ptr[2]);
	return m_dvertex_ptr;
}


}// end of namespace

