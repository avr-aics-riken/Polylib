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

#ifndef polylib_vtree_h
#define polylib_vtree_h

#include "common/BBox.h"
#include "common/PolylibStat.h"
#include "common/PolylibCommon.h"
#include "polygons/Vertex.h"

#include <vector>
//#define DEBUG_VTREE
namespace PolylibNS {

class Vertex;
class BBox;
class PrivateTriangle;
class VNode;
class VElement;

////////////////////////////////////////////////////////////////////////////
///
/// クラス:VTree
/// リーフを三角形ポリゴンとするKD木クラスです。
///
////////////////////////////////////////////////////////////////////////////

class VTree {
public:
	///
    /// コンストラクタ。
	///
	/// @param[in] max_elem	最大要素数。
	/// @param[in] bbox		VTreeのbox範囲。
	/// @param[in] tri_list	木構造の元になるポリゴンのリスト。
	///
	VTree(
		int			max_elem, 
		const BBox			bbox, 
		std::vector<PrivateTriangle*>	*tri_list
	);

	///
	/// デストラクタ。
	///
	~VTree();

	///
	/// 木構造を消去する。
	///
	void destroy();

	///
	/// KD木探索により、指定矩形領域に含まれる三角形ポリゴンを抽出する。
	///
	///  @param[in] bbox	検索範囲を示す矩形領域。
	///  @param[in] every	true:3頂点が全て検索領域に含まれるものを抽出。
	///						false:1頂点でも検索領域に含まれるものを抽出。
	///  @return	抽出したポリゴンリストのポインタ。
	///  @attention MPIPolylib用のメソッドなので、ユーザは利用しないで下さい。
	///  @attention	オーバーロードメソッドあり。

	std::vector<PrivateTriangle*>* search(
		BBox	*bbox, 
		bool	every
	) const;

	///
	/// KD木探索により、指定矩形領域に含まれるポリゴンを抽出する。
	///
	///  @param[in]		bbox		検索範囲を示す矩形領域。
	///  @param[in]		every		true:3頂点が全て検索領域に含まれるものを抽出。
	///								false:1頂点でも検索領域に含まれるものを抽出。
	///  @param[in,out] tri_list	抽出した三角形ポリゴンリストへのポインタ。
	///  @return	POLYLIB_STATで定義される値が返る。
	///  @attention	オーバーロードメソッドあり。
	///
	POLYLIB_STAT search(
		BBox *bbox, 
		bool	every, 
		std::vector<PrivateTriangle*>	*tri_list
	) const;

	///
	/// KD木探索により、指定位置に最も近いポリゴンを検索する。
	///
	///  @param[in]     pos     指定位置
	///  @return    検索されたポリゴン
	///
	const PrivateTriangle* search_nearest(
		const Vec3<REAL_TYPE>&    pos
	) const;

	///
	/// KD木探索により、指定位置に最も近いポリゴンを検索する。
	///
	///  @param[in]     vn      検索対象のノードへのポインタ。
	///  @param[in]     pos     指定位置
	///  @return    検索されたポリゴン
	///
	const PrivateTriangle* search_nearest_recursive(
		VNode       *vn, 
		const Vec3<REAL_TYPE>&    pos
	) const;

	///
	/// KD木クラスが利用しているメモリ量を返す。
	///
	///  @return	利用中のメモリ量(byte)
	///
	unsigned int memory_size();

private:
	///
	/// 三角形をKD木構造に組み込む際に、どのノードへ組み込むかを検索する。
	///
	///  @param[in]		vn		検索対象のノードへのポインタ。
	///  @param[in]		elm		組み込む三角形。
	///  @param[in,out]	vnode	検索結果。
	///
	void traverse(
		VNode		*vn, 
		VElement	*elm, 
		VNode		**vnode
	) const;

	///
	/// 三角形ポリゴンをKD木構造から検索する。
	///
	///  @param[in]		vn		検索対象のノードへのポインタ。
	///  @param[in]		bbox	VTreeのbox範囲。
	///  @param[in]		every	true:ポリゴンの頂点がすべて含まれるNodeを検索。
	///							false:それ以外。
	///  @param[in,out]	vlist	検索結果配列へのポインタ。
	///
	void search_recursive(
		VNode					*vn, 
		const BBox				&bbox, 
		bool					every, 
		std::vector<VElement*>	*vlist
	) const;

	///
	/// 初期化処理
	///
	///  @param[in] max_elem	最大要素数。
	///  @param[in] bbox		VTreeのbox範囲。
	///  @param[in] tri_list	木構造の元になるポリゴンのリスト。
	///  @return	POLYLIB_STATで定義される値が返る。
	///
	POLYLIB_STAT create(
		int					max_elem, 
		const BBox				bbox, 
		std::vector<PrivateTriangle*>	*tri_list
	);

	///
	/// KD木の総ノード数と総ポリゴン数を数える。
	///
	///  @param[in]	 vnode		親ノードへのポインタ。
	///  @param[out] node_cnt	ノード数。
	///  @param[out] poly_cnt	ポリゴン数。
	///
	void node_count(
		VNode	*vnode, 
		unsigned int	*node_cnt, 
		unsigned int	*poly_cnt
	);

	//=======================================================================
	// クラス変数
	//=======================================================================
	/// ルートノードへのポインタ。
	VNode	*m_root;

	/// リーフノードが所持できる最大要素数。
	int		m_max_elements;

#ifdef DEBUG_VTREE
	std::vector<VNode*> m_vnode;
#endif
};

}

#endif  // vtree_h

