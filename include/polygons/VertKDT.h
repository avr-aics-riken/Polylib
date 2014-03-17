#ifndef polylib_vertkdt_h
#define polylib_vertkdt_h
#include <vector>
#include "common/Vec3.h"
#include "common/BBox.h"
#include "common/PolylibCommon.h"
#include "common/PolylibStat.h"
#include "polygons/Vertex.h"
#include "polygons/VertexList.h"

using namespace Vec3class;

namespace PolylibNS{


class VertKDTNode;
class VertKDTElem;

////////////////////////////////////////////////////////////////////////////
///
/// クラス:VertKDT
/// リーフをVertexとするKD木のクラスです。
///
////////////////////////////////////////////////////////////////////////////

class VertKDT {
public:
	///
	/// コンストラクタ。
	///
	/// @param[in] max_elem	最大要素数。
	/// @param[in] bbox VertKDTのbox範囲。
	/// @param[in] vert_list 木構造の元になるvertexのリスト
	///
	VertKDT(int max_elem, 
		const BBox bbox, 
		std::vector<Vertex*>* vert_list);
	//	    VertexList* vert_list);
	//	    VertexList* vert_list);
	///
	/// コンストラクタ。
	///
	/// @param[in] max_elem	最大要素数。
	///
	VertKDT(int max_elem){
		m_max_elements=max_elem;
		m_root = NULL;
	}

	///
	/// デストラクタ。
	///
	~VertKDT();

	///
	/// 木構造を消去する。
	///
	void destroy();

	///
	/// KD木探索により、指定位置に最も近いVertexを検索する。
	///
	///  @param[in]     pos     指定位置
	///  @return    検索されたVertex
	///
	const Vertex* search_nearest(
		const Vec3<PL_REAL>&    pos
		) const;

	///
	/// KD木探索により、指定位置に最も近いVertexを検索する。
	///
	///  @param[in]     vn      検索対象のノードへのポインタ。
	///  @param[in]     pos     指定位置
	///  @return    検索されたVertex
	///
	const Vertex* search_nearest_recursive(
		VertKDTNode       *vn, 
		const Vec3<PL_REAL>&    pos
		) const;

	///
	/// KD木クラスが利用しているメモリ量を返す。
	///
	///  @return	利用中のメモリ量(byte)
	///
	unsigned int memory_size();


	///root node のBBoxを返す
	BBox get_root_bbox();
	///root node のBBoxを 設定する。
	void set_root_bbox(const BBox& box);

	///
	/// kd 木にvertexを追加する。
	///
	/// @param[in] v 頂点クラス
	/// @return ステータス

	POLYLIB_STAT add(Vertex* v);

	///
	/// kd 木にvertexを追加する。
	/// BBoxにvが入っていなければ、make_upper を用いて再帰的に拡大する。
	/// 最後はadd を用いる。
	///
	/// @param[in] v 頂点クラス
	/// @return ステータス

	POLYLIB_STAT add2(Vertex* v);

	///
	/// BBoxを拡大する。vが入ると思われる方向に拡大する。
	/// ある軸について、vがBBoxのminより小さい時、に左方向に拡大。
	/// それ以外は右方向へ拡大する。
	///
	/// @param[in] v 頂点クラス
	/// @return ステータス

	POLYLIB_STAT make_upper(Vertex* v);


	///
	/// KD木　再構築用関数
	///
	///  @param[in] bbox		VertKDTのbox範囲。
	///  @param[in] vertex_list	木構造の元になるVertexのリスト。
	///  @return	POLYLIB_STATで定義される値が返る。
	///
	POLYLIB_STAT create(
		const BBox	bbox, 
		std::vector<Vertex*>* vert_list
		);


	// 
	int n_create();


private:
	///
	/// vertexをKD木構造に組み込む際に、どのノードへ組み込むかを検索する。
	///
	///  @param[in]	vn 検索対象のノードへのポインタ。
	///  @param[in] elm	組み込むVertex
	///  @param[in,out] vnode 検索結果。
	///
	void traverse(
		VertKDTNode	*vn, 
		VertKDTElem	*elm, 
		VertKDTNode	**vnode
		) const;

	///
	/// vertexをKD木構造から検索する。
	///
	///  @param[in]		vn		検索対象のノードへのポインタ。
	///  @param[in]		bbox	VertKDTのbox範囲。
	///  @param[in,out]	vlist	検索結果配列へのポインタ。
	///
	void search_recursive(
		VertKDTNode	*vn, 
		const BBox	&bbox, 
		std::vector<VertKDTElem*>	*vlist
		) const;

	///
	/// 初期化処理
	///
	///  @param[in] max_elem	最大要素数。
	///  @param[in] bbox		VertKDTのbox範囲。
	///  @param[in] vertex_list	木構造の元になるVertexのリスト。
	///  @return	POLYLIB_STATで定義される値が返る。
	///
	POLYLIB_STAT create(int	max_elem, 
		const BBox	bbox, 
		//VertexList* vert_list
		std::vector<Vertex*>* vert_list
		);


	///
	/// KD木の総ノード数と総ポリゴン数を数える。
	///
	///  @param[in]	 vnode		親ノードへのポインタ。
	///  @param[out] node_cnt	ノード数。
	///  @param[out] poly_cnt	ポリゴン数。
	///
	void node_count(
		VertKDTNode	*vnode, 
		unsigned int	*node_cnt, 
		unsigned int	*poly_cnt
		);

	//=======================================================================
	// クラス変数
	//=======================================================================
	/// ルートノードへのポインタ。
	VertKDTNode	*m_root;

	/// リーフノードが所持できる最大要素数。
	int		m_max_elements;

	int m_n_create;
#ifdef DEBUG_VERTKDT
	std::vector<VertKDTNode*> m_vnode;
#endif
};// end of class 

}// end of namespace PolylibNS
#endif //polylib_vertkdt_h
