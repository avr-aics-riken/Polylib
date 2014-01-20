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

#ifndef polylib_trimesh_h
#define polylib_trimesh_h

#include <vector>
#include <map>
#include <algorithm>
#include "file_io/TriMeshIO.h"
#include "polygons/VertKDT.h"
#include "polygons/DVertexManager.h"
#include "polygons/DVertex.h"
#include "polygons/Triangle.h"
#include "polygons/DVertexTriangle.h"

#define M_MAX_ELEMENTS 15	/// VTreeのノードが持つ最大要素数

namespace PolylibNS {

  template <typename T> class Polygons;
  template <typename T> class VTree;
  template <typename T> class PrivateTriangle;
  template <typename T> class VertexList;
  template <typename T> class VertKDT;
  class DVertexManager;
  template <typename T> class DVertex;

  template <typename T> struct PrivTriaLess;
  template <typename T> struct PrivTriaEqual;


  ////////////////////////////////////////////////////////////////////////////
  ///
  /// クラス:TriMesh
  /// 三角形ポリゴン集合を管理するクラス（KD木用に特化したクラス)。
  ///
  ////////////////////////////////////////////////////////////////////////////
  template <typename T>
  class TriMesh : public Polygons<T> {
  public:
    ///
    /// コンストラクタ。
    ///
    TriMesh();

    ///
    /// コンストラクタ。
    ///
    /// @param[in] tolerance 頂点同一性チェックの基準値
    TriMesh(T tolerance);

    ///
    /// デストラクタ。
    ///
    ~TriMesh();

    ///
    /// TriMeshクラスで管理する三角形ポリゴンリストを初期化し、引数で与えら
    /// れる三角形ポリゴンリストを設定する。
    /// 三角形ポリゴン用のメモリ領域は、Polylib内で新たに確保される。
    ///
    ///  @param[in] trias 設定する三角形ポリゴンリスト。
    ///
    void init(
	      const std::vector<PrivateTriangle<T>*>	*trias
	      );


    ///
    /// TriMeshクラスで管理する三角形ポリゴンリストを初期化し、引数で与えら
    /// れる三角形ポリゴンリストを設定する。
    /// 三角形ポリゴン用のメモリ領域は、TriMesh 内で新たに確保する。
    ///
    ///  @param[in] vertlist 設定する三角形ポリゴン頂点リスト。
    ///  @param[in] idlist 三角形のid。
    ///  @param[in] n_start_tri vertlistの頂点開始位置
    ///  @param[in] n_start_id idlistのid開始位置
    ///  @param[in] n_tri 加える三角形の数

  void init(const T* vertlist,
	    const int* idlist,
	    const int n_start_tri,
	    const int n_start_id,
	    const unsigned int n_tri);


   /// 三角形ポリゴンリストを初期化し、引数で与えら
   /// れる三角形ポリゴンリストを設定する。
   /// 三角形ポリゴン用のメモリ領域は、TriMesh 内で新たに確保する。
   ///
   ///  @param[in] vertlist 設定する三角形ポリゴン頂点リスト。
   ///  @param[in] idlist 三角形のid。
   ///  @param[in] scalarlist 設定するスカラーデータのリスト
   ///  @param[in] vectorlist 設定するベクターデータのリスト
   ///  @param[in] n_start_tri vertlistの頂点開始位置
   ///  @param[in] n_start_id idlistのid開始位置
   ///  @param[in] n_start_scalar scalarlistの開始位置
   ///  @param[in] n_start_vector vectorlistの開始位置
   ///  @param[in] n_tri 加える三角形の数
   ///  @param[in] n_scalar 頂点あたりのスカラーデータの数
   ///  @param[in] n_vector 頂点あたりのベクターデータの数

   virtual void init_dvertex(const T* vertlist,
			     const int* idlist,
			     const T* scalarlist,
			     const T* vectorlist,
			     const int n_start_tri,
			     const int n_start_id,
			     const int n_start_scalar,
			     const int n_start_vector,
			     const unsigned int n_tri,
			     const int n_scalar,
			     const int n_vector
			     );



    ///
    /// 三角形ポリゴンリストに引数で与えられる三角形の複製を追加する。
    ///
    /// @param[in] trias	設定する三角形ポリゴンリスト。
    /// @attention m_idが重複するインスタンスは追加されない。
    /// @attention KD木の再構築は行わない。
    ///
    void add(
	     const std::vector<PrivateTriangle<T>*>  *trias
	     );


    ///
    /// 三角形ポリゴンリストに引数で与えられる三角形の複製を追加する。
    ///
    ///  @param[in] vertlist 設定する三角形ポリゴン頂点リスト。
    ///  @param[in] idlist 三角形のid。
    ///  @param[in] n_start_tri vertlistの頂点開始位置
    ///  @param[in] n_start_id idlistのid開始位置
    ///  @param[in] n_tri 加える三角形の数

  void add(const T* vertlist,
	    const int* idlist,
	    const int n_start_tri,
	    const int n_start_id,
	    const unsigned int n_tri);



  ///
  /// 三角形ポリゴンリストに引数で与えられる三角形(DVertexTriangle)を追加する。
  ///
   ///  @param[in] vertlist 設定する三角形ポリゴン頂点リスト。
   ///  @param[in] idlist 三角形のid。
   ///  @param[in] scalarlist 設定するスカラーデータのリスト
   ///  @param[in] vectorlist 設定するベクターデータのリスト
   ///  @param[in] n_start_tri vertlistの頂点開始位置
   ///  @param[in] n_start_id idlistのid開始位置
   ///  @param[in] n_start_scalar scalarlistの開始位置
   ///  @param[in] n_start_vector vectorlistの開始位置
   ///  @param[in] n_tri 加える三角形の数
   ///  @param[in] n_scalar 頂点あたりのスカラーデータの数
   ///  @param[in] n_vector 頂点あたりのベクターデータの数

   virtual void add_dvertex(const T* vertlist,
			     const int* idlist,
			     const T* scalarlist,
			     const T* vectorlist,
			     const int n_start_tri,
			     const int n_start_id,
			     const int n_start_scalar,
			     const int n_start_vector,
			     const unsigned int n_tri,
			     const int n_scalar,
			     const int n_vector
			     );

    ///
    /// ファイルからデータの初期化。
    ///
    ///  @param[in] fmap	ファイル名、ファイルフォーマット。
    ///  @return PLSTAT_OK=成功/false=失敗
    ///
    POLYLIB_STAT import(
			const std::map<std::string, std::string> fmap,
			T scale = 1.0
			);

    ///
    /// Polygonsクラスに含まれる全ポリゴン情報からKD木を作成する。
    ///
    ///  @return	POLYLIB_STATで定義される値が返る。
    ///
    virtual POLYLIB_STAT build();

    ///
    /// TriMeshクラスが管理している三角形ポリゴン数を返す。
    ///
    int triangles_num();

    ///
    /// KD木探索により、指定矩形領域に含まれる三角形ポリゴンを抽出する。
    ///
    ///  @param[in] bbox	検索範囲を示す矩形領域。
    ///  @param[in] every	true:3頂点が全て検索領域に含まれるものを抽出。
    ///						false:1頂点でも検索領域に含まれるものを抽出。
    ///  @return	抽出したポリゴンリストのポインタ。
    ///	 @attention	三角形ポリゴンのメモリ領域は新たにPolylib内で確保される。
    ///  @attention MPIPolylib内での利用が目的なので、ユーザは使用しないこと。
    ///  @attention	オーバーロードメソッドあり。
    ///
    const std::vector<PrivateTriangle<T>*>* search(
						   BBox<T>	*bbox, 
						   bool	every
						   ) const;



    ///
    /// KD木探索により、指定矩形領域に含まれるポリゴンを抽出する。
    ///
    ///  @param[in]		bbox		検索範囲を示す矩形領域
    ///  @param[in]		every		true:3頂点が全て検索領域に含まれるものを抽出。
    ///								false:1頂点でも検索領域に含まれるものを抽出。
    ///  @param[in,out] tri_list	抽出した三角形ポリゴンリストへのポインタ。
    ///  @return	POLYLIB_STATで定義される値が返る。
    ///  @attention	tri_listで戻される三角形ポリゴンのポインタは、Polylib内で
    ///				保持されるアドレス値なので、ユーザはdeleteしないで下さい。
    ///  @attention	オーバーロードメソッドあり。
    ///
    POLYLIB_STAT search(
			BBox<T>							*bbox,
			bool							every,
			std::vector<PrivateTriangle<T>*>	*tri_list
			) const;

