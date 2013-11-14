#ifndef polylib_vertkdt_h
#define polylib_vertkdt_h
#include <vector>
#include "common/Vec3.h"
#include "common/BBox.h"
#include "common/PolylibCommon.h"
#include "common/PolylibStat.h"
#include "polygons/Vertex.h"
#include "polygons/VertexList.h"


namespace PolylibNS{


////////////////////////////////////////////////////////////////////////////
///
///  クラス:VertKDTElem
///  VertexKD木の要素クラスです。
///
////////////////////////////////////////////////////////////////////////////

  template <typename T>
  class VertKDTElem{
  public:
    /// 
    /// コンストラクタ。
    ///
    /// @param[in] 
    /// @attention ポインタを格納するが、参照のみ。deleteは行わない。
    /// 
    VertKDTElem(Vertex<T>* vertex){
      m_vertex=vertex;
    }

    //setter/getter
    ///
    /// return vertex.
    ///
    Vertex<T>* get_vertex(){
      return m_vertex;
    }

    //setter/getter

    ///
    /// return vertex.
    ///
    Vec3<T>* get_pos() const{
      // PL_DBGOSH << __func__ << " " <<(*m_vertex)[0]
      // 		<< " " <<(*m_vertex)[1]
      // 		<< " " <<(*m_vertex)[2]
      // 		<<std::endl;
      return (Vec3<T>*) m_vertex;
    }

  private:
    ///
    /// vertex
 ///
    Vertex<T>* m_vertex; 
  };


////////////////////////////////////////////////////////////////////////////
///
///  クラス:VertKDTNode
///  VertexKD木の要素クラスです。
///
////////////////////////////////////////////////////////////////////////////

  template <typename T>
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
    bool is_leaf() const {
      return m_left == 0;
    }
    
    ///
    /// BBoxの値を取得
    /// 
    /// @return bbox。
    ///
    BBox<T> get_bbox() const {
      return m_bbox;
    }

    ///
    /// BBoxの値を設定
    /// 
    /// @param[in] bbox。


