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
template <typename T>
class Triangle {
public:
	///
	/// コンストラクタ。
	///
	Triangle(){};

	///
      	/// コンストラクタ。
	///
	/// @param[in] vertex_ptr ポリゴンの頂点。
	/// @attention 面積と法線はvertexを元に自動計算される。
	///
	Triangle(
		 //Vec3<T>	vertex[3]
		 Vertex<T>* vertex_ptr[3]
	) {
		m_vertex_ptr[0] = vertex_ptr[0];
		m_vertex_ptr[1] = vertex_ptr[1];
		m_vertex_ptr[2] = vertex_ptr[2];
		calc_normal();
		calc_area();
	}

	///
        /// コンストラクタ。
	///
	/// @param[in] vertex_ptr	ポリゴンの頂点。
	/// @param[in] normal	法線。
	/// @attention 面積はvertexを元に自動計算される。
	///
	Triangle(
		 //Vec3<T>	vertex[3], 
		 Vertex<T>* vertex_ptr[3],
		Vec3<T>	normal
	) {
		m_vertex_ptr[0] = vertex_ptr[0];
		m_vertex_ptr[1] = vertex_ptr[1];
		m_vertex_ptr[2] = vertex_ptr[2];
		m_normal = normal;
		calc_area();
	}

	///
	/// コンストラクタ。
	///
	/// @param[in] vertex_ptr	ポリゴンの頂点。
	/// @param[in] normal	法線。
	/// @param[in] area		ポリゴンの面積。
	///
	Triangle(
		 //Vec3<T>	vertex[3], 

		 Vertex<T>* vertex_ptr[3],
		 Vec3<T>	normal, 
		 T	area
	) {
		m_vertex_ptr[0] = vertex_ptr[0];
		m_vertex_ptr[1] = vertex_ptr[1];
		m_vertex_ptr[2] = vertex_ptr[2];
		m_normal = normal;
		m_area = area;
	}

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
			  //Vec3<T>	vertex[3], 
		 Vertex<T>* vertex_ptr[3],
		bool	calc_normal, 
		bool	calc_area
	) {
		m_vertex_ptr[0] = vertex_ptr[0];
		m_vertex_ptr[1] = vertex_ptr[1];
		m_vertex_ptr[2] = vertex_ptr[2];
		if(calc_normal) this->calc_normal();
		if(calc_area) this->calc_area();
	}

	///
	/// vertexの配列を取得。
	///
	/// @return vertexの配列。
	///
	// Vec3<T>* get_vertex() const {
	//  return const_cast<Vec3<T>*>(m_vertex);
	//}

	//  Vec3<T>* get_vertex() const {
	//   return const_cast<Vec3<T>*>( *m_vertex_ptr );
	// }


  virtual Vertex<T>** get_vertex() const {
	  //	return const_cast<Vec3<T>*>(m_vertex_ptr);
	  return const_cast<Vertex<T>**>( m_vertex_ptr);
	}

	///
	/// 法線ベクトルを取得。
	///
	/// @return 法線ベクトル。
	///
  virtual Vec3<T> get_normal() const {
		return m_normal;
	}

	///
	/// 面積を取得。
	///
	/// @return 面積。
	///
  virtual T get_area() const {
	  // std::cout << __func__<<" " <<m_area<<std::endl;
		return m_area;
	}

	///
	/// ユーザ定義IDを設定。
	///
	///
  virtual void set_exid( int id ) {
		m_exid = id;
	}

	///
	/// ユーザ定義IDを取得。
	///
	/// @return ユーザ定義ID。
	///
  virtual int get_exid() const {
		return m_exid;
	}

	///
	/// ユーザ定義状態変数を設定。
	///
	///
  virtual void set_shell( int val ) {
		m_shell = val;
	}

	///
	/// ユーザ定義状態変数を取得。
	///
	/// @return ユーザ定義状態変数。
	///
  virtual int get_shell() const {
		return m_shell;
	}

