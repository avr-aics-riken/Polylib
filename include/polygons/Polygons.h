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

#ifndef polylib_polygons_h
#define polylib_polygons_h

#include <vector>
#include <map>
#include <iomanip>
#include "polygons/VTree.h"
#include "polygons/Vertex.h"
#include "polygons/DVertex.h"
#include "polygons/VertexList.h"
#include "polygons/Triangle.h"
#include "polygons/DVertexTriangle.h"
#include "common/tt.h"
#include "common/axis.h"
#include "common/Vec3.h"
#include "common/BBox.h"
#include "common/PolylibStat.h"
#include "common/PolylibCommon.h"

namespace PolylibNS {

template <typename T> class Triangle;
template <typename T> class PrivateTriangle;

////////////////////////////////////////////////////////////////////////////
///
/// クラス:Polygons
/// 三角形ポリゴン集合を管理する純粋仮想クラスです。
///
////////////////////////////////////////////////////////////////////////////
 template <typename T>
   class Polygons {
 public:
   ///
   /// コンストラクタ。
   ///
   Polygons(){};

   ///
   /// デストラクタ。
   ///
   virtual ~Polygons() = 0;

   ///
   /// 引数で与えられる三角形ポリゴンリストの複製を設定する。
   /// @param[in] trias 設定する三角形ポリゴンリスト。
   /// @attention オーバーロードメソッドあり。
   ///
   virtual void init(
		     const std::vector<PrivateTriangle<T>*>	*trias
		     ) = 0;

   /// 三角形ポリゴンリストを初期化し、引数で与えら
   /// れる三角形ポリゴンリストを設定する。
   /// 三角形ポリゴン用のメモリ領域は、TriMesh 内で新たに確保する。
   ///
   ///  @param[in] vertlist 設定する三角形ポリゴン頂点リスト。
   ///  @param[in] idlist 三角形のid。
   ///  @param[in] n_start_tri vertlistの頂点開始位置
   ///  @param[in] n_start_id idlistのid開始位置
   ///  @param[in] n_tri 加える三角形の数

   virtual void init(const T* vertlist,
		     const int* idlist,
		     const int n_start_tri,
		     const int n_start_id,
		     const unsigned int n_tri)=0;



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
			     )=0;




   ///
   /// 三角形ポリゴンリストに引数で与えられる三角形を追加する。
   ///
   ///  @param[in] trias 設定する三角形ポリゴンリスト。
   ///
   virtual void add(
		    const std::vector<PrivateTriangle<T>*>		*trias
		    ) = 0;


   /// 三角形ポリゴンリストに引数で与えられる三角形を追加する。
   ///  @param[in] vertlist 設定する三角形ポリゴン頂点リスト。
   ///  @param[in] idlist 三角形のid。
   ///  @param[in] n_start_tri vertlistの頂点開始位置
   ///  @param[in] n_start_id idlistのid開始位置
   ///  @param[in] n_tri 加える三角形の数

