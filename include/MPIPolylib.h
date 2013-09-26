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

#ifndef MPIPolylib_h
#define MPIPolylib_h

#include <vector>
#include <map>
#include "mpi.h"
#include "Polylib.h"
#include "groups/PolygonGroup.h"



// MPI通信用メッセージタグ
#define	MPITAG_NUM_CONFIG			1
#define	MPITAG_CONFIG				2
#define	MPITAG_NUM_TRIAS			3
#define MPITAG_TRIA_IDS				4
#define MPITAG_TRIAS				5

//#define PL_MPI_REAL MPI_DOUBLE

#if PL_REAL==float
#define PL_MPI_REAL MPI_FLOAT
#else
#define PL_MPI_REAL MPI_DOUBLE
#endif


namespace PolylibNS {
////////////////////////////////////////////////////////////////////////////
///
/// クラス:ParallelInfo
/// 並列プロセス情報。
///
////////////////////////////////////////////////////////////////////////////
template <typename T>
struct ParallelInfo {
	/// MPIコミュニケータ
	MPI_Comm m_comm;

	/// ランク数
	int m_rank;

	/// 計算領域情報
	CalcAreaInfo<T> m_area;

	/// migrate除外三角形IDマップ(k:グループID, v:三角形IDリスト)
	std::map< int, std::vector<int> > m_exclusion_map;
};

////////////////////////////////////////////////////////////////////////////
///
/// クラス:MPIPolylib
/// ポリゴンを管理する為の並列版クラスライブラリです。
///
////////////////////////////////////////////////////////////////////////////
template <typename T>
class MPIPolylib : public Polylib<T> {
public:
	///
	/// インスタンス取得。本クラスはsingltonクラスです。
	///
	/// @return MPIPolylibクラスのインスタンス
	///
	static MPIPolylib<T>* get_instance();

	///
	/// 並列計算関連情報の設定と初期化を行う。
	/// 全rankで各々設定を行い、その領域情報を全rankへ配信する。
	///
	///  @param[in] comm	MPIコミュニケーター
	///  @param[in] bpos	自PE担当領域の基点座標
	///  @param[in] bbsize	同、計算領域のボクセル数
	///  @param[in] gcsize	同、ガイドセルのボクセル数
	///  @param[in] dx		同、ボクセル１辺の長さ
	///  @return POLYLIB_STATで定義される値が返る。
	///
	POLYLIB_STAT
	init_parallel_info(
		MPI_Comm comm,
		T bpos[3],
		unsigned int bbsize[3],
		unsigned int gcsize[3],
		T dx[3]
	);

	///
	/// Polylib::load()のオーバライドメソッド。
	/// @attention 並列環境では利用できません。
	///
	/// @param[in] config_filename	初期化ファイル名。
	/// @return 常に PLSTAT_NG が返ります。
	///
	POLYLIB_STAT
	load(
		std::string config_filename
	){ return PLSTAT_NG; };

	///
	/// rank0によるデータ構築。
	/// 指定された設定ファイルをrank0にて読み込み、グループ階層構造の構築
	/// およびポリゴンデータの構築を行う。
	/// グループ階層構造は全rankにb_castされ、情報を共有する。
	/// ポリゴンデータは各rank領域毎のデータが分配される。
	///
	/// @param[in] config_filename	初期化ファイル名。未指定時はデフォルトファイルを読む。
	/// @return	POLYLIB_STATで定義される値が返る。
	///
	POLYLIB_STAT
	load_rank0(
		std::string config_filename = "",
		T scale = 1.0
	);

	///
	/// 全rank並列でのデータ構築。
	/// 指定された設定ファイルを各rankにて読み込み、グループ階層構造の構築、
	/// およびポリゴンデータの構築を行う。
	/// @attention 各rankが読み込むファイルに記述されたグループ階層構造が一致している必要がある。
	///
	/// @param[in] config_filename	初期化ファイル名。未指定時はデフォルトファイルを読む。
	/// @param[in] id_format		三角形IDファイルの入力形式。
	/// @return	POLYLIB_STATで定義される値が返る。
	///
	POLYLIB_STAT
	load_parallel( 
		std::string config_filename = "",
		ID_FORMAT	id_format = ID_BIN
	);

	///
	/// Polylib::save()のオーバライドメソッド。
	/// @attention 並列環境では利用できません。
	///
	/// @param[out] p_config_filename	初期化ファイル名。
	/// @return 常に PLSTAT_NG が返ります。
	///
	POLYLIB_STAT
	save(
		std::string *p_config_filename
	){ return PLSTAT_NG; };

	///
	/// rank0によるデータ保存。
	/// rank0の本クラスインスタンスが保持するグループ階層構造を設定ファイルに書き出す。
	/// 同時に各rankに分散するポリゴンデータもrank0に集められ、指定されたフォーマットの
	/// STLファイルにrank0で書き出す。
	/// 設定ファイル命名規則は以下の通り
	///   polylib_config_付加文字列.xml
	///   polylib_config_付加文字列.tpp
	/// STLファイル命名規則は以下の通り
	///   ポリゴングループ名称_付加文字列.拡張子
	///
	/// @param[out] p_config_filename	設定ファイル名返却用stringインスタンスへのポインタ
	/// @param[in]  stl_format			STLファイルフォーマット。 "stl_a":アスキー形式　"stl_b":バイナリ形式
    /// @param[in]  extend				ファイル名に付加する文字列。省略可。省略
	///									した場合は、付加文字列として本メソッド呼
	///									び出し時の年月日時分秒(YYYYMMDD24hhmmss)
	///									を用いる。
	/// @return	POLYLIB_STATで定義される値が返る。
	/// @attention 出力引数p_config_filenameの返却値はrank0でのみ有効
	///
	POLYLIB_STAT
	save_rank0(
		std::string *p_config_filename,
		std::string stl_format,
		std::string extend = ""
	);

	///
	/// 全rank並列でのデータ保存。
	/// 各rankの本クラスインスタンスが保持するグループ階層構造を設定ファイルに各rank毎に書き出す。
	/// 同時にポリゴンデータも指定されたフォーマットのSTLファイルに各rank毎に書き出す。
	/// 設定ファイル命名規則は以下の通り
	///   polylib_config_ランク番号_付加文字列.xml
	///   polylib_config_ランク番号_付加文字列.tpp
	/// STLファイル命名規則は以下の通り
	///   ポリゴングループ名称_ランク番号_付加文字列.拡張子
	///
	/// @param[out] p_config_filename	設定ファイル名返却用stringインスタンスへのポインタ
	/// @param[in] stl_format	STLファイルフォーマット。 "stl_a":アスキー形式　"stl_b":バイナリ形式
    /// @param[in]  extend				ファイル名に付加する文字列。省略可。省略
	///									した場合は、付加文字列として本メソッド呼
	///									び出し時の年月日時分秒(YYYYMMDD24hhmmss)
	///									を用いる。
	/// @param[in] id_format	三角形IDファイルの出力形式。
	/// @return	POLYLIB_STATで定義される値が返る。
	///
	POLYLIB_STAT
	save_parallel(
		std::string *p_config_filename,
		std::string stl_format,
		std::string extend = "",
		ID_FORMAT	id_format = ID_BIN
	);

	///
	/// ポリゴン座標の移動。
	/// 本クラスインスタンス配下の全PolygonGroupのmoveメソッドが呼び出される。
	///
	/// @param[in] params	移動計算要パラメタセット。
	/// @return	POLYLIB_STATで定義される値が返る。
	///
	POLYLIB_STAT
	move(
		PolylibMoveParams &params
	);

	///
	/// ポリゴンデータのPE間移動。
	/// 本クラスインスタンス配下の全PolygonGroupのポリゴンデータについて、
	/// moveメソッドにより移動した三角形ポリゴン情報を隣接PE間でやり取りする。
	///
	/// @return	POLYLIB_STATで定義される値が返る。
	///
	POLYLIB_STAT
	migrate();

	///
	/// m_myprocの内容をget
	/// @return 自PE領域情報
	///
	ParallelInfo<T> get_myproc(){ return m_myproc; };

	///
	/// MPIPolylibが利用中の概算メモリ量を返す
	///
	/// @return 利用中のメモリ量(byte)
	///
	unsigned int used_memory_size();

protected:
	///
	/// コンストラクタ。
	/// singletonのため非公開。本クラスインスタンス取得にはget_instance()を利用する。
	///
	MPIPolylib();

	///
	/// デストラクタ。
	///
	~MPIPolylib();

	///
	/// 指定されたグループ以下の階層構造をツリー形式で標準出力に出力する。
	///  @param p	表示対象となるグループのポインタ。
	///  @param tab	階層の深さを示すスペース。
	///  @attention プロセス毎に動作する。
    ///   出力にランク数が加わる以外は非並列版と同じ。
	///
	void show_group_name(PolygonGroup<T>* p, std::string tab);

	///
	/// 設定ファイル内容を他rankへbroadcastする。
	///
	/// @param[in] config_contents 初期化ファイル内容。
	/// @return	POLYLIB_STATで定義される値が返る。
	///
	POLYLIB_STAT
	broadcast_config(
		std::string config_contents
	);

	///
	/// 各PE領域内ポリゴン情報を全rankに送信
	///
	/// @return	POLYLIB_STATで定義される値が返る。
	///
	POLYLIB_STAT
	send_polygons_to_all();

	///
	/// グループID＆グループ内三角形数の送信情報を作成。
	/// 
	/// @param[in,out] p_vec 情報追加先ベクタ
	/// @param[in] group_id グループID
	/// @param[in] p_trias グループ内三角形リスト
	/// @return	POLYLIB_STATで定義される値が返る。
	///
	POLYLIB_STAT
	pack_num_trias(
		std::vector<int>* p_vec,
		int group_id,
		const std::vector<PrivateTriangle<T>*>* p_trias
	);

	///
	/// 三角形の送信情報を作成。
	/// 
	/// @param[in,out] p_vec 情報追加先ベクタ
	/// @param[in] p_trias グループ内三角形リスト
	/// @return	POLYLIB_STATで定義される値が返る。
	///
	POLYLIB_STAT
	pack_trias(
		std::vector<T>* p_vec,
		const std::vector<PrivateTriangle<T>*>* p_trias
	);

