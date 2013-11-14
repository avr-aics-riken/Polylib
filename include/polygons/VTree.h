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


template <typename T> class BBox;
template <typename T> class PrivateTriangle;

////////////////////////////////////////////////////////////////////////////
///
/// クラス:VElement
/// KD木構造の要素クラスです。
///
////////////////////////////////////////////////////////////////////////////

template <typename T>
class VElement {
public:
	///
	/// コンストラクタ。
	///
	/// @param[in] tri ポリゴン情報のポインタ。
	/// @attention ポインタを格納するが、参照のみ。deleteは行わない。
	///
	VElement(
		PrivateTriangle<T>* tri
	);

	//=======================================================================
	// Setter/Getter
	//=======================================================================
	///
	/// triangle。
	///
	PrivateTriangle<T>* get_triangle() {
		return m_tri;
	}

	///
	/// Center position of bbox on triangle.
	/// 
	Vec3<T> get_pos() const {
		return m_pos;
	}

	///
	/// Bounding box of this triangle
	///
	BBox<T> get_bbox() const {
		return m_bbox;
	}

private:
	//=======================================================================
	// クラス変数
	//=======================================================================
	/// triangle
	PrivateTriangle<T>	*m_tri;

	/// Center position of bbox on triangle.
	Vec3<T>			m_pos;

	/// Bounding box of this triangle
	BBox<T>			m_bbox;
};

////////////////////////////////////////////////////////////////////////////
///  
/// VNodeクラス
/// KD木構造のノードクラスです。
///  
////////////////////////////////////////////////////////////////////////////
template <typename T>
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
	bool is_leaf() const {
	  return m_left == 0;
	}

	///
	/// BBox<T>の値を取得。
	///
	/// @return bbox。
	///
	BBox<T> get_bbox() const {
		return m_bbox;
	}

	///
	/// BBox<T>の値を設定。
	///
	/// @param[in] bbox。
	///
	void set_bbox(const BBox<T>& bbox) {
		m_bbox = bbox;
	}

	///
	/// 検索用BBox<T>を取得。
	///
	/// @return 検索用bbox。
	///
	BBox<T> get_bbox_search() const {
		return m_bbox_search;
	}

	///
	/// このノードのBounding Boxを引数で与えられる要素を含めた大きさに変更する。
	///
	/// @param[in] p 要素。
	///
	void set_bbox_search(const VElement<T> *p) {
		m_bbox_search.add(p->get_bbox().min);
		m_bbox_search.add(p->get_bbox().max);
	}

	///
	/// 左のNodeを取得。
	///
	/// @return 左のNode。
	///
	VNode<T>* get_left() {
		return m_left;
	}

	///
	/// 右のNodeを取得。
	///
	/// @return 右のNode。
	///
	VNode<T>* get_right() {
		return m_right;
	}

	///
	/// Axisを取得。
	///
	/// @return axis。
	///
	AxisEnum get_axis() const {
		return m_axis;
	}

	///
	/// Axisを設定。
	///
	/// @param[in] axis。
	///
	void set_axis(const AxisEnum axis) {
		m_axis = axis;
	}

	///
	/// 要素のリストを取得。
	///
	/// @return 要素のリスト。
	///
	std::vector<VElement<T>*>& get_vlist() {
		return m_vlist;
	}

	///
	/// 木の要素を設定。
	///
	/// @param[in] elm。
	///
	void set_element(VElement<T>* elm) {
		m_vlist.push_back(elm);
	}

	///
 	/// ノードが所持する要素の数を取得。
	///
 	/// @return 要素数。
	///
	int get_elements_num() const {
		return m_vlist.size();
	}

#ifdef USE_DEPTH
	///
	/// ノードの深さ情報を取得。
	///
	/// @return ノードの深さ。
	///
	int get_depth() const {
		return m_depth;
	}

	///
	/// ノードの深さ情報の設定。
	///
	/// @param[in] depth ノードの深さ。
	///
	void set_depth(int depth) {
		m_depth = depth;
	}
#endif

private:
	//=======================================================================
	// クラス変数
	//=======================================================================
	/// 本ノードの左下側ノード。
	VNode<T>					*m_left;

	/// 本ノードの右下側ノード。
	VNode<T>					*m_right;

	/// KD木生成用のBouding Box。
	BBox<T>					m_bbox;

	/// KD木の軸の方向インデックス。
	AxisEnum				m_axis;

	/// ノードの管理する要素リスト。
	std::vector<VElement<T>*>	m_vlist;

	/// KD木検索用のBouding Box。
	BBox<T>					m_bbox_search;

