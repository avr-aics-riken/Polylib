// -*- Mode: c++ -*-
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

#ifndef polylib_polygongroup_h
#define polylib_polygongroup_h

#include "common/PolylibCommon.h"
#include "common/PolylibStat.h"
#include "common/Vec3.h"
#include "TextParser.h"
#include <vector>
#include <map>

namespace PolylibNS {

class TryMesh;
class Polylib;
class Polygons;
class PolylibMoveParams;
class PrivateTriangle;
class DVertexTriangle;
class BBox;
class VertexList;
class VertKDT;
class VTree;

////////////////////////////////////////////////////////////////////////////
///
/// クラス:PolygonGroup
/// ポリゴングループを管理するクラスです。
///
////////////////////////////////////////////////////////////////////////////

class PolygonGroup {
public:
	///
	/// コンストラクタ
	///
	PolygonGroup();

	///
	/// コンストラクタ
	///
	/// @param[in] tolerance 頂点同一性チェックの基準値（距離）
	PolygonGroup(REAL_TYPE tolerance);

	///
	/// デストラクタ
	///
	virtual ~PolygonGroup();

	///
	/// 引数で与えられる三角形ポリゴンリストを複製し、KD木の生成を行う。
	///
	///  @param[in] tri_list	設定する三角形ポリゴンリスト。
	///  @param[in] clear		true:ポリゴン複製、面積計算、KD木生成を行う。
	///  						false:面積計算、KD木生成だけを行う。
	///  @return	POLYLIB_STATで定義される値が返る。
	///  @attention TriMeshクラスのinit()参照。オーバーロードメソッドあり。
	///
	POLYLIB_STAT init(
		const std::vector<PrivateTriangle*>		*tri_list, 
		bool									clear = true
		);


	///
	/// 引数で与えられる三角形ポリゴンリストを複製し、KD木の生成を行う。
	///
	///  @param[in] vertlist 設定する三角形ポリゴン頂点リスト。
	///  @param[in] idlist 三角形のid。
	///  @param[in] n_start_tri vertlistの頂点開始位置
	///  @param[in] n_start_id idlistのid開始位置
	///  @param[in] n_tri 加える三角形の数
	///  @return	POLYLIB_STATで定義される値が返る。
	///  @attention TriMeshクラスのinit()参照。オーバーロードメソッドあり。

	POLYLIB_STAT init(const REAL_TYPE* vertlist,
		const int* idlist,
		const int n_start_tri,
		const int n_start_id,
		const unsigned int n_tri);

	///
	/// 引数で与えられる三角形ポリゴンリスト(DVertexTrianle)を複製し、KD木の生成を行う。
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
	///  @return	POLYLIB_STATで定義される値が返る。
	///  @attention TriMeshクラスのinit()参照。オーバーロードメソッドあり。