    ///
    void set_bbox(const BBox<T>& bbox ) {
      // std::cout << "VertKDTNode::"<< __func__ <<std::endl;
      m_bbox=bbox;
      //  std::cout << "VertKDTNode::" << __func__ <<std::endl;
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
     void set_bbox_search(const VertKDTElem<T>* p) {
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
    VertKDTNode<T>* get_left() {
      return m_left;
    }
    
    ///
    /// 右のNodeを取得。
    ///
    /// @return 右のNode。
    ///
    VertKDTNode<T>* get_right() {
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
    std::vector<VertKDTElem<T>*>& get_vlist() {
      return m_vlist;
    }

    ///
    /// 木の要素を設定。
    ///
    /// @param[in] elm。
    ///
    void set_element(VertKDTElem<T>* elm) {
      
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

    ///
    ///　下位の left ノードを設定。
    ///
    void set_left_node(VertKDTNode<T>* lnode){
      m_left = lnode;
    }


    ///
    ///　下位の right ノードを設定。
    ///
    void set_right_node(VertKDTNode<T>* rnode){
      m_right = rnode;
    }



  private:

    //=======================================================================
    // クラス変数
    //=======================================================================
    /// 本ノードの左下側ノード。
    VertKDTNode<T>					*m_left;
    
    /// 本ノードの右下側ノード。
    VertKDTNode<T>					*m_right;
    
    /// KD木生成用のBouding Box。
    BBox<T>					m_bbox;
    
    /// KD木の軸の方向インデックス。
    AxisEnum				m_axis;
    
    /// ノードの管理する要素リスト。
    std::vector<VertKDTElem<T>*>	m_vlist;
    
    /// KD木検索用のBouding Box。
    BBox<T>					m_bbox_search;

  };

  ////////////////////////////////////////////////////////////////////////////
  ///
  /// クラス:VertKDT
  /// リーフをVertexとするKD木のクラスです。
  ///
  ////////////////////////////////////////////////////////////////////////////
  template <typename T>
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
	    const BBox<T> bbox, 
	    std::vector<Vertex<T>*>* vert_list);
	    //	    VertexList<T>* vert_list);
	    //	    VertexList<T>* vert_list);
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
    const Vertex<T>* search_nearest(
				    const Vec3<T>&    pos
				    ) const;

    ///
    /// KD木探索により、指定位置に最も近いVertexを検索する。
    ///
    ///  @param[in]     vn      検索対象のノードへのポインタ。
    ///  @param[in]     pos     指定位置
    ///  @return    検索されたVertex
    ///
    const Vertex<T>* search_nearest_recursive(
					      VertKDTNode<T>       *vn, 
					      const Vec3<T>&    pos
					      ) const;

    ///
    /// KD木クラスが利用しているメモリ量を返す。
    ///
    ///  @return	利用中のメモリ量(byte)
    ///
    unsigned int memory_size();


    ///root node のBBoxを返す
    BBox<T> get_root_bbox(){
      return m_root->get_bbox();
    }
    ///root node のBBoxを 設定する。
    void set_root_bbox(const BBox<T>& box){
      if(m_root==NULL) m_root=new VertKDTNode<T>;
      return m_root->set_bbox(box);
    }

    ///
    /// kd 木にvertexを追加する。
    ///
    /// @param[in] v 頂点クラス
    /// @return ステータス

    POLYLIB_STAT add(Vertex<T>* v);

    ///
    /// kd 木にvertexを追加する。
    /// BBoxにvが入っていなければ、make_upper を用いて再帰的に拡大する。
    /// 最後はadd を用いる。
    ///
    /// @param[in] v 頂点クラス
    /// @return ステータス

    POLYLIB_STAT add2(Vertex<T>* v);

    ///
    /// BBoxを拡大する。vが入ると思われる方向に拡大する。
    /// ある軸について、vがBBoxのminより小さい時、に左方向に拡大。
    /// それ以外は右方向へ拡大する。
    ///
    /// @param[in] v 頂点クラス
    /// @return ステータス

    POLYLIB_STAT make_upper(Vertex<T>* v);


    ///
    /// KD木　再構築用関数
    ///
    ///  @param[in] bbox		VertKDTのbox範囲。
    ///  @param[in] vertex_list	木構造の元になるVertexのリスト。
    ///  @return	POLYLIB_STATで定義される値が返る。
    ///
    POLYLIB_STAT create(
			const BBox<T>	bbox, 
			std::vector<Vertex<T>*>* vert_list
			){
      create(m_max_elements,bbox,vert_list);
    }


    // 
    int n_create(){return m_n_create;}


  private:
    ///
    /// vertexをKD木構造に組み込む際に、どのノードへ組み込むかを検索する。
    ///
    ///  @param[in]	vn 検索対象のノードへのポインタ。
    ///  @param[in] elm	組み込むVertex
    ///  @param[in,out] vnode 検索結果。
    ///
    void traverse(
		  VertKDTNode<T>	*vn, 
		  VertKDTElem<T>	*elm, 
		  VertKDTNode<T>	**vnode
		  ) const;

    ///
    /// vertexをKD木構造から検索する。
    ///
    ///  @param[in]		vn		検索対象のノードへのポインタ。
    ///  @param[in]		bbox	VertKDTのbox範囲。
    ///  @param[in,out]	vlist	検索結果配列へのポインタ。
    ///
    void search_recursive(
			  VertKDTNode<T>	*vn, 
			  const BBox<T>	&bbox, 
			  std::vector<VertKDTElem<T>*>	*vlist
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
			const BBox<T>	bbox, 
			//VertexList<T>* vert_list
			std::vector<Vertex<T>*>* vert_list
			);


    ///
    /// KD木の総ノード数と総ポリゴン数を数える。
    ///
    ///  @param[in]	 vnode		親ノードへのポインタ。
    ///  @param[out] node_cnt	ノード数。
    ///  @param[out] poly_cnt	ポリゴン数。
    ///
    void node_count(
		    VertKDTNode<T>	*vnode, 
		    unsigned int	*node_cnt, 
		    unsigned int	*poly_cnt
		    );

    //=======================================================================
    // クラス変数
    //=======================================================================
    /// ルートノードへのポインタ。
    VertKDTNode<T>	*m_root;

    /// リーフノードが所持できる最大要素数。
    int		m_max_elements;

    int m_n_create;
#ifdef DEBUG_VERTKDT
    std::vector<VertKDTNode<T>*> m_vnode;
#endif
  };


// VertKDTNode

// public /////////////////////////////////////////////////////////////////////
template <typename T>
VertKDTNode<T>::VertKDTNode()
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
VertKDTNode<T>::~VertKDTNode()
{
  //#define DEBUG
#ifdef DEBUG
	PL_DBGOSH << __func__ << " in " <<std::endl;
#endif

  typename std::vector<VertKDTElem<T>*>::iterator itr = m_vlist.begin();
	for (; itr != m_vlist.end(); itr++) {
		delete *itr;
	}
	m_vlist.clear();
	if (m_left!=NULL) {delete m_left; m_left=NULL;}
	if (m_right!=NULL){delete m_right; m_right=NULL;}
	//#undef DEBUG
}

// public /////////////////////////////////////////////////////////////////////
template <typename T>
void VertKDTNode<T>::split(const int& max_elem)
{

  m_left = new VertKDTNode<T>();
  m_right = new VertKDTNode<T>();

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

  typename std::vector<VertKDTElem<T>*>::const_iterator itr = m_vlist.begin();

  for (; itr != m_vlist.end(); itr++) {

    Vertex<T>* tmpvtx=(*itr)->get_vertex();
    Vec3<T> elempos=(Vec3<T>) *tmpvtx;
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
 template <typename T>
 VertKDT<T>::VertKDT(
		     int	max_elem, 
		     const BBox<T>	bbox, 
		     std::vector<Vertex<T>*>* vert_list) 
		       //		       VertexList<T>* vert_list) 
   {
     m_root = NULL;
     create(max_elem, bbox, vert_list);
   }
 
// public /////////////////////////////////////////////////////////////////////
 template <typename T>
   VertKDT<T>::~VertKDT()
   {
     destroy();
   }

// public /////////////////////////////////////////////////////////////////////
template <typename T>
void VertKDT<T>::destroy()
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
template <typename T>
unsigned int VertKDT<T>::memory_size() {
	VertKDTNode<T>			*vnode;
	unsigned int	node_cnt = 1;		// ノード数
	unsigned int	poly_cnt = 0;		// ポリゴン数
	unsigned int	size;

	if ((vnode = m_root->get_left()) != NULL) {; 
		node_count(vnode, &node_cnt, &poly_cnt);
	}
	//#define DEBUG_VTREE
#ifdef DEBUG_VTREE
	PL_DBGOSH << "VertKDT<T>::memory_size1():node,poly=" << node_cnt << "," 
			  << poly_cnt << std::endl;
#endif
		
	if ((vnode = m_root->get_right()) != NULL) {; 
		node_count(vnode, &node_cnt, &poly_cnt);
	}
#ifdef DEBUG_VTREE
	PL_DBGOSH << "VertKDT<T>::memory_size2():node,poly=" << node_cnt << "," 
			  << poly_cnt << std::endl;
#endif

	size  = sizeof(VertKDT<T>);
	size += sizeof(VertKDTNode<T>)	 * node_cnt;
	size += sizeof(VertKDTElem<T>) * poly_cnt;
	return size;
	//#undef DEBUG_VTREE
}

// public /////////////////////////////////////////////////////////////////////
template <typename T>
const Vertex<T>* VertKDT<T>::search_nearest(
	const Vec3<T>&	pos
) const {
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
template <typename T>
const Vertex<T>* VertKDT<T>::search_nearest_recursive(
	VertKDTNode<T>   *vn,
	const Vec3<T>&	pos
) const {
  //#define DEBUG

	if (vn->is_leaf()) {
		const Vertex<T>* tri_min = 0;
		//float dist2_min = 0.0;
		T dist2_min = 0.0;
 
		// ノード内のポリゴンから最も近い物を探す(リニアサーチ)
		typename std::vector<VertKDTElem<T>*>::const_iterator itr = vn->get_vlist().begin();
		for (; itr != vn->get_vlist().end(); itr++) {
			const Vertex<T>* tri = (*itr)->get_vertex();
			Vec3<T> c = (Vec3<T>) (*tri);
			T dist2 = (c - pos).lengthSquared();
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
		VertKDTNode<T> *vn1, *vn2;
		AxisEnum axis = vn->get_axis();
		if (pos[axis] < (vn->get_left()->get_bbox()).max[axis]) {
			vn1 = vn->get_left();
			vn2 = vn->get_right();
		} else {
			vn1 = vn->get_right();
			vn2 = vn->get_left();
		}
		const Vertex<T>* tri = search_nearest_recursive(vn1, pos);
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

template <typename T>  
POLYLIB_STAT VertKDT<T>::add2(Vertex<T>* v){
  //#define DEBUG
#ifdef DEBUG
	  PL_DBGOSH << "VertKDT<T>::"<<__func__ << " "<< *v << std::endl;    
#endif// DEBUG
  
   BBox<T> rootbbox=get_root_bbox();

  if(rootbbox.contain((Vec3<T>) *v)){
    return add(v);
  }else{

#ifdef DEBUG
	  PL_DBGOSH << "VertKDT<T>::"<<__func__ << " "<< *v << std::endl;    
#endif// DEBUG

    make_upper(v);
    return add2(v);
  }
  //#undef DEBUG
}
template <typename T>  
POLYLIB_STAT VertKDT<T>::make_upper(Vertex<T>* v){
  
  //#define DEBUG
#ifdef DEBUG
  PL_DBGOSH << "VertKDT<T>::"<<__func__ << " "<< *v<< std::endl;    
#endif// DEBUG

  BBox<T> rootbbox=get_root_bbox();
  Vec3<T> min=rootbbox.min;
  Vec3<T> max=rootbbox.max;
  Vec3<T> uppermin=rootbbox.min;
  Vec3<T> uppermax=rootbbox.max;
  Vec3<T> pairmin=rootbbox.min;
  Vec3<T> pairmax=rootbbox.max;

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
      << "[ERROR]VertKDT<T>::make_upper():Can't find axis."
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
  BBox<T> upperbbox(uppermin,uppermax);
  BBox<T> pairbbox(pairmin,pairmax);
  VertKDTNode<T>* new_root = new VertKDTNode<T>;
  VertKDTNode<T>* pair_node = new VertKDTNode<T>;


#ifdef DEBUG
  PL_DBGOSH << "VertKDT<T>::"<<__func__ << " min "<< min<< std::endl;    
  PL_DBGOSH << "VertKDT<T>::"<<__func__ << " max "<< max<< std::endl;    
  PL_DBGOSH << "VertKDT<T>::"<<__func__ << " uppermin "<< uppermin<< std::endl;    
  PL_DBGOSH << "VertKDT<T>::"<<__func__ << " uppermax "<< uppermax<< std::endl;    
  PL_DBGOSH << "VertKDT<T>::"<<__func__ << " pairmin "<< pairmin<< std::endl;    
  PL_DBGOSH << "VertKDT<T>::"<<__func__ << " pairmax "<< pairmax<< std::endl;    
  PL_DBGOSH << "VertKDT<T>::"<<__func__ << " length "<< rootbbox.length(upperaxis)<< std::endl;    
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

template <typename T>  
POLYLIB_STAT VertKDT<T>::add(Vertex<T>* v){

  //#define DEBUG
#ifdef DEBUG
  PL_DBGOSH <<"VertKDT::" <<__func__ << " start"<<std::endl;
#endif

    // make a new vertex
    VertKDTElem<T>* elm = NULL;
    elm = new VertKDTElem<T>(v);

#ifdef DEBUG
  PL_DBGOSH <<"VertKDT::" <<__func__ << " VertKDTElem made"<<std::endl;
#endif

    //std::cout<< "VertKDT add 1" << std::endl;
    VertKDTNode<T>* vnode = NULL;
    traverse(m_root, elm, &vnode);
    //std::cout<< "VertKDT add 2" << std::endl;

#ifdef DEBUG
  PL_DBGOSH <<"VertKDT::" <<__func__ << " traverse ended"<<std::endl;
#endif



    // the vtx didn't find in the tree
    // add a new vertex
    if (vnode == NULL) {
      PL_ERROSH
      	<< "[ERROR]VertKDT<T>::add():Can't find appropriate node"
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
template <typename T>
void VertKDT<T>::traverse(VertKDTNode<T>* vn, VertKDTElem<T>* elm, VertKDTNode<T>** vnode) const
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


	//Vec3<T> vtx = elm->pos();
	Vertex<T> *tmpvtx =elm->get_vertex();
	Vec3<T> vtx = (Vec3<T>) *tmpvtx;
#ifdef SQ_RADIUS
	//float& sqdist = q->m_sqdist;
	T& sqdist = q->m_sqdist;
#endif
	AxisEnum axis = vn->get_axis();
	//float x = vn->get_left()->get_bbox().max[axis];
	T x = vn->get_left()->get_bbox().max[axis];

#ifdef DEBUG 
	PL_DBGOSH<<"VertKDT::"<<__func__<<" traverse recursively "<<std::endl;
#endif

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
void VertKDT<T>::search_recursive(
	VertKDTNode<T>				*vn, 
	const BBox<T>			&bbox, 
	std::vector<VertKDTElem<T>*>	*vlist
) const {
#ifdef DEBUG_VTREE
try{
	PL_DBGOSH << "VertKDT<T>::search_recursive:@@@----------------------@@@" << std::endl;
#endif
	if (vn->is_leaf()) {
	  typename std::vector<VertKDTElem<T>*>::const_iterator itr = vn->get_vlist().begin();
		for (; itr != vn->get_vlist().end(); itr++) {
			// determine between bbox and 3 vertices of each triangle.

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

				// // determine between bbox and bbox crossed
				// BBox<T> e_bbox = (*itr)->get_bbox();

				// if (e_bbox.crossed(bbox) == true) {
				// 		vlist->push_back(*itr);
				// }

		}
#ifdef USE_DEPTH
		PL_DBGOSH << "VertKDT<T>::search_recursive:depth=" << vn->get_depth() 
				  << ",elem= " << vn->get_vlist().size() << std::endl;
#endif
#ifdef DEBUG_VTREE
	PL_DBGOSH << "VertKDT<T>::search_recursive:@@@-return---------------@@@" << std::endl;
#endif
		return;
	}

#ifdef DEBUG_VTREE
	Vec3<T> min = bbox.getPoint(0);
	Vec3<T> max = bbox.getPoint(7);
	PL_DBGOSH << "VertKDT<T>::min(" << min << "),max(" << max << ")" << std::endl;
#endif

	BBox<T> lbox = vn->get_left()->get_bbox_search();
	BBox<T> rbox = vn->get_right()->get_bbox_search();

	if (lbox.crossed(bbox) == true) {
#ifdef USE_DEPTH
		PL_DBGOSH << "VertKDT<T>::search_recursive:left=" << vn->get_depth() << std::endl;
#endif
		search_recursive(vn->get_left(), bbox, vlist);
	}

	if (rbox.crossed(bbox) == true) {
#ifdef USE_DEPTH
	PL_DBGOSH << "VertKDT<T>::search_recursive:right=" << vn->get_depth() << std::endl;
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
template <typename T>
#ifdef SQ_RADIUS
POLYLIB_STAT VertKDT<T>::create(T sqradius) {
#else
POLYLIB_STAT VertKDT<T>::create(
	int		max_elem, 
	const BBox<T>	bbox, 
	std::vector<Vertex<T>*>*	vert_list
	//VertexList<T>*	vert_list
) {
#endif

  m_n_create++;
#if 0
  Vec3<T> min=bbox.getPoint(0);
  Vec3<T> max=bbox.getPoint(7);
  std::cout << __func__ << " min " <<min<<std::endl;
  std::cout << __func__ << " max " <<max<<std::endl;
#endif

  // std::cout<< "VertKDT create start" << std::endl;
  destroy();
  m_max_elements = max_elem;
  m_root = new VertKDTNode<T>();
  m_root->set_bbox(bbox);
  m_root->set_axis(AXIS_X);
  //typename std::vector<PrivateTriangle<T>*>::iterator itr;
  //	std::vector<Vertex<T>*> tmp_vertexlist=vert_list->get_vertex_lists();
	
  typename std::vector<Vertex<T>*>::const_iterator itr;

  for (itr = vert_list->begin();
       itr != vert_list->end();
       itr++) {
    // make a new vertex
    VertKDTElem<T>* elm = NULL;
    elm = new VertKDTElem<T>(*itr);
    //std::cout<< "VertKDT create 1" << std::endl;
    VertKDTNode<T>* vnode = NULL;
    traverse(m_root, elm, &vnode);
    //std::cout<< "VertKDT create 2" << std::endl;

    // the vtx didn't find in the tree
    // add a new vertex
    if (vnode == NULL) {
      PL_ERROSH 
	<< "[ERROR]VertKDT<T>::create():Can't find appropriate node" 
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
template <typename T>
void VertKDT<T>::node_count(
	VertKDTNode<T>			*parent, 
	unsigned int	*node_cnt, 
	unsigned int	*tri_cnt
) {
	VertKDTNode<T>	*vnode;

#ifdef DEBUG_VTREE
	PL_DBGOSH << "VertKDT<T>::node_count1():" << *node_cnt << std::endl;
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
#endif //polylib_vertkdt_h