	///
	/// 三角形IDの送信情報を作成。
	/// 
	/// @param[in,out] p_vec 情報追加先ベクタ
	/// @param[in] p_trias グループ内三角形リスト
	/// @return	POLYLIB_STATで定義される値が返る。
	///
	POLYLIB_STAT
	pack_tria_ids(
		std::vector<int>* p_vec,
		const std::vector<PrivateTriangle<T>*>* p_trias
	);

	///
	/// 自領域内ポリゴンのみ抽出してポリゴン情報を再構築。
	/// migrate実行後に行う。
	/// 
	/// @return	POLYLIB_STATで定義される値が返る。
	///
	POLYLIB_STAT
	erase_outbounded_polygons();

	///
	/// ポリゴングループ定義情報をrank0から受信し、グループ階層構造を構築。
	///
	/// @return	POLYLIB_STATで定義される値が返る。
	///
	POLYLIB_STAT
	broadcast_config_from_rank0();

	///
	/// 自領域に必要なポリゴン情報をrank0から受信
	/// 
	/// @return	POLYLIB_STATで定義される値が返る。
	///
	POLYLIB_STAT
	receive_polygons_from_rank0();

	///
	/// 他rankからポリゴン情報をrank0で受信
	///
	POLYLIB_STAT
	gather_polygons();

	///
	/// rank0へポリゴン情報を送信
	///
	POLYLIB_STAT
	send_polygons_to_rank0();

	///
	/// 移動除外三角形IDリストの作成
	///
	POLYLIB_STAT
	select_excluded_trias( PolygonGroup<T> *p_pg );

protected:
	///
	/// プロセス担当領域クラスのポインタを返す
	///  @param[in] rank ランク数
	///  @return プロセス担当領域クラスのポインタ
	///
	ParallelInfo<T>* get_proc(int rank);

	/// 自PE担当領域情報
	ParallelInfo<T> m_myproc;

	/// 自PEを除く全PE担当領域情報リスト
	std::vector<ParallelInfo<T>*> m_other_procs;

	/// 隣接PE担当領域情報リスト
	std::vector<ParallelInfo<T>*> m_neibour_procs;

	/// 自プロセスのランク数
	int m_myrank;

	/// 全プロセス数
	int m_numproc;