	POLYLIB_STAT init_dvertex(const REAL_TYPE* vertlist,
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
	/// 引数で与えられる三角形ポリゴンリスト(DVertexTrianle)を作成する。
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
	///  @return	POLYLIB_STATで定義される値が返る。
	///  @attention TriMeshクラスのinit()参照。オーバーロードメソッドあり。

	POLYLIB_STAT add_dvertex(const REAL_TYPE* vertlist,
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


	//
	/// PolygonGroupツリーの作成。
	/// 設定ファイルの内容を再帰的に呼び出し、PolygonGroupツリーを作成する。
	///
	///  @param[in] polylib		Polygonクラスのインスタンス
	///  @param[in] parent		親グループ
	///  @param[in] tp 　　　　　　　　　　　　TextParser のインスタンス
	///  @return	POLYLIB_STATで定義される値が返る。
	///
	virtual POLYLIB_STAT build_group_tree(
		Polylib					*polylib,
		PolygonGroup			*parent,
		TextParser* tp
		);



	//
	/// PolygonGroupツリーの作成。DVertex 新規作成用
	/// name で表されたパスにポリゴンツリーを再帰的に作成する。
	/// 
	///
	///  @param[in] polylib Polygonクラスのインスタンス
	///  @param[in] parent	親グループ
	///  @param[in] path    作成するパス。作成するごとに上位のパスを消して再帰する。

	///  @return	POLYLIB_STATで定義される値が返る。
	///
	virtual POLYLIB_STAT build_group_tree(Polylib *polylib,
		PolygonGroup	*parent,
		std::string path
		);


	//
	/// DVertex 追加作成用
	/// 
	/// @param[in] nscalar スカラーデータ数
	/// @param[in] nvector ベクターデータ数
	///  @return	POLYLIB_STATで定義される値が返る。
	///

	POLYLIB_STAT replace_DVertex(int nscalar,int nvector);

	//
	/// DVertex 追加作成用
	/// 
	/// @param[in] nscalar スカラーデータ数
	/// @param[in] nvector ベクターデータ数
	///  @return	POLYLIB_STATで定義される値が返る。
	///

	POLYLIB_STAT prepare_DVertex(int nscalar,int nvector);

	//
	/// DVertex 追加作成用
	/// 
	/// @param[in] v 頂点座標（３点）
	///  @return	polygonへのpointer
	///

	DVertexTriangle* add_DVertex_Triangle(Vec3<REAL_TYPE>* v);


	//
	/// DVertex 追加作成後の重複頂点削除
	/// KD木の構築
	///

	void finalize_DVertex();



	///
	/// 三角形ポリゴンの法線ベクトルの計算、面積の計算、KD木の生成を行う。
	/// 三角形ポリゴンはTriMeshクラスが管理している。
	///
	///  @return	POLYLIB_STATで定義される値が返る。
	///  @attention	TriMeshクラスのbuild()参照。
	///
	POLYLIB_STAT build_polygon_tree();

	///
	/// STLファイルからポリゴン情報を読み込み、TriMeshクラスに登録する。
	///
	///  @return POLYLIB_STATで定義される値が返る。
	///  @attention TriMeshクラスのimport()参照。
	///
	POLYLIB_STAT load_stl_file(REAL_TYPE scale=1.0);

	///
	/// 三角形ポリゴンIDファイルからポリゴンIDを読み込み、m_internal_idに登録する。
	///
	///  @param[in] id_format	三角形IDファイルの入力形式。
	///  @return	POLYLIB_STATで定義される値が返る。
	///
	POLYLIB_STAT load_id_file(
		ID_FORMAT		id_format
		);

	///
	/// TriMeshクラスが管理しているポリゴン情報をSTLファイルに出力する。
	/// TextParser 対応版
	///  @param[in] rank_no	ファイル名に付加するランク番号。
	///  @param[in] extend	ファイル名に付加する自由文字列。
	///  @param[in] format	STLファイルフォーマット。
	///  @param[in,out] stl_fname_map stl ファイル名とポリゴングループのパス
	///  @return	POLYLIB_STATで定義される値が返る。
	///  @attention TriMeshIOクラスのsave()参照。オーバーロードメソッドあり。
	///
	POLYLIB_STAT save_stl_file(
		std::string		rank_no,
		std::string		extend,
		std::string		format,
		std::map<std::string,std::string>& stl_fname_map
		);

	///
	/// 三角形ポリゴンIDファイルにポリゴンIDを出力する。IDファイル名は、
	/// 階層化されたグループ名_ランク番号_自由文字列.id。
	///
	///  @param[in] rank_no		ファイル名に付加するランク番号。
	///  @param[in] extend		ファイル名に付加する自由文字列。
	///  @param[in] id_format	三角形IDファイルの出力形式。
	///  @return	POLYLIB_STATで定義される値が返る。
	/// 
	POLYLIB_STAT save_id_file(
		std::string 	rank_no,
		std::string		extend,
		ID_FORMAT		id_format
		);


	///
	/// 設定ファイルに出力するTextParserのリーフを編集する.
	/// デフォルトでは何もしない。
	/// CarGroup.cxx の例を参照.
	///
	///  @param[in] pointer to TextParser 
	///  @param[in] rank_no	ファイル名に付加するランク番号。
	///  @param[in] extend	ファイル名に付加する自由文字列。
	///  @param[in] format	STLファイルフォーマット。
	///  @return	POLYLIB_STATで定義される値が返る。
	///  @attention  do nothing by default
	///

	virtual POLYLIB_STAT mk_param_tag(
		TextParser* pt,
		std::string		rank_no,
		std::string		extend,
		std::string		format
		);


	///
	/// 三角形ポリゴン移動メソッド。virtual用の関数なので処理はない。
	///
	///  @param[in] params	Polylib.hで宣言しているパラメタセットクラス。
	///  @return	POLYLIB_STATで定義される値が返る。
	///
	virtual POLYLIB_STAT move(
		PolylibMoveParams	&params
		);

	///
	/// KD木探索により、指定矩形領域に含まれるポリゴンを抽出する。
	///
	///  @param[in] bbox	矩形領域。
	///  @param[in]	every	true:3頂点が全て検索領域に含まれるものを抽出。
	///  					false:1頂点でも検索領域に含まれるものを抽出。
	///  @return	抽出したポリゴンリストのポインタ。
	///  @attention オーバーロードメソッドあり。
	///
	const std::vector<PrivateTriangle*>* search(
		BBox	*bbox, 
		bool	every
		) const;

	///
	/// KD木探索により、指定矩形領域に含まれるポリゴンを抽出する。
	///
	///  @param[in]		bbox		矩形領域。
	///  @param[in]		every		true:3頂点が全て検索領域に含まれるものを抽出。
	///  							false:1頂点でも検索領域に含まれるものを抽出。
	///  @param[in,out] tri_list	抽出した三角形ポリゴンリストのポインタ。
	///  @return	POLYLIB_STATで定義される値が返る。
	///  @attention オーバーロードメソッドあり。
	///
	POLYLIB_STAT search(
		BBox							*bbox, 
		bool							every, 
		std::vector<PrivateTriangle*>	*tri_list
		) const;

	///
	/// 線形探索により、指定矩形領域に含まれるポリゴンを抽出する。
	///
	///  @param[in] bbox	矩形領域。
	///  @param[in]	every	true:3頂点が全て検索領域に含まれるものを抽出。
	///  					false:1頂点でも検索領域に含まれるものを抽出。
	///  @return	抽出したポリゴンリストのポインタ。
	///  @attention	オーバーロードメソッドあり。
	///
	const std::vector<PrivateTriangle*>* linear_search(
		BBox	*bbox, 
		bool	every
		) const;

	///
	/// 線形探索により、指定矩形領域に含まれるポリゴンを抽出する。
	///
	///  @param[in]		bbox		 矩形領域。
	///  @param[in]		every	 	true:3頂点が全て検索領域に含まれるものを抽出。
	///  						 	false:1頂点でも検索領域に含まれるものを抽出。
	///  @param[in,out] tri_list	抽出した三角形ポリゴンリストのポインタ。
	///  @return	POLYLIB_STATで定義される値が返る。
	///  @attention	オーバーロードメソッドあり。
	///
	POLYLIB_STAT linear_search(
		BBox							*bbox, 
		bool							every, 
		std::vector<PrivateTriangle*>	*tri_list
		) const;

	/// 
	/// KD木探索により、指定位置に最も近いポリゴンを検索する。
	///
	///  @param[in]     pos     指定位置
	///  @return    検索されたポリゴン
	///
	const PrivateTriangle* search_nearest(
		const Vec3<REAL_TYPE>&    pos
		) const;

	///
	/// PolygonGroupのフルパス名を取得する。
	///
	///  @return フルパス名。
	///
	std::string acq_fullpath();

	///
	/// カンマ区切りでSTLファイル名リストを取得。
	///
	///  @return ファイル名リスト。
	///
	std::string acq_file_name();

	///
	/// PE領域間移動する三角形ポリゴンリストの取得。
	///
	///  @param[in]	neibour_bbox		隣接PE領域バウンディングボックス。
	///  @param[in]	exclude_tria_ids	領域移動対象外三角形IDリスト。
	///  @return	検索結果三角形リスト。
	///
	const std::vector<PrivateTriangle*>* search_outbounded(
		BBox				neibour_bbox,
		std::vector<int>	*exclude_tria_ids
		);

	///
	/// 三角形リストの追加。
	///
	///  @param[in]	tri_list	三角形ポリゴンリストのポインタ。
	///  @return	POLYLIB_STATで定義される値が返る。
	///  @attention	三角形IDが重複した三角形は追加しない。KD木の再構築はしない。
	///
	POLYLIB_STAT add_triangles(
		std::vector<PrivateTriangle*>	*tri_list
		);



	///
	/// 三角形リストの追加。
	///
	///  @param[in] vertlist 設定する三角形ポリゴン頂点リスト。
	///  @param[in] idlist 三角形のid。
	///  @param[in] n_start_tri vertlistの頂点開始位置
	///  @param[in] n_start_id idlistのid開始位置
	///  @param[in] n_tri 加える三角形の数
	///  @return	POLYLIB_STATで定義される値が返る。
	///  @attention TriMeshクラスのadd()参照。オーバーロードメソッドあり。
	POLYLIB_STAT add_triangles(	
		const REAL_TYPE* vertlist,
		const int* idlist,
		const int n_start_tri,
		const int n_start_id,
		const unsigned int n_tri);


	///
	/// ポリゴン情報を再構築する。（KD木の再構築をおこなう）
	///
	///  @return	POLYLIB_STATで定義される値が返る。
	///
	POLYLIB_STAT rebuild_polygons();

	///
	/// グループ情報（ランク番号、親グループ名、自分のグループ名、ファイル名、
	/// 頂点数、各頂点のXYZ座標値、法線ベクトルのXYZ座標値、面積）を出力する。
	///
	///  @param[in] irank ランク数。
	///  @return	POLYLIB_STATで定義される値が返る。
	///
	POLYLIB_STAT show_group_info(
		int irank = -1
		);

	// add keno 20120331
	/// ポリゴングループの要素数を返す
	int get_group_num_tria( void );

	/// ポリゴンの面積を積算して返す
	REAL_TYPE get_group_area( void );

	/// ポリゴンの縮尺変換＆KD木再構築
	POLYLIB_STAT rescale_polygons( REAL_TYPE scale );

	///
	/// グループ配下の全Triangleオブジェクトのm_exidを更新する。
	///
	/// @param[in] id 更新するID番号。
	/// @return    ステータスコード。
	///
	POLYLIB_STAT set_all_exid_of_trias(
		int id
		);

	//=======================================================================
	// Setter/Getter
	//=======================================================================
	///
	/// クラス名を取得。
	///
	/// @return		クラス名。
	/// @attention	本クラスを継承する場合、継承後のクラス名を返すように変更す
	/// 			ることる。
	///
	static std::string get_class_name();

	///
	/// クラス名を取得。
	///
	/// @return		クラス名。
	/// @attention	継承するクラスのクラス名取得関数get_class_name()を呼び出す。
	///
	virtual std::string whoami();

	///
	/// STLファイル名とファイルフォーマットを設定。
	///
	///  @param[in] fname STLファイル名とファイルフォーマットの対応マップ。
	///
	void set_file_name(std::map<std::string, std::string> fname) ;
	///
	/// STLファイル名とファイルフォーマットの対応マップ取得。
	///
	///  @return STLファイル名とファイルフォーマットの対応マップ。
	///
	std::map<std::string, std::string> get_file_name() const ;
	///
	/// グループ名を設定。
	///
	/// @param[in] name グループ名。
	///
	void set_name(std::string name);

	///
	/// グループ名を取得。
	///
	/// @return グループ名。
	///
	std::string get_name(void);

	///
	/// 親グループのフルパス名を設定。
	///
	/// @param[in] ppath 親グループのフルパス名。
	///
	void set_parent_path(std::string ppath);
	///
	/// 親グループのフルパス名を取得。
	///
	/// @return 親グループのフルパス名。
	///
	std::string get_parent_path(void);
	///
	/// 親グループを設定。
	///
	/// @param[in] p 親グループのポインタ。
	///
	void set_parent(PolygonGroup* p) ;
	///
	/// 親グループを取得。
	///
	/// @return 親グループのポインタ。
	///
	PolygonGroup* get_parent(void);

	///
	/// 子グループを設定。
	///
	/// @param[in] p	子グループのリスト。
	///
	void set_children(std::vector<PolygonGroup*>& p);

	///
	/// 子グループを取得。
	///
	/// @return 子グループのリスト。
	///
	std::vector<PolygonGroup*>& get_children(void);

	///
	/// 子グループを追加。
	///
	/// @param[in] p	子グループ。
	///
	void add_children(PolygonGroup* p) ;

	///
	/// Polygonクラスが管理する頂点リストを取得。
	///
	/// @return  頂点リスト
	///
	VertexList* get_vertexlist() ;

	///
	/// Polygonクラスが管理するKD木クラスを取得。
	///
	/// @return KD木ポリゴンリスト。
	///
	VertKDT *get_vertkdt();
	///
	/// Polygonクラスが管理する三角形ポリゴンリストを取得。
	///
	/// @return 三角形ポリゴンリスト。
	///
	std::vector<PrivateTriangle*>* get_triangles();

	///
	/// Polygonクラスが管理するKD木クラスを取得。
	///
	/// @return KD木ポリゴンリスト。
	///
	VTree *get_vtree() ;

	///
	/// ポリゴングループIDを取得。
	/// メンバー名修正( m_id -> m_internal_id) 2010.10.20
	///
	///  @return ポリゴングループID。
	///
	int get_internal_id() ;
	///
	/// ユーザ定義ラベルを取得。
	/// 追加 2012.08.31
	///
	///  @return ユーザ定義ラベル。
	///
	std::string get_label();

	///
	/// ユーザ定義タイプを取得。
	/// 追加 2013.07.17
	///
	///  @return ユーザ定義タイプ。
	///
	std::string get_type() ;

	///
	/// ユーザ定義IDを取得。
	/// 追加 2010.10.20
	///
	///  @return ユーザ定義ID。
	///
	int get_id() ;

	///
	/// 移動対象フラグを取得。
	///
	///  @return 移動対象フラグ。
	///
	int get_movable() ;

	///
	/// move()による移動前三角形一時保存リストの個数を取得。
	///
	///  @return 一時保存リストサイズ。
	///
	size_t get_num_of_trias_before_move();

	///
	/// test function for Vertex test
	/// 
	void print_vertex() const;

	void show_bbox() const;

	///
	/// configファイルに記述するParamタグのクラス名(value="...")。
	///
	//	static const char *ATT_NAME_config;
	static const char *ATT_NAME_CLASS;

	///
	/// 頂点同一性の基準値　configファイルに記述するParamタグ(value="...")。
	///
	static const char *ATT_NAME_TOLERANCE;



protected:

	///
	/// 設定ファイルから取得したPolygonGroupの情報をインスタンスにセットする。
	/// 
	/// "filepath" に関して、先にfilepathが複数　(filepath[0])が存在するかどうか
	///  をチェックして、複数ならばその処理を行い、filepath の処理は終了する。
	///  複数でないことが分かったら、filepath が単体で存在するかをチェックして、
	///  存在するならば、処理を行う。
	///
	///  @param[in] polylib		Polygonクラスのインスタンス。
	///  @param[in] parent		親グループ。
	///  @param[in] tp              TextParserクラスのインスタンス
	///  @return	POLYLIB_STATで定義される値が返る。
	///
	POLYLIB_STAT setup_attribute (
		Polylib					*polylib,
		PolygonGroup			*parent,
		TextParser *tp
		);

	///
	/// move()メソッド実行により、頂点が隣接セルよりも遠くへ移動した三角形情報
	/// を報告（前処理）。
	///
	///  @return	POLYLIB_STATで定義される値が返る。
	///  @attention 本メソッドはデバッグ用です。
	///  			派生クラスでオーバーライドしたmove()メソッド内で、座標移動
	///				処理前に呼ぶこと。
	///
	POLYLIB_STAT init_check_leaped();

	///
	/// move()メソッド実行により、頂点が隣接セルよりも遠くへ移動した三角形情報
	/// を報告（後処理）。該当する三角形について、以下の情報をcerrへ出力する。
	///		・ポリゴングループID
	///		・三角形ID
	///		・移動前/後の頂点座標
	///
	///  @param[in] origin		計算領域起点座標
	///  @param[in] cell_size	ボクセルサイズ
	///  @return	POLYLIB_STATで定義される値が返る。
	///  @attention	本メソッドはデバッグ用です。
	///  			派生クラスでオーバーライドしたmove()メソッド内で、座標移動
	///				処理後に呼ぶこと。
	///
	POLYLIB_STAT check_leaped(
		Vec3<REAL_TYPE> origin,
		Vec3<REAL_TYPE> cell_size
		);

	///
	/// 2点が隣接ボクセルよりも離れているか？
	///
	///  @param[in] origin		計算領域起点座標。
	///  @param[in] cell_size	ボクセルサイズ。
	///  @param[in] pos1			点(1)。
	///  @param[in] pos2			点(2)。
	///  @return	true:2点が隣接ボクセルよりも離れている。
	///
	bool is_far(
		Vec3<REAL_TYPE> origin,
		Vec3<REAL_TYPE> cell_size,
		Vec3<REAL_TYPE> pos1,
		Vec3<REAL_TYPE> pos2
		);




private:
	///
	/// STLファイル名を作成。ファイル名は、以下の通り。
	/// グループ名のフルパス_ランク番号_自由文字列.フォーマット文字列。
	///
	///  @param[in] rank_no	ファイル名に付加するランク番号。
	///  @param[in] extend	ファイル名に付加する自由文字列。
	///  @param[in] format	STLファイルフォーマット。
	///  @return	POLYLIB_STATで定義される値が返る。
	///
	char *mk_stl_fname(
		std::string		rank_no,
		std::string		extend,
		std::string		format
		);


	///
	/// STLファイル名を作成。ファイル名は、以下の通り。
	/// グループ名のフルパス_ランク番号_自由文字列.フォーマット文字列。
	/// TextParser 対応版
	///
	///  @param[in] rank_no	ファイル名に付加するランク番号。
	///  @param[in] extend	ファイル名に付加する自由文字列。
	///  @param[in] format	STLファイルフォーマット。
	///  @param[in,out] stl_fname_map stl ファイル名とポリゴングループのパス
	///  @return	POLYLIB_STATで定義される値が返る。
	///
	char *mk_stl_fname(
		std::string		rank_no,
		std::string		extend,
		std::string		*format,
		std::map<std::string,std::string>& stl_fname_map
		);

	///
	/// 三角形ポリゴンIDファイル名を作成。ファイル名は、以下の通り。
	/// グループ名のフルパス_ランク番号_自由文字列.id。
	///
	///  @param[in] rank_no	ファイル名に付加するランク番号。
	///  @param[in] extend	ファイル名に付加する自由文字列。
	///  @return	POLYLIB_STATで定義される値が返る。
	///
	char *mk_id_fname(
		std::string		extend,
		std::string		rank_no
		);

	///
	/// 全PolygonGroupに一意のグループIDを作成する。
	///
	///  @return	グループID。
	///
	int create_global_id();


protected:
	//=======================================================================
	// クラス変数
	//=======================================================================
	/// グループID。
	// メンバー名修正( m_id -> m_internal_id) 2010.10.20
	int					m_internal_id;

	/// 自グループ名。
	std::string			m_name;

	/// 親グループのパス名。
	std::string			m_parent_path;

	/// 親グループへのポインタ。
	PolygonGroup			*m_parent;

	/// 子グループへのポインタリスト。
	std::vector<PolygonGroup*>	m_children;

	/// STLファイル名とファイル形式。
	std::map<std::string, std::string>	m_file_name;

	/// 三角形Polygonsクラス。
	Polygons				*m_polygons;

	/// moveメソッドにより移動するグループか？
	bool					m_movable;

	/// KD木の再構築が必要か？
	bool					m_need_rebuild;

	/// move()による移動前三角形一時保存リスト。
	std::vector<PrivateTriangle*>		*m_trias_before_move;

	/// ユーザ定義ラベル : (追加 2012.08.31)
	std::string							m_label;

	/// ユーザ定義タイプ : (追加 2013.07.17)
	std::string							m_type;

	///  頂点同一性チェックの判定基準 (追加 2013.09.03)
	REAL_TYPE m_tolerance;

private:
	/// ユーザ定義id : (追加 2010.10.20)
	int							m_id;

	/// ユーザ定義IDが設定されたか？：（追加 2013.06.17)
	bool						m_id_defined;



	//	DVertexManager* m_DVM_ptr;

};

} //namespace PolylibNS

#endif //polylib_polygongroup_h
