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
#include "polygons/Triangle.h"
#include "common/PolylibCommon.h"

namespace PolylibNS{

////////////////////////////////////////////////////////////////////////////
///
/// クラス:Triangle
/// 入出力用インターフェースクラスであり、本ヘッダに対応する.cxxファイルは存在
/// しない。
///
////////////////////////////////////////////////////////////////////////////

Triangle::Triangle(){}


///
/// コンストラクタ。
///
/// @param[in] tria コピー元。
///
Triangle::Triangle(
	const Triangle  &tria
) {
	m_vertex_ptr[0] = tria.m_vertex_ptr[0];
	m_vertex_ptr[1] = tria.m_vertex_ptr[1];
	m_vertex_ptr[2] = tria.m_vertex_ptr[2];
	m_normal = tria.m_normal;
	m_area = tria.m_area;
	m_exid = tria.m_exid;
	m_shell = tria.m_shell;
}

///
/// コンストラクタ。
///
/// @param[in] vertex_ptr ポリゴンの頂点。
/// @attention 面積と法線はvertexを元に自動計算される。
///
Triangle::Triangle(
	//Vec3	vertex[3]
	Vertex* vertex_ptr[3]
) {

	if(vertex_ptr[0]!=NULL && 
		vertex_ptr[1]!=NULL && 
		vertex_ptr[2]!=NULL ){

			m_vertex_ptr[0] = vertex_ptr[0];
			m_vertex_ptr[1] = vertex_ptr[1];
			m_vertex_ptr[2] = vertex_ptr[2];

	} else {
		PL_ERROS << __func__<<" pointer is null p0="<< vertex_ptr[0]
		<<" p1="<< vertex_ptr[1]
		<<" p2="<< vertex_ptr[2]<<std::endl;

	}
	calc_normal();
	calc_area();
	m_exid = 0;
	m_shell = 0;
}

///
/// コンストラクタ。
///
/// @param[in] vertex_ptr	ポリゴンの頂点。
/// @param[in] normal	法線。
/// @attention 面積はvertexを元に自動計算される。
///
Triangle::Triangle(
	//Vec3	vertex[3], 
	Vertex* vertex_ptr[3],
	Vec3<PL_REAL>	normal
	) {
		m_vertex_ptr[0] = vertex_ptr[0];
		m_vertex_ptr[1] = vertex_ptr[1];
		m_vertex_ptr[2] = vertex_ptr[2];
		m_normal = normal;
		calc_area();
		m_exid = 0;
		m_shell = 0;
}

///
/// コンストラクタ。
///
/// @param[in] vertex_ptr	ポリゴンの頂点。
/// @param[in] normal	法線。
/// @param[in] area		ポリゴンの面積。
///
Triangle::Triangle(
	//Vec3	vertex[3], 

	Vertex* vertex_ptr[3],
	Vec3<PL_REAL>	normal, 
	PL_REAL	area
	) {
		m_vertex_ptr[0] = vertex_ptr[0];
		m_vertex_ptr[1] = vertex_ptr[1];
		m_vertex_ptr[2] = vertex_ptr[2];
		m_normal = normal;
		m_area = area;
		m_exid = 0;
		m_shell = 0;
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
void Triangle::set_vertexes(
	//Vec3	vertex[3], 
	Vertex* vertex_ptr[3],
	bool	calc_normal_bool, 
	bool	calc_area_bool
	) {
		m_vertex_ptr[0] = vertex_ptr[0];
		m_vertex_ptr[1] = vertex_ptr[1];
		m_vertex_ptr[2] = vertex_ptr[2];
		if(calc_normal_bool) this->calc_normal();
		if(calc_area_bool) this->calc_area();
}

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


Vertex** Triangle::get_vertex() const {
	//	return const_cast<Vec3<PL_REAL>*>(m_vertex_ptr);
	return const_cast<Vertex**>( m_vertex_ptr);
}

///
/// 法線ベクトルを取得。
///
/// @return 法線ベクトル。
///
Vec3<PL_REAL> Triangle::get_normal() const {
	return m_normal;
}

///
/// 面積を取得。
///
/// @return 面積。
///
PL_REAL Triangle::get_area() const {
	// std::cout << __func__<<" " <<m_area<<std::endl;
	return m_area;
}

///
/// ユーザ定義IDを設定。
///
///
void Triangle::set_exid( int id ) {
	m_exid = id;
}

///
/// ユーザ定義IDを取得。
///
/// @return ユーザ定義ID。
///
int Triangle::get_exid() const {
	return m_exid;
}

///
/// ユーザ定義状態変数を設定。
///
///
void Triangle::set_shell( int val ) {
	m_shell = val;
}

///
/// ユーザ定義状態変数を取得。
///
/// @return ユーザ定義状態変数。
///
int Triangle::get_shell() const {
	return m_shell;
}


///
/// 法線ベクトル算出。
///
void Triangle::calc_normal() {

	// double演算に変更 2013.10.10 tkawanab

	Vec3<double> vd[3];
	vd[0].assign( m_vertex_ptr[0]->x, m_vertex_ptr[0]->y, m_vertex_ptr[0]->z );
	vd[1].assign( m_vertex_ptr[1]->x, m_vertex_ptr[1]->y, m_vertex_ptr[1]->z );
	vd[2].assign( m_vertex_ptr[2]->x, m_vertex_ptr[2]->y, m_vertex_ptr[2]->z );

	Vec3<double> ad = vd[1] - vd[0];
	Vec3<double> bd = vd[2] - vd[0];
	Vec3<double> normald = (cross(ad,bd)).normalize();
	m_normal[0] = normald[0];
	m_normal[1] = normald[1];
	m_normal[2] = normald[2];
}

///
/// 面積算出。 double に変更 2015-02-21 keno
///
void Triangle::calc_area() {
  Vec3<double> vd[3];
  vd[0].assign( m_vertex_ptr[0]->x, m_vertex_ptr[0]->y, m_vertex_ptr[0]->z );
  vd[1].assign( m_vertex_ptr[1]->x, m_vertex_ptr[1]->y, m_vertex_ptr[1]->z );
  vd[2].assign( m_vertex_ptr[2]->x, m_vertex_ptr[2]->y, m_vertex_ptr[2]->z );
  
  Vec3<double> a = vd[1] - vd[0];
  Vec3<double> b = vd[2] - vd[0];

	double ab = dot(a,b);
	double f = a.lengthSquared()*b.lengthSquared() - ab*ab;
	if (f<0.0) f=0.0;

	m_area = (PL_REAL)0.5*sqrt(f);
	// std::cout << "a("<<a<<") b("<<b<<")"<<std::endl;
	// std::cout << __func__ <<" "<<m_area <<" "<< f << " " << al << " "<< bl << " "<< ab <<std::endl;
}

} //namespace PolylibNS

