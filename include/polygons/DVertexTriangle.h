// -*- Mode: c++-mode -*-
////////////////////////////////////////////////////////////////////////////
///
/// クラス:DVertexTriangleクラス
/// DVertexを頂点に持つTriagnleクラス
///
////////////////////////////////////////////////////////////////////////////

#ifndef polylib_dvertex_triangle_h
#define polylib_dvertex_triangle_h

#include "polygons/Triangle.h"
#include "polygons/DVertex.h"
namespace PolylibNS{

template <typename T>
class DVertexTriangle : public  PrivateTriangle<T>{
public:

	///
	/// コンストラクタ。
	///
	/// @param[in] vertex_ptr	ポリゴンの頂点(DVertex)へのポインタ。
	/// @param[in] id		三角形ポリゴンID。
	///
	DVertexTriangle(
			DVertex<T>*	vertex_ptr[3] ,
			int		id
			) : PrivateTriangle<T>( (Vertex<T>**)vertex_ptr,id) {
			//			) : PrivateTriangle<T>( vertex_ptr,id) {
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
	DVertexTriangle(
			DVertex<T>*	vertex_ptr[3], 
			Vec3<T>	normal, 
			int		id
			) : PrivateTriangle<T>((Vertex<T>**)vertex_ptr, normal,id) {
	  m_dvertex_ptr=vertex_ptr;
	}

	///
	/// コンストラクタ。
	///
	/// @param[in] vertex_ptr	ポリゴンの頂点へのポインタ。
	/// @param[in] normal	法線。
	/// @param[in] area		ポリゴンの面積。
	/// @param[in] id		三角形ポリゴンID。
	///
	DVertexTriangle(
		DVertex<T>*	vertex_ptr[3], 
		Vec3<T>	normal, 
		T	area, 
		int		id
			) : Triangle<T>((Vertex<T>**)vertex_ptr, normal, area.id) {
	  m_dvertex_ptr=vertex_ptr;
	}

	///
	/// コンストラクタ。
	///
	/// @param[in] tri		ポリゴン。
	/// @param[in] id		三角形ポリゴンID。
	///
	DVertexTriangle(
		PrivateTriangle<T>	tri, 
		int			id
			) : PrivateTriangle<T>(tri.get_vertex(), tri.get_normal(),id) {
	  
	}

	///
	/// コンストラクタ。
	///
	/// @param[in] tri		ポリゴン。
        /// 
	///
	DVertexTriangle(
		const DVertexTriangle<T> &tri 
			) : PrivateTriangle<T>(tri.get_vertex(), tri.get_normal()) {

	}

	///
	/// コンストラクタ。
	///
	/// @param[in] dim		ポリゴン頂点座標配列。
	/// @param[in] id		三角形ポリゴンID。
	///
	DVertexTriangle(
		T		*dim,
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
		m_dvertex_ptr=dynamic_cast<DVertex<T>**>(this->m_vertex_ptr);
	}


  //  getter
	DVertex<T>** get_DVertex() {
	  
	  //	  std::cout <<__func__<<" "<<m_dvertex_ptr <<std::endl;
	  m_dvertex_ptr[0]=dynamic_cast<DVertex<T>*>(this->m_vertex_ptr[0]);
	  m_dvertex_ptr[1]=dynamic_cast<DVertex<T>*>(this->m_vertex_ptr[1]);
	  m_dvertex_ptr[2]=dynamic_cast<DVertex<T>*>(this->m_vertex_ptr[2]);
	  return m_dvertex_ptr;
	}

private:
  DVertex<T>* m_dvertex_ptr[3];

};

}// end of namespace
#endif //polylib_dvertex_triangle_h
