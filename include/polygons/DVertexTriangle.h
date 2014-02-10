// -*- Mode: c++-mode -*-
////////////////////////////////////////////////////////////////////////////
///
/// クラス:DVertexTriangleクラス
/// DVertexを頂点に持つTriagnleクラス
///
////////////////////////////////////////////////////////////////////////////

#ifndef polylib_dvertex_triangle_h
#define polylib_dvertex_triangle_h

#include "polygons/PrivateTriangle.h"

namespace PolylibNS{

class DVertex;

class DVertexTriangle : public  PrivateTriangle{
public:

	///
	/// コンストラクタ。
	///
	/// @param[in] vertex_ptr	ポリゴンの頂点(DVertex)へのポインタ。
	/// @param[in] id		三角形ポリゴンID。
	///
	DVertexTriangle(
		DVertex*	vertex_ptr[3] ,
		int		id
		);

	///
	/// コンストラクタ。
	///
	/// @param[in] vertex_ptr	ポリゴンの頂点へのポインタ。
	/// @param[in] normal	法線。
	/// @param[in] id		三角形ポリゴンID。
	///
	DVertexTriangle(
		DVertex*	vertex_ptr[3], 
		Vec3<REAL_TYPE>	normal, 
		int		id
		);
	///
	/// コンストラクタ。
	///
	/// @param[in] vertex_ptr	ポリゴンの頂点へのポインタ。
	/// @param[in] normal	法線。
	/// @param[in] area		ポリゴンの面積。
	/// @param[in] id		三角形ポリゴンID。
	///
	DVertexTriangle(
		DVertex*	vertex_ptr[3], 
		Vec3<REAL_TYPE>	normal, 
		REAL_TYPE	area, 
		int		id
		) ;

	///
	/// コンストラクタ。
	///
	/// @param[in] tri		ポリゴン。
	/// @param[in] id		三角形ポリゴンID。
	///
	DVertexTriangle(
		PrivateTriangle	tri, 
		int			id
		);

	///
	/// コンストラクタ。
	///
	/// @param[in] tri		ポリゴン。
	/// 
	///
	DVertexTriangle(
		const DVertexTriangle &tri 
		);

	///
	/// コンストラクタ。
	///
	/// @param[in] dim		ポリゴン頂点座標配列。
	/// @param[in] id		三角形ポリゴンID。
	///
	DVertexTriangle(
		REAL_TYPE		*dim,
		int			id
		);


	//  getter
	DVertex** get_DVertex() ;

private:
	DVertex* m_dvertex_ptr[3];

};

}// end of namespace
#endif //polylib_dvertex_triangle_h
