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

#ifndef polylib_vnode_h
#define polylib_vnode_h

#include "common/BBox.h"
#include "common/PolylibStat.h"
#include "common/PolylibCommon.h"
#include "polygons/Vertex.h"

#include <vector>
//#define DEBUG_VTREE
namespace PolylibNS {

class Vertex;
class BBox;
class VElement;

////////////////////////////////////////////////////////////////////////////
///  
/// VNodeクラス
/// KD木構造のノードクラスです。
///  
////////////////////////////////////////////////////////////////////////////

class VNode {
public:
	///
	/// コンストラクタ。
	///
	VNode();

	///
	/// デストラクタ。
	///
	~VNode();

	///
	/// ノードを２つの子供ノードに分割する。
	///
	void split(const int& max_elem);

#ifdef USE_DEPTH
	///
	/// ノードの深さ情報のダンプ。
	///
	/// @param[in] n タブの出力個数。
	///
	void dump_depth(int n);
#endif

	//=======================================================================
	// Setter/Getter
	//=======================================================================
	///
	/// ノードがリーフかどうかの判定結果。
	///
	/// @return true=リーフ/false=リーフでない。
	///
	bool is_leaf() const;

	///
	/// BBoxの値を取得。
	///
	/// @return bbox。
	///
	BBox get_bbox() const ;

	///
	/// BBoxの値を設定。
	///
	/// @param[in] bbox。
	///
	void set_bbox(const BBox& bbox) ;

	///
	/// 検索用BBoxを取得。
	///
	/// @return 検索用bbox。
	///
	BBox get_bbox_search() const;

	///
	/// このノードのBounding Boxを引数で与えられる要素を含めた大きさに変更する。
	///
	/// @param[in] p 要素。
	///
	void set_bbox_search(const VElement *p) ;
	///
	/// 左のNodeを取得。
	///
	/// @return 左のNode。
	///
	VNode* get_left() ;
	///
	/// 右のNodeを取得。
	///
	/// @return 右のNode。
	///
	VNode* get_right() ;
	///
	/// Axisを取得。
	///
	/// @return axis。
	///
	AxisEnum get_axis() const ;
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
	std::vector<VElement*>& get_vlist();
	///
	/// 木の要素を設定。
	///
	/// @param[in] elm。
	///
	void set_element(VElement* elm);

	///
	/// ノードが所持する要素の数を取得。
	///
	/// @return 要素数。
	///
	int get_elements_num() const ;
#ifdef USE_DEPTH
	///
	/// ノードの深さ情報を取得。
	///
	/// @return ノードの深さ。
	///
	int get_depth() const;

	///
	/// ノードの深さ情報の設定。
	///
	/// @param[in] depth ノードの深さ。
	///
	void set_depth(int depth) ;
#endif

private:
	//=======================================================================
	// クラス変数
	//=======================================================================
	/// 本ノードの左下側ノード。
	VNode					*m_left;

	/// 本ノードの右下側ノード。
	VNode					*m_right;

	/// KD木生成用のBouding Box。
	BBox					m_bbox;

	/// KD木の軸の方向インデックス。
	AxisEnum				m_axis;

	/// ノードの管理する要素リスト。
	std::vector<VElement*>	m_vlist;

	/// KD木検索用のBouding Box。
	BBox					m_bbox_search;

#ifdef USE_DEPTH
	/// ノードの深さ情報(未使用)。
	int						m_depth;
#endif
};

}

#endif  // polylib_vnode_h

