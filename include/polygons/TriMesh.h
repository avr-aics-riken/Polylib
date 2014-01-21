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
#include "polygons/Polygons.h"
#include "common/BBox.h"
#include <map>
#include <vector>
namespace PolylibNS {

  class VTree;
  class VertKDT;
  class DVertexManager;
  class VertexList;
  class DVertexTriangle;
  class PrivateTriangle;

  ////////////////////////////////////////////////////////////////////////////
  ///
  /// クラス:TriMesh
  /// 三角形ポリゴン集合を管理するクラス（KD木用に特化したクラス)。
  ///
  ////////////////////////////////////////////////////////////////////////////
  
class TriMesh : public Polygons {
  public:
    ///
    /// コンストラクタ。
    ///
    TriMesh();

    ///
    /// コンストラクタ。
    ///
    /// @param[in] tolerance 頂点同一性チェックの基準値
    TriMesh(REAL_TYPE tolerance);

    ///
    /// デストラクタ。
    ///
    virtual ~TriMesh();

    ///
    /// TriMeshクラスで管理する三角形ポリゴンリストを初期化し、引数で与えら
    /// れる三角形ポリゴンリストを設定する。
    /// 三角形ポリゴン用のメモリ領域は、Polylib内で新たに確保される。
    ///
    ///  @param[in] trias 設定する三角形ポリゴンリスト。
    ///
    void init(
	      const std::vector<PrivateTriangle*>	*trias
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

  void init(const REAL_TYPE* vertlist,
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

   virtual void init_dvertex(const REAL_TYPE* vertlist,
			     const int* idlist,
			     const REAL_TYPE* scalarlist,
			     const REAL_TYPE* vectorlist,
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
	     const std::vector<PrivateTriangle*>  *trias
	     );


    ///
    /// 三角形ポリゴンリストに引数で与えられる三角形の複製を追加する。
    ///
    ///  @param[in] vertlist 設定する三角形ポリゴン頂点リスト。
    ///  @param[in] idlist 三角形のid。
    ///  @param[in] n_start_tri vertlistの頂点開始位置
    ///  @param[in] n_start_id idlistのid開始位置
    ///  @param[in] n_tri 加える三角形の数

  void add(const REAL_TYPE* vertlist,
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

   virtual void add_dvertex(const REAL_TYPE* vertlist,
			     const int* idlist,
			     const REAL_TYPE* scalarlist,
			     const REAL_TYPE* vectorlist,
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
			REAL_TYPE scale = 1.0
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
    const std::vector<PrivateTriangle*>* search(
						   BBox	*bbox, 
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
			BBox							*bbox,
			bool							every,
			std::vector<PrivateTriangle*>	*tri_list
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
    const std::vector<PrivateTriangle*>* linear_search(
							  BBox	*q_bbox, 
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
			       BBox							*q_bbox, 
			       bool							every,
			       std::vector<PrivateTriangle*>	*tri_list
			       ) const;

    ///
    /// KD木探索により、指定位置に最も近いポリゴンを検索する。
    ///
    ///  @param[in]     pos     指定位置
    ///  @return 検索されたポリゴン
    ///
    const PrivateTriangle* search_nearest(
					     const Vec3<REAL_TYPE>&    pos
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

   virtual DVertexTriangle* add_DVertex_Triangle(Vec3<REAL_TYPE>* v);
   	//
	/// DVertex 追加作成後の重複頂点削除
	/// 
	///
  
   virtual void finalize_DVertex();

    //=======================================================================
    // Setter/Getter
    //=======================================================================
    ///
    /// TriMeshクラスが管理しているBoundingBoxを返す。
    ///
    BBox get_bbox() const ;

    ///
    /// KD木クラスを取得。
    ///
    /// @return KD木クラス。
    ///
    VertKDT *get_vertkdt() const;
    ///
    /// KD木クラスを取得。
    ///
    /// @return KD木クラス。
    ///
    VTree *get_vtree() const ;
    ///
    /// DVertexManager
    ///
    /// @return KD木クラス。
    ///
    DVertexManager* DVM() const;
    ///
    /// hasDVertex
    ///
    /// @return KD木クラス。
    ///
    bool hasDVertex() const ;

    virtual void print_memory_size() const ;

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
    void replace_vertex_list(VertexList *vlist);


    /// 
    /// 重複頂点の削除
    ///
	//    virtual void vtx_compaction();
    void vtx_compaction();


    //=======================================================================
    // クラス変数
    //=======================================================================
    /// 全三角形ポリゴンを外包するBoundingBox。
    BBox	m_bbox;

    /// KD木クラス。
    VTree	*m_vtree;

    /// KD木クラス。
    VertKDT	*m_vertKDT;

    // DVertexManager
    DVertexManager* m_DVM_ptr;

    /// MAX要素数。
    int		m_max_elements;
    /// 2 点の同一性チェックのtolerance
    REAL_TYPE m_tolerance ;

};// end of class


} //namespace PolylibNS


#endif  // polylib_trimesh_h
