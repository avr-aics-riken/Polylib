#include "polygons/VertKDT.h"

#include <vector>
#include "common/Vec3.h"
#include "common/BBox.h"
#include "common/PolylibCommon.h"
#include "common/PolylibStat.h"
#include "polygons/Vertex.h"
#include "polygons/VertexList.h"
#include "polygons/VertKDTNode.h"
#include "polygons/VertKDTElem.h"


namespace PolylibNS{






////////////////////////////////////////////////////////////////////////////
///
/// クラス:VertKDT
/// リーフをVertexとするKD木のクラスです。
///
////////////////////////////////////////////////////////////////////////////

///root node のBBoxを返す
BBox VertKDT::get_root_bbox(){
	return m_root->get_bbox();
}
///root node のBBoxを 設定する。
void VertKDT::set_root_bbox(const BBox& box){
	if(m_root==NULL) m_root=new VertKDTNode;
	return m_root->set_bbox(box);
}


///
/// KD木　再構築用関数
///
///  @param[in] bbox		VertKDTのbox範囲。
///  @param[in] vertex_list	木構造の元になるVertexのリスト。
///  @return	POLYLIB_STATで定義される値が返る。
///
POLYLIB_STAT VertKDT::create(
	const BBox	bbox, 
	std::vector<Vertex*>* vert_list
	){

		return create(m_max_elements,bbox,vert_list);

}


// 
int VertKDT::n_create(){
	return m_n_create;
}

// VertKDTNode

// public /////////////////////////////////////////////////////////////////////

VertKDTNode::VertKDTNode()
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

VertKDTNode::~VertKDTNode()
{
	//#define DEBUG
#ifdef DEBUG
	PL_DBGOSH << __func__ << " in " <<std::endl;
#endif

	std::vector<VertKDTElem*>::iterator itr = m_vlist.begin();
	for (; itr != m_vlist.end(); itr++) {
		delete *itr;
	}
	m_vlist.clear();
	if (m_left!=NULL) {delete m_left; m_left=NULL;}
	if (m_right!=NULL){delete m_right; m_right=NULL;}
	//#undef DEBUG
}

// public /////////////////////////////////////////////////////////////////////

void VertKDTNode::split(const int& max_elem)
{

	m_left = new VertKDTNode();
	m_right = new VertKDTNode();

	BBox left_bbox = m_bbox;
	BBox right_bbox = m_bbox;

	REAL_TYPE x = .5 * (m_bbox.min[m_axis] + m_bbox.max[m_axis]);
	left_bbox.max[m_axis] = x;
	right_bbox.min[m_axis] = x;

	m_left->set_bbox(left_bbox);
	m_right->set_bbox(right_bbox);

	// Vec3<REAL_TYPE> lmin=left_bbox.getPoint(0);
	// Vec3<REAL_TYPE> lmax=left_bbox.getPoint(7);
	// Vec3<REAL_TYPE> rmin=right_bbox.getPoint(0);
	// Vec3<REAL_TYPE> rmax=right_bbox.getPoint(7);
	// std::cout<< __func__ << " nelement " <<m_vlist.size()<<std::endl;
	// std::cout<< __func__ << " lmin " <<lmin<<std::endl;
	// std::cout<< __func__ << " lmax " <<lmax<<std::endl;
	// std::cout<< __func__ << " rmin " <<rmin<<std::endl;
	// std::cout<< __func__ << " rmax " <<rmax<<std::endl;


#ifdef USE_DEPTH
	m_left->m_depth = m_depth+1;
	m_right->m_depth = m_depth+1;
#endif

	std::vector<VertKDTElem*>::const_iterator itr = m_vlist.begin();

	for (; itr != m_vlist.end(); itr++) {

		Vertex* tmpvtx=(*itr)->get_vertex();
		Vec3<REAL_TYPE> elempos=(Vec3<REAL_TYPE>) *tmpvtx;
		//    if ((*itr)->get_pos()[m_axis] < x) {
		if (elempos[m_axis] < x) {
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


//VertKDT


// public /////////////////////////////////////////////////////////////////////

VertKDT::VertKDT(
	int	max_elem, 
	const BBox	bbox, 
	std::vector<Vertex*>* vert_list) 
	//		       VertexList* vert_list) 
{
	m_root = NULL;
	create(max_elem, bbox, vert_list);
}

// public /////////////////////////////////////////////////////////////////////

VertKDT::~VertKDT()
{
	destroy();
}

// public /////////////////////////////////////////////////////////////////////

void VertKDT::destroy()
{
	//#define DEBUG
#ifdef DEBUG
	PL_DBGOSH << __func__ << " in " <<std::endl;
#endif

	if (m_root) {
		delete m_root;
		m_root = NULL;
	}
	//#undef DEBUG
}


// public /////////////////////////////////////////////////////////////////////

unsigned int VertKDT::memory_size() {
	VertKDTNode			*vnode;
	unsigned int	node_cnt = 1;		// ノード数
	unsigned int	poly_cnt = 0;		// ポリゴン数
	unsigned int	size;

	if ((vnode = m_root->get_left()) != NULL) {; 
	node_count(vnode, &node_cnt, &poly_cnt);
	}
	//#define DEBUG_VTREE
#ifdef DEBUG_VTREE
	PL_DBGOSH << "VertKDT::memory_size1():node,poly=" << node_cnt << "," 
		<< poly_cnt << std::endl;
#endif

	if ((vnode = m_root->get_right()) != NULL) {; 
	node_count(vnode, &node_cnt, &poly_cnt);
	}
#ifdef DEBUG_VTREE
	PL_DBGOSH << "VertKDT::memory_size2():node,poly=" << node_cnt << "," 
		<< poly_cnt << std::endl;
#endif

	size  = sizeof(VertKDT);
	size += sizeof(VertKDTNode)	 * node_cnt;
	size += sizeof(VertKDTElem) * poly_cnt;
	return size;
	//#undef DEBUG_VTREE
}

// public /////////////////////////////////////////////////////////////////////

const Vertex* VertKDT::search_nearest(
	const Vec3<REAL_TYPE>&	pos) const 
{
	if (m_root == 0) {
		std::cerr << "Polylib::VertKDT::Error" << std::endl;
		return 0;
	}
	//#define DEBUG
#ifdef DEBUG
	PL_DBGOSH << "VertKDT::"<<__func__<<" pos  "<<pos<<std::endl;
#endif
	//#undef DEBUG
	return search_nearest_recursive(m_root, pos);
}

// public /////////////////////////////////////////////////////////////////////

const Vertex* VertKDT::search_nearest_recursive(
	VertKDTNode   *vn,
	const Vec3<REAL_TYPE>&	pos
	) const 
{
	//#define DEBUG

	if (vn->is_leaf()) {
		const Vertex* tri_min = 0;
		//float dist2_min = 0.0;
		REAL_TYPE dist2_min = 0.0;

		// ノード内のポリゴンから最も近い物を探す(リニアサーチ)
		std::vector<VertKDTElem*>::const_iterator itr = vn->get_vlist().begin();
		for (; itr != vn->get_vlist().end(); itr++) {
			const Vertex* tri = (*itr)->get_vertex();
			Vec3<REAL_TYPE> c = (Vec3<REAL_TYPE>) (*tri);
			REAL_TYPE dist2 = (c - pos).lengthSquared();
			if (tri_min == 0 || dist2 < dist2_min) {
				tri_min = tri;
				dist2_min = dist2;
			}

#ifdef DEBUG
			PL_DBGOSH << "VertKDT::"<<__func__<<" c "<< c <<" pos "<<pos<<std::endl;
			PL_DBGOSH << "VertKDT::"<<__func__<<" dist2 "<< dist2 <<" dist2_min "<<dist2_min<<std::endl;
			PL_DBGOSH << "VertKDT::"<<__func__<<" tri_min "<< tri_min <<" tri "<< tri<<std::endl;
#endif

		}
		//	PL_DBGOSH << "VertKDT::"<<__func__<<" return  tri_min "<< tri_min <<std::endl;
		return tri_min;  // 要素数が0の場合は，0が返る
	} else {
		// 基準点が存在する方のサイドから検索
		VertKDTNode *vn1, *vn2;
		AxisEnum axis = vn->get_axis();
		if (pos[axis] < (vn->get_left()->get_bbox()).max[axis]) {
			vn1 = vn->get_left();
			vn2 = vn->get_right();
		} else {
			vn1 = vn->get_right();
			vn2 = vn->get_left();
		}
		const Vertex* tri = search_nearest_recursive(vn1, pos);
		if (tri) {
			// 近い方のサイドにポリゴンがあったら，そのままリターン
			return tri;
		} else {
			// もしなかったら，逆サイドを検索
			return search_nearest_recursive(vn2, pos);
		}
	}
	return 0; // never happens
	//#undef DEBUG
}

// public


POLYLIB_STAT VertKDT::add2(Vertex* v)
{
	//#define DEBUG
#ifdef DEBUG
	PL_DBGOSH << "VertKDT::"<<__func__ << " "<< *v << std::endl;    
#endif// DEBUG

	BBox rootbbox=get_root_bbox();

	if(rootbbox.contain((Vec3<REAL_TYPE>) *v)){
		return add(v);
	}else{

#ifdef DEBUG
		PL_DBGOSH << "VertKDT::"<<__func__ << " "<< *v << std::endl;    
#endif// DEBUG

		make_upper(v);
		return add2(v);
	}
	//#undef DEBUG
}

POLYLIB_STAT VertKDT::make_upper(Vertex* v)
{

	//#define DEBUG
#ifdef DEBUG
	PL_DBGOSH << "VertKDT::"<<__func__ << " "<< *v<< std::endl;    
#endif// DEBUG

	BBox rootbbox=get_root_bbox();
	Vec3<REAL_TYPE> min=rootbbox.min;
	Vec3<REAL_TYPE> max=rootbbox.max;
	Vec3<REAL_TYPE> uppermin=rootbbox.min;
	Vec3<REAL_TYPE> uppermax=rootbbox.max;
	Vec3<REAL_TYPE> pairmin=rootbbox.min;
	Vec3<REAL_TYPE> pairmax=rootbbox.max;

	AxisEnum axis= m_root->get_axis();
	AxisEnum upperaxis;

	if(axis==AXIS_X){
		upperaxis=AXIS_Z;
	} else if(axis==AXIS_Y){
		upperaxis=AXIS_X;
	} else if(axis==AXIS_Z){
		upperaxis=AXIS_Y;
	} else {
		PL_ERROSH
			<< "[ERROR]VertKDT::make_upper():Can't find axis."
			<< std::endl;
	}

	bool lower=*v[upperaxis]<min[upperaxis];

	if(lower){
		uppermin[upperaxis]=min[upperaxis]-rootbbox.length(upperaxis);
		pairmin[upperaxis]=min[upperaxis]-rootbbox.length(upperaxis);
		pairmax[upperaxis]=min[upperaxis];
	} else {
		uppermax[upperaxis]=max[upperaxis]+rootbbox.length(upperaxis);
		pairmin[upperaxis]=max[upperaxis];
		pairmax[upperaxis]=max[upperaxis]+rootbbox.length(upperaxis);
	}
	BBox upperbbox(uppermin,uppermax);
	BBox pairbbox(pairmin,pairmax);
	VertKDTNode* new_root = new VertKDTNode;
	VertKDTNode* pair_node = new VertKDTNode;


#ifdef DEBUG
	PL_DBGOSH << "VertKDT::"<<__func__ << " min "<< min<< std::endl;    
	PL_DBGOSH << "VertKDT::"<<__func__ << " max "<< max<< std::endl;    
	PL_DBGOSH << "VertKDT::"<<__func__ << " uppermin "<< uppermin<< std::endl;    
	PL_DBGOSH << "VertKDT::"<<__func__ << " uppermax "<< uppermax<< std::endl;    
	PL_DBGOSH << "VertKDT::"<<__func__ << " pairmin "<< pairmin<< std::endl;    
	PL_DBGOSH << "VertKDT::"<<__func__ << " pairmax "<< pairmax<< std::endl;    
	PL_DBGOSH << "VertKDT::"<<__func__ << " length "<< rootbbox.length(upperaxis)<< std::endl;    
	//  int a;
	//  std::cin >> a;
#endif// DEBUG


	new_root->set_bbox(upperbbox);
	new_root->set_axis(upperaxis);

	pair_node->set_bbox(pairbbox);
	pair_node->set_axis(upperaxis);

	if(lower) {
		new_root->set_left_node(pair_node);
		new_root->set_right_node(m_root);
	} else {
		new_root->set_left_node(m_root);
		new_root->set_right_node(pair_node);
	}
	m_root = new_root;

	return PLSTAT_OK;
	//#undef DEBUG
}

// public


POLYLIB_STAT VertKDT::add(Vertex* v)
{

	//#define DEBUG
#ifdef DEBUG
	PL_DBGOSH <<"VertKDT::" <<__func__ << " start"<<std::endl;
#endif

	// make a new vertex
	VertKDTElem* elm = NULL;
	elm = new VertKDTElem(v);

#ifdef DEBUG
	PL_DBGOSH <<"VertKDT::" <<__func__ << " VertKDTElem made"<<std::endl;
#endif

	//std::cout<< "VertKDT add 1" << std::endl;
	VertKDTNode* vnode = NULL;
	traverse(m_root, elm, &vnode);
	//std::cout<< "VertKDT add 2" << std::endl;

#ifdef DEBUG
	PL_DBGOSH <<"VertKDT::" <<__func__ << " traverse ended"<<std::endl;
#endif



	// the vtx didn't find in the tree
	// add a new vertex
	if (vnode == NULL) {
		PL_ERROSH
			<< "[ERROR]VertKDT::add():Can't find appropriate node"
			<< std::endl;
		return PLSTAT_NODE_NOT_FIND;
	}

#ifdef DEBUG
	PL_DBGOSH <<"VertKDT::" <<__func__ << " traverse ended"<<std::endl;
#endif


	// find node to add a new triangle
	vnode->set_element(elm);
	vnode->set_bbox_search(elm);


	if (vnode->get_elements_num() > m_max_elements) {
		vnode->split(m_max_elements);
	}
	return PLSTAT_OK;
}



// private ////////////////////////////////////////////////////////////////////

void VertKDT::traverse(VertKDTNode* vn, VertKDTElem* elm, VertKDTNode** vnode) const
{

	//#define DEBUG
#ifdef DEBUG 
	PL_DBGOSH<<"VertKDT::"<<__func__<<" start"<<std::endl;
#endif


	// --- ims ---<
	// set bbox for search triangle
	//tmpoff
	vn->set_bbox_search(elm);
	// --- ims --->

#ifdef DEBUG 
	PL_DBGOSH<<"VertKDT::"<<__func__<<" set_bbox_search "<<std::endl;
#endif


	if (vn->is_leaf()) {
		if (*vnode == 0) {
			*vnode = vn;
		}
		return;
	}

#ifdef DEBUG 
	PL_DBGOSH<<"VertKDT::"<<__func__<<" vn is not leaf "<<std::endl;
#endif


	//Vec3<REAL_TYPE> vtx = elm->pos();
	Vertex *tmpvtx =elm->get_vertex();
	Vec3<REAL_TYPE> vtx = (Vec3<REAL_TYPE>) *tmpvtx;
#ifdef SQ_RADIUS
	//float& sqdist = q->m_sqdist;
	REAL_TYPE& sqdist = q->m_sqdist;
#endif
	AxisEnum axis = vn->get_axis();
	//float x = vn->get_left()->get_bbox().max[axis];
	REAL_TYPE x = vn->get_left()->get_bbox().max[axis];

#ifdef DEBUG 
	PL_DBGOSH<<"VertKDT::"<<__func__<<" traverse recursively "<<std::endl;
#endif

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

void VertKDT::search_recursive(
	VertKDTNode				*vn, 
	const BBox			&bbox, 
	std::vector<VertKDTElem*>	*vlist
	) const 
{
#ifdef DEBUG_VTREE
	try{
		PL_DBGOSH << "VertKDT::search_recursive:@@@----------------------@@@" << std::endl;
#endif
		if (vn->is_leaf()) {
			std::vector<VertKDTElem*>::const_iterator itr = vn->get_vlist().begin();
			for (; itr != vn->get_vlist().end(); itr++) {
				// determine between bbox and 3 vertices of each triangle.

				bool iscontain = true;
				//const Vec3<REAL_TYPE> *temp = (*itr)->get_triangle()->get_vertex();

				Vertex* temp=(*itr)->get_vertex();

				for (int i = 0; i < 3; i++) {
					if (bbox.contain( (Vec3<REAL_TYPE>) *(temp[i]) ) == false)  {
						iscontain = false;
						break;
					}
				}
				if (iscontain == true) {
					vlist->push_back(*itr);
				}

				// // determine between bbox and bbox crossed
				// BBox e_bbox = (*itr)->get_bbox();

				// if (e_bbox.crossed(bbox) == true) {
				// 		vlist->push_back(*itr);
				// }

			}
#ifdef USE_DEPTH
			PL_DBGOSH << "VertKDT::search_recursive:depth=" << vn->get_depth() 
				<< ",elem= " << vn->get_vlist().size() << std::endl;
#endif
#ifdef DEBUG_VTREE
			PL_DBGOSH << "VertKDT::search_recursive:@@@-return---------------@@@" << std::endl;
#endif
			return;
		}

#ifdef DEBUG_VTREE
		Vec3<REAL_TYPE> min = bbox.getPoint(0);
		Vec3<REAL_TYPE> max = bbox.getPoint(7);
		PL_DBGOSH << "VertKDT::min(" << min << "),max(" << max << ")" << std::endl;
#endif

		BBox lbox = vn->get_left()->get_bbox_search();
		BBox rbox = vn->get_right()->get_bbox_search();

		if (lbox.crossed(bbox) == true) {
#ifdef USE_DEPTH
			PL_DBGOSH << "VertKDT::search_recursive:left=" << vn->get_depth() << std::endl;
#endif
			search_recursive(vn->get_left(), bbox, vlist);
		}

		if (rbox.crossed(bbox) == true) {
#ifdef USE_DEPTH
			PL_DBGOSH << "VertKDT::search_recursive:right=" << vn->get_depth() << std::endl;
#endif
			search_recursive(vn->get_right(), bbox, vlist);
		}
#ifdef DEBUG_VTREE
	}
	catch(char *str) {
		std::cout << str;
	}
#endif
}


// private ////////////////////////////////////////////////////////////////////

#ifdef SQ_RADIUS
POLYLIB_STAT VertKDT::create(REAL_TYPE sqradius) {
#else
POLYLIB_STAT VertKDT::create(
	int		max_elem, 
	const BBox	bbox, 
	std::vector<Vertex*>*	vert_list
	//VertexList*	vert_list
	) {
#endif

		m_n_create++;
#if 0
		Vec3<REAL_TYPE> min=bbox.getPoint(0);
		Vec3<REAL_TYPE> max=bbox.getPoint(7);
		std::cout << __func__ << " min " <<min<<std::endl;
		std::cout << __func__ << " max " <<max<<std::endl;
#endif

		// std::cout<< "VertKDT create start" << std::endl;
		destroy();
		m_max_elements = max_elem;
		m_root = new VertKDTNode();
		m_root->set_bbox(bbox);
		m_root->set_axis(AXIS_X);
		//std::vector<PrivateTriangle*>::iterator itr;
		//	std::vector<Vertex*> tmp_vertexlist=vert_list->get_vertex_lists();

		std::vector<Vertex*>::const_iterator itr;

		for (itr = vert_list->begin();
			itr != vert_list->end();
			itr++) {
				// make a new vertex
				VertKDTElem* elm = NULL;
				elm = new VertKDTElem(*itr);
				//std::cout<< "VertKDT create 1" << std::endl;
				VertKDTNode* vnode = NULL;
				traverse(m_root, elm, &vnode);
				//std::cout<< "VertKDT create 2" << std::endl;

				// the vtx didn't find in the tree
				// add a new vertex
				if (vnode == NULL) {
					PL_ERROSH 
						<< "[ERROR]VertKDT::create():Can't find appropriate node" 
						<< std::endl;
					return PLSTAT_NODE_NOT_FIND;
				}

				// find node to add a new triangle
				vnode->set_element(elm);

				//std::cout<< "VertKDT create 4" << std::endl;
				// set bbox for search triangle
				//tmp off
				vnode->set_bbox_search(elm);

				// std::cout<< "VertKDT create 5" << std::endl;


				if (vnode->get_elements_num() > m_max_elements) {
					vnode->split(m_max_elements);

#ifdef DEBUG_VTREE
					// m_vnode.push_back(vnode->get_left());
					// m_vnode.push_back(vnode->get_right());

					this->m_vnode.push_back(vnode->get_left());
					this->m_vnode.push_back(vnode->get_right());
#endif
					//  std::cout<< "VertKDT create 6" << std::endl;

				}

		}

#ifdef USE_DEPTH
		m_root->dump_depth(0);
#endif

		//  std::cout<< "VertKDT create end" << std::endl;
		return PLSTAT_OK;
}

// private ////////////////////////////////////////////////////////////////////

void VertKDT::node_count(
	VertKDTNode			*parent, 
	unsigned int	*node_cnt, 
	unsigned int	*tri_cnt
	) {
		VertKDTNode	*vnode;

#ifdef DEBUG_VTREE
		PL_DBGOSH << "VertKDT::node_count1():" << *node_cnt << std::endl;
#endif
		if ((vnode = parent->get_left()) != NULL) {; 
		(*node_cnt)++;
		node_count(vnode, node_cnt, tri_cnt);
		}
		else {
			(*tri_cnt) += parent->get_elements_num();
		}
#ifdef DEBUG_VTREE
		PL_DBGOSH << "VertKDT::node_count2():" << *node_cnt << std::endl;
#endif
		if ((vnode = parent->get_right()) != NULL) {; 
		(*node_cnt)++;
		node_count(vnode, node_cnt, tri_cnt);
		}
}

}// end of namespace PolylibNS