#ifdef USE_DEPTH
	/// ノードの深さ情報(未使用)。
	int						m_depth;
#endif
};

////////////////////////////////////////////////////////////////////////////
///
/// クラス:VTree
/// リーフを三角形ポリゴンとするKD木クラスです。
///
////////////////////////////////////////////////////////////////////////////
template <typename T>
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
		const BBox<T>			bbox, 
		std::vector<PrivateTriangle<T>*>	*tri_list
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

	std::vector<PrivateTriangle<T>*>* search(
		BBox<T>	*bbox, 
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
		BBox<T> *bbox, 
		bool	every, 
		std::vector<PrivateTriangle<T>*>	*tri_list
	) const;

	///
	/// KD木探索により、指定位置に最も近いポリゴンを検索する。
	///
	///  @param[in]     pos     指定位置
	///  @return    検索されたポリゴン
	///
	const PrivateTriangle<T>* search_nearest(
		const Vec3<T>&    pos
	) const;

	///
	/// KD木探索により、指定位置に最も近いポリゴンを検索する。
	///
	///  @param[in]     vn      検索対象のノードへのポインタ。
	///  @param[in]     pos     指定位置
	///  @return    検索されたポリゴン
	///
	const PrivateTriangle<T>* search_nearest_recursive(
		VNode<T>       *vn, 
		const Vec3<T>&    pos
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
		VNode<T>		*vn, 
		VElement<T>	*elm, 
		VNode<T>		**vnode
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
		VNode<T>					*vn, 
		const BBox<T>				&bbox, 
		bool					every, 
		std::vector<VElement<T>*>	*vlist
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
		const BBox<T>				bbox, 
		std::vector<PrivateTriangle<T>*>	*tri_list
	);

	///
	/// KD木の総ノード数と総ポリゴン数を数える。
	///
	///  @param[in]	 vnode		親ノードへのポインタ。
	///  @param[out] node_cnt	ノード数。
	///  @param[out] poly_cnt	ポリゴン数。
	///
	void node_count(
		VNode<T>	*vnode, 
		unsigned int	*node_cnt, 
		unsigned int	*poly_cnt
	);

	//=======================================================================
	// クラス変数
	//=======================================================================
	/// ルートノードへのポインタ。
	VNode<T>	*m_root;

	/// リーフノードが所持できる最大要素数。
	int		m_max_elements;

#ifdef DEBUG_VTREE
	std::vector<VNode<T>*> m_vnode;
#endif
};



//moved from src 
/* #ifdef DEBUG_VTREE */
/* template <typename T> */
/* static std::vector<VNode<T>*> m_vnode; */
/* #endif */



// VElement 
// public /////////////////////////////////////////////////////////////////////
template <typename T>
VElement<T>::VElement(
	PrivateTriangle<T>* tri
) {
	m_tri = tri;
	Vertex<T>** tmp=tri->get_vertex();
	for(int i=0; i<3; i++){
	  m_bbox.add( (Vec3<T>) (*(tmp[i])) );
	}
	m_pos = m_bbox.center();
}

// VNode

// public /////////////////////////////////////////////////////////////////////
template <typename T>
VNode<T>::VNode()
{
	m_left = NULL;
	m_right = NULL;
	m_axis = AXIS_X;
	m_bbox_search.init();
#ifdef USE_DEPTH
	m_depth = 0;
#endif
}

// public /////////////////////////////////////////////////////////////////////
template <typename T>
VNode<T>::~VNode()
{
  typename std::vector<VElement<T>*>::iterator itr = m_vlist.begin();
	for (; itr != m_vlist.end(); itr++) {
		delete *itr;
	}
	m_vlist.clear();
	if (m_left!=NULL) {delete m_left; m_left=NULL;}
	if (m_right!=NULL){delete m_right; m_right=NULL;}
}

