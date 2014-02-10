// -*- Mode: c++ -*-
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

#ifndef polylib_privatetriangle_h
#define polylib_privatetriangle_h

#include "common/Vec3.h"
#include "polygons/Vertex.h"
#include "polygons/Triangle.h"

namespace PolylibNS{


////////////////////////////////////////////////////////////////////////////
///
/// クラス:PrivateTriangleクラス
/// Polylib内のデータ保存用の基本クラスです。
///
////////////////////////////////////////////////////////////////////////////


class PrivateTriangle : public Triangle {
public:

	///
	/// コンストラクタ。
	///
	PrivateTriangle();

	///
	/// コンストラクタ。
	///
	/// @param[in] vertex_ptr	ポリゴンの頂点へのポインタ。
	/// @param[in] id		三角形ポリゴンID。
	/// @param[in] exid		三角形ポリゴンのユーザ定義ID。
	///
	PrivateTriangle(
		//Vec3<REAL_TYPE>	vertex[3], 
		Vertex*	vertex_ptr[3] ,
		int		id,
		int		exid
		);
	///
	/// コンストラクタ。
	///
	/// @param[in] vertex_ptr	ポリゴンの頂点へのポインタ。
	/// @param[in] normal	法線。
	/// @param[in] id		三角形ポリゴンID。
	///
	PrivateTriangle(
		//Vec3<REAL_TYPE>	vertex[3], 
		Vertex*	vertex_ptr[3], 
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
	PrivateTriangle(
		//Vec3<REAL_TYPE>	vertex[3], 
		Vertex*	vertex_ptr[3], 
		Vec3<REAL_TYPE>	normal, 
		REAL_TYPE	area, 
		int		id
		);

	///
	/// コンストラクタ。
	///
	/// @param[in] tri		ポリゴン。
	/// @param[in] id		三角形ポリゴンID。
	///
	PrivateTriangle(
		const Triangle	tri, 
		int			id
		);
	///
	/// コンストラクタ。
	///
	/// @param[in] tri		ポリゴン。
	/// 
	///
	PrivateTriangle(
		const PrivateTriangle &tri 
		);

	///
	/// コンストラクタ。
	///
	/// @param[in] dim		ポリゴン頂点座標配列。
	/// @param[in] id		三角形ポリゴンID。
	///
	PrivateTriangle(
		REAL_TYPE		*dim,
		int			id
		);

	///
	/// コンストラクタ。
	///
	/// @param[in] dim    ポリゴン頂点座標配列。
	/// @param[in] id    三角形ポリゴンID。
	/// @param[in] exid    三角形ポリゴンのユーザ定義ID。
	///
	PrivateTriangle(
		REAL_TYPE	*dim,
		int		id,
		int		exid
		);


	//=======================================================================
	// Setter/Getter
	//=======================================================================
	///
	/// 三角形ポリゴンIDを設定。
	///
	///  @param[in] id	三角形ポリゴンID。
	///
	virtual void set_id(int id);

	///
	/// 三角形ポリゴンIDを返す。
	///
	///  @return 三角形ポリゴンID。
	///
	virtual int get_id() const;

protected:
	//=======================================================================
	// クラス変数
	//=======================================================================
	/// PolygonGroup内で一意となる三角形ポリゴンID。
	///
	int m_id;
};




} //namespace PolylibNS

#endif  // polylib_privatetriangle_h

