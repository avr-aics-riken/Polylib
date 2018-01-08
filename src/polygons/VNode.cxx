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

#include "polygons/VNode.h"
#include "polygons/VElement.h"

//#define DEBUG_VTREE
namespace PolylibNS {



//=======================================================================
// Setter/Getter
//=======================================================================
///
/// ノードがリーフかどうかの判定結果。
///
/// @return true=リーフ/false=リーフでない。
///
bool VNode::is_leaf() const {
	return m_left == 0;
}

///
/// BBoxの値を取得。
///
/// @return bbox。
///
BBox VNode::get_bbox() const {
	return m_bbox;
}

///
/// BBoxの値を設定。
///
/// @param[in] bbox。
///
void VNode::set_bbox(const BBox& bbox) {
	m_bbox = bbox;
}

///
/// 検索用BBoxを取得。
///
/// @return 検索用bbox。
///
BBox VNode::get_bbox_search() const {
	return m_bbox_search;
}

///
/// このノードのBounding Boxを引数で与えられる要素を含めた大きさに変更する。
///
/// @param[in] p 要素。
///
void VNode::set_bbox_search(const VElement *p) {
	m_bbox_search.add(p->get_bbox().min);
	m_bbox_search.add(p->get_bbox().max);
}

///
/// 左のNodeを取得。
///
/// @return 左のNode。
///
VNode* VNode::get_left() {
	return m_left;
}

///
/// 右のNodeを取得。
///
/// @return 右のNode。
///
VNode* VNode::get_right() {
	return m_right;
}

///
/// Axisを取得。
///
/// @return axis。
///
AxisEnum VNode::get_axis() const {
	return m_axis;
}

///
/// Axisを設定。
///
/// @param[in] axis。
///
void VNode::set_axis(const AxisEnum axis) {
	m_axis = axis;
}

///
/// 要素のリストを取得。
///
/// @return 要素のリスト。
///
std::vector<VElement*>& VNode::get_vlist() {
	return m_vlist;
}

///
/// 木の要素を設定。
///
/// @param[in] elm。
///
void VNode::set_element(VElement* elm) {
	m_vlist.push_back(elm);
}

///
/// ノードが所持する要素の数を取得。
///
/// @return 要素数。
///
int VNode::get_elements_num() const {
	return m_vlist.size();
}

#ifdef USE_DEPTH
///
/// ノードの深さ情報を取得。
///
/// @return ノードの深さ。
///
int VNode::get_depth() const {
	return m_depth;
}

///
/// ノードの深さ情報の設定。
///
/// @param[in] depth ノードの深さ。
///
void VNode::set_depth(int depth) {
	m_depth = depth;
}
#endif




// VNode

// public /////////////////////////////////////////////////////////////////////

VNode::VNode()
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

VNode::~VNode()
{
	std::vector<VElement*>::iterator itr = m_vlist.begin();
	for (; itr != m_vlist.end(); itr++) {
		delete *itr;
	}
	m_vlist.clear();
	if (m_left!=NULL) {delete m_left; m_left=NULL;}
	if (m_right!=NULL){delete m_right; m_right=NULL;}
}

// public /////////////////////////////////////////////////////////////////////

void VNode::split(const int& max_elem)
{

	m_left = new VNode();
	m_right = new VNode();

	BBox left_bbox = m_bbox;
	BBox right_bbox = m_bbox;

	PL_REAL x = .5 * (m_bbox.min[m_axis] + m_bbox.max[m_axis]);
	left_bbox.max[m_axis] = x;
	right_bbox.min[m_axis] = x;

	m_left->set_bbox(left_bbox);
	m_right->set_bbox(right_bbox);

	// Vec3<PL_REAL> lmin=left_bbox.getPoint(0);
	// Vec3<PL_REAL> lmax=left_bbox.getPoint(7);
	// Vec3<PL_REAL> rmin=right_bbox.getPoint(0);
	// Vec3<PL_REAL> rmax=right_bbox.getPoint(7);
	// std::cout<< __func__ << " nelement " <<m_vlist.size()<<std::endl;
	// std::cout<< __func__ << " lmin " <<lmin<<std::endl;
	// std::cout<< __func__ << " lmax " <<lmax<<std::endl;
	// std::cout<< __func__ << " rmin " <<rmin<<std::endl;
	// std::cout<< __func__ << " rmax " <<rmax<<std::endl;



#ifdef USE_DEPTH
	m_left->m_depth = m_depth+1;
	m_right->m_depth = m_depth+1;
#endif
	std::vector<VElement*>::const_iterator itr = m_vlist.begin();

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

} //namespace PolylibNS