// public /////////////////////////////////////////////////////////////////////
template <typename T>
void VNode<T>::split(const int& max_elem)
{

  m_left = new VNode<T>();
  m_right = new VNode<T>();

  BBox<T> left_bbox = m_bbox;
  BBox<T> right_bbox = m_bbox;

  T x = .5 * (m_bbox.min[m_axis] + m_bbox.max[m_axis]);
  left_bbox.max[m_axis] = x;
  right_bbox.min[m_axis] = x;

  m_left->set_bbox(left_bbox);
  m_right->set_bbox(right_bbox);

  // Vec3<T> lmin=left_bbox.getPoint(0);
  // Vec3<T> lmax=left_bbox.getPoint(7);
  // Vec3<T> rmin=right_bbox.getPoint(0);
  // Vec3<T> rmax=right_bbox.getPoint(7);
  // std::cout<< __func__ << " nelement " <<m_vlist.size()<<std::endl;
  // std::cout<< __func__ << " lmin " <<lmin<<std::endl;
  // std::cout<< __func__ << " lmax " <<lmax<<std::endl;
  // std::cout<< __func__ << " rmin " <<rmin<<std::endl;
  // std::cout<< __func__ << " rmax " <<rmax<<std::endl;



#ifdef USE_DEPTH
  m_left->m_depth = m_depth+1;
  m_right->m_depth = m_depth+1;
#endif
  typename std::vector<VElement<T>*>::const_iterator itr = m_vlist.begin();

  for (; itr != m_vlist.end(); itr++) {
    if ((*itr)->get_pos()[m_axis] < x) {
      m_left->m_vlist.push_back((*itr));
      m_left->set_bbox_search((*itr));
    }
    else {
      m_right->m_vlist.push_back((*itr));
      m_right->set_bbox_search((*itr));
    }
  }
  m_vlist.clear();

  // set the next axis to split a bounding box
  AxisEnum axis;
  if (m_axis == AXIS_Z)  axis = AXIS_X;
  else if (m_axis == AXIS_X) axis = AXIS_Y;
  else	axis = AXIS_Z;

  m_left->set_axis(axis);
  m_right->set_axis(axis);
  if (m_left->get_elements_num() > max_elem) {
    m_left->split(max_elem);
  }
  if (m_right->get_elements_num() > max_elem) {
    m_right->split(max_elem);
  }
}

//VTree


// public /////////////////////////////////////////////////////////////////////
template <typename T>
VTree<T>::VTree(
	int							max_elem, 
	const BBox<T>					bbox, 
	std::vector<PrivateTriangle<T>*>	*tri_list
) {
	m_root = NULL;
	create(max_elem, bbox, tri_list);
}

// public /////////////////////////////////////////////////////////////////////
template <typename T>
VTree<T>::~VTree()
{
	destroy();
}

// public /////////////////////////////////////////////////////////////////////
template <typename T>
void VTree<T>::destroy()
{
	if (m_root) {
		delete m_root;
		m_root = NULL;
	}
}

// public /////////////////////////////////////////////////////////////////////

template <typename T>
std::vector<PrivateTriangle<T>*>* VTree<T>::search(
	BBox<T>	*bbox, 
	bool	every
) const {
  //#define DEBUG_VTREE
#ifdef DEBUG
	PL_DBGOSH << "VTree::search1:@@@------------------------@@@" << std::endl;
#endif

#ifdef DEBUG_VTREE
	PL_DBGOSH << "VTree::search1:@@@------------------------@@@" << std::endl;
	Vec3<T> min = bbox->getPoint(0);
	Vec3<T> max = bbox->getPoint(7);
	PL_DBGOSH << "VTree::search1:min(" << min << "),max(" << max << ")" << std::endl;
#endif

	if (m_root == 0) {
	  std::cerr << "Polylib::vtree::Error" << std::endl;
		exit(1);
	}
	std::vector<VElement<T>*> vlist;
	//PL_DBGOSH << "VTree::search search_recursive start" << std::endl;
	search_recursive(m_root, *bbox, every, &vlist);
	//PL_DBGOSH << "VTree::search search_recursive end" << std::endl;
	typename std::vector<VElement<T>*>::iterator itr=vlist.begin();

#ifdef DEBUG_VTREE
	PL_DBGOSH << "VTree::search_recursive end" << std::endl;
	/* for (; itr != vlist.end(); itr++) { */
	/* 	PL_DBGOSH << "VTree::search:tid=" << (*itr)->get_triangle()->get_id()  */
	/* 			  << std::endl; */
	/* } */
	itr=vlist.begin();
#endif

	std::vector<PrivateTriangle<T>*> *tri_list = new std::vector<PrivateTriangle<T>*>;
	for (; itr != vlist.end(); itr++) {

#ifdef MEMCOPY_TYPE
		PrivateTriangle<T> *tri = new PrivateTriangle<T>(
				(*itr)->getVertex(), (*itr)->get_normal(),
				(*itr)->getArea(), (*itr)->get_id()) ;

#ifdef DEBUG_VTREE
		PL_DBGOSH << "VTree::get_id:" << tri->get_id() << std::endl;
#endif
		tri_list->push_back(*tri);
		delete(tri);
#endif

		tri_list->push_back((*itr)->get_triangle());

	}
	return tri_list;
	///#undef DEBUG_VTREE
}

