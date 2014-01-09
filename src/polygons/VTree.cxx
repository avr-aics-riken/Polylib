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

#include "polygons/VTree.h"
#include "polygons/PrivateTriangle.h"
#include "polygons/VNode.h"
#include "polygons/VElement.h"
#include <string>

//#define DEBUG_VTREE
namespace PolylibNS {

////////////////////////////////////////////////////////////////////////////
///  
/// VNodeクラス
/// KD木構造のノードクラスです。
///  
////////////////////////////////////////////////////////////////////////////




//VTree


// public /////////////////////////////////////////////////////////////////////

VTree::VTree(
	int							max_elem, 
	const BBox					bbox, 
	std::vector<PrivateTriangle*>	*tri_list
) {
	m_root = NULL;
	create(max_elem, bbox, tri_list);
}

// public /////////////////////////////////////////////////////////////////////

VTree::~VTree()
{
	destroy();
}

// public /////////////////////////////////////////////////////////////////////

void VTree::destroy()
{
	if (m_root) {
		delete m_root;
		m_root = NULL;
	}
}

// public /////////////////////////////////////////////////////////////////////


std::vector<PrivateTriangle*>* VTree::search(
	BBox	*bbox, 
	bool	every
) const {
  //#define DEBUG_VTREE
#ifdef DEBUG
	PL_DBGOSH << "VTree::search1:@@@------------------------@@@" << std::endl;
#endif

#ifdef DEBUG_VTREE
	PL_DBGOSH << "VTree::search1:@@@------------------------@@@" << std::endl;
	Vec3<REAL_TYPE> min = bbox->getPoint(0);
	Vec3<REAL_TYPE> max = bbox->getPoint(7);
	PL_DBGOSH << "VTree::search1:min(" << min << "),max(" << max << ")" << std::endl;
#endif

	if (m_root == 0) {
	  std::cerr << "Polylib::vtree::Error" << std::endl;
		exit(1);
	}
	std::vector<VElement*> vlist;
	//PL_DBGOSH << "VTree::search search_recursive start" << std::endl;
	search_recursive(m_root, *bbox, every, &vlist);
	//PL_DBGOSH << "VTree::search search_recursive end" << std::endl;
	std::vector<VElement*>::iterator itr=vlist.begin();

#ifdef DEBUG_VTREE
	PL_DBGOSH << "VTree::search_recursive end" << std::endl;
	/* for (; itr != vlist.end(); itr++) { */
	/* 	PL_DBGOSH << "VTree::search:tid=" << (*itr)->get_triangle()->get_id()  */
	/* 			  << std::endl; */
	/* } */
	itr=vlist.begin();
#endif

	std::vector<PrivateTriangle*> *tri_list = new std::vector<PrivateTriangle*>;
	for (; itr != vlist.end(); itr++) {

#ifdef MEMCOPY_TYPE
		PrivateTriangle *tri = new PrivateTriangle(
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

POLYLIB_STAT VTree::search(
	BBox						*bbox, 
	bool						every, 
	std::vector<PrivateTriangle*>	*tri_list
) const {
#ifdef DEBUG_VTREE
	PL_DBGOSH << "VTree::search2:@@@------------------------@@@" << std::endl;
	Vec3<REAL_TYPE> min = bbox->getPoint(0);
	Vec3<REAL_TYPE> max = bbox->getPoint(7);
	PL_DBGOSH << "VTree::min(" << min << "),max(" << max << ")" << std::endl;
#endif

	if (m_root == 0) {
		std::cout << "Error" << std::endl;
		return PLSTAT_ROOT_NODE_NOT_EXIST;
	}
	std::vector<VElement*> vlist;
	search_recursive(m_root, *bbox, every, &vlist);

	std::vector<VElement*>::iterator itr=vlist.begin();

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
		PrivateTriangle *tri = new PrivateTriangle;

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

unsigned int VTree::memory_size() {
	VNode			*vnode;
	unsigned int	node_cnt = 1;		// ノード数
	unsigned int	poly_cnt = 0;		// ポリゴン数
	unsigned int	size;
	
	if ((vnode = m_root->get_left()) != NULL) {; 
		node_count(vnode, &node_cnt, &poly_cnt);
	}
#ifdef DEBUG_VTREE
	PL_DBGOSH << "VTree::memory_size1():node,poly=" << node_cnt << "," 
			  << poly_cnt << std::endl;
#endif

	if ((vnode = m_root->get_right()) != NULL) {; 
		node_count(vnode, &node_cnt, &poly_cnt);
	}
#ifdef DEBUG_VTREE
	PL_DBGOSH << "VTree::memory_size2():node,poly=" << node_cnt << "," 
			  << poly_cnt << std::endl;
#endif

	size  = sizeof(VTree);
	size += sizeof(VNode)	 * node_cnt;
	size += sizeof(VElement) * poly_cnt;

	return size;

	
}

// public /////////////////////////////////////////////////////////////////////

const PrivateTriangle* VTree::search_nearest(
	const Vec3<REAL_TYPE>&	pos
) const {
	if (m_root == 0) {
	  std::cerr << "Polylib::vtree::Error" << std::endl;
		return 0;
	}
 
		 return search_nearest_recursive(m_root, pos);
}
 
// public /////////////////////////////////////////////////////////////////////

const PrivateTriangle* VTree::search_nearest_recursive(
	VNode		   *vn,
	const Vec3<REAL_TYPE>&	pos
) const {
	if (vn->is_leaf()) {
		const PrivateTriangle* tri_min = 0;
		//float dist2_min = 0.0;
		REAL_TYPE dist2_min = 0.0;
 
		// ノード内のポリゴンから最も近い物を探す(リニアサーチ)
		std::vector<VElement*>::const_iterator itr = vn->get_vlist().begin();
		for (; itr != vn->get_vlist().end(); itr++) {
			const PrivateTriangle* tri = (*itr)->get_triangle();

			//const Vec3<REAL_TYPE> *v = tri->get_vertex();
			// Vec3<REAL_TYPE> c((v[0][0]+v[1][0]+v[2][0])/3.0,
			// 		(v[0][1]+v[1][1]+v[2][1])/3.0,
			// 		(v[0][2]+v[1][2]+v[2][2])/3.0);

			Vertex** vtx = tri->get_vertex();
			Vec3<REAL_TYPE> v0,v1,v2;
			v0=*(vtx[0]);
			v1=*(vtx[1]);
			v2=*(vtx[2]);

			Vec3<REAL_TYPE> c((v0[0]+v1[0]+v2[0])/3.0,
					(v0[1]+v1[1]+v2[1])/3.0,
					(v0[2]+v1[2]+v2[2])/3.0);
			//float dist2 = (c - pos).lengthSquared();
			REAL_TYPE dist2 = (c - pos).lengthSquared();
			if (tri_min == 0 || dist2 < dist2_min) {
				tri_min = tri;
				dist2_min = dist2;
			}
		}
		return tri_min;  // 要素数が0の場合は，0が返る
	} else {
		// 基準点が存在する方のサイドから検索
		VNode *vn1, *vn2;
		AxisEnum axis = vn->get_axis();
		if (pos[axis] < (vn->get_left()->get_bbox()).max[axis]) {
			vn1 = vn->get_left();
			vn2 = vn->get_right();
		} else {
			vn1 = vn->get_right();
			vn2 = vn->get_left();
		}
		const PrivateTriangle* tri = search_nearest_recursive(vn1, pos);
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

void VTree::traverse(VNode* vn, VElement* elm, VNode** vnode) const
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

	Vec3<REAL_TYPE> vtx = elm->get_pos();
#ifdef SQ_RADIUS
	//float& sqdist = q->m_sqdist;
	REAL_TYPE& sqdist = q->m_sqdist;
#endif
	AxisEnum axis = vn->get_axis();
	//float x = vn->get_left()->get_bbox().max[axis];
	REAL_TYPE x = vn->get_left()->get_bbox().max[axis];
	if (vtx[axis] < x) {
		traverse(vn->get_left(), elm, vnode);
#ifdef SQ_RADIUS
		//float d = x - vtx[axis];
		REAL_TYPE d = x - vtx[axis];
		if (d*d < sqdist) {
			traverse(vn->get_right(), elm, vnode);
		}
#endif
	}
	else {
		traverse(vn->get_right(), elm, vnode);
#ifdef SQ_RADIUS
		//float d = vtx[axis] - x;
		REAL_TYPE d = vtx[axis] - x;
		if (d*d < sqdist) {
			traverse(vn->get_left(), elm, vnode);
		}
#endif
	}
}

// private ////////////////////////////////////////////////////////////////////

void VTree::search_recursive(
	VNode				*vn, 
	const BBox			&bbox, 
	bool				every, 
	std::vector<VElement*>	*vlist
) const {
  //#define DEBUG_VTREE
#ifdef DEBUG_VTREE
try{
	PL_DBGOSH << "VTree::search_recursive:@@@----------------------@@@ " 
		  << vn << " " << vn->get_left() << " "<< vn->get_right()<< std::endl;
#endif
	if (vn->is_leaf()) {
#ifdef DEBUG_VTREE
	PL_DBGOSH << "VTree::search_recursive:@@@--------at leaf----------@@@" 
		  << vn << " " << vn->get_left() << " "<< vn->get_right()<< std::endl;
#endif
	std::vector<VElement*>::const_iterator itr = vn->get_vlist().begin();
#ifdef DEBUG_VTREE
	PL_DBGOSH << "VTree::search_recursive:" 
		  << vn->get_vlist().size() << std::endl;
#endif
		for (; itr != vn->get_vlist().end(); itr++) {

#ifdef DEBUG_VTREE
	PL_DBGOSH << "VTree::search_recursive: itr " 
		  << *itr 
		  << " "<<(*itr)->get_triangle() 		  
		  << " "<<(*itr)->get_triangle()->get_vertex()<< std::endl;
	PL_DBGOSH << "VTree::search_recursive: v0"
		  <<*((*itr)->get_triangle()->get_vertex()[0])<< std::endl;
	PL_DBGOSH << "VTree::search_recursive: v1"
		  <<*((*itr)->get_triangle()->get_vertex()[1])<< std::endl;
	PL_DBGOSH << "VTree::search_recursive: v2"
		  <<*((*itr)->get_triangle()->get_vertex()[2])<< std::endl;
	PL_DBGOSH << "VTree::search_recursive: "
		  << " ("<<((*itr)->get_bbox()).min
		  << ") ("<<((*itr)->get_bbox()).max
		  <<")"
		  << std::endl;
#endif

		// determine between bbox and 3 vertices of each triangle.
			if (every == true) {
				bool iscontain = true;
				//const Vec3<REAL_TYPE> *temp = (*itr)->get_triangle()->get_vertex();
				Vertex** temp=(*itr)->get_triangle()->get_vertex();
				for (int i = 0; i < 3; i++) {
				  if (bbox.contain( (Vec3<REAL_TYPE>) *(temp[i]) ) == false)  {
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
			  PL_DBGOSH << "VTree::search_recursive: every == false " <<std::endl;
#endif
				BBox e_bbox = (*itr)->get_bbox();
				if (e_bbox.crossed(bbox) == true) {
#ifdef DEBUG_VTREE
				  PL_DBGOSH << "VTree::search_recursive: crossed true " <<std::endl;
#endif
				  vlist->push_back(*itr);
#ifdef DEBUG_VTREE
				  PL_DBGOSH << "VTree::search_recursive: add itr to vlist " <<std::endl;
#endif


				} else {

#ifdef DEBUG_VTREE
				  PL_DBGOSH << "VTree::search_recursive: crossed false " <<std::endl;
#endif
				}

				//PL_DBGOSH << "VTree::search_recursive: reached? " <<std::endl;
#ifdef DEBUG_VTREE
				PL_DBGOSH << "VTree::search_recursive: min " 
					  << e_bbox.min<< " max "
					  <<e_bbox.max<< " " 
					  << e_bbox.crossed(bbox) <<std::endl;
#endif

			}
			
		}
#ifdef USE_DEPTH
		PL_DBGOSH << "VTree::search_recursive:depth=" << vn->get_depth() 
				  << ",elem= " << vn->get_vlist().size() << std::endl;
#endif
#ifdef DEBUG_VTREE
	PL_DBGOSH << "VTree::search_recursive:@@@-return---------------@@@" << std::endl;
#endif
		return;
	}

#ifdef DEBUG_VTREE
#ifdef DEBUG_VTREE
	PL_DBGOSH << "VTree::search_recursive:@@@--------at Node----------@@@" << std::endl;
#endif

	Vec3<REAL_TYPE> min = bbox.getPoint(0);
	Vec3<REAL_TYPE> max = bbox.getPoint(7);
	PL_DBGOSH << "VTree::search_recursive min(" << min << "),max(" << max << ")" << std::endl;
#endif

	BBox lbox = vn->get_left()->get_bbox_search();
	BBox rbox = vn->get_right()->get_bbox_search();

	if (lbox.crossed(bbox) == true) {
#ifdef USE_DEPTH
		PL_DBGOSH << "VTree::search_recursive:left=" << vn->get_depth() << std::endl;
#endif
		search_recursive(vn->get_left(), bbox, every, vlist);
	}

	if (rbox.crossed(bbox) == true) {
#ifdef USE_DEPTH
	PL_DBGOSH << "VTree::search_recursive:right=" << vn->get_depth() << std::endl;
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

#ifdef SQ_RADIUS
//POLYLIB_STAT VTree::create(float sqradius) {
POLYLIB_STAT VTree::create(REAL_TYPE sqradius) {
#else
POLYLIB_STAT VTree::create(
	int							max_elem, 
	const BBox					bbox, 
	std::vector<PrivateTriangle*>	*tri_list
) {
#endif

#if 0
  Vec3<REAL_TYPE> min=bbox.getPoint(0);
  Vec3<REAL_TYPE> max=bbox.getPoint(7);
  std::cout << __func__ << " min " <<min<<std::endl;
  std::cout << __func__ << " max " <<max<<std::endl;
#endif

  //  std::cout<< "VTree create start" << std::endl;
	destroy();
	m_max_elements = max_elem;
	m_root = new VNode();
	m_root->set_bbox(bbox);
	m_root->set_axis(AXIS_X);
	std::vector<PrivateTriangle*>::iterator itr;
	for (itr = tri_list->begin(); itr != tri_list->end(); itr++) {
		// make a new triangle
		VElement* elm = NULL;
		elm = new VElement(*itr);
		//std::cout<< "VTree create 1" << std::endl;
		VNode* vnode = NULL;
		traverse(m_root, elm, &vnode);
		//std::cout<< "VTree create 2" << std::endl;

		// the vtx didn't find in the tree
		// add a new vertex
		if (vnode == NULL) {
		  PL_ERROSH 
		    << "[ERROR]VTree::create():Can't find appropriate node" 
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

void VTree::node_count(
	VNode			*parent, 
	unsigned int	*node_cnt, 
	unsigned int	*tri_cnt
) {
	VNode	*vnode;


#ifdef DEBUG_VTREE
	PL_DBGOSH << "VTree::node_count1():" << *node_cnt << std::endl;
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