   virtual void add(const T* vertlist,
		    const int* idlist,
		    const int n_start_tri,
		    const int n_start_id,
		    const unsigned int n_tri)=0;


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
			    )=0;


   ///
   /// STLファイルを読み込みデータの初期化。
   ///
   ///  @param[in] fname	ファイル名とファイルフォーマットのmap。
   ///  @return	POLYLIB_STATで定義される値が返る。
   ///
   virtual POLYLIB_STAT import(
			       const std::map<std::string, std::string>	fname,
			       T scale = 1.0
			       ) = 0;

   ///
   /// Polygonsクラスに含まれる全ポリゴン情報からKD木を作成する。
   ///
   ///  @return	POLYLIB_STATで定義される値が返る。
   ///
   virtual POLYLIB_STAT build() = 0;

   ///
   /// Polygonsクラスに含まれる頂点情報KD木を作成。重複点を削除
   ///
   ///  @return	POLYLIB_STATで定義される値が返る。
   ///
   //   virtual void vtx_compaction() = 0;

   ///
   /// Polygonsクラスで保持する三角形ポリゴンの総数を返す。
   /// 
   ///  @return 三角形ポリゴンの総数。
   ///
   virtual int triangles_num() = 0;

   ///
   /// KD木探索により、指定矩形領域に含まれる三角形ポリゴンを抽出する。
   ///
   ///  @param[in] bbox	検索範囲を示す矩形領域。
   ///  @param[in] every	true:3頂点が全て検索領域に含まれるものを抽出。
   ///						false:1頂点でも検索領域に含まれるものを抽出。
   ///  @return	抽出したポリゴンリストのポインタ。
   ///  @attention MPIPolylib内でのみ利用するため、ユーザは使用しないで下さい。
   ///  @attention オーバーロードメソッドあり。
   ///
   virtual const std::vector<PrivateTriangle<T>*>* search(
							  BBox<T>	*bbox, 
							  bool	every
							  ) const = 0;

   ///
   /// KD木探索により、指定矩形領域に含まれるポリゴンを抽出する。
   ///
   ///  @param[in]  bbox			検索範囲を示す矩形領域。
   ///  @param[in]  every			true:3頂点が全て検索領域に含まれるものを抽出。
   ///								false:1頂点でも検索領域に含まれるものを抽出。
   ///  @param[in,out] tri_list	抽出した三角形ポリゴンリストへのポインタ。
   ///  @return	POLYLIB_STATで定義される値が返る。
   ///  @attention オーバーロードメソッドあり。
   ///
   virtual POLYLIB_STAT search(
			       BBox<T>							*bbox, 
			       bool							every, 
			       std::vector<PrivateTriangle<T>*>	*tri_list
			       ) const = 0;

   ///
   /// 線形探索により、指定矩形領域に含まれる三角形ポリゴンを抽出する。
   ///
   ///  @param[in] bbox	検索範囲を示す矩形領域。
   ///  @param[in] every	true:3頂点が全て検索領域に含まれるものを抽出。
   ///						false:1頂点でも検索領域に含まれるものを抽出。
   ///  @return	抽出したポリゴンリストのポインタ。
   ///  @attention MPIPolylib内でのみ利用するため、ユーザは使用しないで下さい。
   ///  @attention オーバーロードメソッドあり。
   ///
   virtual const std::vector<PrivateTriangle<T>*>* linear_search(
								 BBox<T>	*bbox, 
								 bool	every
								 ) const = 0;

   ///
   /// 線形探索により、指定矩形領域に含まれるポリゴンを抽出する。
   ///
   ///  @param[in]  bbox			検索範囲を示す矩形領域。
   ///  @param[in]  every			true:3頂点が全て検索領域に含まれるものを抽出。
   ///								false:1頂点でも検索領域に含まれるものを抽出。
   ///  @param[in,out] tri_list	抽出した三角形ポリゴンリストのポインタ。
   ///  @return	POLYLIB_STATで定義される値が返る。
   ///  @attention オーバーロードメソッドあり。
   ///
   virtual POLYLIB_STAT linear_search(
				      BBox<T>							*bbox, 
				      bool							every, 
				      std::vector<PrivateTriangle<T>*>	*tri_list
				      ) const = 0;

   ///
   /// KD木探索により、指定位置に最も近いポリゴンを検索する。
   ///
   ///  @param[in]     pos     指定位置
   ///  @return    検索されたポリゴン
   ///
   virtual const PrivateTriangle<T>* search_nearest(
						    const Vec3<T>&    pos
						    ) const = 0;

   ///
   /// 配下の全ポリゴンのm_exid値を指定値にする。
   ///
   /// @param[in]	id	指定値
   ///
   virtual POLYLIB_STAT set_all_exid(
				     const int id
				     ) const = 0;

   /// Vertex -> DVertex  へのリプレース
   ///
   /// @param[in] nscalar スカラーデータ数
   /// @param[in] nvector ベクトルデータ数

   virtual POLYLIB_STAT replace_DVertex(int nscalar,int nvector){
     return PLSTAT_OK;
   };

   /// Vertex -> DVertex  への準備
   ///
   /// @param[in] nscalar スカラーデータ数
   /// @param[in] nvector ベクトルデータ数

   virtual POLYLIB_STAT prepare_DVertex(int nscalar,int nvector){
     return PLSTAT_OK;
   };

	//
	/// DVertex 追加作成用
	/// 
	/// @param[in] v 頂点座標（３点）
	///  @return    polygonへのpointer
	///

   virtual DVertexTriangle<T>* add_DVertex_Triangle(Vec3<T>* v){

#define DEBUG
#ifdef DEBUG
  PL_DBGOSH << "Polygons::"<< __func__
 	    <<" v0 "<<v[0]
	    <<" v1 "<<v[1]
	    <<" v2 "<<v[2]
	    << std::endl;
#endif

     return NULL;
   }

   	//
	/// DVertex 追加作成後の重複頂点削除
	/// 
	///
  
   virtual void finalize_DVertex(){
     //do nothing?
   }





   //=======================================================================
   // Setter/Getter
   //=======================================================================
   ///
   /// 三角形ポリゴンのリストを取得。
   ///
   /// @return 三角形ポリゴンのリスト。
   ///
   std::vector<PrivateTriangle<T>*> *get_tri_list() const {
     return m_tri_list;
   }

   /// VertexListを取得。
   ///
   /// @return VertexList 頂点リストクラス
   ///
   VertexList<T>* get_vtx_list() const {
     return m_vertex_list;
   }



   ///
   /// VertexKD木クラスを取得。
   ///
   /// @return KD木クラス。
   ///
   virtual VertKDT<T> *get_vertkdt() const = 0;


   ///
   /// KD木クラスを取得。
   ///
   /// @return KD木クラス。
   ///
   virtual VTree<T> *get_vtree() const = 0;


   /// print_vertex
   /// test function for Vertex Class
   ///
   virtual void print_vertex(){
	  
     //  std::cout << __func__ << " begin"<<std::endl;
     /// codes for VertexList
     const std::vector<Vertex<T>*>* vl_ptr=m_vertex_list->get_vertex_lists();

     for(int i=0;i<vl_ptr->size();++i){
       //	  std::cout << __func__ << " begin"<<std::endl;
       Vertex<T> test=*((*vl_ptr)[i]);
       //	  std::cout << __func__ << " begin"<<std::endl;
       PL_DBGOS << __func__ <<" VertexList "<<i 
		<< " "<<test[AXIS_X] 
		<< " "<<test[AXIS_Y] 
		<< " "<<test[AXIS_Z] <<std::endl;;
     }

   }


   virtual void print_memory_size() const =0;

	
   ///
   /// TriMeshクラスが管理しているBoundingBoxを返す。
   ///
   virtual BBox<T> get_bbox() const=0;

    ///
    /// hasDVertex
    ///
    /// @return DVertex を持っているか
    ///
   virtual bool hasDVertex() const =0;

 private:
   ///
   /// 三角形ポリゴンリストの初期化。
   ///
   virtual void init_tri_list() = 0;

 protected:
   //=======================================================================
   // クラス変数
   //=======================================================================
   /// 三角形ポリゴンのリスト。
   std::vector<PrivateTriangle<T>*>	*m_tri_list;
   //	std::vector<Vertex<T>*>	*m_vtx_list;
   VertexList<T>	*m_vertex_list;
   T tolerance;
  
 };

///
/// デストラクタ
///  @attention 継承しているクラスから呼び出されるために必要。
///
template <typename T>
Polygons<T>::~Polygons() {}


} //namespace PolylibNS

#endif //polylib_polygons_h