// public /////////////////////////////////////////////////////////////////////
template <typename T>
POLYLIB_STAT VTree<T>::search(
	BBox<T>						*bbox, 
	bool						every, 
	std::vector<PrivateTriangle<T>*>	*tri_list
) const {
#ifdef DEBUG_VTREE
	PL_DBGOSH << "VTree::search2:@@@------------------------@@@" << std::endl;
	Vec3<T> min = bbox->getPoint(0);
	Vec3<T> max = bbox->getPoint(7);
	PL_DBGOSH << "VTree::min(" << min << "),max(" << max << ")" << std::endl;
#endif

	if (m_root == 0) {
		std::cout << "Error" << std::endl;
		return PLSTAT_ROOT_NODE_NOT_EXIST;
	}
	std::vector<VElement<T>*> vlist;
	search_recursive(m_root, *bbox, every, &vlist);

	typename std::vector<VElement<T>*>::iterator itr=vlist.begin();

#ifdef DEBUG_VTREE
	for (; itr != vlist.end(); itr++) {
		PL_DBGOSH << "VTree::search:tid=" << (*itr)->get_triangle()->get_id() 
				  << std::endl;
	}
#endif

// 木を検索して返ってきた結果を返り値に変換する。

	itr = vlist.begin();
	for (; itr != vlist.end(); itr++) {
#ifdef MEMCOPY_TYPE
		PrivateTriangle<T> *tri = new PrivateTriangle<T>;

		tri->m_id =  (*itr)->m_id;
		tri->m_area =  (*itr)->m_area;
		for (int j=0; j<3; j++) {
			(tri->m_vertex[j])[AXIS_X] = (((*itr)->m_vertex)[j])[AXIS_X];
			(tri->m_vertex[j])[AXIS_Y] = (((*itr)->m_vertex)[j])[AXIS_Y];
			(tri->m_vertex[j])[AXIS_Z] = (((*itr)->m_vertex)[j])[AXIS_Z];
		}
		tri->m_normal[0] = ((*itr)->m_normal)[0];
		tri->m_normal[1] = ((*itr)->m_normal)[1];
		tri->m_normal[2] = ((*itr)->m_normal)[2];

		tri_list->push_back(tri);
#endif
		tri_list->push_back( (*itr)->get_triangle() );
	}
	return PLSTAT_OK;
}

// public /////////////////////////////////////////////////////////////////////
template <typename T>
unsigned int VTree<T>::memory_size() {
	VNode<T>			*vnode;
	unsigned int	node_cnt = 1;		// ノード数
	unsigned int	poly_cnt = 0;		// ポリゴン数
	unsigned int	size;
	
	if ((vnode = m_root->get_left()) != NULL) {; 
		node_count(vnode, &node_cnt, &poly_cnt);
	}
#ifdef DEBUG_VTREE
	PL_DBGOSH << "VTree<T>::memory_size1():node,poly=" << node_cnt << "," 
			  << poly_cnt << std::endl;
#endif

	if ((vnode = m_root->get_right()) != NULL) {; 
		node_count(vnode, &node_cnt, &poly_cnt);
	}
#ifdef DEBUG_VTREE
	PL_DBGOSH << "VTree<T>::memory_size2():node,poly=" << node_cnt << "," 
			  << poly_cnt << std::endl;
#endif

	size  = sizeof(VTree<T>);
	size += sizeof(VNode<T>)	 * node_cnt;
	size += sizeof(VElement<T>) * poly_cnt;

	return size;

	
}

// public /////////////////////////////////////////////////////////////////////
template <typename T>
const PrivateTriangle<T>* VTree<T>::search_nearest(
	const Vec3<T>&	pos
) const {
	if (m_root == 0) {
	  std::cerr << "Polylib::vtree::Error" << std::endl;
		return 0;
	}
 
		 return search_nearest_recursive(m_root, pos);
}
 
