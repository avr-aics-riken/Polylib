/*
###################################################################################
#
# Polylib - Polygon Management Library
#
# Copyright (c) 2010-2011 VCAD System Research Program, RIKEN.
# All rights reserved.
#
# Copyright (c) 2012-2015 Advanced Institute for Computational Science (AICS), RIKEN.
# All rights reserved.
#
# Copyright (c) 2016-2017 Research Institute for Information Technology (RIIT), Kyushu University.
# All rights reserved.
#
###################################################################################
*/

#ifndef polylib_vertexlist_h
#define polylib_vertexlist_h

#include <vector>
#include <map>
#include <iostream>
#include <string>

#include "common/Vec3.h"
#include "common/BBox.h"
#include "common/PolylibCommon.h"
#include "common/PolylibStat.h"
#include "polygons/Vertex.h"


using namespace Vec3class;

//#define DEBUG
//#define VertexListDEBUG

namespace PolylibNS{

class Vertex;
class VertKDT;

////////////////////////////////////////////////////////////////////////////
///
/// クラス:vertex_list
///   polygon の頂点クラスVertex  を収めるクラス。
///
///
////////////////////////////////////////////////////////////////////////////


class VertexList{

private:
	/// Vertex の格納場所
	std::vector<Vertex*>* m_vertex_list;
	/// 同一性チェックの基準値
	PL_REAL m_tolerance;
	/// 同一性チェックの基準値の2乗
	PL_REAL m_tolerance_2;
	/// Vertex  用KD木
	VertKDT* m_vkdt;

	/// KD木生成用BBox
	BBox m_bbox;

	//番号検索用 map
	std::map<Vertex*, std::vector<Vertex*>::size_type>* m_num_map;

#ifdef  VertexListDEBUG
	std::map<Vertex*, int> m_pointer_count;
#endif

public:

	/// コンストラクタ　基準値
	VertexList();

	VertexList(PL_REAL tolerance);

	/// コンストラクタ
	///
	VertexList(VertKDT* vkdt,PL_REAL tolerance);

	/// デストラクタ
	///
	/// Vertex  は削除。削除する場合は、vtx_clearを呼ぶ。
	/// VertKDT  は削除しない。削除する場合は、 外部から

	~VertexList();


	///Vertex 用KD木のセット
	///
	/// @param[in]  vkdt 利用するKD木のポインタ
	void setKDT(VertKDT* vkdt);

	/// Vertex  用KD木
	VertKDT* getKDT();


	///  Vertexの格納場所へのポインタ
	const std::vector<Vertex*>* get_vertex_lists();
	std::vector<Vertex*>* get_vertex_lists_mod();  //!<  Vertexの格納場所へのポインタ

	/// Vertex の追加 同一性チェック無し。
	void vtx_add_nocheck(Vertex* v);

	int vtx_add_i(Vertex* v); //!< Vertexの追加、m_vertex_listのindexを返す。同一性チェック済み。
	Vertex* vtx_add(Vertex* v); //!< Vertexの追加、その頂点のポインタを示す。同一性チェック済み。
	Vertex* vtx_add_KDT(Vertex* v); //!< Vertexの追加、その頂点のポインタを示す。同一性チェック済み。

	Vertex* ith(int i);//!< i番目Vertexを取り出す。
	Vertex* ith(long i);//!< i番目Vertexを取り出す。

	long ith(Vertex* vtx_ptr) const ;

	/// 頂点の数
	///
	/// @return 頂点の数
	std::vector<Vertex*>::size_type size();

	/// bbox をVertKDTへ設定
	void set_bbox() ;
	/// bbox をVertKDTを取得
	BBox get_bbox() const ;

	/// 三角形ポリゴンの面出力時、頂点の番号を取得する場合に、番号のテーブルを準備する。
	///
	///  後始末は index_map_clear()で行う。
	void prepare_num_out();

	/// 三角形ポリゴンの面出力時、頂点の番号を取得する
	///
	/// @param[in] Vertex のポインタ
	/// @return 頂点番号
	/// @attention prepare_num_out() を呼び出す。
	std::vector<Vertex*>::size_type vtx_index(Vertex* v);

	/// 三角形ポリゴンの面出力時、頂点の番号を取得する場合の、番号のテーブルを削除する。
	void index_map_clear();


	/// 基準値を設定する
	void set_tolerance(const PL_REAL tol);

	/// 基準
	PL_REAL tolerance() const ;

	///  プリント
	void print() const ;

	// setter and getter  for tolerance


	/// 基準値の取得
	PL_REAL get_tolerance();

	/// 重複頂点の削除
	POLYLIB_STAT vertex_compaction(std::map<Vertex*,Vertex*>* vertex_map);


	//  private:
	/// Vertex の解放

	void vtx_clear();



};//end of class

} //end of namespace PolylibNS

//#undef DEBUG
#endif // polylib_vertexlist_h