    ///
    /// 線形探索により、指定矩形領域に含まれる三角形ポリゴンを抽出する。
    ///
    ///  @param[in] q_bbox	検索範囲を示す矩形領域。
    ///  @param[in] every	true:3頂点が全て検索領域に含まれるものを抽出。
    ///						false:1頂点でも検索領域に含まれるものを抽出。
    ///  @return	抽出したポリゴンリストのポインタ。
    ///	 @attention	三角形ポリゴンのメモリ領域は新たにPolylib内で確保される。
    ///  @attention MPIPolylib内での利用が目的なので、ユーザは使用しないこと。
    ///
    const std::vector<PrivateTriangle<T>*>* linear_search(
							  BBox<T>	*q_bbox, 
							  bool	every
							  ) const;

    ///
    /// 線形探索により、指定矩形領域に含まれるポリゴンを抽出する。
    ///
    ///  @param[in]		q_bbox		検索範囲を示す矩形領域。
    ///  @param[in]		every		true:3頂点が全て検索領域に含まれるものを抽出。
    ///								false:1頂点でも検索領域に含まれるものを抽出。
    ///  @param[in,out] tri_list	抽出した三角形ポリゴンリストへのポインタ。
    ///  @return	POLYLIB_STATで定義される値が返る。
    ///  @attention	tri_listで戻される三角形ポリゴンのポインタは、Polylib内で
    ///				保持されるアドレス値なので、ユーザはdeleteしないで下さい。
    ///  @attention	オーバーロードメソッドあり。
    ///
    POLYLIB_STAT linear_search(
			       BBox<T>							*q_bbox, 
			       bool							every,
			       std::vector<PrivateTriangle<T>*>	*tri_list
			       ) const;

    ///
    /// KD木探索により、指定位置に最も近いポリゴンを検索する。
    ///
    ///  @param[in]     pos     指定位置
    ///  @return 検索されたポリゴン
    ///
    const PrivateTriangle<T>* search_nearest(
					     const Vec3<T>&    pos
					     ) const;

    ///
    /// 配下の全ポリゴンのm_exid値を指定値にする。
    ///
    /// @param[in]	id	指定値
    ///
    POLYLIB_STAT set_all_exid(
			      const int	id
			      ) const;

    
   /// Vertex -> DVertex  へのリプレース
   ///
   /// @param[in] nscalar スカラーデータ数
   /// @param[in] nvector ベクトルデータ数

   virtual POLYLIB_STAT replace_DVertex(int nscalar,int nvector);

   /// Vertex -> DVertex  への準備
   ///
   /// @param[in] nscalar スカラーデータ数
   /// @param[in] nvector ベクトルデータ数

   virtual POLYLIB_STAT prepare_DVertex(int nscalar,int nvector);


	//
	/// DVertex 追加作成用
	/// 
	/// @param[in] v 頂点座標（３点）
	///  @return    polygonへのpointer
	///

   virtual DVertexTriangle<T>* add_DVertex_Triangle(Vec3<T>* v);
   	//
	/// DVertex 追加作成後の重複頂点削除
	/// 
	///
  
   virtual void finalize_DVertex(){
     if(this->m_vertex_list!=NULL && this->m_tri_list!=NULL ){
       if(this->m_vertKDT!=NULL)      delete this->m_vertKDT;
       this->m_vertKDT = new VertKDT<T>(m_max_elements);
       this->m_vertex_list->setKDT(this->m_vertKDT);

       // for(int i =0 ;i<this->m_vertex_list->size();++i){
       // 	 PL_DBGOSH << "TriMesh::"<<__func__<< " before vec "<<i<<" "<< *(this->m_vertex_list->get_vertex_lists()->at(i))<<std::endl;
       // }
       // for(int i =0 ;i<this->m_tri_list->size();++i){
       // 	 PL_DBGOSH << "TriMesh::"<<__func__<< " before tri_id "<<i<<" "<< this->m_tri_list->at(i)->get_id()<<std::endl;
       // }

       vtx_compaction();
       // for(int i =0 ;i<this->m_vertex_list->size();++i){
       // 	 PL_DBGOSH << "TriMesh::"<<__func__<< " after vec "<<i<<" "<< *(this->m_vertex_list->get_vertex_lists()->at(i))<<std::endl;
       // }
       // for(int i =0 ;i<this->m_tri_list->size();++i){
       // 	 PL_DBGOSH << "TriMesh::"<<__func__<< " after tri_id "<<i<<" "<< this->m_tri_list->at(i)->get_id()<<std::endl;
       // }


       build();
     } else {
       PL_DBGOSH<<__func__<<" not performed!!!"<<std::endl;
     }
   }



    
    //=======================================================================
    // Setter/Getter
    //=======================================================================
    ///
    /// TriMeshクラスが管理しているBoundingBoxを返す。
    ///
    BBox<T> get_bbox() const {
      return m_bbox;
    }

    ///
    /// KD木クラスを取得。
    ///
    /// @return KD木クラス。
    ///
    VertKDT<T> *get_vertkdt() const {
      return m_vertKDT;
    }

    ///
    /// KD木クラスを取得。
    ///
    /// @return KD木クラス。
    ///
    VTree<T> *get_vtree() const {
      return m_vtree;
    }
    ///
    /// DVertexManager
    ///
    /// @return KD木クラス。
    ///
    DVertexManager* DVM() const {
      return m_DVM_ptr;
    }
    ///
    /// hasDVertex
    ///
    /// @return KD木クラス。
    ///
    bool hasDVertex() const {
      if(m_DVM_ptr==NULL) return false;
      return true;
    }

    virtual void print_memory_size() const {
      unsigned int memsize_vtree=m_vtree->memory_size();
      unsigned int memsize_vkdt=m_vertKDT->memory_size();
      unsigned int memsize_pt_list=
	(sizeof(PrivateTriangle<T>)+sizeof(PrivateTriangle<T>*))
	*(this->m_tri_list->size());
      unsigned int memsize_vt_list=(sizeof(Vertex<T>)+sizeof(Vertex<T>*))
	*(this->m_vertex_list->size());
      unsigned int memsize_all=
	memsize_vt_list+memsize_pt_list+memsize_vkdt+memsize_vtree;
      PL_DBGOSH<< "TriMesh<T>::"<<__func__<<std::endl;
      PL_DBGOSH<< "size of VertexList<T>      "<< memsize_vt_list<<std::endl;
      PL_DBGOSH<< "size of VertKDT<T>         "<<memsize_vkdt<<std::endl;
      PL_DBGOSH<< "size of PrivateTriangle<T> "<<memsize_pt_list<<std::endl;
      PL_DBGOSH<< "size of VTree<T>           "<<memsize_vtree<<std::endl;
      PL_DBGOSH<< "---------------------------------------------"<<std::endl;
      PL_DBGOSH<< "total size                 "<<memsize_all<<std::endl;
    }

  private:
    ///
    /// 三角形ポリゴンリストの初期化。
    ///
    void init_tri_list();

    ///
    /// 頂点リストの初期化。
    ///
    void init_vertex_list();

    ///
    /// 頂点リストのいれかえ
    ///
    void replace_vertex_list(VertexList<T> *vlist);


    /// 
    /// 重複頂点の削除
    ///
    //    virtual void vtx_compaction();
    void vtx_compaction();


    //=======================================================================
    // クラス変数
    //=======================================================================
    /// 全三角形ポリゴンを外包するBoundingBox。
    BBox<T>	m_bbox;

    /// KD木クラス。
    VTree<T>	*m_vtree;

    /// KD木クラス。
    VertKDT<T>	*m_vertKDT;

    // DVertexManager
    DVertexManager* m_DVM_ptr;