protected:
        ///
	/// 法線ベクトル算出。
	///
  virtual void calc_normal() {
		// double演算に変更 2013.10.10 tkawanab
		Vec3<double> vd[3];
		vd[0].assign( m_vertex_ptr[0]->t[0], m_vertex_ptr[0]->t[1], m_vertex_ptr[0]->t[2] );
		vd[1].assign( m_vertex_ptr[1]->t[0], m_vertex_ptr[1]->t[1], m_vertex_ptr[1]->t[2] );
		vd[2].assign( m_vertex_ptr[2]->t[0], m_vertex_ptr[2]->t[1], m_vertex_ptr[2]->t[2] );
		Vec3<double> ad = vd[1] - vd[0];
		Vec3<double> bd = vd[2] - vd[0];

		Vec3<double> normald = (cross(ad,bd)).normalize();
		m_normal[0] = normald[0];
		m_normal[1] = normald[1];
		m_normal[2] = normald[2];
	}

	///
	/// 面積算出。
	///
  virtual void calc_area() {
	  Vec3<T> a = *(m_vertex_ptr[1]) - *(m_vertex_ptr[0]);
	  Vec3<T> b = *(m_vertex_ptr[2]) - *(m_vertex_ptr[0]);
		T al = a.length();
		T bl = b.length();
		T ab = dot(a,b);
		T f = al*al*bl*bl - ab*ab;
		if(f<0.0) f=0.0;
		//m_area = 0.5*sqrtf(f);
		m_area = 0.5*sqrt(f);
		// std::cout << "a("<<a<<") b("<<b<<")"<<std::endl;
		// std::cout << __func__ <<" "<<m_area <<" "<< f << " " << al << " "<< bl << " "<< ab <<std::endl;
	}

	//=======================================================================
	// クラス変数
	//=======================================================================
	/// 三角形の頂点座標（反時計回りで並んでいる）。
	//	Vec3<T>	m_vertex[3];

	/// changed with Vertex and VertexList class since Polylib version 3.0
	/// 三角形の頂点座標（反時計回りで並んでいる）。
	Vertex<T>* m_vertex_ptr[3];

	/// 三角形の法線ベクトル。
	Vec3<T>	m_normal;

	/// 三角形の面積。
	T	m_area;

	/// 三角形のユーザ定義ID
	int     m_exid;

	/// 三角形のユーザ定義状態変数
	int		m_shell;
};

////////////////////////////////////////////////////////////////////////////
///
/// クラス:PrivateTriangleクラス
/// Polylib内のデータ保存用の基本クラスです。
///
////////////////////////////////////////////////////////////////////////////

template <typename T>
class PrivateTriangle : public Triangle<T> {
public:
	///
	/// コンストラクタ。
	///
	/// @param[in] vertex_ptr	ポリゴンの頂点へのポインタ。
	/// @param[in] id		三角形ポリゴンID。
	///
	PrivateTriangle(
			//Vec3<T>	vertex[3], 
			Vertex<T>*	vertex_ptr[3] ,
		int		id
	) : Triangle<T>(vertex_ptr) {
		m_id = id;
	}

	///
	/// コンストラクタ。
	///
	/// @param[in] vertex_ptr	ポリゴンの頂点へのポインタ。
	/// @param[in] normal	法線。
	/// @param[in] id		三角形ポリゴンID。
	///
	PrivateTriangle(
			//Vec3<T>	vertex[3], 
		Vertex<T>*	vertex_ptr[3], 
		Vec3<T>	normal, 
		int		id
	) : Triangle<T>(vertex_ptr, normal) {
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
	PrivateTriangle(
			//Vec3<T>	vertex[3], 
		Vertex<T>*	vertex_ptr[3], 
		Vec3<T>	normal, 
		T	area, 
		int		id
	) : Triangle<T>(vertex_ptr, normal, area) {
		m_id = id;
	}

	///
	/// コンストラクタ。
	///
	/// @param[in] tri		ポリゴン。
	/// @param[in] id		三角形ポリゴンID。
	///
	PrivateTriangle(
		Triangle<T>	tri, 
		int			id
	) : Triangle<T>(tri.get_vertex(), tri.get_normal()) {
		m_id = id;
	}

	///
	/// コンストラクタ。
	///
	/// @param[in] tri		ポリゴン。
        /// 
	///
	PrivateTriangle(
		const PrivateTriangle<T> &tri 
	) : Triangle<T>(tri.get_vertex(), tri.get_normal()) {
		m_id = tri.m_id;
	}

	///
	/// コンストラクタ。
	///
	/// @param[in] dim		ポリゴン頂点座標配列。
	/// @param[in] id		三角形ポリゴンID。
	///
	PrivateTriangle(
		T		*dim,
		int			id
	){
		for( int i=0; i<3; i++ ) {
		  this->m_vertex_ptr[i]->t[0] = *dim++;
		  this->m_vertex_ptr[i]->t[1] = *dim++;
		  this->m_vertex_ptr[i]->t[2] = *dim++;
		}
		m_id = id;
		this->calc_normal();
		this->calc_area();
	}

	//=======================================================================
	// Setter/Getter
	//=======================================================================
	///
	/// 三角形ポリゴンIDを設定。
	///
	///  @param[in] id	三角形ポリゴンID。
	///
  virtual void set_id(int id)				{m_id = id;}

	///
	/// 三角形ポリゴンIDを返す。
	///
	///  @return 三角形ポリゴンID。
	///
  virtual int get_id() const				{return m_id;}




 
protected:
	//=======================================================================
	// クラス変数
	//=======================================================================
	/// PolygonGroup内で一意となる三角形ポリゴンID。
	///
	int m_id;
};




} //namespace PolylibNS

#endif  // polylib_triangle_h

