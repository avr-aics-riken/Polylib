#include "polygons/VertKDTNode.h"

#include <vector>
#include "common/Vec3.h"
#include "common/BBox.h"
#include "common/PolylibCommon.h"
#include "common/PolylibStat.h"
#include "polygons/Vertex.h"
#include "polygons/VertexList.h"
#include "polygons/VertKDTElem.h"

using namespace Vec3class;

namespace PolylibNS{


////////////////////////////////////////////////////////////////////////////
///
///  クラス:VertKDTNode
///  VertexKD木の要素クラスです。
///
////////////////////////////////////////////////////////////////////////////


///
/// ノードがリーフかどうか判定する。
/// 
/// @return true=リーフ/false=リーフでない。
///  
bool VertKDTNode::is_leaf() const 
{
	return m_left == 0;
}

///
/// BBoxの値を取得
/// 
/// @return bbox。
///
BBox VertKDTNode::get_bbox() const {
	return m_bbox;
}

///
/// BBoxの値を設定
/// 
/// @param[in] bbox。


///
void VertKDTNode::set_bbox(const BBox& bbox ) {
	// std::cout << "VertKDTNode::"<< __func__ <<std::endl;
	m_bbox=bbox;
	//  std::cout << "VertKDTNode::" << __func__ <<std::endl;
}

///
/// 検索用BBoxを取得。
///
/// @return 検索用bbox。
///
BBox VertKDTNode::get_bbox_search() const {
	return m_bbox_search;
}

///
/// このノードのBounding Boxを引数で与えられる要素を含めた大きさに変更する。
///
/// @param[in] p 要素。
///
void VertKDTNode::set_bbox_search(const VertKDTElem* p) {
	// m_bbox_search.add(p->get_bbox().min);
	// m_bbox_search.add(p->get_bbox().max);
	//       PL_DBGOSH<<__func__<<std::endl;
	//p->get_pos();
	m_bbox_search.add( *( p->get_pos() ) );
	//       PL_DBGOSH<<__func__<<" end"<<std::endl;
}

/// 
/// 左のNodeを取得。
///
/// @return 左のNode。
///
VertKDTNode* VertKDTNode::get_left() {
	return m_left;
}

///
/// 右のNodeを取得。
///
/// @return 右のNode。
///
VertKDTNode* VertKDTNode::get_right() {
	return m_right;
}

///
/// Axisを取得。
///
/// @return axis。
///
AxisEnum VertKDTNode::get_axis() const {
	return m_axis;
}

///
/// Axisを設定。
///
/// @param[in] axis。
///
void VertKDTNode::set_axis(const AxisEnum axis) {
	m_axis = axis;
}

///
/// 要素のリストを取得。
///
/// @return 要素のリスト。
///
std::vector<VertKDTElem*>& VertKDTNode::get_vlist() {
	return m_vlist;
}

///
/// 木の要素を設定。
///
/// @param[in] elm。
///
void VertKDTNode::set_element(VertKDTElem* elm) {

	m_vlist.push_back(elm);
}

///
/// ノードが所持する要素の数を取得。
///
/// @return 要素数。
///
int VertKDTNode::get_elements_num() const {
	return m_vlist.size();
}

///
///　下位の left ノードを設定。
///
void VertKDTNode::set_left_node(VertKDTNode* lnode){
	m_left = lnode;
}


///
///　下位の right ノードを設定。
///
void VertKDTNode::set_right_node(VertKDTNode* rnode){
	m_right = rnode;
}


}// end of namespace PolylibNS

