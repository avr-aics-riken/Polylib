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
# Copyright (c) 2016-2018 Research Institute for Information Technology (RIIT), Kyushu University.
# All rights reserved.
#
###################################################################################
*/

#ifndef polylib_VertKDTNode_h
#define polylib_VertKDTNode_h
#include <vector>
#include "common/Vec3.h"
#include "common/BBox.h"
#include "common/PolylibCommon.h"
#include "common/PolylibStat.h"
#include "polygons/Vertex.h"
#include "polygons/VertexList.h"

using namespace Vec3class;

namespace PolylibNS{

////////////////////////////////////////////////////////////////////////////
///
///  クラス:VertKDTNode
///  VertexKD木の要素クラスです。
///
////////////////////////////////////////////////////////////////////////////
class VertKDTElem;

class VertKDTNode{
public:
	///
	/// コンストラクタ。
	///
	VertKDTNode();

	///
	/// デストラクタ。
	///
	~VertKDTNode();

	///
	/// ノードを２つの子供ノードに分割する。
	///
	void split(const int& max_elem);

	///
	/// ノードがリーフかどうか判定する。
	///
	/// @return true=リーフ/false=リーフでない。
	///
	bool is_leaf() const;

	///
	/// BBoxの値を取得
	///
	/// @return bbox。
	///
	BBox get_bbox() const;

	///
	/// BBoxの値を設定
	///
	/// @param[in] bbox。


	///
	void set_bbox(const BBox& bbox );

	///
	/// 検索用BBoxを取得。
	///
	/// @return 検索用bbox。
	///
	BBox get_bbox_search() const ;

	///
	/// このノードのBounding Boxを引数で与えられる要素を含めた大きさに変更する。
	///
	/// @param[in] p 要素。
	///
	void set_bbox_search(const VertKDTElem* p) ;

	///
	/// 左のNodeを取得。
	///
	/// @return 左のNode。
	///
	VertKDTNode* get_left();

	///
	/// 右のNodeを取得。
	///
	/// @return 右のNode。
	///
	VertKDTNode* get_right() ;

	///
	/// Axisを取得。
	///
	/// @return axis。
	///
	AxisEnum get_axis() const;

	///
	/// Axisを設定。
	///
	/// @param[in] axis。
	///
	void set_axis(const AxisEnum axis) ;

	///
	/// 要素のリストを取得。
	///
	/// @return 要素のリスト。
	///
	std::vector<VertKDTElem*>& get_vlist();

	///
	/// 木の要素を設定。
	///
	/// @param[in] elm。
	///
	void set_element(VertKDTElem* elm) ;

	///
	/// ノードが所持する要素の数を取得。
	///
	/// @return 要素数。
	///
	int get_elements_num() const ;

	///
	///　下位の left ノードを設定。
	///
	void set_left_node(VertKDTNode* lnode);


	///
	///　下位の right ノードを設定。
	///
	void set_right_node(VertKDTNode* rnode);



private:

	//=======================================================================
	// クラス変数
	//=======================================================================
	/// 本ノードの左下側ノード。
	VertKDTNode					*m_left;

	/// 本ノードの右下側ノード。
	VertKDTNode					*m_right;

	/// KD木生成用のBouding Box。
	BBox					m_bbox;

	/// KD木の軸の方向インデックス。
	AxisEnum				m_axis;

	/// ノードの管理する要素リスト。
	std::vector<VertKDTElem*>	m_vlist;

	/// KD木検索用のBouding Box。
	BBox					m_bbox_search;


};// end of class

}// end of namespace PolylibNS
#endif //polylib_VertKDTNode_h
