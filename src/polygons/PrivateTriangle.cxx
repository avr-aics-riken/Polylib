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
#include "polygons/PrivateTriangle.h"
#include "common/PolylibCommon.h"

namespace PolylibNS{


	///
	/// コンストラクタ。
	///
	PrivateTriangle::PrivateTriangle(){}

	///
	/// コンストラクタ。
	///
	/// @param[in] vertex_ptr	ポリゴンの頂点へのポインタ。
	/// @param[in] id		三角形ポリゴンID。
	///
	PrivateTriangle::PrivateTriangle(
		//Vec3<REAL_TYPE>	vertex[3], 
		Vertex*	vertex_ptr[3] ,
		int		id
		) : Triangle(vertex_ptr) {
			m_id = id;
	}

	///
	/// コンストラクタ。
	///
	/// @param[in] vertex_ptr	ポリゴンの頂点へのポインタ。
	/// @param[in] normal	法線。
	/// @param[in] id		三角形ポリゴンID。
	///
	PrivateTriangle::PrivateTriangle(
		//Vec3<REAL_TYPE>	vertex[3], 
		Vertex*	vertex_ptr[3], 
		Vec3<REAL_TYPE>	normal, 
		int		id
		) : Triangle(vertex_ptr, normal) {
			m_id = id;
	}

	///
	/// コンストラクタ。
	///
	/// @param[in] vertex_ptr	ポリゴンの頂点へのポインタ。
	/// @param[in] normal	法線。
	/// @param[in] area		ポリゴンの面積。
	/// @param[in] id		三角形ポリゴンID。
	///
	PrivateTriangle::PrivateTriangle(
		//Vec3<REAL_TYPE>	vertex[3], 
		Vertex*	vertex_ptr[3], 
		Vec3<REAL_TYPE>	normal, 
		REAL_TYPE	area, 
		int		id
		) : Triangle(vertex_ptr, normal, area) {
			m_id = id;
	}

	///
	/// コンストラクタ。
	///
	/// @param[in] tri		ポリゴン。
	/// @param[in] id		三角形ポリゴンID。
	///
	PrivateTriangle::PrivateTriangle(
		Triangle	tri, 
		int			id
		) : Triangle(tri.get_vertex(), tri.get_normal()) {
			m_id = id;
	}

	///
	/// コンストラクタ。
	///
	/// @param[in] tri		ポリゴン。
	/// 
	///
	PrivateTriangle::PrivateTriangle(
		const PrivateTriangle &tri 
		) : Triangle(tri.get_vertex(), tri.get_normal()) {
			m_id = tri.m_id;
	}

	///
	/// コンストラクタ。
	///
	/// @param[in] dim		ポリゴン頂点座標配列。
	/// @param[in] id		三角形ポリゴンID。
	///
	PrivateTriangle::PrivateTriangle(
		REAL_TYPE		*dim,
		int			id
		){
			PL_ERROS << "PrivateTriangle(T,int) is obsolete. !!!NOTHING IS DONE!!!" << std::endl;
	}


	// 	PrivateTriangle(
	// 		T		*dim,
	// 		int			id
	// 	){
	// #define DEBUG
	// #ifdef DEBUG
	// 	  PL_DBGOSH << "PrivateTriangle started." << std::endl;
	// #endif
	// 		for( int i=0; i<3; i++ ) {
	// 		  PL_DBGOSH << "PrivateTriangle  0 i ."<<i << std::endl;
	// 		  this->m_vertex_ptr[i]->t[0] = *dim++;
	// 		  PL_DBGOSH << "PrivateTriangle  1 i ."<<i << std::endl;
	// 		  this->m_vertex_ptr[i]->t[1] = *dim++;
	// 		  PL_DBGOSH << "PrivateTriangle  2 i ."<<i << std::endl;
	// 		  this->m_vertex_ptr[i]->t[2] = *dim++;
	// 		  PL_DBGOSH << "PrivateTriangle  3 i ."<<i << std::endl;
	// 		}
	// 		m_id = id;
	// 		this->calc_normal();
	// 		this->calc_area();

	// #ifdef DEBUG
	// 	  PL_DBGOSH << "PrivateTriangle called." << std::endl;
	// #endif
	// #undef DEBUG
	// 	}

	//=======================================================================
	// Setter/Getter
	//=======================================================================
	///
	/// 三角形ポリゴンIDを設定。
	///
	///  @param[in] id	三角形ポリゴンID。
	///
	void PrivateTriangle::set_id(int id)				
	{
		m_id = id;
	}

	///
	/// 三角形ポリゴンIDを返す。
	///
	///  @return 三角形ポリゴンID。
	///
	int PrivateTriangle::get_id() const				
	{
		return m_id;
	}


} //namespace PolylibNS