// public /////////////////////////////////////////////////////////////////////
template <typename T>
const PrivateTriangle<T>* VTree<T>::search_nearest_recursive(
	VNode<T>		   *vn,
	const Vec3<T>&	pos
) const {
	if (vn->is_leaf()) {
		const PrivateTriangle<T>* tri_min = 0;
		//float dist2_min = 0.0;
		T dist2_min = 0.0;
 
		// ノード内のポリゴンから最も近い物を探す(リニアサーチ)
		typename std::vector<VElement<T>*>::const_iterator itr = vn->get_vlist().begin();
		for (; itr != vn->get_vlist().end(); itr++) {
			const PrivateTriangle<T>* tri = (*itr)->get_triangle();

			//const Vec3<T> *v = tri->get_vertex();
			// Vec3<T> c((v[0][0]+v[1][0]+v[2][0])/3.0,
			// 		(v[0][1]+v[1][1]+v[2][1])/3.0,
			// 		(v[0][2]+v[1][2]+v[2][2])/3.0);

			Vertex<T>** vtx = tri->get_vertex();
			Vec3<T> v0,v1,v2;
			v0=*(vtx[0]);
			v1=*(vtx[1]);
			v2=*(vtx[2]);

			Vec3<T> c((v0[0]+v1[0]+v2[0])/3.0,
					(v0[1]+v1[1]+v2[1])/3.0,
					(v0[2]+v1[2]+v2[2])/3.0);
			//float dist2 = (c - pos).lengthSquared();
			T dist2 = (c - pos).lengthSquared();
			if (tri_min == 0 || dist2 < dist2_min) {
				tri_min = tri;
				dist2_min = dist2;
			}
		}
		return tri_min;  // 要素数が0の場合は，0が返る
	} else {
		// 基準点が存在する方のサイドから検索
		VNode<T> *vn1, *vn2;
		AxisEnum axis = vn->get_axis();
		if (pos[axis] < (vn->get_left()->get_bbox()).max[axis]) {
			vn1 = vn->get_left();
			vn2 = vn->get_right();
		} else {
			vn1 = vn->get_right();
			vn2 = vn->get_left();
		}
		const PrivateTriangle<T>* tri = search_nearest_recursive(vn1, pos);
		if (tri) {
			// 近い方のサイドにポリゴンがあったら，そのままリターン
			return tri;
		} else {
			// もしなかったら，逆サイドを検索
			return search_nearest_recursive(vn2, pos);
		}
	}
}

// private ////////////////////////////////////////////////////////////////////
template <typename T>
void VTree<T>::traverse(VNode<T>* vn, VElement<T>* elm, VNode<T>** vnode) const
{
// --- ims ---<
	// set bbox for search triangle
	vn->set_bbox_search(elm);
// --- ims --->

	if (vn->is_leaf()) {
		if (*vnode == 0) {
			*vnode = vn;
		}
		return;
	}

	Vec3<T> vtx = elm->get_pos();
#ifdef SQ_RADIUS
	//float& sqdist = q->m_sqdist;
	T& sqdist = q->m_sqdist;
#endif
	AxisEnum axis = vn->get_axis();
	//float x = vn->get_left()->get_bbox().max[axis];
	T x = vn->get_left()->get_bbox().max[axis];
	if (vtx[axis] < x) {
		traverse(vn->get_left(), elm, vnode);
#ifdef SQ_RADIUS
		//float d = x - vtx[axis];
		T d = x - vtx[axis];
		if (d*d < sqdist) {
			traverse(vn->get_right(), elm, vnode);
		}
#endif
	}
	else {
		traverse(vn->get_right(), elm, vnode);
#ifdef SQ_RADIUS
		//float d = vtx[axis] - x;
		T d = vtx[axis] - x;
		if (d*d < sqdist) {
			traverse(vn->get_left(), elm, vnode);
		}
#endif
	}
}