    /// MAX要素数。
    int		m_max_elements;
    /// 2 点の同一性チェックのtolerance
    T m_tolerance ;
  };

// public /////////////////////////////////////////////////////////////////////
template <typename T>
TriMesh<T>::TriMesh()
{
	m_vtree = NULL;
	this->m_tri_list = NULL;
	//this->m_vtx_list = NULL;
	this->m_vertKDT = NULL;
	//std::cout << __func__ << " VertKDT is deleting "<<this-> m_vertKDT<< std::endl;
	this->m_vertex_list = NULL;
	m_max_elements = M_MAX_ELEMENTS;
	m_tolerance = 1.0e-10; // tempolary
	m_DVM_ptr=NULL;
}
// public /////////////////////////////////////////////////////////////////////
template <typename T>
TriMesh<T>::TriMesh(T tolerance)
{
	m_vtree = NULL;
	this->m_tri_list = NULL;
	//this->m_vtx_list = NULL;
	this->m_vertex_list = NULL;
	m_vertKDT = NULL;
	//std::cout << __func__ << " VertKDT is deleting "<<this-> m_vertKDT<< std::endl;
	m_max_elements = M_MAX_ELEMENTS;
	m_tolerance = tolerance;
	m_DVM_ptr=NULL;
}

// public /////////////////////////////////////////////////////////////////////
template <typename T>
TriMesh<T>::~TriMesh()
{
	delete m_vtree;
	if (this->m_tri_list != NULL) {
	  typename std::vector<PrivateTriangle<T>*>::iterator itr;
		for (itr = this->m_tri_list->begin(); itr != this->m_tri_list->end(); itr++) {
			delete *itr;
		}
		this->m_tri_list->clear();
	}

	delete this->m_tri_list;

	// if (this->m_vtx_list != NULL) {
	//   typename std::vector<Vertex<T>*>::iterator itr;
	// 	for (itr = this->m_vtx_list->begin(); itr != this->m_vtx_list->end(); itr++) {
	// 		delete *itr;
	// 	}
	// 	this->m_vtx_list->clear();
	// }

	// delete this->m_vtx_list;

	if (this->m_vertex_list != NULL) {
	  delete this->m_vertex_list;
	}

	if(m_DVM_ptr!=NULL){
	  delete m_DVM_ptr;
	}

}


// public /////////////////////////////////////////////////////////////////////
template <typename T>
  void TriMesh<T>::init(const T* vertlist,
			const int* idlist,
			const int n_start_tri,
			const int n_start_id,
			const unsigned int n_tri)
{
  //#define DEBUG
#ifdef DEBUG
  PL_DBGOSH << "TriMesh<T>::"<<__func__<< " in."<<std::endl;
#endif


  init_tri_list();
  init_vertex_list();
#ifdef DEBUG
  PL_DBGOSH << "TriMesh<T>::"<<__func__<< " end of init tri_list and vert_list"<<std::endl;
  PL_DBGOSH <<"test "<<idlist[n_start_id]<<" "<< idlist[n_start_id+1]<<std::endl;
#endif

  for(int i=0;i<n_tri;++i) {
    int id=n_start_tri+i*9;
    Vertex<T>* vtx_tri[3];
    for(int j=0;j<3;++j){
      Vertex<T>* v=new Vertex<T>(vertlist[id+j*3],vertlist[id+j*3+1],vertlist[id+j*3+2]);
      
      // vtx_tri[j]=this->m_vertex_list->vtx_add_KDT(v);
      //if(vtx_tri[j]!=v) delete v;
      //はじめに、重複を許して作成する。
      this->m_vertex_list->vtx_add_nocheck(v);
      vtx_tri[j]=v;
    }
    //int id2=n_start_id+i;
    //PrivateTriangle<T>* tri=new PrivateTriangle<T>(vtx_tri,idlist[id2]);
    PrivateTriangle<T>* tri=new PrivateTriangle<T>(vtx_tri,idlist[n_start_id+i]);
    this->m_tri_list->push_back(tri);
  }

  //最後に。頂点重複をチェックして、ポリゴン情報内のポインタも付け替える。
  //  POLYLIB_STAT status = TriMeshIO::vtx_compaction<T>(this->m_vertex_list,
  //this->m_tri_list);

  //  POLYLIB_STAT status = vtx_compaction();

  vtx_compaction();

#ifdef DEBUG
  PL_DBGOSH << "TriMesh<T>::"<<__func__<< " end of vtx_comaction"<<std::endl;
#endif
  //#undef DEBUG
 }

// public /////////////////////////////////////////////////////////////////////
template <typename T>
  void TriMesh<T>::init_dvertex(const T* vertlist,
				const int* idlist,
				const T* scalarlist,
				const T* vectorlist,
				const int n_start_tri,
				const int n_start_id,
				const int n_start_scalar,
				const int n_start_vector,
				const unsigned int n_tri,
				const int n_scalar,
				const int n_vector
				)

{
  //#define DEBUG
#ifdef DEBUG
  PL_DBGOSH << "TriMesh<T>::"<<__func__<< " in."<<std::endl;
#endif

  int scalarindex=0;
  int vectorindex=0;


  init_tri_list();
  init_vertex_list();
#ifdef DEBUG
  PL_DBGOSH << "TriMesh<T>::"<<__func__<< " end of init tri_list and vert_list"<<std::endl;
  PL_DBGOSH <<"test "<<idlist[n_start_id]<<" "<< idlist[n_start_id+1]<<std::endl;
#endif

  for(int i=0;i<n_tri;++i) {
    int id=n_start_tri+i*9;
    DVertex<T>* vtx_tri[3];
    for(int j=0;j<3;++j){
      DVertex<T>* dv=new DVertex<T>(m_DVM_ptr);
      Vec3<T> vec(vertlist[id+j*3],vertlist[id+j*3+1],vertlist[id+j*3+2]);

      Vertex<T>* v=dv;

      *v=vec;
      for(int iscalar=0;iscalar<m_DVM_ptr->nscalar();iscalar++){
	dv->set_scalar(iscalar,
		       scalarlist[n_start_scalar+scalarindex]);
	scalarindex++;
      }
      for(int ivector=0;ivector<m_DVM_ptr->nvector();ivector++){
	Vec3<T> data_vec(vectorlist[n_start_scalar+scalarindex],
			 vectorlist[n_start_scalar+scalarindex+1],
			 vectorlist[n_start_scalar+scalarindex+2]);
	dv->set_vector(ivector,data_vec);
	vectorindex+=3;
      }

      // vtx_tri[j]=this->m_vertex_list->vtx_add_KDT(v);
      //if(vtx_tri[j]!=v) delete v;
      //はじめに、重複を許して作成する。
      this->m_vertex_list->vtx_add_nocheck(v);
      vtx_tri[j]=dv;
    }
    //int id2=n_start_id+i;
    //PrivateTriangle<T>* tri=new PrivateTriangle<T>(vtx_tri,idlist[id2]);
    DVertexTriangle<T>* dtri=new DVertexTriangle<T>(vtx_tri,idlist[n_start_id+i]);
    PrivateTriangle<T>* tri=dtri;
    this->m_tri_list->push_back(tri);
  }

  //最後に。頂点重複をチェックして、ポリゴン情報内のポインタも付け替える。
  //  POLYLIB_STAT status = TriMeshIO::vtx_compaction<T>(this->m_vertex_list,
  //this->m_tri_list);

  //  POLYLIB_STAT status = vtx_compaction();

  vtx_compaction();

#ifdef DEBUG
  PL_DBGOSH << "TriMesh<T>::"<<__func__<< " end of vtx_comaction"<<std::endl;
#endif

 }



// public /////////////////////////////////////////////////////////////////////
 template <typename T>
  void TriMesh<T>::add_dvertex(const T* vertlist,
				const int* idlist,
				const T* scalarlist,
				const T* vectorlist,
				const int n_start_tri,
				const int n_start_id,
				const int n_start_scalar,
				const int n_start_vector,
				const unsigned int n_tri,
				const int n_scalar,
				const int n_vector
				)
  {

    //#define DEBUG
#ifdef DEBUG
    PL_DBGOSH << "TriMesh::"<<__func__<< " in."<<this<<std::endl;
    PL_DBGOSH << "TriMesh::"<<__func__<< " in."<<this->m_tri_list<<std::endl;
    PL_DBGOSH << "TriMesh::"<<__func__<< " in."<<this->m_vertex_list<<std::endl;
#endif
  int scalarindex=0;
  int vectorindex=0;

  if(n_tri==0) return;// PLSTAT_OK; // do nothing

    if (this->m_tri_list == NULL) {
      PL_DBGOSH << "TriMesh::"<<__func__<< " create vector<PrivateTrianle<T>> ? right?"<<std::endl;
      this->m_tri_list = new std::vector<PrivateTriangle<T>*>;
    }
    if (this->m_vertex_list == NULL) {
    PL_DBGOSH << "TriMesh::"<<__func__<< " create VertexList ? right?"<<std::endl;
       this->m_vertex_list = new VertexList<T>;
    }
    
#ifdef DEBUG
    PL_DBGOSH << "TriMesh::"<<__func__<< " create vertex and triangle nvtx_before "<< this->m_vertex_list->size()<<std::endl;
#endif

    for(int i=0;i<n_tri;++i) {
      int id=n_start_tri+i*9;
      DVertex<T>* vtx_tri[3];
      for(int j=0;j<3;++j){
	DVertex<T>* dv=new DVertex<T>(m_DVM_ptr);
	Vec3<T> vec(vertlist[id+j*3],
	    vertlist[id+j*3+1],
	    vertlist[id+j*3+2]);

	Vertex<T> * v=dv;
	*v=vec;
#ifdef DEBUG
	PL_DBGOSH << "TriMesh::"<<__func__<< " going to add vertex ("<< vec <<")"<<std::endl;
#endif


      for(int iscalar=0;iscalar<m_DVM_ptr->nscalar();iscalar++){
	dv->set_scalar(iscalar,
		       scalarlist[n_start_scalar+scalarindex]);
	scalarindex++;
      }
      for(int ivector=0;ivector<m_DVM_ptr->nvector();ivector++){
	Vec3<T> data_vec(vectorlist[n_start_scalar+vectorindex],
			 vectorlist[n_start_scalar+vectorindex+1],
			 vectorlist[n_start_scalar+vectorindex+2]);
	
#ifdef DEBUG
	PL_DBGOSH << "TriMesh::"<<__func__
		  << " " << i
		  << " " << j
 		  << " " << ivector
		  <<" ("<< data_vec <<")"<<std::endl;
#endif
	
	dv->set_vector(ivector,data_vec);
	vectorindex+=3;
      }



	//vtx_tri[j]=this->m_vertex_list->vtx_add_KDT(v);
	//if(vtx_tri[j]!=v) delete v;
	
	this->m_vertex_list->vtx_add_nocheck((Vertex<T>*)dv);
	vtx_tri[j]=dv;
      }
      int id2=n_start_id+i;
      DVertexTriangle<T>* tri=new DVertexTriangle<T>(vtx_tri,idlist[id2]);
      //      PrivateTriangle<T>* tri=new PrivateTriangle<T>(vtx_tri,idlist[id2]);
      this->m_tri_list->push_back((PrivateTriangle<T>*)tri);
    }
    

#ifdef DEBUG
    PL_DBGOSH << "TriMesh::"<<__func__<< " create vertex and triangle nvtx_after "<< this->m_vertex_list->size()<<std::endl;
    for(int i =0 ;i<this->m_vertex_list->size();++i){
      PL_DBGOSH << "TriMesh::"<<__func__<< " vec "<<i<<" "<< *(this->m_vertex_list->get_vertex_lists()->at(i))<<std::endl;
    }
    for(int i =0 ;i<this->m_tri_list->size();++i){
      PL_DBGOSH << "TriMesh::"<<__func__<< " tri_id "<<i<<" "<< this->m_tri_list->at(i)->get_id()<<std::endl;
    }
#endif




#ifdef DEBUG
    PL_DBGOSH << "TriMesh::"<<__func__<< " sort"<<std::endl;
#endif

    // 三角形リストをID順にソート
    std::sort( this->m_tri_list->begin(),
	       this->m_tri_list->end(),
	       PrivTriaLess<T>() );
    
#ifdef DEBUG
    PL_DBGOSH << "TriMesh::"<<__func__<< " erase"<<std::endl;
#endif


    //DVertexの場合は行わない。
    // ID重複ぶんを削除
    // this->m_tri_list->erase(
    // 			    std::unique(this->m_tri_list->begin(), this->m_tri_list->end(), PrivTriaEqual<T>()),
    // 			    this->m_tri_list->end());

// #ifdef DEBUG
//     PL_DBGOSH << "TriMesh::"<<__func__<< " create vertex and triangle nvtx_after "<< this->m_vertex_list->size()<<std::endl;
//     for(int i =0 ;i<this->m_vertex_list->size();++i){
//       PL_DBGOSH << "TriMesh::"<<__func__<< " vec "<<i<<" "<< *(this->m_vertex_list->get_vertex_lists()->at(i))<<std::endl;
//     }
//     for(int i =0 ;i<this->m_tri_list->size();++i){
//       PL_DBGOSH << "TriMesh::"<<__func__<< " tri_id "<<i<<" "<< this->m_tri_list->at(i)->get_id()<<std::endl;
//     }
// #endif


#ifdef DEBUG
    PL_DBGOSH << "TriMesh::"<<__func__<< " end."<<std::endl;
#endif
    //#undef DEBUG
  }






// public /////////////////////////////////////////////////////////////////////
template <typename T>
void TriMesh<T>::init(const std::vector<PrivateTriangle<T>*>* trias)
{
  //#define DEBUG
#ifdef DEBUG
  PL_DBGOSH<< "TriMesh<T>::" << __func__ <<std::endl;
#endif
  init_tri_list();
#ifdef DEBUG
  PL_DBGOSH<< "TriMesh<T>::" << __func__ << " init_tri_list end"<<std::endl;
#endif

  VertKDT<T>* new_vertKDT=new VertKDT<T>(this->m_max_elements);
  VertexList<T>* new_vertex_list= new VertexList<T>(new_vertKDT,this->m_tolerance);
  //  std::map<Vertex<T>*,int> vmap;

  typename std::vector<PrivateTriangle<T>*>::const_iterator itr;

  //first correct vertices
  for (itr = trias->begin(); itr != trias->end(); itr++) {
    try{
      Vertex<T>** tmpvert=(*itr)->get_vertex();
      if(tmpvert==0) throw "tmpvert";
      //      PL_DBGOSH<< "TriMesh<T>::" << __func__ << " get_vertex "<< tmpvert <<std::endl;
      Vertex<T>* tmpvert_in[3];

      // make deep copy!
      Vertex<T>* tmp = new Vertex<T>(*(tmpvert[0]));
      //      PL_DBGOSH<< "TriMesh<T>::" << __func__ << " make new Vertex 0 "<< tmp <<std::endl;
      if(tmp==0) throw "tmp";
      new_vertex_list->vtx_add_nocheck(tmp);
      tmpvert_in[0]=tmp;
      Vertex<T>* tmp1 = new Vertex<T>(*(tmpvert[1]));
      //PL_DBGOSH<< "TriMesh<T>::" << __func__ << " make new Vertex 1 "<< tmp1 <<std::endl;
      if(tmp1==0) throw "tmp1";
      new_vertex_list->vtx_add_nocheck(tmp1);
      tmpvert_in[1]=tmp1;
      Vertex<T>* tmp2 = new Vertex<T>(*(tmpvert[2]));
      //PL_DBGOSH<< "TriMesh<T>::" << __func__ << " make new Vertex 2 "<< tmp2 <<std::endl;
      if(tmp2==0) throw "tmp2";
      new_vertex_list->vtx_add_nocheck(tmp2);
      tmpvert_in[2]=tmp2;

      // new_vertex_list->vtx_add_nocheck(tmpvert[0]);
      // new_vertex_list->vtx_add_nocheck(tmpvert[1]);
      // new_vertex_list->vtx_add_nocheck(tmpvert[2]);
      /* vmap[tmpvert[0]]=1; */
      /* vmap[tmpvert[1]]=1; */
      /* vmap[tmpvert[2]]=1; */
      //    PrivateTriangle<T>* tri=new PrivateTriangle<T>( (*itr)->get_vertex(),
      PrivateTriangle<T>* tri=new PrivateTriangle<T>( tmpvert_in,
						      (*itr)->get_normal(), 
						      (*itr)->get_area(),
						      (*itr)->get_id()
						      );
      this->m_tri_list->push_back(tri);
    }
    catch (char* str) {
      std::cerr << "TriMesh::"<<__func__<<" error in "<<str<<std::endl;
    }
  }

#ifdef DEBUG
  PL_DBGOSH<< "TriMesh<T>::" << __func__ << " create tempolary VertexList and tri_list"<<std::endl;
#endif


  /* init_vertex_list(); */
  /* m_vertKDT=new_vertKDT; */
  /* this->m_vertex_list=new_vertex_list; */
  replace_vertex_list(new_vertex_list);



  //  POLYLIB_STAT status = TriMeshIO::vtx_compaction<T>(new_vertex_list, this->m_tri_list);
  //  POLYLIB_STAT status = vtx_compaction();
  vtx_compaction();

  //#undef DEBUG
}


// public /////////////////////////////////////////////////////////////////////
// std::sort用ファンクタ
template <typename T>
struct PrivTriaLess{
	bool operator()( const PrivateTriangle<T> *l, const PrivateTriangle<T> *r ) const
	{
		return l->get_id() < r->get_id();
	}
};
// std::equal用ファンクタ
template <typename T>
struct PrivTriaEqual{
	bool operator()( const PrivateTriangle<T> *l, const PrivateTriangle<T> *r ) const
	{
		return l->get_id() == r->get_id();
	}
};

 
// public /////////////////////////////////////////////////////////////////////
 template <typename T>
 void TriMesh<T>::add(const T* vertlist,
		      const int* idlist,
		      const int n_start_tri,
		      const int n_start_id,
		      const unsigned int n_tri)
   {
#ifdef DEBUG
   PL_DBGOSH << "TriMesh::add start"<<std::endl;
#endif
     if (this->m_tri_list == NULL) {
       this->m_tri_list = new std::vector<PrivateTriangle<T>*>;
     }
     if (this->m_vertex_list == NULL) {
       this->m_vertex_list = new VertexList<T>;
     }
#ifdef DEBUG
     PL_DBGOSH << "TriMesh::add VertexList and PrivateTriangle is ready."<<std::endl;
#endif

     for(int i=0;i<n_tri;++i) {
       int id=n_start_tri+i*9;
       Vertex<T>* vtx_tri[3];
       for(int j=0;j<3;++j){
	 Vertex<T>* v=new Vertex<T>(vertlist[id+j*3],vertlist[id+j*3+1],vertlist[id+j*3+2]);


	 //vtx_tri[j]=this->m_vertex_list->vtx_add_KDT(v);
	 //if(vtx_tri[j]!=v) delete v;
	 this->m_vertex_list->vtx_add_nocheck(v);
	 /* PL_DBGOSH << "TriMesh::add Triangle. vtx "<< j << " " <<v<<std::endl; */
	 /* PL_DBGOSH << "TriMesh::add Triangle. vtx "<< j << " " <<*v<<std::endl; */
	 //   なくなっていた１行
	 vtx_tri[j]=v;
	 //   なくなっていた１行

       }
       int id2=n_start_id+i;

       if(vtx_tri[0]!=NULL &&vtx_tri[1]!=NULL &&vtx_tri[2]!=NULL){
	 //PL_DBGOSH << __func__  << " Vertex pointer is checked."<<std::endl;
       } else {
	 PL_ERROSH << __func__ 
		   << " NULL pointer "<< vtx_tri[0]
		   <<" "<< vtx_tri[1]
		   <<" "<< vtx_tri[2]<<std::endl;
       } 

       PrivateTriangle<T>* tri=new PrivateTriangle<T>(vtx_tri,idlist[id2]);

       /* PL_DBGOSH << "TriMesh::add Triangle. triangle "<< i  */
       /* 		 << " id "<<idlist[id2]<<std::endl; */

       this->m_tri_list->push_back(tri);

       /* PL_DBGOSH << "TriMesh::add Triangle. triangle "<< i <<std::endl; */
       /* // */
     }
     
     //PL_DBGOSH << "TriMesh::add Triangle. add vtx"<<std::endl;

     // 三角形リストをID順にソート
     std::sort( this->m_tri_list->begin(), this->m_tri_list->end(), PrivTriaLess<T>() );
#ifdef DEBUG
     PL_DBGOSH << "TriMesh::add Triangle. sort"<<std::endl;
#endif
     // Comment
     // Vertex および PrivateTriangle が delete されていない。
     // 2013-Jan.-10th T. Suzuki

#ifdef DEBUG     
     for(int i=0;i<this->m_tri_list->size();++i){
       PL_DBGOSH << " before id "<<(*this->m_tri_list)[i]->get_id() <<std::endl;
     }
#endif
     
     // ID重複ぶんを削除
     this->m_tri_list->erase(
			     std::unique(this->m_tri_list->begin(), 
					 this->m_tri_list->end(), 
					 PrivTriaEqual<T>()),
			     this->m_tri_list->end());

#ifdef DEBUG
     for(int i=0;i<this->m_tri_list->size();++i){
       PL_DBGOSH << " after id "<<(*this->m_tri_list)[i]->get_id() <<std::endl;
     }
     PL_DBGOSH << "TriMesh::add end"<<std::endl;
#endif

   }


// public /////////////////////////////////////////////////////////////////////
template <typename T>
void
TriMesh<T>::add(
	const std::vector<PrivateTriangle<T>*> *trias
)
{
  //#define DEBUG
#ifdef DEBUG
  PL_DBGOSH << "TriMesh<T>::add_triangles() in." << std::endl;
#endif
	unsigned int i;

	if (this->m_tri_list == NULL) {
	  this->m_tri_list = new std::vector<PrivateTriangle<T>*>;
	}
	if (this->m_vertex_list == NULL) {
	  this->m_vertex_list = new VertexList<T>;
	}

	// ひとまず全部追加
	for( i=0; i<trias->size(); i++ ) {
		this->m_tri_list->push_back( new PrivateTriangle<T>( *(trias->at(i)) ) );
	}

	// 三角形リストをID順にソート
	std::sort( this->m_tri_list->begin(), this->m_tri_list->end(), PrivTriaLess<T>() );

	// ID重複ぶんを削除
	this->m_tri_list->erase(
	std::unique(this->m_tri_list->begin(), this->m_tri_list->end(), PrivTriaEqual<T>()),
		this->m_tri_list->end());
#ifdef DEBUG
  PL_DBGOSH << "TriMesh<T>::add_triangles() in." << std::endl;
#endif

#undef DEBUG
}

// public /////////////////////////////////////////////////////////////////////
template <typename T>
POLYLIB_STAT TriMesh<T>::import(const std::map<std::string, std::string> fmap, T scale)
{

  //      PL_DBGOSH << __func__ << " scale "<< scale <<std::endl;

	init_tri_list();
	//PL_DBGOSH << __func__ << " scale 1 "<< scale <<std::endl;
	init_vertex_list();
	//PL_DBGOSH << __func__ << " scale 2 "<< scale <<std::endl;

	POLYLIB_STAT ret = TriMeshIO::load<T>(this->m_vertex_list,
					      this->m_tri_list, fmap, scale);
	if(ret!=PLSTAT_OK) return ret;

	vtx_compaction();		

	return ret;

	//return TriMeshIO::load<T>(this->m_tri_list, fmap, scale);
}


// public /////////////////////////////////////////////////////////////////////
template <typename T>
POLYLIB_STAT TriMesh<T>::build()
{
  //#define DEBUG
#ifdef DEBUG
  PL_DBGOSH << "TriMesh::build() start" << std::endl;
#endif // DEBUG
  
  BBox<T> bbox;

  //	BBox<T> bbox=this->m_vertex_list->get_bbox();

	// now bbox is taken from VertKDT.


// #ifdef DEBUG
//   PL_DBGOSH << "TriMesh::build() test" << std::endl;
//   BBox<T> vtx_bbox=this->m_vertex_list->get_bbox();
//   PL_DBGOSH << "TriMesh::build() vertex_list bbox min"
// 	    << vtx_bbox.min<<"max"<< vtx_bbox.max <<std::endl;

//   PL_DBGOSH << "TriMesh::build() vertex_list 0 1 2 "
// 	    << this->m_vertex_list->ith(0)<<" "
// 	    << this->m_vertex_list->ith(1)<<" "
// 	    << this->m_vertex_list->ith(2)<<std::endl;
// #endif // DEBUG
  int counter=0;
	typename std::vector<PrivateTriangle<T>*>::iterator itr;
	/// TriMeshクラスに含まれる全三角形ポリゴンを外包するBoundingBoxを計算
	bbox.init();
	for (itr = this->m_tri_list->begin(); itr != this->m_tri_list->end(); itr++) {
	  counter++;
#ifdef DEBUG
	  PL_DBGOSH << "TriMesh::build() polygon loop " << counter << std::endl;
#endif // DEBUG 


	  //const Vec3<T>* vtx_arr = (*itr)->get_vertex();

	  Vertex<T>** vtx_arr= (*itr)->get_vertex();
#ifdef DEBUG
  PL_DBGOSH << "TriMesh::build() polygon get_vertex "
	    << vtx_arr[0]<<" "
	    << vtx_arr[1]<<" "
	    << vtx_arr[2]<< std::endl;
#endif // DEBUG

  for (int i = 0; i < 3; i++) {
#ifdef DEBUG
	  PL_DBGOSH << "TriMesh::build() vertex normal " << (*(vtx_arr[i]))[0] <<std::endl;
	  PL_DBGOSH << "TriMesh::build() vertex normal " << (*(vtx_arr[i]))[1] <<std::endl;
	  PL_DBGOSH << "TriMesh::build() vertex normal " << (*(vtx_arr[i]))[2] <<std::endl;
#endif // DEBUG

	  //   if( ( (*(vtx_arr[i]))[0] > 1000) || ( (*(vtx_arr[i]))[1] > 1000) ||( (*(vtx_arr[i]))[2] > 1000) ){
	  // PL_DBGOSH << "TriMesh::build() vertex error " << (*(vtx_arr[i]))[0] <<std::endl;
	  // PL_DBGOSH << "TriMesh::build() vertex error " << (*(vtx_arr[i]))[1] <<std::endl;
	  // PL_DBGOSH << "TriMesh::build() vertex error " << (*(vtx_arr[i]))[2] <<std::endl;
	  //     }


	  bbox.add(  (Vec3<T>) (*(vtx_arr[i])) );
	  }

	}

	m_bbox = bbox;
//#define DEBUG
#ifdef DEBUG
	Vec3<T> min = m_bbox.getPoint(0);
	Vec3<T> max = m_bbox.getPoint(7);
	PL_DBGOSH << "TriMesh::build:min=(" <<min<< "),max=(" <<max<< ")" << std::endl;
#endif
	//PL_DBGOSH << "TriMesh::build:min=(" <<m_bbox.min<< "),max=(" <<m_bbox.max<< ")" << std::endl;

	//#undef DEBUG
	// 木構造作成
	if (m_vtree != NULL) delete m_vtree;
	m_vtree = new VTree<T>(m_max_elements, m_bbox, this->m_tri_list);
	// if (m_vertKDT!=NULL) delete m_vertKDT;
	// m_vertKDT = new VertKDT<T>(m_max_elements, m_bbox, this->m_vertex_list);
#ifdef DEBUG
	PL_DBGOSH << "TriMesh::build: end of vtree " << std::endl;
#endif

	return PLSTAT_OK;
	//#undef DEBUG
}

// public /////////////////////////////////////////////////////////////////////
template <typename T>
int TriMesh<T>::triangles_num() {
	if (this->m_tri_list == NULL)		return 0;
	else						return this->m_tri_list->size();
}

// public /////////////////////////////////////////////////////////////////////
template <typename T>
const std::vector<PrivateTriangle<T>*> *TriMesh<T>::search(
	BBox<T>	*bbox, 
	bool	every
) const {
  //#define DEBUG
#ifdef DEBUG
	Vec3<T> min = bbox->getPoint(0);
	Vec3<T> max = bbox->getPoint(7);
	PL_DBGOSH << "TriMesh::search:min=(" <<min<< "),max=(" <<max<< ")" << std::endl;
#endif

	return m_vtree->search(bbox, every);
	//#undef DEBUG
}

// public /////////////////////////////////////////////////////////////////////
template <typename T>
POLYLIB_STAT TriMesh<T>::search(
	BBox<T>						*bbox, 
	bool						every, 
	std::vector<PrivateTriangle<T>*>	*tri_list
) const {
	return m_vtree->search(bbox, every, tri_list);
}

// public /////////////////////////////////////////////////////////////////////
template <typename T>
const std::vector<PrivateTriangle<T>*>* TriMesh<T>::linear_search(
	BBox<T>	*q_bbox, 
	bool	every
) const {
	std::vector<PrivateTriangle<T>*>		   *tri_list = new std::vector<PrivateTriangle<T>*>;
	typename std::vector<PrivateTriangle<T>*>::iterator itr;

	for (itr = this->m_tri_list->begin(); itr != this->m_tri_list->end(); itr++) {
		BBox<T> bbox;
		bbox.init();
		//const Vec3<T>* vtx_arr = (*itr)->get_vertex();
		Vertex<T>** vtx_arr = (*itr)->get_vertex();
		for (int i = 0; i < 3; i++) {
		  bbox.add( (Vec3<T>) *vtx_arr[i]);
		}
		if (every == true) {
		  if (q_bbox->contain( (Vec3<T>) *vtx_arr[0]) == true && 
		      q_bbox->contain( (Vec3<T>) *vtx_arr[1]) == true &&
		      q_bbox->contain( (Vec3<T>) *vtx_arr[2]) == true)
			{
				tri_list->push_back(*itr);
			}
		}
		else {
#ifdef OLD_DEF
			if (bbox.crossed(*q_bbox) == true				||
				bbox.contain(q_bbox->getPoint(0)) == true	||
				q_bbox->crossed(bbox) == true				||
				q_bbox->contain(bbox.getPoint(0)) == true) {
#else
			if (bbox.crossed(*q_bbox) == true) {
#endif
				tri_list->push_back(*itr);
			}
		}
	}
	return tri_list;
}

// public /////////////////////////////////////////////////////////////////////
template <typename T>
POLYLIB_STAT TriMesh<T>::linear_search(
	BBox<T>						*q_bbox, 
	bool						every, 
	std::vector<PrivateTriangle<T>*>	*tri_list
) const {
	if (tri_list == NULL) return PLSTAT_ARGUMENT_NULL;

	typename std::vector<PrivateTriangle<T>*>::iterator itr;

	for (itr = this->m_tri_list->begin(); itr != this->m_tri_list->end(); itr++) {
		BBox<T> bbox;
		bbox.init();
		//const Vec3<T>* vtx_arr = (*itr)->get_vertex();
		Vertex<T>** vtx_arr = (*itr)->get_vertex();

		for (int i = 0; i < 3; i++) {
		  bbox.add( (Vec3<T>) *vtx_arr[i]);
		}
		if (every == true) {
		  if (q_bbox->contain( (Vec3<T>) *vtx_arr[0]) == true	&&
		      q_bbox->contain( (Vec3<T>) *vtx_arr[1]) == true	&&
		      q_bbox->contain( (Vec3<T>) *vtx_arr[2]) == true) {
		    tri_list->push_back(*itr);
#ifdef DEBUG
			PL_DBGOSH << "TriMesh<T>::linear_search:IN TRUE" << std::endl;
			PL_DBGOSH << "     vertex 0:" << (Vec3<T>) *vtx_arr[0] << std::endl;
			PL_DBGOSH << "     vertex 1:" << (Vec3<T>) *vtx_arr[1] << std::endl;
			PL_DBGOSH << "     vertex 2:" << (Vec3<T>) *vtx_arr[2] << std::endl;
#endif
			}
		}
		else {
#ifdef OLD_DEF
			if (bbox.crossed(*q_bbox) == true				||
				q_bbox->crossed(bbox) == true				||
				bbox.contain(q_bbox->getPoint(0)) == true	||
				q_bbox->contain(bbox.getPoint(0)) == true) {
#else
			if (bbox.crossed(*q_bbox) == true) {
#endif
				tri_list->push_back(*itr);
#ifdef DEBUG
				PL_DBGOSH << "TriMesh::linear_search:IN FALSE" << std::endl;
#endif
			}
		}
#ifdef DEBUG
		for (int i=0; i<8; i++) {
			PL_DBGOSH << "TriMesh::linear_search:q_box[" << i << "]:" 
					  << q_bbox->getPoint(i) << std::endl;
		}
	    PL_DBGOSH << "TriMesh::linear_searc:" << " id:" << (*itr)->get_id()
				  << ",v(" << vtx_arr << ")" << std::endl;
#endif
	}
	return PLSTAT_OK;
}

// public /////////////////////////////////////////////////////////////////////
template <typename T>
const PrivateTriangle<T>* TriMesh<T>::search_nearest(
	const Vec3<T>&    pos
) const {
	return m_vtree->search_nearest(pos);
}

// public /////////////////////////////////////////////////////////////////////
template <typename T>
POLYLIB_STAT TriMesh<T>::set_all_exid(
	const int    id
) const {
	// 全ポリゴンのm_exidをidで上書き
  typename std::vector<PrivateTriangle<T>*>::iterator itr;
	for (itr = this->m_tri_list->begin(); itr != this->m_tri_list->end(); itr++) {
		(*itr)->set_exid( id );
	}
	return PLSTAT_OK;
}

// private ////////////////////////////////////////////////////////////////////
template <typename T>
void TriMesh<T>::init_tri_list()
{
  if (this->m_tri_list == NULL) {
    this->m_tri_list = new std::vector<PrivateTriangle<T>*>;

    
  }
  else {
    typename std::vector<PrivateTriangle<T>*>::iterator itr;
    for (itr = this->m_tri_list->begin();
	 itr != this->m_tri_list->end();
	 itr++) {
      delete *itr;
    }
    
    this->m_tri_list->clear();

  }
  if(m_vtree!=NULL) {
    delete m_vtree;
    m_vtree=NULL;
  }
  
}

// private ////////////////////////////////////////////////////////////////////
template <typename T>
void TriMesh<T>::init_vertex_list()
  {
    //#define DEBUG
#ifdef DEBUG
	PL_DBGOSH << __func__ << " in " <<std::endl;
#endif
    if (this->m_vertex_list == NULL) {
      //std::cout << __func__ << " VertexList is null"<< std::endl;
    } else {
      //std::cout << __func__ << " VertexList is deleting"<< std::endl;
      this->m_vertex_list->vtx_clear();
      delete this->m_vertex_list;

    }
#ifdef DEBUG
	PL_DBGOSH << __func__ << " clear vertex " <<std::endl;
#endif
    if (this->m_vertKDT == NULL) {
      //std::cout << __func__ << " VertKDT is null"<< std::endl;
    } else {
      //std::cout << __func__ << " VertKDT is deleting "<<this-> m_vertKDT<< std::endl;
      delete this->m_vertKDT;
    }
#ifdef DEBUG
	PL_DBGOSH << __func__ << " clear KDT " <<std::endl;
#endif

    this->m_vertKDT = new VertKDT<T>(m_max_elements);
    //std::cout << __func__ << " VertKDT end"<< std::endl;
    this->m_vertex_list = new VertexList<T>(this->m_vertKDT,this->m_tolerance);
    //std::cout << __func__ << " vertex end"<< std::endl;
    //this->m_vertex_list = new VertexList<T>;
#ifdef DEBUG
	PL_DBGOSH << __func__ << " end " <<std::endl;
#endif
	//#undef DEBUG
  }

// private ////////////////////////////////////////////////////////////////////
template <typename T>
  void TriMesh<T>::replace_vertex_list(VertexList<T>* vlist)
  {
    //#define DEBUG
#ifdef DEBUG
	PL_DBGOSH << __func__ << " in " <<std::endl;
#endif
    if (this->m_vertex_list == NULL) {
      //std::cout << __func__ << " VertexList is null"<< std::endl;
    } else {
      //std::cout << __func__ << " VertexList is deleting"<< std::endl;
      this->m_vertex_list->vtx_clear();
      delete this->m_vertex_list;
    }
#ifdef DEBUG
	PL_DBGOSH << __func__ << " clear vertex " <<std::endl;
#endif
    if (this->m_vertKDT == NULL) {
      //std::cout << __func__ << " VertKDT is null"<< std::endl;
    } else {
      //std::cout << __func__ << " VertKDT is deleting "<<this-> m_vertKDT<< std::endl;
      delete this->m_vertKDT;
    }
#ifdef DEBUG
	PL_DBGOSH << __func__ << " clear KDT " <<std::endl;
#endif

	//    m_vertKDT = new VertKDT<T>(m_max_elements);
    //std::cout << __func__ << " VertKDT end"<< std::endl;
    //    vlist->setKDT(m_vertKDT);
    this->m_vertex_list = vlist;
    m_vertKDT = vlist->getKDT();
    //    this->m_vertex_list = new VertexList<T>(this->m_vertKDT,this->m_tolerance);
    //std::cout << __func__ << " vertex end"<< std::endl;
    //this->m_vertex_list = new VertexList<T>;


#ifdef DEBUG
	PL_DBGOSH << __func__ << " end " <<std::endl;
#endif
	//#undef DEBUG
  }



// private ////////////////////////////////////////////////////////////////////
 template <typename T>
   void TriMesh<T>::vtx_compaction()
 {
   //#define DEBUG
#ifdef DEBUG
   PL_DBGOSH << "vtx_compaction" <<std::endl;
#endif

   std::map<Vertex<T>*,Vertex<T>*>* vtx_map=new  std::map<Vertex<T>*,Vertex<T>*>;
;
   this->m_vertex_list->vertex_compaction(vtx_map);
   typename std::vector<PrivateTriangle<T>*>::iterator itr;
   
#ifdef DEBUG
    int counter =0;
#endif
   for(itr=this->m_tri_list->begin();
       itr!=this->m_tri_list->end();
       itr++){

     Vertex<T>**  tmp_list=  (*itr)->get_vertex();
     bool vertex_replace=false;
#ifdef DEBUG
     PL_DBGOSH << "pointer "<<tmp_list[0]<< " "<<tmp_list[1]<<" "<<tmp_list[2]<<std::endl;
#endif
     for(int i=0;i<3;i++){
#ifdef DEBUG
       if(counter<10)
	 PL_DBGOSH << "pointer map old "<<tmp_list[i]<< " new "<<(*vtx_map)[tmp_list[i]]<<" "<<*(tmp_list[i])<<std::endl;
#endif
       if((*vtx_map)[tmp_list[i]]==tmp_list[i]){
	 // do nothing
       } else { // substitute 
#ifdef DEBUG
	 if(counter<10)
	   PL_DBGOSH << "pointer swaped old "<<tmp_list[i]<< " new "<<(*vtx_map)[tmp_list[i]]<<std::endl;
#endif
	 tmp_list[i]=(*vtx_map)[tmp_list[i]];
	 vertex_replace=true;
       }
       
     }
     if(vertex_replace){ //vertex replaced
#ifdef DEBUG
       PL_DBGOSH << "vertex replaced "<< *itr <<std::endl;
#endif

       (*itr)->set_vertexes(tmp_list,true,true);
       
       if((*itr)->get_area()==0){ //zero area check
	 PL_DBGOSH << __func__ 
		   << " Warning : polygon contains a triangle that its area is zero." << std::endl;
	 PL_DBGOSH <<  "vertex0 ("<< *(tmp_list[0]) <<")"<<std::endl;
	 PL_DBGOSH <<  "vertex1 ("<< *(tmp_list[1]) <<")"<<std::endl;
	 PL_DBGOSH <<  "vertex2 ("<< *(tmp_list[2]) <<")"<<std::endl;

       }
     } else {
       // vertex not replaced
       (*itr)->set_vertexes(tmp_list,false,false);
     }
     
#ifdef DEBUG
           counter++;
#endif
   }

   //#undef DEBUG
}
 //// public ///////////////////////////////
 template <typename T> 
   POLYLIB_STAT TriMesh<T>::replace_DVertex(int nscalar,int nvector){
   //#define DEBUG
#ifdef DEBUG
   PL_DBGOSH << __func__ << std::endl;
#endif

   m_DVM_ptr = new DVertexManager(nscalar,nvector);
   std::map<Vertex<T>*,Vertex<T>*> ptr_map;
#ifdef DEBUG
   PL_DBGOSH << __func__ << " 1"<<std::endl;
#endif

   VertexList<T>* new_dv=new VertexList<T>(this->m_tolerance);

#ifdef DEBUG
   PL_DBGOSH << __func__ << " 2"<<std::endl;
#endif
   
   const std::vector<Vertex<T>*>* old_vlist=this->m_vertex_list->get_vertex_lists();

#ifdef DEBUG
   PL_DBGOSH << __func__ << " 3"<<std::endl;
#endif


   for(typename std::vector<Vertex<T>*>::const_iterator itr = old_vlist->begin();

       itr != old_vlist->end();
       itr++){

#ifdef DEBUG
     PL_DBGOSH << __func__ << "3 1"<<std::endl;
#endif

     Vec3<T> pos((*(*itr))[0],(*(*itr))[1],(*(*itr))[2]);

#ifdef DEBUG
     PL_DBGOSH << __func__ << "3 2"<< pos <<std::endl;
#endif

     Vertex<T>* dv;
     DVertex<T>* tmp = new DVertex<T>(m_DVM_ptr) ;
     dv=tmp;

#ifdef DEBUG
     PL_DBGOSH << __func__ << " 3 3"<< std::endl;
#endif

     *dv=pos;

#ifdef DEBUG
     PL_DBGOSH << __func__ << "3 4"<<std::endl;
#endif

     new_dv->vtx_add_nocheck( dv);

#ifdef DEBUG
     PL_DBGOSH << __func__ << "3 5"<< std::endl;
#endif

     ptr_map.insert(std::pair<Vertex<T>*,Vertex<T>*>( (*itr) , dv) );

#ifdef DEBUG
     PL_DBGOSH << __func__ << "3 6"<<std::endl;
#endif

   }

#ifdef DEBUG
     PL_DBGOSH << __func__ << " 4"<<std::endl;
#endif

       typename std::vector<PrivateTriangle<T>*>* 
	 tmp_dvertex_tri_list=new std::vector<PrivateTriangle<T>*>;

   //  三角形リストの頂点ポインタ付け替え
     if (this->m_tri_list != NULL) {
#ifdef DEBUG
     PL_DBGOSH << __func__ << " 4 1"<<std::endl;
#endif


       typename std::vector<PrivateTriangle<T>*>::iterator itr;
       for (itr = this->m_tri_list->begin(); itr != this->m_tri_list->end(); itr++) {

#ifdef DEBUG
     PL_DBGOSH << __func__ << " 4 2"<<std::endl;
#endif

	 Vertex<T>** tmpvert=(*itr)->get_vertex();
	 DVertex<T>* tmpdvert[3];

#ifdef DEBUG
     PL_DBGOSH << __func__ << " 4 2 1"<<std::endl;
#endif
     
	 tmpvert[0]=ptr_map[tmpvert[0]];
	 tmpvert[1]=ptr_map[tmpvert[1]];
	 tmpvert[2]=ptr_map[tmpvert[2]];

#ifdef DEBUG
     PL_DBGOSH << __func__ << " 4 2 2"<<std::endl;
#endif
     
	 tmpdvert[0]=dynamic_cast<DVertex<T>*>(tmpvert[0]);
	 tmpdvert[1]=dynamic_cast<DVertex<T>*>(tmpvert[1]);
	 tmpdvert[2]=dynamic_cast<DVertex<T>*>(tmpvert[2]);

	 // (*itr)->set_vertexes(tmpvert,true,true);
	 //make new DVertexTriangle

#ifdef DEBUG
     PL_DBGOSH << __func__ << " 4 3"<<std::endl;
#endif

	 DVertexTriangle<T>* new_dv_tri= new DVertexTriangle<T>(tmpdvert,(*itr)->get_id());
#ifdef DEBUG
     PL_DBGOSH << __func__ << " 4 4"<<std::endl;
#endif

     tmp_dvertex_tri_list->push_back((PrivateTriangle<T>*)new_dv_tri);

       }


     }
#ifdef DEBUG
     PL_DBGOSH << __func__ << " 5"<<std::endl;
#endif


     
     init_vertex_list();
     this->m_vertex_list=new_dv;


     init_tri_list();
     this->m_tri_list= tmp_dvertex_tri_list;
       
     build();

#ifdef DEBUG
   PL_DBGOSH << __func__ << " end"<<std::endl;
#endif
   //#undef DEBUG
   return PLSTAT_OK;
 }
 /////////////////////////

 template <typename T> 
   POLYLIB_STAT TriMesh<T>::prepare_DVertex(int nscalar,int nvector){
   //#define DEBUG
#ifdef DEBUG
   PL_DBGOSH << __func__ << std::endl;
#endif
   init_tri_list();
   init_vertex_list();
   m_DVM_ptr = new DVertexManager(nscalar,nvector);
#ifdef DEBUG
   PL_DBGOSH << __func__ 
	     << " nscalar="<<m_DVM_ptr->nscalar()
	     << " nvector="<<m_DVM_ptr->nvector()
     //<< " size="<<m_DVM_ptr->size() 
	     << " this " <<this
	     << std::endl;
#endif
   //#undef DEBUG   



   return PLSTAT_OK;
 }
 /////////////////////////

 template <typename T>
   DVertexTriangle<T>* 
   TriMesh<T>::add_DVertex_Triangle(Vec3<T>* v){

   //#define DEBUG
#ifdef DEBUG
   PL_DBGOSH << "TriMesh::"<< __func__<<" "<<this<<std::endl;
  PL_DBGOSH << "TriMesh::"<< __func__
 	    <<" v0 "<<v[0]
	    <<" v1 "<<v[1]
	    <<" v2 "<<v[2]
	    << std::endl;
#endif

  //   Vertex<T>* vtx_list[3];   
  DVertex<T>* vtx_list[3];   
  //PL_DBGOSH << "TriMesh::"<< __func__<< " 1" << " this "<<this <<std::endl;
  for(int i=0;i<3;i++){
    Vertex<T>* dv;

#ifdef DEBUG
     if(m_DVM_ptr==NULL){
              PL_DBGOSH << __func__  << "m_DVM_ptr is NULL"<<std::endl;
     }else {
       PL_DBGOSH << __func__ 
		 << " nscalar="<<m_DVM_ptr->nscalar()
		 << " nvector="<<m_DVM_ptr->nvector()
	 //<< " size="<<m_DVM_ptr->size()
		 << std::endl;
     }
#endif


     //     PL_DBGOSH << "TriMesh::"<< __func__<< " 2 1 "<< m_DVM_ptr<<std::endl;
     DVertex<T>* tmp = new DVertex<T>(m_DVM_ptr) ;
     //PL_DBGOSH << "TriMesh::"<< __func__<< " 2 1 "<<*tmp<<std::endl;
     //PL_DBGOSH << "TriMesh::"<< __func__<< " 2 1 "<<tmp<<std::endl;
     dv=tmp;

       //   if(m_DVM_ptr->size()==4){
       // PL_DBGOSH << "TriMesh::"<< __func__<< " 2 1 1"<<std::endl;
       //     DVertex<T,float>* tmp = new DVertex<T,float>(m_DVM_ptr) ;
       // PL_DBGOSH << "TriMesh::"<< __func__<< " 2 1 1"<<std::endl;
       //   }   else if(m_DVM_ptr->size()==8){
       // PL_DBGOSH << "TriMesh::"<< __func__<< " 2 1 2"<<std::endl;
       //     DVertex<T,double>* tmp = new  DVertex<T,double>(m_DVM_ptr);
       // PL_DBGOSH << "TriMesh::"<< __func__<< " 2 1 2"<<std::endl;
       //     dv=tmp;
       //   }
     //PL_DBGOSH << "TriMesh::"<< __func__<< " 2 2 "<<std::endl;
       *dv=v[i];
       //PL_DBGOSH << "TriMesh::"<< __func__<< " 2 3 "<<std::endl;
   //   *tmp=*dv;
   //vtx_list[i]=dv;
   vtx_list[i]=tmp;

   //PL_DBGOSH << "TriMesh::"<< __func__<< " 2 4 "<<*tmp<<std::endl;
   if(this->m_vertex_list!=NULL)  this->m_vertex_list->vtx_add_nocheck((Vertex<T>*)tmp);
     else    PL_DBGOSH << "TriMesh::"<< __func__<< " null VertexList"<<std::endl;
   }
  //  PL_DBGOSH << "TriMesh::"<< __func__<< " VertexList ptr"<<this->m_vertex_list;
  //PL_DBGOSH << "TriMesh::"<< __func__<< "add tringle"<<std::endl;

   int n_tri=this->m_tri_list->size();

   DVertexTriangle<T>* ret=new DVertexTriangle<T>(vtx_list,n_tri);
   
   this->m_tri_list->push_back( (PrivateTriangle<T>*) ret);

   return ret;
   }

} //namespace PolylibNS

#endif  // polylib_trimesh_h
