// -*- Mode: c++ -*-
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

#ifndef polylib_triangle_h
#define polylib_triangle_h

#include "common/Vec3.h"
#include "polygons/Vertex.h"

namespace PolylibNS{

////////////////////////////////////////////////////////////////////////////
///
/// クラス:Triangle
/// 入出力用インターフェースクラスであり、本ヘッダに対応する.cxxファイルは存在
/// しない。
///
////////////////////////////////////////////////////////////////////////////

class Triangle {
public:
	///
	/// コンストラクタ。
	///
	Triangle();

	///
	/// コンストラクタ。
	///
	/// @param[in] tria コピー元。
	///
	Triangle(
		const Triangle  &tria
	) ;

	///
	/// コンストラクタ。
	///
	/// @param[in] vertex_ptr ポリゴンの頂点。
	/// @attention 面積と法線はvertexを元に自動計算される。
	///
	Triangle(
		//Vec3	vertex[3]
		Vertex* vertex_ptr[3]
	) ;

	///
	/// コンストラクタ。
	///
	/// @param[in] vertex_ptr	ポリゴンの頂点。
	/// @param[in] normal	法線。
	/// @attention 面積はvertexを元に自動計算される。
	///
	Triangle(
		//Vec3	vertex[3], 
		Vertex* vertex_ptr[3],
		Vec3<REAL_TYPE>	normal
		) ;

	///
	/// コンストラクタ。
	///
	/// @param[in] vertex_ptr	ポリゴンの頂点。
	/// @param[in] normal	法線。
	/// @param[in] area		ポリゴンの面積。
	///
	Triangle(
		//Vec3	vertex[3], 

		Vertex* vertex_ptr[3],
		Vec3<REAL_TYPE>	normal, 
		REAL_TYPE	area
		) ;

	//=======================================================================
	// Setter/Getter
	//=======================================================================
	///
	/// 頂点を設定。
	///
	/// @param[in] vertex_ptr		三角形の3頂点。
	/// @param[in] calc_normal	法線ベクトルを再計算するか？
	/// @param[in] calc_area	面積を再計算するか？
	///
	virtual void set_vertexes(
		//Vec3	vertex[3], 
		Vertex* vertex_ptr[3],
		bool	calc_normal, 
		bool	calc_area
		) ;

	///
	/// vertexの配列を取得。
	///
	/// @return vertexの配列。
	///
	// Vec3* get_vertex() const {
	//  return const_cast<Vec3*>(m_vertex);
	//}

	//  Vec3* get_vertex() const {
	//   return const_cast<Vec3*>( *m_vertex_ptr );
	// }


	virtual Vertex** get_vertex() const ;

	///
	/// 法線ベクトルを取得。
	///
	/// @return 法線ベクトル。
	///
	virtual Vec3<REAL_TYPE> get_normal() const ;
	///
	/// 面積を取得。
	///
	/// @return 面積。
	///
	virtual REAL_TYPE get_area() const ;
	///
	/// ユーザ定義IDを設定。
	///
	///
	virtual void set_exid( int id ) ;
	///
	/// ユーザ定義IDを取得。
	///
	/// @return ユーザ定義ID。
	///
	virtual int get_exid() const ;

	///
	/// ユーザ定義状態変数を設定。
	///
	///
	virtual void set_shell( int val ) ;
	///
	/// ユーザ定義状態変数を取得。
	///
	/// @return ユーザ定義状態変数。
	///
	virtual int get_shell() const;

protected:
	///
	/// 法線ベクトル算出。
	///
	virtual void calc_normal() ;
	///
	/// 面積算出。
	///
	virtual void calc_area();

	//=======================================================================
	// クラス変数
	//=======================================================================
	/// 三角形の頂点座標（反時計回りで並んでいる）。
	//	Vec3<REAL_TYPE>	m_vertex[3];

	/// changed with Vertex and VertexList class since Polylib version 3.0
	/// 三角形の頂点座標（反時計回りで並んでいる）。
	Vertex* m_vertex_ptr[3];

	/// 三角形の法線ベクトル。
	Vec3<REAL_TYPE>	m_normal;

	/// 三角形の面積。
	REAL_TYPE	m_area;

	/// 三角形のユーザ定義ID
	int     m_exid;

	/// 三角形のユーザ定義状態変数
	int		m_shell;
};


} //namespace PolylibNS

#endif  // polylib_triangle_h