// private ////////////////////////////////////////////////////////////////////
template <typename T>
void VTree<T>::search_recursive(
	VNode<T>				*vn, 
	const BBox<T>			&bbox, 
	bool				every, 
	std::vector<VElement<T>*>	*vlist
) const {
  //#define DEBUG_VTREE
#ifdef DEBUG_VTREE
try{
	PL_DBGOSH << "VTree<T>::search_recursive:@@@----------------------@@@ " 
		  << vn << " " << vn->get_left() << " "<< vn->get_right()<< std::endl;
#endif
	if (vn->is_leaf()) {
#ifdef DEBUG_VTREE
	PL_DBGOSH << "VTree<T>::search_recursive:@@@--------at leaf----------@@@" 
		  << vn << " " << vn->get_left() << " "<< vn->get_right()<< std::endl;
#endif
	typename std::vector<VElement<T>*>::const_iterator itr = vn->get_vlist().begin();
#ifdef DEBUG_VTREE
	PL_DBGOSH << "VTree<T>::search_recursive:" 
		  << vn->get_vlist().size() << std::endl;
#endif
		for (; itr != vn->get_vlist().end(); itr++) {

#ifdef DEBUG_VTREE
	PL_DBGOSH << "VTree<T>::search_recursive: itr " 
		  << *itr 
		  << " "<<(*itr)->get_triangle() 		  
		  << " "<<(*itr)->get_triangle()->get_vertex()<< std::endl;
	PL_DBGOSH << "VTree<T>::search_recursive: v0"
		  <<*((*itr)->get_triangle()->get_vertex()[0])<< std::endl;
	PL_DBGOSH << "VTree<T>::search_recursive: v1"
		  <<*((*itr)->get_triangle()->get_vertex()[1])<< std::endl;
	PL_DBGOSH << "VTree<T>::search_recursive: v2"
		  <<*((*itr)->get_triangle()->get_vertex()[2])<< std::endl;
	PL_DBGOSH << "VTree<T>::search_recursive: "
		  << " ("<<((*itr)->get_bbox()).min
		  << ") ("<<((*itr)->get_bbox()).max
		  <<")"
		  << std::endl;
#endif

		// determine between bbox and 3 vertices of each triangle.
			if (every == true) {
				bool iscontain = true;
				//const Vec3<T> *temp = (*itr)->get_triangle()->get_vertex();
				Vertex<T>** temp=(*itr)->get_triangle()->get_vertex();
				for (int i = 0; i < 3; i++) {
				  if (bbox.contain( (Vec3<T>) *(temp[i]) ) == false)  {
						iscontain = false;
						break;
					}
				}
				if (iscontain == true) {
					vlist->push_back(*itr);
				}
			}
			else{
				// determine between bbox and bbox crossed

#ifdef DEBUG_VTREE
			  PL_DBGOSH << "VTree<T>::search_recursive: every == false " <<std::endl;
#endif
				BBox<T> e_bbox = (*itr)->get_bbox();
				if (e_bbox.crossed(bbox) == true) {
#ifdef DEBUG_VTREE
				  PL_DBGOSH << "VTree<T>::search_recursive: crossed true " <<std::endl;
#endif
				  vlist->push_back(*itr);
#ifdef DEBUG_VTREE
				  PL_DBGOSH << "VTree<T>::search_recursive: add itr to vlist " <<std::endl;
#endif


				} else {

#ifdef DEBUG_VTREE
				  PL_DBGOSH << "VTree<T>::search_recursive: crossed false " <<std::endl;
#endif
				}

				//PL_DBGOSH << "VTree<T>::search_recursive: reached? " <<std::endl;
#ifdef DEBUG_VTREE
				PL_DBGOSH << "VTree<T>::search_recursive: min " 
					  << e_bbox.min<< " max "
					  <<e_bbox.max<< " " 
					  << e_bbox.crossed(bbox) <<std::endl;
#endif

			}
			
		}
#ifdef USE_DEPTH
		PL_DBGOSH << "VTree<T>::search_recursive:depth=" << vn->get_depth() 
				  << ",elem= " << vn->get_vlist().size() << std::endl;
#endif
#ifdef DEBUG_VTREE
	PL_DBGOSH << "VTree<T>::search_recursive:@@@-return---------------@@@" << std::endl;
#endif
		return;
	}

#ifdef DEBUG_VTREE
#ifdef DEBUG_VTREE
	PL_DBGOSH << "VTree<T>::search_recursive:@@@--------at Node----------@@@" << std::endl;
#endif

	Vec3<T> min = bbox.getPoint(0);
	Vec3<T> max = bbox.getPoint(7);
	PL_DBGOSH << "VTree<T>::search_recursive min(" << min << "),max(" << max << ")" << std::endl;
#endif

	BBox<T> lbox = vn->get_left()->get_bbox_search();
	BBox<T> rbox = vn->get_right()->get_bbox_search();

	if (lbox.crossed(bbox) == true) {
#ifdef USE_DEPTH
		PL_DBGOSH << "VTree<T>::search_recursive:left=" << vn->get_depth() << std::endl;
#endif
		search_recursive(vn->get_left(), bbox, every, vlist);
	}

	if (rbox.crossed(bbox) == true) {
#ifdef USE_DEPTH
	PL_DBGOSH << "VTree<T>::search_recursive:right=" << vn->get_depth() << std::endl;
#endif
		search_recursive(vn->get_right(), bbox, every, vlist);
	}
#ifdef DEBUG_VTREE
}
catch(char *str) {
	std::cout << str;
}
#endif

//#undef DEBUG_VTREE
}