	/// 自プロセスが利用するコミュニケーター
	MPI_Comm m_mycomm;
};


// public /////////////////////////////////////////////////////////////////////
template <typename T>
MPIPolylib<T>*
MPIPolylib<T>::get_instance() {
	static MPIPolylib instance;
	return &instance;
}


// public /////////////////////////////////////////////////////////////////////
template <typename T>
POLYLIB_STAT
MPIPolylib<T>::init_parallel_info(
	MPI_Comm comm,
	T bpos[3], 
	unsigned int bbsize[3], 
	unsigned int gcsize[3], 
	T dx[3]
)
{
#ifdef DEBUG
	PL_DBGOSH << "MPIPolylib::init_parallel_info() in. " << std::endl;
#endif
	int i;

	// MPI情報の設定
	m_mycomm = comm;
	MPI_Comm_rank(comm, &m_myrank);
	MPI_Comm_size(comm, &m_numproc);

	// デバッグ出力用ランク番号文字列を設定
	std::ostringstream ostr;
	ostr << m_myrank;
	gs_rankno = "(rk:";
	gs_rankno += ostr.str();
	gs_rankno += ")";

#ifdef DEBUG
	PL_DBGOSH << "m_myrank: " << m_myrank << " m_numproc: " << m_numproc << std::endl;
#endif

	T bbsize_f[3], gcsize_f[3];
	for (i = 0; i < 3; i++) {
		bbsize_f[i] = (T)bbsize[i];
		gcsize_f[i] = (T)gcsize[i];
	}

	Vec3<T> v_bbsize(bbsize_f[0],bbsize_f[1],bbsize_f[2]);
	Vec3<T> v_gcsize(gcsize_f[0],gcsize_f[1],gcsize_f[2]);
	Vec3<T> v_bpos(bpos[0],bpos[1],bpos[2]);
	Vec3<T> v_dx(dx[0],dx[1],dx[2]);

	// 自PE領域情報を設定
	m_myproc.m_comm = comm;
	m_myproc.m_rank = m_myrank;
	m_myproc.m_area.m_bpos = v_bpos;
	m_myproc.m_area.m_bbsize = v_bbsize;
	m_myproc.m_area.m_gcsize = v_gcsize;
	m_myproc.m_area.m_dx = dx;
	m_myproc.m_area.m_gcell_min = v_bpos-( v_gcsize )*v_dx;
	m_myproc.m_area.m_gcell_max = v_bpos+( v_bbsize+v_gcsize )*v_dx;
	m_myproc.m_area.m_gcell_bbox.add(m_myproc.m_area.m_gcell_min);
	m_myproc.m_area.m_gcell_bbox.add(m_myproc.m_area.m_gcell_max);

#ifdef DEBUG
	PL_DBGOSH << "(my_rank:" << m_myrank << "):" <<"bpos      :" << v_bpos  << std::endl;
	PL_DBGOSH << "(my_rank:" << m_myrank << "):" <<"bbsize    :" << v_bbsize << std::endl;
	PL_DBGOSH << "(my_rank:" << m_myrank << "):" <<"gcsize    :" << v_gcsize << std::endl;
	PL_DBGOSH << "(my_rank:" << m_myrank << "):" <<"dx        :" << v_dx << std::endl;
	PL_DBGOSH << "(my_rank:" << m_myrank << "):" <<"gcell_min :"
		 << m_myproc.m_area.m_gcell_min << std::endl;
	PL_DBGOSH << "(my_rank:" << m_myrank << "):" <<"gcell_max :"
		 << m_myproc.m_area.m_gcell_max << std::endl;
#endif

	// 送信データ作成
	T send_buf[12];
	for (i = 0; i < 3; i++) {
		send_buf[i] = v_bpos[i];
	}
	for (i = 0; i < 3; i++) {
		send_buf[3+i] = v_bbsize[i];
	}
	for (i = 0; i < 3; i++) {
		send_buf[6+i] = v_gcsize[i];
	}
	for (i = 0; i < 3; i++) {
		send_buf[9+i] = v_dx[i];
	}

	// 受信領域確保
	T* recv_buf = new T[12 * m_numproc];

	// Allgather通信を行う
	if (MPI_Allgather(send_buf, 12, PL_MPI_REAL, recv_buf, 12, PL_MPI_REAL, comm) != MPI_SUCCESS) {
		PL_ERROSH << "[ERROR]MPIPolylib::init_parallel_info():MPI_Allgather "
				  << "faild." << std::endl;
		return PLSTAT_MPI_ERROR;
	} else {
#ifdef DEBUG
	  PL_DBGOSH << m_myrank << std::endl;
#endif //DEBUG
	}

	// 受信データの展開
	for (int irank = 0; irank < m_numproc; irank++) {
		// 自PE領域情報はスキップ
		if( irank == m_myrank ) continue;
		for (i = 0; i < 3; i++) {
			v_bpos[i] = recv_buf[i + 12*irank];
		}
		for (i = 0; i < 3; i++) {
			v_bbsize[i] = recv_buf[3 + i + 12*irank];
		} 
		for (i = 0; i < 3; i++) {
			v_gcsize[i] = recv_buf[6 + i + 12*irank];
		} 
		for (i = 0; i < 3; i++) {
			v_dx[i]	= recv_buf[9 + i + 12*irank];
		}
#ifdef DEBUG
		PL_DBGOSH << "(rank:" << irank << "):" <<"bpos  :" << v_bpos  << std::endl;
		PL_DBGOSH << "(rank:" << irank << "):" <<"bbsize:" << v_bbsize << std::endl;
		PL_DBGOSH << "(rank:" << irank << "):" <<"gcsize:" << v_gcsize << std::endl;
		PL_DBGOSH << "(rank:" << irank << "):" <<"dx    :" << v_dx << std::endl;
#endif
		ParallelInfo<T>* proc = new (ParallelInfo<T>);
		proc->m_comm = comm;
		proc->m_rank = irank;
		proc->m_area.m_bpos = v_bpos;
		proc->m_area.m_bbsize = v_bbsize;
		proc->m_area.m_gcsize = v_gcsize;
		proc->m_area.m_dx = v_dx;
		proc->m_area.m_gcell_min = v_bpos-( v_gcsize )*v_dx;
		proc->m_area.m_gcell_max = v_bpos+( v_bbsize+v_gcsize )*v_dx;
		proc->m_area.m_gcell_bbox.add(proc->m_area.m_gcell_min);
		proc->m_area.m_gcell_bbox.add(proc->m_area.m_gcell_max);

		// 全PE領域情報リストに追加
		m_other_procs.push_back(proc);

		// 自PE領域と隣接するPE領域情報はm_neibour_procsにも追加
		if( m_myproc.m_area.m_gcell_bbox.crossed(proc->m_area.m_gcell_bbox) ) {
			m_neibour_procs.push_back(proc);
#ifdef DEBUG
		PL_DBGOSH << m_myrank << ": " << "neighbour rank:" << proc->m_rank  << std::endl;
#endif
		}
	}
	// 受信領域あとしまつ
	delete[] recv_buf;

	return PLSTAT_OK;
}


// #if 0
// // old tp version
// // public /////////////////////////////////////////////////////////////////////
// template <typename T>
// POLYLIB_STAT
// MPIPolylib<T>::load_rank0(
// 	std::string config_filename
// )
// {
// #ifdef DEBUG
// 	PL_DBGOSH << m_myrank << ": " << "MPIPolylib::load_rank0() in. " << std::endl;
// #endif
// 	POLYLIB_STAT ret;
// 	std::string       config_contents;

// 	// for tp
// 	try {
// 		PolylibConfig base(config_filename);
// 		ret = this->make_group_tree(&base);
// 		if( ret != PLSTAT_OK ) return ret;
// 	}
// 	catch( POLYLIB_STAT e ){
// 		return e;
// 	}

// 	if( m_myrank == 0 ) {

// 	  // for tp
// #if 0
// 	  // 設定ファイル読み込み。
// 	  if( (ret = Polylib::load_config_file( &config_contents, config_filename ))
// 	      != PLSTAT_OK ) {
// 	    PL_ERROSH << "[ERROR]MPIPolylib::load_rank0():Polylib::load_config() faild. returns:"
// 		      << PolylibStat2::String(ret) << std::endl;
// 	    return ret;
// 	  }

// 	  // グループ階層構造構築。
// 	  //if( (ret = Polylib::make_group_tree( config_contents )) != PLSTAT_OK ) {
// 	  if( (ret = this->make_group_tree( config_contents )) != PLSTAT_OK ) {
// 	    PL_ERROSH << "[ERROR]MPIPolylib::load_rank0():Polylib::make_group_tree() faild. returns:"
// 		      << PolylibStat2::String(ret) << std::endl;
// 	    return ret;
// 	  }

// 	  // 設定ファイルの内容を他PEへブロードキャストする
// 	  if( (ret = broadcast_config( config_contents )) != PLSTAT_OK ) {
// 	    PL_ERROSH << "[ERROR]MPIPolylib::load_rank0():broadcast_config() faild. returns:"
// 		      << PolylibStat2::String(ret) << std::endl;
// 	    return ret;
// 	  }
// #endif

// 	  // ポリゴン情報を構築 (三角形IDファイルは不要なので、第二引数はダミー)
// 	  if( (ret = this->load_polygons(false, ID_BIN)) != PLSTAT_OK ) {
// 	    PL_ERROSH << "[ERROR]MPIPolylib::load_rank0():load_polygons() faild. returns:" << PolylibStat2::String(ret) << std::endl;
// 	    return ret;
// 	  }

// 	  // ポリゴン情報を他PEへ配信する。
// 	  if( (ret = send_polygons_to_all()) != PLSTAT_OK ) {
// 	    PL_ERROSH << "[ERROR]MPIPolylib::load_rank0():send_polygons_to_all() faild. returns:" << PolylibStat2::String(ret) << std::endl;
// 	    return ret;
// 	  }

// 	  // 他PE領域ポリゴン情報を削除して自領域分のみでデータ構造再構築
// 	  if( (ret = erase_outbounded_polygons()) != PLSTAT_OK ) {
// 	    PL_ERROSH << "[ERROR]MPIPolylib::load_rank0():erase_outbounded_polygons() faild. returns:" << PolylibStat2::String(ret) << std::endl;
// 	    return ret;
// 	  }
// 	}
// 	else {

// 	  // for tp
// #if 0
// 	  // 設定ファイルの内容をrank0から受信する
// 	  if( (ret = broadcast_config_from_rank0()) != PLSTAT_OK ) {
// 	    PL_ERROSH << "[ERROR]MPIPolylib::load_rank0():broadcast_config_from_rank0() faild. returns:" << PolylibStat2::String(ret) << std::endl;
// 	    return ret;
// 	  }
// #endif

// 	  // ポリゴン情報をrank0から受信する。
// 	  if( (ret = receive_polygons_from_rank0()) != PLSTAT_OK ) {
// 	    PL_ERROSH << "[ERROR]MPIPolylib::load_rank0():receive_polygons_from_rank0() faild. returns:" << PolylibStat2::String(ret) << std::endl;
// 	    return ret;
// 	  }
// 	}

// 	return PLSTAT_OK;

// }
// #endif



// new tp version  without  PolylibConfig
// public /////////////////////////////////////////////////////////////////////
template <typename T>
POLYLIB_STAT
MPIPolylib<T>::load_rank0(
	std::string config_filename,
	T scale
)
{
#define TIME_MEASURE
#ifdef TIME_MEASURE
  double time1=0;
  double time2=0;
  double time3=0;
  double time4=0;
  time1=MPI_Wtime();

#endif // TIME_MEASURE

  //#define DEBUG
#ifdef DEBUG
	PL_DBGOSH << m_myrank << ": " << "MPIPolylib::load_rank0() in. " << std::endl;
#endif
	POLYLIB_STAT ret;
	std::string       config_contents;

	// for tp
	try {
	  //PolylibConfig base(config_filename);
	  this->tp->read(config_filename);

#ifdef DEBUG
	PL_DBGOSH << m_myrank << ": " << "MPIPolylib::load_rank0() tp->read end. " << std::endl;
#endif


		//ret = make_group_tree(&base);
		// only on rank0 ?
		ret = this->make_group_tree(this->tp);

#ifdef DEBUG
	PL_DBGOSH << m_myrank << ": " << "MPIPolylib::load_rank0() make_group_tree end. " << std::endl;
#endif

		if( ret != PLSTAT_OK ) return ret;
	}
	catch( POLYLIB_STAT e ){
		return e;
	}

	if( m_myrank == 0 ) {

// // for tp
// #if 0
// 	  // 設定ファイル読み込み。
// 	  if( (ret = Polylib::load_config_file( &config_contents, config_filename ))
// 	      != PLSTAT_OK ) {
// 	    PL_ERROSH
// 	      << "[ERROR]MPIPolylib::load_rank0():Polylib::load_config() faild."
// 	      <<" returns:" << PolylibStat2::String(ret) << std::endl;
// 	    return ret;
// 	  }

// 	  // グループ階層構造構築。
// 	  //	  if( (ret = Polylib::make_group_tree( config_contents )) != PLSTAT_OK ) {
// 	  	  if( (ret = this->make_group_tree( config_contents )) != PLSTAT_OK ) {
// 	    PL_ERROSH << "[ERROR]MPIPolylib::load_rank0():Polylib::make_group_tree() faild. returns:"
// 		      << PolylibStat2::String(ret) << std::endl;
// 	    return ret;
// 	  }

// 	  // 設定ファイルの内容を他PEへブロードキャストする
// 	  if( (ret = broadcast_config( config_contents )) != PLSTAT_OK ) {
// 	    PL_ERROSH << "[ERROR]MPIPolylib::load_rank0():broadcast_config() faild. returns:"
// 		      << PolylibStat2::String(ret) << std::endl;
// 	    return ret;
// 	  }
// #endif

	  // ポリゴン情報を構築 (三角形IDファイルは不要なので、第二引数はダミー)

#ifdef DEBUG
	PL_DBGOSH << m_myrank << ": " << "MPIPolylib::load_rank0() load_polygons start. " << std::endl;
#endif

	  if( (ret = this->load_polygons(false, ID_BIN, scale)) != PLSTAT_OK ) {
	    PL_ERROSH << "[ERROR]MPIPolylib::load_rank0():load_polygons() faild."
		      <<" returns:" << PolylibStat2::String(ret) << std::endl;
	    return ret; 
	  }


#ifdef TIME_MEASURE
  time2=MPI_Wtime();
#endif // TIME_MEASURE

#ifdef DEBUG
	PL_DBGOSH << m_myrank << ": " << "MPIPolylib::load_rank0() load_polygons end. " << std::endl;
#endif


	  // ポリゴン情報を他PEへ配信する。
	  if( (ret = send_polygons_to_all()) != PLSTAT_OK ) {
	    PL_ERROSH << "[ERROR]MPIPolylib::load_rank0():send_polygons_to_all()"
		      <<" faild. returns:" << PolylibStat2::String(ret) << std::endl;
	    return ret;
	  }

#ifdef TIME_MEASURE
  time3=MPI_Wtime();
#endif // TIME_MEASURE

#ifdef DEBUG
	PL_DBGOSH << m_myrank << ": " << "MPIPolylib::load_rank0() send_polyons_to_all. " << std::endl;
#endif

	  // 他PE領域ポリゴン情報を削除して自領域分のみでデータ構造再構築
	  if( (ret = erase_outbounded_polygons()) != PLSTAT_OK ) {
	    PL_ERROSH << "[ERROR]MPIPolylib::load_rank0():erase_outbounded_polygons()"
		      <<" faild. returns:" << PolylibStat2::String(ret) << std::endl;
	    return ret;
	  }

	} else { //for other rank

#ifdef DEBUG
	PL_DBGOSH << m_myrank << ": " << "MPIPolylib::load_rank0() erase_outbounded_polygons. " << std::endl;
#endif


// // for tp
// #if 0
// 	  // 設定ファイルの内容をrank0から受信する
// 	  if( (ret = broadcast_config_from_rank0()) != PLSTAT_OK ) {
// 	    PL_ERROSH << "[ERROR]MPIPolylib::load_rank0():broadcast_config_from_rank0()"
// 		      <<" faild. returns:" << PolylibStat2::String(ret) << std::endl;
// 	    return ret;
// 	  }
// #endif

	    // ポリゴン情報をrank0から受信する。
	    if( (ret = receive_polygons_from_rank0()) != PLSTAT_OK ) {
	      PL_ERROSH << "[ERROR]MPIPolylib::load_rank0():receive_polygons_from_rank0()"
			<<" faild. returns:" << PolylibStat2::String(ret) << std::endl;
	      return ret;
	    }
	}
 
#ifdef TIME_MEASURE
  time4=MPI_Wtime();
    PL_DBGOSH << m_myrank << ": " << "MPIPolylib::load_rank0() time measure" << std::endl;
    PL_DBGOSH << m_myrank << ":time1 " << time1 << std::endl;
    PL_DBGOSH << m_myrank << ":time2 " << time2-time1 << std::endl;
    PL_DBGOSH << m_myrank << ":time3 " << time3-time1 << std::endl;
    PL_DBGOSH << m_myrank << ":time4 " << time4-time1 << std::endl;

#endif // TIME_MEASURE

#undef TIME_MEASURE

	return PLSTAT_OK;

	//#undef DEBUG
}


// public /////////////////////////////////////////////////////////////////////
template <typename T>
POLYLIB_STAT
MPIPolylib<T>::load_parallel( 
	std::string config_filename,
	ID_FORMAT	id_format
)
{
#ifdef DEBUG
	PL_DBGOSH << "MPIPolylib::load_parallel() in. " << std::endl;
#endif
	POLYLIB_STAT ret;

	// 設定ファイル読み込み、グループ階層構造構築、ポリゴン情報構築。
	// MPIPolylib::save_parallel()で保存された設定ファイルであることが前提
	if( (ret = Polylib<T>::load_with_idfile( config_filename, id_format )) != PLSTAT_OK ) {
		PL_ERROSH << "[ERROR]MPIPolylib::load_parallel():Polylib::load() faild. returns:" << PolylibStat2::String(ret) << std::endl;
		return ret;
	}

	return PLSTAT_OK;
}

// #if 0 
// // old version
// // public /////////////////////////////////////////////////////////////////////
// template <typename T>
// POLYLIB_STAT
// MPIPolylib<T>::save_rank0(
// 	std::string *p_config_filename,
// 	std::string stl_format,
// 	std::string extend
// )
// {
// #ifdef DEBUG
// 	PL_DBGOSH << "MPIPolylib::save_rank0() in. " << std::endl;
// #endif
// 	POLYLIB_STAT	ret;

// 	if( m_myrank == 0 ) {

// 		// 他rankからポリゴン情報を受信
// 		if( (ret = gather_polygons()) != PLSTAT_OK ) {
// 			PL_ERROSH << "[ERROR]MPIPolylib::save_rank0():gather_polygons() faild. returns:"
// 					  << PolylibStat2::String(ret) << std::endl;
// 			return ret;
// 		}

// 		// グループ階層構造、ポリゴン情報をファイルへ保存
// // for tp
// #if 0
// 		if( (ret = Polylib::save( p_config_filename, stl_format, extend )) != PLSTAT_OK ) {
// 			PL_ERROSH << "[ERROR]MPIPolylib::save_rank0():Polylib::save() faild. returns:"
// 					  << PolylibStat2::String(ret) << std::endl;
// 			return ret;
// 		}
// #endif




// 	}
// 	else {

// 		// rank0へポリゴン情報を送信
// 		if( (ret = send_polygons_to_rank0()) != PLSTAT_OK ) {
// 			PL_ERROSH << "[ERROR]MPIPolylib::save_rank0():send_polygons_to_rank0() faild. returns:"
// 					  << PolylibStat2::String(ret) << std::endl;
// 			return ret;
// 		}
// 	}
// 	return PLSTAT_OK;
// }
// #endif

// new version 
// public /////////////////////////////////////////////////////////////////////
template <typename T>
POLYLIB_STAT
MPIPolylib<T>::save_rank0(
		       std::string *p_config_filename,
		       std::string stl_format,
		       std::string extend
		       )
{
#ifdef DEBUG
  PL_DBGOSH << "MPIPolylib::save_rank0() in. " << std::endl;
#endif
  POLYLIB_STAT	ret;

  if( m_myrank == 0 ) {
    // 他rankからポリゴン情報を受信
    if( (ret = gather_polygons()) != PLSTAT_OK ) {
      PL_ERROSH << "[ERROR]MPIPolylib::save_rank0():gather_polygons() faild. returns:"
		<< PolylibStat2::String(ret) << std::endl;
      return ret;
    }
    // グループ階層構造、ポリゴン情報をファイルへ保存

    // for tp

    if( (ret = Polylib<T>::save( p_config_filename, stl_format, extend )) != PLSTAT_OK ) {
      PL_ERROSH << "[ERROR]MPIPolylib::save_rank0():Polylib::save() faild. returns:"
		<< PolylibStat2::String(ret) << std::endl;
      return ret;
    }
  }
  else {

    // rank0へポリゴン情報を送信
    if( (ret = send_polygons_to_rank0()) != PLSTAT_OK ) {
      PL_ERROSH << "[ERROR]MPIPolylib::save_rank0():send_polygons_to_rank0() faild."
		<<" returns:"<< PolylibStat2::String(ret) << std::endl;
      return ret;
    }
  }
  return PLSTAT_OK;
}


// public /////////////////////////////////////////////////////////////////////
template <typename T>
POLYLIB_STAT
MPIPolylib<T>::save_parallel(
	std::string *p_config_filename,
	std::string stl_format,
	std::string extend,
	ID_FORMAT	id_format
)
{
#ifdef DEBUG
	PL_DBGOSH << "MPIPolylib::save_parallel() in. " << std::endl;
#endif
	POLYLIB_STAT ret;

	// 各ランク毎に保存
	if( (ret = Polylib<T>::save_with_rankno( p_config_filename, m_myrank, m_numproc-1, extend, stl_format, id_format)) != PLSTAT_OK ) {
		PL_ERROSH << "[ERROR]MPIPolylib::save_parallel():Polylib::save_with_rankno():failed. returns:" << PolylibStat2::String(ret) << std::endl;
		return ret;
	}
	return PLSTAT_OK;
}


// public ////////////////////////////////////////////////////////////////////
template <typename T>
POLYLIB_STAT
MPIPolylib<T>::move(
	PolylibMoveParams &params
)
{
#ifdef DEBUG
	PL_DBGOSH << "MPIPolylib::move() in. " << std::endl;
#endif
	POLYLIB_STAT ret;
	typename std::vector<PolygonGroup<T>*>::iterator group_itr;
	PolygonGroup<T> *p_pg;

	// 各ポリゴングループのmove()を実行
	// 全グループに対して
	for (group_itr = this->m_pg_list.begin(); group_itr != this->m_pg_list.end(); group_itr++) {
			p_pg = (*group_itr);

		// 移動する可能性のあるポリゴングループのみ対象
		if( p_pg->get_movable() ) {

			// move実行前から隣接PE領域に懸かっている三角形をmigrate対象除外リストに載せる
			if( (ret = select_excluded_trias( p_pg )) != PLSTAT_OK ) {
				PL_ERROSH << "[ERROR]MPIPolylib::move():select_exclude_trias() failed. returns:" << PolylibStat2::String(ret) << std::endl;
				return ret;
			}

			// move実行
			if( (ret = p_pg->move( params )) != PLSTAT_OK ) {
				PL_ERROSH << "[ERROR]MPIPolylib::move():(*group_itr)->move() failed. returns:" << PolylibStat2::String(ret) << std::endl;
				return ret;
			}

			// KD木を再構築 (三角形同士の位置関係が変化したため、再構築が必要)
			if( (ret = p_pg->rebuild_polygons()) != PLSTAT_OK ) {
				PL_ERROSH << "[ERROR]MPIPolylib::move():(*group_itr)->rebuild_polygons() failed. returns:" << PolylibStat2::String(ret) << std::endl;
				return ret;
			}
		}
	}
	return PLSTAT_OK;
}


// public /////////////////////////////////////////////////////////////////////
template <typename T>
POLYLIB_STAT
MPIPolylib<T>::migrate(
)
{
#ifdef DEBUG
	PL_DBGOSH << "MPIPolylib::migrate() in. " << std::endl;
#endif
	POLYLIB_STAT ret;
	unsigned int i, j;
	typename std::vector<PolygonGroup<T>*>::iterator group_itr;
	PolygonGroup<T> *p_pg;
	typename std::vector<ParallelInfo<T>*>::iterator procs_itr;

	std::vector<PrivateTriangle<T>*> const *p_trias;
	std::vector<int>   send_num_trias;
	int          *p_send_num_trias_array;
	std::vector<int>   send_tria_ids;
	int          *p_send_tria_ids_array;
	std::vector<T> send_trias;
	T        *p_send_trias_array;

	// 送信バッファ領域をMPI_WaitAll()後に纏めてdeleteするための
	// 配列アドレス記憶用ベクタ
	std::vector<int*>  send_num_trias_bufs;
	std::vector<int*>  send_tria_ids_bufs;
	std::vector<T*> send_trias_bufs;

	// 送信用MPI_Reqeust配列を確保
	MPI_Request *mpi_reqs = new MPI_Request[ m_neibour_procs.size() * 3 ]; // 隣接PEごとに3回Isendする
	MPI_Status  *mpi_stats = new MPI_Status[ m_neibour_procs.size() * 3 ];
	int reqs_pos = 0;

	//隣接PEごとに移動三角形情報を送信
	for (procs_itr = m_neibour_procs.begin(); procs_itr != m_neibour_procs.end(); procs_itr++) {

		// 送信用一時データ初期化
		send_num_trias.clear();
		send_trias.clear();
		send_tria_ids.clear();
		
		// 全ポリゴングループに対して
		for( group_itr=this->m_pg_list.begin(); group_itr!=this->m_pg_list.end(); group_itr++ ) {
			p_pg = (*group_itr);
			p_trias = NULL;

			// ポリゴン情報を持つグループだけ
			//if( p_pg->get_triangles() != NULL
			// && p_pg->get_triangles()->size() != 0 ) {

			// 移動する可能性のあるポリゴングループのみ対象
#ifdef DEBUG
PL_DBGOSH << "pg_name:" << p_pg->get_name() << " movable:" << p_pg->get_movable() << std::endl;
#endif
			if( p_pg->get_movable() ) {

				// 当該隣接PE領域への移動除外三角形IDリストを取得
			  typename std::map< int, std::vector<int> >::iterator const itr =
					(*procs_itr)->m_exclusion_map.find( p_pg->get_internal_id() );

				// 当該隣接PE領域内にある移動フラグONの三角形を取得
				p_trias = p_pg->search_outbounded(
					(*procs_itr)->m_area.m_gcell_bbox, &((*itr).second) );
			}

			// グループIDと当該グループの三角形数の対を送信データに追加
			pack_num_trias( &send_num_trias, p_pg->get_internal_id(), p_trias );

			// 三角形情報を送信データに追加
			pack_trias( &send_trias, p_trias );

			// 三角形ID情報を送信データに追加
			pack_tria_ids( &send_tria_ids, p_trias );

			// search結果の後始末
			if( p_trias ) delete p_trias;
		}

		//-----  送信データをシリアライズ
		// 送信データ初期化
		p_send_num_trias_array = NULL;
		p_send_tria_ids_array  = NULL;
		p_send_trias_array     = NULL;

		// グループID,グループ毎三角形数リスト
		if( send_num_trias.size() > 0 ) {
			p_send_num_trias_array = new int[ send_num_trias.size() ];
		}
		for( i=0; i<send_num_trias.size(); i++ ) {
			p_send_num_trias_array[i] = send_num_trias[i];
		}
		// 三角形IDリスト
		if( send_tria_ids.size() > 0 ) {
			p_send_tria_ids_array  = new int[ send_tria_ids.size() ];
		}
		for( i=0; i<send_tria_ids.size(); i++ ) {
			p_send_tria_ids_array[i] = send_tria_ids[i];
		}
		// 三角形頂点リスト
		if( send_trias.size() > 0 ) {
			p_send_trias_array = new T[ send_trias.size() ];
		}
		for( i=0; i<send_trias.size(); i++ ) {
			p_send_trias_array[i] = send_trias[i];
		}

		// 送信データの先頭アドレスを記憶（MPI_Wait後にdeleteするため）
		send_num_trias_bufs.push_back( p_send_num_trias_array );
		send_tria_ids_bufs.push_back( p_send_tria_ids_array );
		send_trias_bufs.push_back( p_send_trias_array );

		// 当該PEへ非同期送信 (MPI_Wait()は後でまとめて行う)
#ifdef DEBUG
		PL_DBGOSH << "sending polygons rank:" << m_myrank <<  "->rank:"
				  << (*procs_itr)->m_rank << " ";
		for( i=0; i< send_num_trias.size(); i+=2 ) {
			PL_DBGOS << "(gid:" << send_num_trias[i] 
					 << ",num_tria:" << send_num_trias[i+1] << ")";
		}
		PL_DBGOS << std::endl;
#endif
		if (MPI_Isend( p_send_num_trias_array, send_num_trias.size(),
					MPI_INT, (*procs_itr)->m_rank, MPITAG_NUM_TRIAS,
					m_mycomm, &mpi_reqs[reqs_pos++] ) != MPI_SUCCESS) {
			PL_ERROSH << "[ERROR]MPIPolylib::migrate():MPI_Isend,"
					  << "MPITAG_NUM_TRIAS faild." << std::endl;
			return PLSTAT_MPI_ERROR;
		}
		if (MPI_Isend( p_send_tria_ids_array,  send_tria_ids.size(),
					MPI_INT, (*procs_itr)->m_rank, MPITAG_TRIA_IDS,
					m_mycomm, &mpi_reqs[reqs_pos++] ) != MPI_SUCCESS) {
			PL_ERROSH << "[ERROR]MPIPolylib::migrate():MPI_Isend,"
					  << " MPITAG_TRIA_IDS faild." << std::endl;
			return PLSTAT_MPI_ERROR;
		}
		if (MPI_Isend( p_send_trias_array,     send_trias.size(),
					PL_MPI_REAL, (*procs_itr)->m_rank, MPITAG_TRIAS,
					m_mycomm, &mpi_reqs[reqs_pos++] ) != MPI_SUCCESS) {
			PL_ERROSH << "[ERROR]MPIPolylib::migrate():MPI_Isend,"
					  << " MPITAG_TRIAS faild." << std::endl;
			return PLSTAT_MPI_ERROR;
		}
	}

	//隣接PEごとに移動三角形情報を受信
	for (procs_itr = m_neibour_procs.begin(); procs_itr != m_neibour_procs.end(); procs_itr++) {
		int pos_id, pos_tria;
		MPI_Request mpi_req;
		MPI_Status  mpi_stat;

		// グループIDとグループ毎三角形数の対を非同期受信
		// グループ情報は各rank共有しているのでグループ数は予め分かっている
		int *p_intarray = new int[ this->m_pg_list.size()*2 ];
		if (MPI_Irecv( p_intarray, this->m_pg_list.size()*2, MPI_INT, (*procs_itr)->m_rank,
					MPITAG_NUM_TRIAS, m_mycomm, &mpi_req ) != MPI_SUCCESS) {
			PL_ERROSH << "[ERROR]MPIPolylib::migrate():MPI_Irecv,"
					  << ",MPITAG_NUM_TRIAS faild." << std::endl;
			return PLSTAT_MPI_ERROR;
		}
		if (MPI_Wait( &mpi_req, &mpi_stat ) != MPI_SUCCESS) {
			PL_ERROSH << "[ERROR]MPIPolylib::migrate():MPI_Wait,"
					  << "MPITAG_NUM_TRIAS faild." << std::endl;
			return PLSTAT_MPI_ERROR;
		}
#ifdef DEBUG
		PL_DBGOSH << "receiving polygons rank:" << (*procs_itr)->m_rank
				  <<  "->rank:" << m_myrank << " ";
		for( i=0; i< this->m_pg_list.size()*2-1; i+=2 ) {
			PL_DBGOS << "(gid:" << p_intarray[i] 
					 << ",num_tria:" << p_intarray[i+1] << ")";
		}
		PL_DBGOS << std::endl;
#endif

		// 受信三角形数を算出
		int total_tria_num = 0;
		for( i=1; i<this->m_pg_list.size() * 2; i+=2 ){
			total_tria_num += p_intarray[i];
		}

		// 三角形IDリストを非同期受信
		int *p_idarray = new int[ total_tria_num ];
		if (MPI_Irecv( p_idarray,  total_tria_num, MPI_INT, (*procs_itr)->m_rank,
					 MPITAG_TRIA_IDS, m_mycomm, &mpi_req ) != MPI_SUCCESS) {
			PL_ERROSH << "[ERROR]MPIPolylib::migrate():MPI_Irecv,"
					  << "MPI_INT faild." << std::endl;
			return PLSTAT_MPI_ERROR;
		}
		if (MPI_Wait( &mpi_req, &mpi_stat ) != MPI_SUCCESS) {
			PL_ERROSH << "[ERROR]MPIPolylib::migrate():MPI_Wait,"
					  << "MPI_INT faild." << std::endl;
			return PLSTAT_MPI_ERROR;
		}

		// 三角形リストを非同期受信
		T *p_triaarray = new T[ total_tria_num*3*3 ];
		if (MPI_Irecv( p_triaarray, total_tria_num*3*3, PL_MPI_REAL, (*procs_itr)->m_rank,
					MPITAG_TRIAS, m_mycomm, &mpi_req ) != MPI_SUCCESS) {
			PL_ERROSH << "[ERROR]MPIPolylib::migrate():MPI_Irecv,PL_MPI_REAL:"
					  << " faild." << std::endl;
			return PLSTAT_MPI_ERROR;
		}
		if (MPI_Wait( &mpi_req, &mpi_stat ) != MPI_SUCCESS) {
			PL_ERROSH << "[ERROR]MPIPolylib::migrate():MPI_Wait,PL_MPI_REAL"
					  << " faild." << std::endl;
			return PLSTAT_MPI_ERROR;
		}

		// 各ポリゴングループに対して三角形情報を追加
		pos_id = 0;
		pos_tria = 0;
		for( i=0; i<this->m_pg_list.size()*2-1; i+=2 ){

			// ポリゴングループID
			int pg_id = p_intarray[i];

			// 当該ポリゴングループの三角形数
			unsigned int num_trias = p_intarray[i+1];

			// グループIDのポリゴングループインスタンス取得
			PolygonGroup<T>* p_pg = this->get_group( pg_id );
			if( p_pg == NULL ) {
				PL_ERROSH << "[ERROR]MPIPolylib::migrate():invalid pg_id:"
					  	<< pg_id << std::endl;
				return PLSTAT_NG;
			}

			// PrivateTriangle<T>のベクタ - 受信データ配列からベクタへの変換用
			std::vector<PrivateTriangle<T>*> tria_vec;

			// ベクタに受信データ内容をコピー
			for( j=0; j<num_trias; j++ ) {
				tria_vec.push_back(
						new PrivateTriangle<T>(&p_triaarray[pos_tria], p_idarray[pos_id]) );
				pos_id++;
				pos_tria+=9;
			}

			// ポリゴングループに三角形リストを追加
			if( (ret = p_pg->add_triangles( &tria_vec )) != PLSTAT_OK ) {
				PL_ERROSH << "[ERROR]MPIPolylib::migrate():p_pg->add_triangles() failed. returns:"
						  << PolylibStat2::String(ret) << std::endl;
				return ret;
			}

			// ベクタの内容あとしまつ
			for( j=0; j<num_trias; j++ ) {
				delete tria_vec.at(j);
			}
		}

		// 受信領域あとしまつ
		delete[] p_intarray;
		delete[] p_idarray;
		delete[] p_triaarray;
	}

	// MPI_Isend()を纏めてアンロック
	if (MPI_Waitall( m_neibour_procs.size()*3, mpi_reqs, mpi_stats ) != MPI_SUCCESS) {
		PL_ERROSH << "[ERROR]MPIPolylib::migrate():MPI_Waitall failed." << std::endl;
		return PLSTAT_MPI_ERROR;
	}

	// 送信データ領域をdelete
	for( i=0; i<send_num_trias_bufs.size(); i++ ) {
		delete[] send_num_trias_bufs.at(i);
	}
	for( i=0; i<send_tria_ids_bufs.size(); i++ ) {
		delete[] send_tria_ids_bufs.at(i);
	}
	for( i=0; i<send_trias_bufs.size(); i++ ) {
		delete[] send_trias_bufs.at(i);
	}

	// 移動してきた三角形を含めたKD木を再構築
	for (group_itr = this->m_pg_list.begin(); group_itr != this->m_pg_list.end(); group_itr++) {
		p_pg = (*group_itr);

		// 移動可能グループだけ
		if( p_pg->get_movable() && p_pg->get_triangles() != NULL ) {

			// KD木を再構築
			if( (ret=p_pg->rebuild_polygons()) != PLSTAT_OK ) {
				PL_ERROSH << "[ERROR]MPIPolylib::migrate():p_pg->rebuild_polygons() failed. returns:"
						  << PolylibStat2::String(ret) << std::endl;
				return ret;
			}
		}
	}
	
	// 自PE領域外ポリゴン情報を消去
	if( erase_outbounded_polygons() != PLSTAT_OK ) {
		PL_ERROSH << "[ERROR]MPIPolylib::migrate():rebuild_polygons() failed." << std::endl;
	}

	// 後始末 2010.08.24
	delete[] mpi_reqs;
	delete[] mpi_stats;

#ifdef DEBUG
	PL_DBGOSH << "MPIPolylib::migrate() out normaly." << std::endl;
#endif
	return PLSTAT_OK;
}


// public /////////////////////////////////////////////////////////////////////
template <typename T>
ParallelInfo<T>* MPIPolylib<T>::get_proc(int rank)
{
  typename std::vector<ParallelInfo<T>*>::iterator itr;
	itr = m_other_procs.begin();
	for (; itr != m_other_procs.end(); itr++) {
		if ((*itr)->m_rank == rank) {
			return(*itr);
		}
	}
	return NULL;
}


// public /////////////////////////////////////////////////////////////////////
template <typename T>
unsigned int MPIPolylib<T>::used_memory_size()
{
	unsigned int								size;
	typename std::map< int, std::vector<int> >::iterator	ex;
	typename std::vector<ParallelInfo<T> *>::iterator			pi;

	// Polylibクラスが管理している領域を取得
	size = Polylib<T>::used_memory_size();

	// 自PE担当領域情報
	size += sizeof(ParallelInfo<T>);
	size += m_myproc.m_exclusion_map.size() * (sizeof(int)+sizeof(std::vector<int>));
	for (ex = m_myproc.m_exclusion_map.begin(); 
							ex != m_myproc.m_exclusion_map.end(); ex++) {
		size += ex->second.size() * sizeof(int);
	}

	// 自PEを除く全PE担当領域情報リスト
	size += sizeof(std::vector<ParallelInfo<T> *>);
	for (pi = m_other_procs.begin(); pi != m_other_procs.end(); pi++) {
		size += sizeof(ParallelInfo<T>);
		size += (*pi)->m_exclusion_map.size() * (sizeof(int)+sizeof(std::vector<int>));
		for (ex = (*pi)->m_exclusion_map.begin(); 
									ex != (*pi)->m_exclusion_map.end(); ex++) {
			size += ex->second.size() * sizeof(int);
		}
	}

	// 隣接PE担当領域情報リスト
	size += sizeof(std::vector<ParallelInfo<T> *>);
	size += m_neibour_procs.size() * sizeof(std::vector<ParallelInfo<T> *>);

	// 自プロセスのランク数、全プロセス数
	size += sizeof(int) * 2;

	// 自プロセスが利用するコミュニケーター
	size += sizeof(MPI_Comm);

	return size;
}


// protected //////////////////////////////////////////////////////////////////
template <typename T> 
MPIPolylib<T>::MPIPolylib() : Polylib<T>()
{
}


// protected //////////////////////////////////////////////////////////////////
template <typename T>
MPIPolylib<T>::~MPIPolylib()
{
  typename std::vector<ParallelInfo<T>*>::iterator itr;

	// 全PE領域情報リストを消去
	for ( itr=m_other_procs.begin(); itr != m_other_procs.end(); itr++ ) {
		delete *itr;
	}
	// 隣接PE領域情報リストについては、m_other_procsの内容と同じインスタンスを保持
	// するため、上記の処理で消去済。
}


// protected //////////////////////////////////////////////////////////////////
template <typename T>
POLYLIB_STAT
MPIPolylib<T>::broadcast_config(
	std::string config_contents
)
{
#ifdef DEBUG
	PL_DBGOSH << "MPIPolylib::broadcast_config() in. " << std::endl;
#endif

	// 文字数を送信(NULL文字も文字数にカウント）
	int data = config_contents.size()+1;
	if (MPI_Bcast( &data, 1, MPI_INT, 0, m_mycomm ) != MPI_SUCCESS) {
		PL_ERROSH << "[ERROR]MPIPolylib::broadcast_config():MPI_Bcast,"
				 << "MPI_INT faild." << std::endl;
		return PLSTAT_MPI_ERROR;
	}

	// ファイル内容文字列の送信
	if (MPI_Bcast( (void*)(config_contents.c_str()), config_contents.size()+1,
				MPI_CHAR, 0, m_mycomm ) != MPI_SUCCESS) {
		PL_ERROSH << "[ERROR]MPIPolylib::broadcast_config():MPI_Bcast,"
				  << "MPI_CHAR faild." << std::endl;
		return PLSTAT_MPI_ERROR;
	}

	return PLSTAT_OK;
}


// protected //////////////////////////////////////////////////////////////////
template <typename T>
POLYLIB_STAT
MPIPolylib<T>::send_polygons_to_all(
)
{
#ifdef DEBUG
	PL_DBGOSH << "MPIPolylib::send_polygons_to_all() in. " << std::endl;
#endif
	unsigned int i;
	typename std::vector<ParallelInfo<T>*>::iterator	proc_itr;
	typename std::vector<PolygonGroup<T>*>::iterator group_itr;
	PolygonGroup<T> *p_pg;
	std::vector<PrivateTriangle<T>*> const *p_trias;

	std::vector<int>   send_num_trias;
	int          *p_send_num_trias_array;
	std::vector<int>   send_tria_ids;
	int          *p_send_tria_ids_array;
	std::vector<T> send_trias;
	T        *p_send_trias_array;

	// 全PEに対して
	for (proc_itr = m_other_procs.begin(); proc_itr != m_other_procs.end(); proc_itr++) {

		// 送信用一時データ初期化
		p_trias = NULL;
		send_num_trias.clear();
		send_trias.clear();
		send_tria_ids.clear();
		
		// 全グループに対して
		for (group_itr = this->m_pg_list.begin(); group_itr != this->m_pg_list.end(); group_itr++) {
			p_pg = (*group_itr);
			p_trias = NULL;

			// ポリゴン情報を持つグループだけ
			if( p_pg->get_triangles() != NULL && p_pg->get_triangles()->size() != 0 ) {

				// 当該PE領域内に一部でも含まれるポリゴンを検索
				p_trias = p_pg->search( &((*proc_itr)->m_area.m_gcell_bbox), false );
			}

			// グループIDと当該グループの三角形数の対を送信データに追加
			pack_num_trias( &send_num_trias, p_pg->get_internal_id(), p_trias );

			// 三角形情報を送信データに追加
			pack_trias( &send_trias, p_trias );

			// 三角形ID情報を送信データに追加
			pack_tria_ids( &send_tria_ids, p_trias );

			// search結果の後始末
			if( p_trias ) delete p_trias;
		}

		//----  送信データをシリアライズ
		// 送信データ配列初期化
		p_send_num_trias_array = NULL;
		p_send_tria_ids_array  = NULL;
		p_send_trias_array     = NULL;
 
		// グループID,グループ毎三角形数リスト
		if( send_num_trias.size() > 0 ) {
			p_send_num_trias_array = new int[ send_num_trias.size() ];
		}
		for( i=0; i<send_num_trias.size(); i++ ) {
			p_send_num_trias_array[i] = send_num_trias[i];
		}
		// 三角形IDリスト
		if( send_tria_ids.size() > 0 ) {
			p_send_tria_ids_array  = new int[ send_tria_ids.size() ];
		}
		for( i=0; i<send_tria_ids.size(); i++ ) {
			p_send_tria_ids_array[i] = send_tria_ids[i];
		}
		// 三角形頂点座標リスト
		if( send_trias.size() > 0 ) {
			p_send_trias_array = new T[ send_trias.size() ];
		}
		for( i=0; i<send_trias.size(); i++ ) {
			p_send_trias_array[i] = send_trias[i];
		}

		// 当該PEへ送信
#ifdef DEBUG
		PL_DBGOSH << "sending polygons rank:0->rank:" << (*proc_itr)->m_rank << " ";
		for( i=0; i< send_num_trias.size(); i+=2 ) {
			PL_DBGOS << "(gid:" << send_num_trias[i] 
					 << ",num_tria:" << send_num_trias[i+1] << ")";
		}
		PL_DBGOS << std::endl;
#endif
		if (MPI_Send( p_send_num_trias_array, send_num_trias.size(), MPI_INT,
			(*proc_itr)->m_rank, MPITAG_NUM_TRIAS, m_mycomm ) != MPI_SUCCESS) {
			PL_ERROSH << "[ERROR]MPIPolylib::send_polygons_to_all():MPI_Send,"
					  << "MPITAG_NUM_TRIAS faild." << std::endl;
			return PLSTAT_MPI_ERROR;
		}
		if (MPI_Send( p_send_tria_ids_array,  send_tria_ids.size(),  MPI_INT,
			(*proc_itr)->m_rank, MPITAG_TRIA_IDS,  m_mycomm ) != MPI_SUCCESS) {
			PL_ERROSH << "[ERROR]MPIPolylib::send_polygons_to_all():MPI_Send,"
					  << "MPITAG_TRIA_IDS faild." << std::endl;
			return PLSTAT_MPI_ERROR;
		}
		if (MPI_Send( p_send_trias_array,     send_trias.size(),     PL_MPI_REAL,
			(*proc_itr)->m_rank, MPITAG_TRIAS,     m_mycomm ) != MPI_SUCCESS) {
			PL_ERROSH << "[ERROR]MPIPolylib::send_polygons_to_all():MPI_Send,"
					  << "MPITAG_TRIAS faild." << std::endl;
			return PLSTAT_MPI_ERROR;
		}

		// 送信データの後始末
		if( p_send_num_trias_array ) delete[] p_send_num_trias_array;
		if( p_send_tria_ids_array )  delete[] p_send_tria_ids_array;
		if( p_send_trias_array )     delete[] p_send_trias_array;
	}
	return PLSTAT_OK;
}


// protected //////////////////////////////////////////////////////////////////
template <typename T>
POLYLIB_STAT
MPIPolylib<T>::pack_num_trias(
	std::vector<int>* p_vec,
	int group_id,
	const std::vector<PrivateTriangle<T>*>* p_trias
)
{
#ifdef DEBUG
	PL_DBGOSH << "MPIPolylib::pack_num_trias() in. " << std::endl;
#endif
	// 出力配列に、グループID、グループ内三角形数、の順に追加
	int num = 0;
	if( p_trias ) num = p_trias->size();
	p_vec->push_back( group_id );
	p_vec->push_back( num );

	return PLSTAT_OK;
}


// protected //////////////////////////////////////////////////////////////////
template <typename T>
POLYLIB_STAT
MPIPolylib<T>::pack_trias(
	std::vector<T>* p_vec,
	const std::vector<PrivateTriangle<T>*>* p_trias
)
{
#ifdef DEBUG
	PL_DBGOSH << "MPIPolylib::pack_trias() in. " << std::endl;
#endif
	if( p_trias == NULL ) return PLSTAT_OK;

	// 出力配列に、三角形頂点座標を順に追加
	for( unsigned int i=0; i<p_trias->size(); i++ ) {
	  Vertex<T>** tmp_vlist=p_trias->at(i)->get_vertex();
		for( unsigned int j=0; j<3; j++ ) {
		  Vertex<T>* vtmp= tmp_vlist[j];
			for( unsigned int k=0; k<3; k++ ) {
			  p_vec->push_back(  (*vtmp)[k] );
			  //p_vec->push_back( p_trias->at(i)->get_vertex()[j][k] );
			}
		}
	}
	return PLSTAT_OK;
}


// protected //////////////////////////////////////////////////////////////////
template <typename T>
POLYLIB_STAT
MPIPolylib<T>::pack_tria_ids(
	std::vector<int>* p_vec,
	const std::vector<PrivateTriangle<T>*>* p_trias
)
{
#ifdef DEBUG
	PL_DBGOSH << "MPIPolylib::pack_tria_ids() in. " << std::endl;
#endif
	if( p_trias == NULL ) return PLSTAT_OK;

	// 出力配列に、三角形IDを順に追加
	for( unsigned int i=0; i<p_trias->size(); i++ ) {
		p_vec->push_back( p_trias->at(i)->get_id() );
	}
	return PLSTAT_OK;
}


// protected //////////////////////////////////////////////////////////////////
template <typename T>
POLYLIB_STAT
MPIPolylib<T>::erase_outbounded_polygons(
)
{
#define DEBUG
#ifdef DEBUG
	PL_DBGOSH << "MPIPolylib::erase_outbounded_polygons() in. " << std::endl;
#endif
	POLYLIB_STAT ret;
	unsigned int i;
	typename std::vector<PolygonGroup<T>*>::iterator group_itr;
	std::vector<PrivateTriangle<T>*> const *p_trias;
	std::vector<PrivateTriangle<T>*>  copy_trias;
	PolygonGroup<T> *p_pg;

	// 各ポリゴングループのポリゴン情報を自領域分のみで再構築
	// 全グループに対して
	for (group_itr = this->m_pg_list.begin(); group_itr != this->m_pg_list.end(); group_itr++) {
		p_pg = (*group_itr);

		// ポリゴン情報を持つグループだけ
		if( p_pg->get_triangles() != NULL && p_pg->get_triangles()->size() != 0 ) {

			// 自領域内に一部でも含まれるポリゴンを検索
			p_trias = p_pg->search( &(m_myproc.m_area.m_gcell_bbox), false );

			// 検索結果のディープコピーを作成
			copy_trias.clear();
			if( p_trias ) {
				for( i=0; i<p_trias->size(); i++ ) {
					copy_trias.push_back( new PrivateTriangle<T>(*(p_trias->at(i))) );
				}
			}

			// 検索結果でポリゴン情報を再構築
			if( (ret = p_pg->init( &copy_trias, true )) != PLSTAT_OK ) {
				PL_ERROSH << "[ERROR]MPIPolylib::erase_outbounded_polygons():p_pg->init() failed. returns:"
						  << PolylibStat2::String(ret) << std::endl;
				return ret;
			}

			// search結果の後始末
			if( p_trias ) delete p_trias;
			for( i=0; i<copy_trias.size(); i++ ) {
				delete copy_trias.at(i);
			}
		}
	}
	return PLSTAT_OK;
#undef DEBUG
}


// protected //////////////////////////////////////////////////////////////////
template <typename T>
POLYLIB_STAT
MPIPolylib<T>::broadcast_config_from_rank0(
)
{
#ifdef DEBUG
	PL_DBGOSH << "MPIPolylib::broadcast_config_from_rank0() in. " << std::endl;
#endif
	POLYLIB_STAT ret;
	int str_len;
	char *p_str;
	std::string config_str;

	// 文字数をb_castで受信(NULL文字も文字数に含まれる)
	if (MPI_Bcast( &str_len, 1, MPI_INT, 0, m_mycomm ) != MPI_SUCCESS) {
		PL_ERROSH << "[ERROR]MPIPolylib::broadcast_config_from_rank0()"
				  << ":MPI_Bcast,MPI_INT faild." << std::endl;
		return PLSTAT_MPI_ERROR;
	}

	// XML文字列をb_castで受信
	p_str = new char[str_len];
	if (MPI_Bcast( p_str, str_len, MPI_CHAR, 0, m_mycomm ) != MPI_SUCCESS) {
		PL_ERROSH << "[ERROR]MPIPolylib::broadcast_config_from_rank0()"
				  << ":MPI_Bcast,MPI_CHAR faild." << std::endl;
		return PLSTAT_MPI_ERROR;
	}
#ifdef DEBUG
	PL_DBGOSH << "received config-string:" << p_str << std::endl;
#endif

	// XML文字列からグループ階層構造を構築
	config_str = p_str;
	if( (ret = this->make_group_tree( config_str )) != PLSTAT_OK ) {
		PL_ERROSH << "[ERROR]MPIPolylib::broadcast_config_from_rank0():make_group_tree() faild. returns:" << ret << std::endl;
		return ret;
	}

	return PLSTAT_OK;
}


// protected //////////////////////////////////////////////////////////////////
template <typename T>
POLYLIB_STAT
MPIPolylib<T>::receive_polygons_from_rank0(
)
{
#ifdef DEBUG
	PL_DBGOSH << "MPIPolylib::receive_polygons_from_rank0() in. " << std::endl;
#endif

	unsigned int i, j;
	unsigned int pos_id, pos_tria;
	MPI_Status mpi_stat;

	// グループIDとグループ毎三角形数の対をrank0から受信
	// グループ情報は配信済みなので、グループ数は予め分かっている
	int *p_intarray = new int[ this->m_pg_list.size() * 2 ];
	if (MPI_Recv( p_intarray, this->m_pg_list.size() * 2, MPI_INT, 0, 
				MPITAG_NUM_TRIAS, m_mycomm, &mpi_stat ) != MPI_SUCCESS) {
		PL_ERROSH << "[ERROR]MPIPolylib::receive_polygons_from_rank0()"
				  << ":MPI_Recv,MPITAG_NUM_TRIAS faild." << std::endl;
		return PLSTAT_MPI_ERROR;
	}
#ifdef DEBUG
	PL_DBGOSH << "    pintarray:(";
	for( i=0; i< this->m_pg_list.size()*2; i++ ) {
		PL_DBGOS << p_intarray[i] << ",";
	}
	PL_DBGOS << ")" << std::endl;
#endif

	// 自領域の全三角形数を算出
	unsigned int total_tria_num = 0;
	for( i=1; i<this->m_pg_list.size() * 2; i+=2 ){
		total_tria_num += p_intarray[i];
	}

	// 三角形IDリストをrank0から受信
	int *p_idarray = new int[ total_tria_num ];
	if (MPI_Recv( p_idarray,  total_tria_num, MPI_INT, 0, 
				MPITAG_TRIA_IDS, m_mycomm, &mpi_stat ) != MPI_SUCCESS) {
		PL_ERROSH << "[ERROR]MPIPolylib::receive_polygons_from_rank0()"
				  << ":MPI_Recv,MPITAG_TRIA_IDS faild." << std::endl;
		return PLSTAT_MPI_ERROR;
	}
#ifdef DEBUG
	PL_DBGOSH << "    pidarray:(";
	for( i=0; i<total_tria_num; i++ ) {
		PL_DBGOS << p_idarray[i] << ",";
	}
	PL_DBGOS << ")" << std::endl;
#endif

	// 三角形リストをrank0から受信
	T *p_triaarray = new T[ total_tria_num * 3 * 3 ];
	if (MPI_Recv( p_triaarray, total_tria_num * 3 * 3, PL_MPI_REAL, 0, 
				MPITAG_TRIAS, m_mycomm, &mpi_stat ) != MPI_SUCCESS) {
		PL_ERROSH << "[ERROR]MPIPolylib::receive_polygons_from_rank0()"
				  << ":MPI_Recv,MPITAG_TRIAS faild." << std::endl;
		return PLSTAT_MPI_ERROR;
	}
#ifdef DEBUG
	PL_DBGOSH << "    ptriaarray:(";
	for( i=0; i<total_tria_num*3*3; i++ ) {
		PL_DBGOS << p_triaarray[i] << ",";
	}
	PL_DBGOS << ")" << std::endl;
#endif

	// 各ポリゴングループに対して三角形情報を設定＆KD木構築
	pos_id = 0;
	pos_tria = 0;
	int n_start_tri=0;
	int n_start_id=0;
	for( i=0; i<this->m_pg_list.size()*2-1; i+=2 ){	// 偶数番目の値を処理

		// ポリゴングループID
		int pg_id = p_intarray[i];

		// 当該ポリゴングループの三角形数
		unsigned int num_trias = p_intarray[i+1];

		// グループIDのポリゴングループインスタンス取得
		PolygonGroup<T>* p_pg = this->get_group( pg_id );
		if( p_pg == NULL ) {
			PL_ERROSH << "[ERROR]MPIPolylib::receive_polygons_from_rank0():invalid pg_id:"
					  << pg_id << std::endl;
			return PLSTAT_NG;
		}



		// ポリゴングループに三角形リストを設定、KD木構築
		if( p_pg->init( p_triaarray, p_idarray, n_start_tri,n_start_id,num_trias ) != PLSTAT_OK ) {
		  PL_ERROSH << "[ERROR]MPIPolylib::receive_polygons_from_rank0():p_pg->init() failed:" << std::endl;
			return PLSTAT_NG;
		}
		n_start_tri+=num_trias*9;
		n_start_id+=num_trias;

		// // PrivateTriangle<T>のベクタ生成
		// std::vector<PrivateTriangle<T>*> tria_vec;

		// // ベクタに受信データ内容を設定
		// for( j=0; j<num_trias; j++ ) {
		// 	tria_vec.push_back( new PrivateTriangle<T>(&p_triaarray[pos_tria], p_idarray[pos_id]) );
		// 	pos_id++;
		// 	pos_tria+=9;
		// }

		// // ポリゴングループに三角形リストを設定、KD木構築
		// if( p_pg->init( &tria_vec, true ) != PLSTAT_OK ) {
		// 	PL_ERROSH << "[ERROR]MPIPolylib::receive_polygons_from_rank0():p_pg->init() failed:" << std::endl;
		// 	return PLSTAT_NG;
		// }

		// // ベクタの内容あとしまつ
		// for( j=0; j<num_trias; j++ ) {
		// 	delete tria_vec.at(j);
		// }

	}

	// 受信領域あとしまつ
	delete[] p_intarray;
	delete[] p_idarray;
	delete[] p_triaarray;

#ifdef DEBUG
	PL_DBGOSH << "MPIPolylib::receive_polygons_from_rank0() out. " << std::endl;
#endif
	return PLSTAT_OK;
}


// protected //////////////////////////////////////////////////////////////////
template <typename T>
POLYLIB_STAT
MPIPolylib<T>::gather_polygons(
)
{
#ifdef DEBUG
	PL_DBGOSH << "MPIPolylib::gather_polygons() in. " << std::endl;
#endif

	POLYLIB_STAT ret;
	unsigned int i, j;
	int rank;
	unsigned int pos_id, pos_tria;
	MPI_Status mpi_stat;

	// 全rankからポリゴン情報を受信
	for( rank=1; rank<m_numproc; rank++ ) {

		// グループIDとグループ毎三角形数の対を受信
		// グループ情報は全rank共通なので、グループ数は予め分かっている
		int *p_intarray = new int[ this->m_pg_list.size() * 2 ];
		if (MPI_Recv( p_intarray, this->m_pg_list.size() * 2, MPI_INT, rank, 
					MPITAG_NUM_TRIAS, m_mycomm, &mpi_stat ) != MPI_SUCCESS) {
			PL_ERROSH << "[ERROR]MPIPolylib::gather_polygons()"
					  << ":MPI_Recv,MPITAG_NUM_TRIAS faild.:rank=" << rank 
					  << std::endl;
			return PLSTAT_MPI_ERROR;
		}

		// 受信する全三角形数を算出
		unsigned int total_tria_num = 0;
		for( i=1; i<this->m_pg_list.size() * 2; i+=2 ){
			total_tria_num += p_intarray[i];
		}

		// 三角形IDリストを受信
		int *p_idarray = new int[ total_tria_num ];
		if (MPI_Recv( p_idarray,  total_tria_num, MPI_INT, rank, 
					MPITAG_TRIA_IDS, m_mycomm, &mpi_stat ) != MPI_SUCCESS) {
			PL_ERROSH << "[ERROR]MPIPolylib::gather_polygons()"
					  << ":MPI_Recv,MPITAG_TRIA_IDS faild.:rank=" << rank 
					  << std::endl;
			return PLSTAT_MPI_ERROR;
		}

		// 三角形リストを受信
		T *p_triaarray = new T[ total_tria_num * 3 * 3 ];
		if (MPI_Recv( p_triaarray, total_tria_num * 3 * 3, PL_MPI_REAL, rank, 
					MPITAG_TRIAS, m_mycomm, &mpi_stat ) != MPI_SUCCESS) {
			PL_ERROSH << "[ERROR]MPIPolylib::gather_polygons()"
					  << ":MPI_Recv,MPITAG_TRIA_IDS faild.:rank=" << rank 
					  << std::endl;
			return PLSTAT_MPI_ERROR;
		}

		// 各ポリゴングループに対して受信した三角形情報を追加
		pos_id = 0;
		pos_tria = 0;
		int n_start_tri=0;
		int n_start_id=0;
	
		for( i=0; i<this->m_pg_list.size() * 2; i++ ){

			// ポリゴングループID
			int pg_id = p_intarray[i];

			// 当該ポリゴングループの三角形数
			unsigned int num_trias = p_intarray[i+1];

			// グループIDのポリゴングループインスタンス取得
			PolygonGroup<T>* p_pg = this->get_group( pg_id );
			if( p_pg == NULL ) {
				PL_ERROSH << "[ERROR]MPIPolylib::gather_polygons():invalid pg_id:"
						  << pg_id << std::endl;
				return PLSTAT_NG;
			}


			//ポリゴングループに三角形リストを追加
			if( p_pg->init( p_triaarray, p_idarray, n_start_tri,n_start_id,num_trias ) != PLSTAT_OK ) {
			  PL_ERROSH << "[ERROR]MPIPolylib::gather_polygons():p_pg->add() failed. returns:" 
				    << PolylibStat2::String(ret) << std::endl;
				return ret;
			}
			n_start_tri+=num_trias*9;
			n_start_id+=num_trias;


			// //PrivateTriangle<T>のベクタ生成
			// std::vector<PrivateTriangle<T>*> tria_vec;

			// //ベクタに受信データ内容を設定
			// for( j=0; j<num_trias; j++ ) {
			// 	tria_vec.push_back( new PrivateTriangle<T>(&p_triaarray[pos_tria], p_idarray[pos_id]) );
			// 	pos_id++;
			// 	pos_tria+=9;
			// }

			// //ポリゴングループに三角形リストを追加
			// if( (ret = p_pg->add_triangles( &tria_vec )) != PLSTAT_OK ) {
			// 	PL_ERROSH << "[ERROR]MPIPolylib::gather_polygons():p_pg->init() failed. returns:" << PolylibStat2::String(ret) << std::endl;
			// 	return ret;
			// }

			// //ベクタの内容あとしまつ
			// for( j=0; j<num_trias; j++ ) {
			// 	delete tria_vec.at(j);
			// }

			// 次のポリゴングループID位置へ
			i++;
		}

		// 受信領域あとしまつ
		if( p_intarray != NULL )  delete[] p_intarray;
		if( p_idarray != NULL )   delete[] p_idarray;
		if( p_triaarray != NULL ) delete[] p_triaarray;
	}

	return PLSTAT_OK;
}


// protected //////////////////////////////////////////////////////////////////
template <typename T>
POLYLIB_STAT
MPIPolylib<T>::send_polygons_to_rank0(
)
{
#ifdef DEBUG
	PL_DBGOSH << "MPIPolylib::send_polygons_to_rank0() in. " << std::endl;
#endif
	unsigned int i;
	typename std::vector<PolygonGroup<T>*>::iterator group_itr;
	PolygonGroup<T> *p_pg;
	std::vector<PrivateTriangle<T>*> const *p_trias;

	std::vector<int>   send_num_trias;
	int          *p_send_num_trias_array;
	std::vector<int>   send_tria_ids;
	int          *p_send_tria_ids_array;
	std::vector<T> send_trias;
	T        *p_send_trias_array;

	// 送信用一時データ初期化
	p_trias = NULL;
	send_num_trias.clear();
	send_trias.clear();
	send_tria_ids.clear();
	
	// 全グループに対して
	for (group_itr = this->m_pg_list.begin(); group_itr != this->m_pg_list.end(); group_itr++) {
		p_pg = (*group_itr);

		// ポリゴン情報を持つグループだけ
		if( p_pg->get_triangles() != NULL && p_pg->get_triangles()->size() != 0 ) {

			// 当該PE領域内に一部でも含まれるポリゴンを検索
			p_trias = p_pg->search( &(m_myproc.m_area.m_gcell_bbox), false );
		}

		// グループIDと当該グループの三角形数の対を送信データに追加
		pack_num_trias( &send_num_trias, p_pg->get_internal_id(), p_trias );

		// 三角形情報を送信データに追加
		pack_trias( &send_trias, p_trias );

		// 三角形ID情報を送信データに追加
		pack_tria_ids( &send_tria_ids, p_trias );

		// search結果の後始末
		if( p_trias ) delete p_trias;
		p_trias = NULL;
	}

	// 送信データをシリアライズ
	p_send_num_trias_array = new int[ send_num_trias.size() ];
	for( i=0; i<send_num_trias.size(); i++ )
		p_send_num_trias_array[i] = send_num_trias[i];

	p_send_tria_ids_array  = new int[ send_tria_ids.size() ];
	for( i=0; i<send_tria_ids.size(); i++ )
		p_send_tria_ids_array[i] = send_tria_ids[i];

	p_send_trias_array =     new T[ send_trias.size() ];
	for( i=0; i<send_trias.size(); i++ )
		p_send_trias_array[i] = send_trias[i];

	// rank0へ送信
#ifdef DEBUG
	PL_DBGOSH << "sending polygons rank:" << m_myrank << " -> rank:0 ";
	for( i=0; i< send_num_trias.size(); i+=2 ) {
		PL_DBGOS << "(gid:" << send_num_trias[i] 
				 << ",num_tria:" << send_num_trias[i+1] << ")";
	}
	PL_DBGOS << std::endl;
#endif
	if (MPI_Send( p_send_num_trias_array, send_num_trias.size(), MPI_INT, 0,
					MPITAG_NUM_TRIAS, m_mycomm ) != MPI_SUCCESS) {
		PL_ERROSH << "[ERROR]MPIPolylib::send_polygons_to_rank0()"
				  << ":MPI_Send,MPITAG_NUM_TRIAS faild." << std::endl;
		return PLSTAT_MPI_ERROR;
	}
	if (MPI_Send( p_send_tria_ids_array,  send_tria_ids.size(),  MPI_INT, 0,
					MPITAG_TRIA_IDS,  m_mycomm ) != MPI_SUCCESS) {
		PL_ERROSH << "[ERROR]MPIPolylib::send_polygons_to_rank0()"
				  << ":MPI_Send,MPITAG_TRIA_IDS faild." << std::endl;
		return PLSTAT_MPI_ERROR;
	}
	if (MPI_Send( p_send_trias_array,     send_trias.size(),   PL_MPI_REAL, 0,
					MPITAG_TRIAS,     m_mycomm ) != MPI_SUCCESS) {
		PL_ERROSH << "[ERROR]MPIPolylib::send_polygons_to_rank0()"
				  << ":MPI_Send,MPITAG_TRIAS faild." << std::endl;
		return PLSTAT_MPI_ERROR;
	}

	// 送信データの後始末
	delete[] p_send_num_trias_array;
	delete[] p_send_tria_ids_array;
	delete[] p_send_trias_array;

	return PLSTAT_OK;
}

// protected //////////////////////////////////////////////////////////////////
template <typename T>
POLYLIB_STAT
MPIPolylib<T>::select_excluded_trias(
	PolygonGroup<T> *p_pg
)
{
#ifdef DEBUG
	PL_DBGOSH << "MPIPolylib::select_excluded_trias() in. " << std::endl;
#endif

	unsigned int i, j;
	std::vector<PrivateTriangle<T>*> const *p_trias;
	std::vector<int> ids;

	// 全隣接PEについて
	for( i=0; i<m_neibour_procs.size(); i++ ) {
		ids.clear();

		// 隣接PE領域(ガイドセル含)に懸かる三角形IDリストを作成
		p_trias = p_pg->search( &(m_neibour_procs.at(i)->m_area.m_gcell_bbox), false );
		for( j=0; j<p_trias->size(); j++ ) {
			ids.push_back( p_trias->at(j)->get_id() );
		}
#ifdef DEBUG
	PL_DBGOSH << "gid:" << p_pg->get_id() << " neibour_rank:" << m_neibour_procs.at(i)->m_rank
			  << " 除外三角形数:" << ids.size() << std::endl;
#endif

		// migrate除外三角形IDマップに追加
		m_neibour_procs.at(i)->m_exclusion_map[p_pg->get_internal_id()] = ids;

		// search結果あとしまつ
		delete p_trias;
	}
	return PLSTAT_OK;

}


} //namespace PolylibNS

#endif // MPIPolylib_h