// private ////////////////////////////////////////////////////////////////////
template <typename T>
#ifdef SQ_RADIUS
//POLYLIB_STAT VTree<T>::create(float sqradius) {
POLYLIB_STAT VTree<T>::create(T sqradius) {
#else
POLYLIB_STAT VTree<T>::create(
	int							max_elem, 
	const BBox<T>					bbox, 
	std::vector<PrivateTriangle<T>*>	*tri_list
) {
#endif

#if 0
  Vec3<T> min=bbox.getPoint(0);
  Vec3<T> max=bbox.getPoint(7);
  std::cout << __func__ << " min " <<min<<std::endl;
  std::cout << __func__ << " max " <<max<<std::endl;
#endif

  //  std::cout<< "VTree create start" << std::endl;
	destroy();
	m_max_elements = max_elem;
	m_root = new VNode<T>();
	m_root->set_bbox(bbox);
	m_root->set_axis(AXIS_X);
	typename std::vector<PrivateTriangle<T>*>::iterator itr;
	for (itr = tri_list->begin(); itr != tri_list->end(); itr++) {
		// make a new triangle
		VElement<T>* elm = NULL;
		elm = new VElement<T>(*itr);
		//std::cout<< "VTree create 1" << std::endl;
		VNode<T>* vnode = NULL;
		traverse(m_root, elm, &vnode);
		//std::cout<< "VTree create 2" << std::endl;

		// the vtx didn't find in the tree
		// add a new vertex
		if (vnode == NULL) {
		  PL_ERROSH 
		    << "[ERROR]VTree<T>::create():Can't find appropriate node" 
		    << std::endl;
		  return PLSTAT_NODE_NOT_FIND;
		}

		// find node to add a new triangle
		vnode->set_element(elm);

		//std::cout<< "VTree create 4" << std::endl;
		// set bbox for search triangle
		vnode->set_bbox_search(elm);

		// std::cout<< "VTree create 5" << std::endl;
		

		if (vnode->get_elements_num() > m_max_elements) {

		  vnode->split(m_max_elements);

#ifdef DEBUG_VTREE
		  // m_vnode.push_back(vnode->get_left());
		  // m_vnode.push_back(vnode->get_right());

		  this->m_vnode.push_back(vnode->get_left());
		  this->m_vnode.push_back(vnode->get_right());
#endif
			//  std::cout<< "VTree create 6" << std::endl;
		  

		}

	}

#ifdef USE_DEPTH
	m_root->dump_depth(0);
#endif

	//  std::cout<< "VTree create end" << std::endl;
	return PLSTAT_OK;
}

// private ////////////////////////////////////////////////////////////////////
template <typename T>
void VTree<T>::node_count(
	VNode<T>			*parent, 
	unsigned int	*node_cnt, 
	unsigned int	*tri_cnt
) {
	VNode<T>	*vnode;


#ifdef DEBUG_VTREE
	PL_DBGOSH << "VTree<T>::node_count1():" << *node_cnt << std::endl;
#endif
	if ((vnode = parent->get_left()) != NULL) {; 
		(*node_cnt)++;
		node_count(vnode, node_cnt, tri_cnt);
	} else {
		(*tri_cnt) += parent->get_elements_num();
	}
#ifdef DEBUG_VTREE
	PL_DBGOSH << "VTree::node_count2():" << *node_cnt << std::endl;
#endif

	if ((vnode = parent->get_right()) != NULL) {; 
		(*node_cnt)++;
		node_count(vnode, node_cnt, tri_cnt);
	}

}




} //namespace PolylibNS

#endif  // vtree_h

