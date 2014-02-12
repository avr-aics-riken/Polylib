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

#include "groups/PolygonGroup.h"

#include <iostream>
#include <string>
#include <iomanip>
#include <algorithm>

#include <sstream>
#include "polygons/PrivateTriangle.h"

#include "common/BBox.h"

#include "polygons/TriMesh.h"
#include "polygons/DVertexManager.h"
#include "file_io/TriMeshIO.h"
#include "file_io/triangle_id.h"

#include "Polylib.h"



namespace PolylibNS {


///
/// 他クラスでも使用するXMLタグ
///

const char* PolygonGroup::ATT_NAME_CLASS = "class_name";


const char* PolygonGroup::ATT_NAME_TOLERANCE = "tolerance";

///
/// 本クラス内でのみ使用するXMLタグ
///
//#define ATT_NAME_NAME		"name"
#define ATT_NAME_PATH		"filepath"
#define ATT_NAME_MOVABLE	"movable"
// ユーザ定義ID追加 2010.10.20
#define ATT_NAME_ID			"id"
// ユーザ定義ラベル追加 2012.08.31
#define ATT_NAME_LABEL		"label"
// ユーザ定義タイプ追加 2013.07.17
#define ATT_NAME_TYPE		"type"

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
std::string PolygonGroup::get_class_name() {
	return "PolygonGroup";
}

///
/// クラス名を取得。
///
/// @return		クラス名。
/// @attention	継承するクラスのクラス名取得関数get_class_name()を呼び出す。
///
std::string PolygonGroup::whoami()		{
	return get_class_name();
}

///
/// STLファイル名とファイルフォーマットを設定。
///
///  @param[in] fname STLファイル名とファイルフォーマットの対応マップ。
///
void PolygonGroup::set_file_name(std::map<std::string, std::string> fname) {
	m_file_name = fname;
}

///
/// STLファイル名とファイルフォーマットの対応マップ取得。
///
///  @return STLファイル名とファイルフォーマットの対応マップ。
///
std::map<std::string, std::string> PolygonGroup::get_file_name() const {
	return m_file_name;
}

///
/// グループ名を設定。
///
/// @param[in] name グループ名。
///
void PolygonGroup::set_name(std::string name){
	m_name = name;
}

///
/// グループ名を取得。
///
/// @return グループ名。
///
std::string PolygonGroup::get_name(void){
	return m_name;
}

///
/// 親グループのフルパス名を設定。
///
/// @param[in] ppath 親グループのフルパス名。
///
void PolygonGroup::set_parent_path(std::string ppath){
	m_parent_path = ppath;
}

///
/// 親グループのフルパス名を取得。
///
/// @return 親グループのフルパス名。
///
std::string PolygonGroup::get_parent_path(void){
	return m_parent_path;
}

///
/// 親グループを設定。
///
/// @param[in] p 親グループのポインタ。
///
void PolygonGroup::set_parent(PolygonGroup* p) {
	m_parent = p;
}

///
/// 親グループを取得。
///
/// @return 親グループのポインタ。
///
PolygonGroup* PolygonGroup::get_parent(void) {
	return m_parent;
}

///
/// 子グループを設定。
///
/// @param[in] p	子グループのリスト。
///
void PolygonGroup::set_children(std::vector<PolygonGroup*>& p) {
	m_children = p;
}

///
/// 子グループを取得。
///
/// @return 子グループのリスト。
///
std::vector<PolygonGroup*>& PolygonGroup::get_children(void) {
	return m_children;
}

///
/// 子グループを追加。
///
/// @param[in] p	子グループ。
///
void PolygonGroup::add_children(PolygonGroup* p) {
	m_children.push_back(p);
}

///
/// 子グループを削除。
///
/// @param[in] p 子グループ。
///
void PolygonGroup::remove_child(PolygonGroup* p) {
	std::vector<PolygonGroup*>::iterator itr;
	for( itr=m_children.begin(); itr!=m_children.end(); itr++ ) {
		if( *itr == p ) {
			itr = m_children.erase(itr);
			break;
		}
	}
}

///
/// Polygonクラスが管理する頂点リストを取得。
///
/// @return  頂点リスト
///
VertexList* PolygonGroup::get_vertexlist() {
	return m_polygons->get_vtx_list();
}

///
/// Polygonクラスが管理するKD木クラスを取得。
///
/// @return KD木ポリゴンリスト。
///
VertKDT* PolygonGroup::get_vertkdt() {
	return m_polygons->get_vertkdt();
}
///
/// Polygonクラスが管理する三角形ポリゴンリストを取得。
///
/// @return 三角形ポリゴンリスト。
///
std::vector<PrivateTriangle*>* PolygonGroup::get_triangles() {
	return m_polygons->get_tri_list();
}

///
/// Polygonクラスが管理するKD木クラスを取得。
///
/// @return KD木ポリゴンリスト。
///
VTree* PolygonGroup::get_vtree() {
	return m_polygons->get_vtree();
}

///
/// ポリゴングループIDを取得。
/// メンバー名修正( m_id -> m_internal_id) 2010.10.20
///
///  @return ポリゴングループID。
///
int PolygonGroup::get_internal_id() {
	return m_internal_id;
}

///
/// ユーザ定義ラベルを取得。
/// 追加 2012.08.31
///
///  @return ユーザ定義ラベル。
///
std::string PolygonGroup::get_label() {
	return m_label;
}

///
/// ユーザ定義タイプを取得。
/// 追加 2013.07.17
///
///  @return ユーザ定義タイプ。
///
std::string PolygonGroup::get_type() {
	return m_type;
}

///
/// ユーザ定義IDを取得。
/// 追加 2010.10.20
///
///  @return ユーザ定義ID。
///
int PolygonGroup::get_id() {
	return m_id;
}

///
/// 移動対象フラグを取得。
///
///  @return 移動対象フラグ。
///
int PolygonGroup::get_movable() {
	return m_movable;
}

///
/// move()による移動前三角形一時保存リストの個数を取得。
///
///  @return 一時保存リストサイズ。
///
size_t PolygonGroup::get_num_of_trias_before_move() {
	if (m_trias_before_move == NULL)	return 0;
	else	return m_trias_before_move->size();
}


///
/// test function for Vertex test
/// 
void PolygonGroup::print_vertex() const{
	m_polygons->print_vertex();
}

void PolygonGroup::show_bbox() const{
	BBox bbox=m_polygons->get_bbox();
	Vec3<REAL_TYPE> min=bbox.getPoint(0);
	Vec3<REAL_TYPE> max=bbox.getPoint(7);
	std::cout<< __func__ << " min " <<min<<std::endl;
	std::cout<< __func__ << " max " <<max<<std::endl;

}






// public /////////////////////////////////////////////////////////////////////

PolygonGroup::PolygonGroup() {
	m_parent	= 0;
	m_polygons	= new TriMesh();
	m_movable	= false;
	m_need_rebuild = false;
	m_trias_before_move = NULL;
	///	m_DVM_ptr=NULL;
}
// public /////////////////////////////////////////////////////////////////////

PolygonGroup::PolygonGroup(REAL_TYPE tolerance) {
	m_parent	= 0;
	m_polygons	= new TriMesh(tolerance);
	m_movable	= false;
	m_need_rebuild = false;
	m_trias_before_move = NULL;
	m_tolerance=tolerance;
	//	m_DVM_ptr=NULL;
}

// public /////////////////////////////////////////////////////////////////////

PolygonGroup::~PolygonGroup()
{
#ifdef DEBUG
	PL_DBGOSH << "Delete PolygonGroup" << std::endl;
#endif
	delete m_polygons;
	if( m_trias_before_move ) {
		for( unsigned int i=0; i<m_trias_before_move->size(); i++ ) {
			delete m_trias_before_move->at(i);
		}
		delete m_trias_before_move;
	}

	// if(m_DVM_ptr!=NULL){
	//   delete m_DVM_ptr;
	// }
}
// public /////////////////////////////////////////////////////////////////////

POLYLIB_STAT PolygonGroup::init(
	const REAL_TYPE* vertlist,
	const int* idlist,
	const int* exidlist,
	const int n_start_tri,
	const int n_start_id,
	const int n_start_exid,
	const unsigned int n_tri){

		//#define DEBUG

#ifdef DEBUG
		PL_DBGOSH <<"PolygonGroup::" << __func__<<" start of Polygons::init." << std::endl;
#endif


		m_polygons->init(vertlist,idlist,exidlist,n_start_tri,n_start_id,n_start_exid,n_tri);
#ifdef DEBUG
		PL_DBGOSH <<"PolygonGroup::" << __func__<<" end of Polygons::init."  <<std::endl;
#endif

		return build_polygon_tree();
		//#undef DEBUG
}


// public /////////////////////////////////////////////////////////////////////

POLYLIB_STAT PolygonGroup::init_dvertex(
	const REAL_TYPE* vertlist,
	const int* idlist,
	const int* exidlist,
	const REAL_TYPE* scalarlist,
	const REAL_TYPE* vectorlist,
	const int n_start_tri,
	const int n_start_id,
	const int n_start_exid,
	const int n_start_scalar,
	const int n_start_vector,
	const unsigned int n_tri,
	const int n_scalar,
	const int n_vector){


		//#define DEBUG

#ifdef DEBUG
		PL_DBGOSH <<"PolygonGroup::" << __func__<<" start of Polygons::init." << std::endl;
#endif



		m_polygons->init_dvertex(vertlist,idlist,exidlist,scalarlist,vectorlist,
			n_start_tri,n_start_id,n_start_exid,n_start_scalar,n_start_vector,
			n_tri,n_scalar,n_vector);

#ifdef DEBUG
		PL_DBGOSH <<"PolygonGroup::" << __func__<<" end of Polygons::init."  <<std::endl;
#endif

		return build_polygon_tree();
		//#undef DEBUG
}


// public /////////////////////////////////////////////////////////////////////

POLYLIB_STAT PolygonGroup::add_dvertex(
	const REAL_TYPE* vertlist,
	const int* idlist,
	const int* exidlist,
	const REAL_TYPE* scalarlist,
	const REAL_TYPE* vectorlist,
	const int n_start_tri,
	const int n_start_id,
	const int n_start_exid,
	const int n_start_scalar,
	const int n_start_vector,
	const unsigned int n_tri,
	const int n_scalar,
	const int n_vector){


		//#define DEBUG
#ifdef DEBUG
		PL_DBGOSH <<"PolygonGroup::" << __func__<<" start of Polygons::init." << std::endl;
#endif



		m_polygons->add_dvertex(vertlist,idlist,exidlist,scalarlist,vectorlist,
			n_start_tri,n_start_id,n_start_exid,n_start_scalar,n_start_vector,
			n_tri,n_scalar,n_vector);

#ifdef DEBUG
		PL_DBGOSH <<"PolygonGroup::" << __func__<<" end of Polygons::init."  <<std::endl;
#endif

		//  return build_polygon_tree();
		return PLSTAT_OK;
		//#undef DEBUG
}




// public /////////////////////////////////////////////////////////////////////

POLYLIB_STAT PolygonGroup::init(
	const std::vector<PrivateTriangle*>	*tri_list, 
	bool clear
	) {
		//#define DEBUG
#ifdef DEBUG
		PL_DBGOSH <<"PolygonGroup::init :clear=" << clear << std::endl;
#endif
		if (clear == true) {
			m_polygons->init(tri_list);

#ifdef DEBUG
			PL_DBGOSH <<"PolygonGroup::init :back from TriMesh::init" <<  std::endl;
#endif

		}
		return build_polygon_tree();
		//#undef DEBUG
}

//TextParser version
// public /////////////////////////////////////////////////////////////////////

POLYLIB_STAT PolygonGroup::build_group_tree(
	Polylib *polylib,
	PolygonGroup *parent,
	TextParser* tp
	) {
#ifdef DEBUG
		PL_DBGOSH << "PolygonGroup::build_group_tree() in." << std::endl;
#endif

		//current で出来ているPolygonGroupに属性をつける。
		POLYLIB_STAT 	ret = setup_attribute(polylib, parent, tp);
		if (ret != PLSTAT_OK) return ret;

		// 元コードの説明
		// PolylibCfgElem がなくなるまでループ
		// すでにelem には、情報が読み込まれていて、それを使用する。
		//

		std::vector<std::string> nodes;

		TextParserError error=TP_NO_ERROR;
		error=tp->getNodes(nodes);


		// tp で　情報を読みながら、ループ
		for(std::vector<std::string>::iterator nodes_iter=nodes.begin();
			nodes_iter != nodes.end();
			nodes_iter++){

				error=tp->changeNode(*nodes_iter);
				if(error!=TP_NO_ERROR){
					PL_ERROSH << "[ERROR]PolygonGroup::build_group_tree():"
						<< " TextParser error " 
						<< tp->TextParserErrorHandler(error,"can not move to ") 
						<< (*nodes_iter) << std::endl;
					return PLSTAT_CONFIG_ERROR;
				}
				//属性を読む。
				std::vector<std::string> leaves;
				error=tp->getLeaves(leaves,1);
				if(error!=TP_NO_ERROR){
					PL_ERROSH << "[ERROR]PolygonGroup::build_group_tree():"
						<< " TextParser error " 
						<< tp->TextParserErrorHandler(error,"can not get leaves ") 
						<< (*nodes_iter) << std::endl;
					return PLSTAT_CONFIG_ERROR;
				}

				// class_name をチェック
				std::string class_name = "PolygonGroup";
				std::vector<std::string>::iterator leaf_iter=find(leaves.begin(),
					leaves.end(),
					ATT_NAME_CLASS);

				if(leaf_iter!=leaves.end()){
					std::string value;
					error=tp->getValue((*leaf_iter),value);
					class_name=value;
				}

				REAL_TYPE tolerance=m_tolerance;
				//TOLERANCE
				leaf_iter=find(leaves.begin(),
					leaves.end(),
					PolygonGroup::ATT_NAME_TOLERANCE);

				if(leaf_iter != leaves.end()){
					//     class_name = *leaf_iter;
					std::string value;
					//TextPraserError  status=tp->getValue((*leaf_iter),value);
					tp->getValue((*leaf_iter),value);
					int ierror;
					tolerance=tp->convertDouble(value,&ierror);
				}


				PolygonGroup* pg;
				pg = polylib->create_polygon_group(class_name,tolerance);
				polylib->add_pg_list(pg);	
				if (pg == NULL) {
					PL_ERROSH << "[ERROR]PolygonGroup::build_group_tree():"
						<< "Unknown Class name:" << class_name << "." << std::endl;
					return PLSTAT_CONFIG_ERROR;
				}

				ret = pg->build_group_tree(polylib, this, tp);

				// go up and next
				tp->changeNode("..");

		}

		return PLSTAT_OK;
}

//TextParser version
// public /////////////////////////////////////////////////////////////////////
/////// 新規 DVertex 追加用
POLYLIB_STAT PolygonGroup::build_group_tree(
	Polylib *polylib,
	PolygonGroup* parent,
	std::string path
	) {
		//#define DEBUG
#ifdef DEBUG
		PL_DBGOSH << "PolygonGroup::build_group_tree() in. " << path << std::endl;
#endif
		//	DT tmp;

		std::string::size_type index=path.find("/");
		std::string parent_name;
		std::string daughter_name;

		if(index!=std::string::npos){ 
			//  "/" found 
			parent_name=path.substr(0,index);
			daughter_name=path.substr(index+1);
#ifdef DEBUG
			PL_DBGOSH << "Polylib::"<< __func__ 
				<<" parent " << parent_name
				<<" daughter " << daughter_name<<std::endl;
#endif
			m_name = parent_name;
			m_internal_id = create_global_id();

			if (parent != NULL){
				m_parent = parent;
				m_parent_path = parent->acq_fullpath();
				parent->add_children(this);
			} else {
#ifdef DEBUG
				PL_DBGOSH << "root "<<parent_name<< " id "<<this <<std::endl;;
#endif
			}

			PolygonGroup* pg;
			std::string class_name="PolygonGroup";
			pg = polylib->create_polygon_group(class_name,m_tolerance);
			polylib->add_pg_list(pg);	

			return pg->build_group_tree(polylib,this,daughter_name);

		} else {
			//  "/" not found 



#ifdef DEBUG
			PL_DBGOSH << "PolygonGroup::build_group_tree() last. " << path << std::endl;
			PL_DBGOSH << " ptr "<<this <<std::endl;;
#endif


			if (parent != NULL){
				m_parent = parent;
				m_parent_path = parent->acq_fullpath();
				parent->add_children(this);
			}

			m_name = path;
			m_internal_id = create_global_id();

			return PLSTAT_OK;
		}

		//#undef DEBUG

}


// public /////////////////////////////////////////////////////////////////////

POLYLIB_STAT PolygonGroup::build_polygon_tree()
{
#ifdef BENCHMARK
	double	st1, st2, ut1, ut2, tt1, tt2;
	bool	ret1, ret2;
	ret1 = getrusage_sec(&ut1, &st1, &tt1);
#endif

	//#define DEBUG
#ifdef DEBUG
	PL_DBGOSH << "PolygonGroup::build_polygon_tree() in.:" << m_name << std::endl;
#endif
	// before rebuild Polygon tree vertex_compaction first.
	//m_polygons->vtx_compaction();

	//木構造の生成
	POLYLIB_STAT ret = m_polygons->build();
#ifdef DEBUG
	PL_DBGOSH << "PolygonGroup::build_polygon_tree() out."<< ret << std::endl;
#endif


	if (ret != PLSTAT_OK) return ret;

#ifdef DEBUG
	PL_DBGOSH << "PolygonGroup::build_polygon_tree() out." << std::endl;
#endif
#ifdef BENCHMARK
	ret2 = getrusage_sec(&ut2,&st2,&tt2);
	if (ret1 == false || ret2 == false) {
		PL_DBGOSH << "Reading STL SYS Time Error" << std::endl;
		PL_DBGOSH << "Reading STL CPU Time Error" << std::endl;
		PL_DBGOSH << "Reading STL Total Time Error" << std::endl;
	}
	else{
		cerr.setf(ios::scientific, ios::floatfield);
		PL_DBGOSH << "Reading STL SYS   Time:" << st2 - st1 << std::endl;
		PL_DBGOSH << "Reading STL CPU   Time:" << ut2 - ut1 << std::endl;
		PL_DBGOSH << "Reading STL Total Time:" << tt2 - tt1 << std::endl;
		cerr.unsetf(ios::scientific);
	}
#endif
	//#undef DEBUG
	return PLSTAT_OK;
}

// public /////////////////////////////////////////////////////////////////////

POLYLIB_STAT PolygonGroup::load_stl_file( REAL_TYPE scale )
{
	//#define DEBUG
#ifdef DEBUG
	PL_DBGOSH << "PolygonGroup:load_stl_file():IN" << std::endl;
#endif
	POLYLIB_STAT ret = m_polygons->import(m_file_name, scale);

#ifdef DEBUG
	PL_DBGOSH << "PolygonGroup:load_stl_file():import finished" << std::endl;
#endif

	if (ret != PLSTAT_OK) return ret;

	// m_idが指定されていたら、その値で全三角形のm_exidを更新
	// added by tkawanab 2013.06.17
	if( m_id_defined ) {
		ret = m_polygons->set_all_exid( m_id );
	}
	if (ret != PLSTAT_OK) return ret;

	POLYLIB_STAT result= build_polygon_tree();


#ifdef DEBUG
	PL_DBGOSH << "PolygonGroup:load_stl_file():import build_finished" << std::endl;
	m_polygons->print_memory_size();
#endif

	return result;
	//#undef DEBUG
}

// public /////////////////////////////////////////////////////////////////////

POLYLIB_STAT PolygonGroup::load_id_file(
	ID_FORMAT	id_format
	) {
#ifdef DEBUG
		PL_DBGOSH << "PolygonGroup:load_id_file():IN" << std::endl;
#endif

		// no stl file, no id file.
		if(m_file_name.size() == 0 ){
			return PLSTAT_OK;	
		}

		// IDはsave_id()関数で一括して出力されるので、ファイル数は必ず1個

		if (m_file_name.size() != 1) {
			PL_ERROSH << "[ERROR]PolygonGroup::load_id_file():Num of files mismatch:" 
				<< m_file_name.size() << std::endl;
			return PLSTAT_NG;	
		}
		std::string	fname	= m_file_name.begin()->first;
		int		pos		= fname.find_last_of(".");

		fname.replace(pos + 1, fname.length(), "id");
#ifdef DEBUG
		PL_DBGOSH << "load_id_file:" << fname.c_str() << std::endl;
#endif
		// ?? return load_id(m_polygons->get_tri_list(), fname, id_format);
		return load_id(m_polygons->get_tri_list(), fname, id_format);
}

// TextParser でのsaveの為、save した stl ファイルを記憶しておく
/////////////////////////////////////////////////////////////////////

POLYLIB_STAT PolygonGroup::save_stl_file(
	std::string	rank_no,
	std::string	extend,
	std::string	format,
	std::map<std::string,std::string>& stl_fname_map
	) {

		char	*fname = mk_stl_fname(rank_no, extend, &format,stl_fname_map);
		//  std::cout <<__func__ <<format << std::endl;

		//	return TriMeshIO::save(m_polygons->get_tri_list(), fname, format);

		return TriMeshIO::save( m_polygons->get_vtx_list(),
			m_polygons->get_tri_list(),
			fname, format);
}

// public /////////////////////////////////////////////////////////////////////

POLYLIB_STAT PolygonGroup::save_id_file(
	std::string		rank_no,
	std::string		extend,
	ID_FORMAT	id_format
	) {
		char	*fname = mk_id_fname(rank_no, extend);
#ifdef DEBUG
		PL_DBGOSH <<  "save_id_file:" << fname << std::endl;
#endif
		return save_id(m_polygons->get_tri_list(), fname, id_format);
}


//TextParser 版

POLYLIB_STAT PolygonGroup::mk_param_tag(
	TextParser* tp,
	std::string	rank_no,
	std::string	extend,
	std::string	format
	) {

		// virtual用の関数なので中身はない
		return PLSTAT_OK;
}


// public /////////////////////////////////////////////////////////////////////

POLYLIB_STAT PolygonGroup::move(
	PolylibMoveParams	&params
	){
#ifdef DEBUG
		PL_DBGOSH <<  "PolygonGroup::move() in." << std::endl;
#endif
		// virtual用の関数なので中身はない
		return PLSTAT_OK;
}

// public /////////////////////////////////////////////////////////////////////

const std::vector<PrivateTriangle*>* PolygonGroup::search(
	BBox	*bbox, 
	bool	every
	) const {
		return m_polygons->search(bbox, every);
}

// public /////////////////////////////////////////////////////////////////////

POLYLIB_STAT PolygonGroup::search(
	BBox						*bbox, 
	bool						every, 
	std::vector<PrivateTriangle*>	*tri_list
	) const {
		return m_polygons->search(bbox, every, tri_list);
}

// public /////////////////////////////////////////////////////////////////////

const std::vector<PrivateTriangle*>* PolygonGroup::linear_search(
	BBox	*bbox, 
	bool	every
	) const {
		return m_polygons->linear_search(bbox, every);
}

// public /////////////////////////////////////////////////////////////////////

POLYLIB_STAT PolygonGroup::linear_search(
	BBox						*bbox, 
	bool						every, 
	std::vector<PrivateTriangle*>	*tri_list
	) const {
		return m_polygons->linear_search(bbox, every, tri_list);
}

// public /////////////////////////////////////////////////////////////////////

std::string PolygonGroup::acq_fullpath() {
	if (m_parent_path.empty() == true)	return m_name;
	else								return m_parent_path + "/" + m_name;
}

// public /////////////////////////////////////////////////////////////////////

std::string PolygonGroup::acq_file_name() {
	std::string	fnames;
	std::map<std::string, std::string>::iterator	it;
	for (it = m_file_name.begin(); it != m_file_name.end(); it++) {
		if (it == m_file_name.begin()) {
			fnames = it->first;
		}
		else {
			fnames.append(",");
			fnames.append(it->first);
		}
	}
	return fnames;
}

// public /////////////////////////////////////////////////////////////////////

const std::vector<PrivateTriangle*>*
	PolygonGroup::search_outbounded(
	BBox neibour_bbox,
	std::vector<int> *exclude_tria_ids
	)
{
	//#define DEBUG
#ifdef DEBUG
	PL_DBGOSH << "PolygonGroup::search_outbounded() in. " << std::endl;
#endif
	std::vector<PrivateTriangle*> *p_trias;

	// 除外IDリストを昇順ソート
	std::sort( exclude_tria_ids->begin(), exclude_tria_ids->end() );
#ifdef DEBUG
	PL_DBGOSH << "PolygonGroup::search_outbounded() neibour box " << neibour_bbox.min<<" " << neibour_bbox.max<<std::endl;
#endif
	// 隣接PE領域(ガイドセル含)に懸かる三角形を検索
	p_trias = (std::vector<PrivateTriangle*>*)search( &neibour_bbox, false );
#ifdef DEBUG
	PL_DBGOSH << "p_trias org num:" << p_trias->size() << std::endl;
#endif

	// 検索結果から除外対象を除く
	std::vector<PrivateTriangle*>::iterator itr;
	for( itr=p_trias->begin(); itr!=p_trias->end(); ) {
		int id = (*itr)->get_id();
		if( std::binary_search(exclude_tria_ids->begin(),
			exclude_tria_ids->end(), id) ) {
				itr = p_trias->erase(itr);
		}
		else {
			itr++;
		}
	}
#ifdef DEBUG
	PL_DBGOSH << "p_trias ret num:" << p_trias->size() << std::endl;
#endif
	return p_trias;
	//#undef DEBUG
}
// public /////////////////////////////////////////////////////////////////////

POLYLIB_STAT
	PolygonGroup::add_triangles(
	const REAL_TYPE* vertlist,
	const int* idlist,
	const int* exidlist,
	const int n_start_tri,
	const int n_start_id,
	const int n_start_exid,
	const unsigned int n_tri){


		//#define DEBUG
#ifdef DEBUG
		PL_DBGOSH << __func__<< " n_tri="<<n_tri << std::endl;
#endif

		if( n_tri==0)	  return PLSTAT_OK;

#ifdef DEBUG
		PL_DBGOSH << __func__<< " add start" << std::endl;
#endif

		m_polygons->add(vertlist, idlist, exidlist, n_start_tri, n_start_id, n_start_exid, n_tri);

#ifdef DEBUG
		PL_DBGOSH << __func__<< " add finished" << std::endl;
#endif


		// KD木要再構築フラグを立てる
		m_need_rebuild = true;
		return PLSTAT_OK;

		//#undef DEBUG
}




// public /////////////////////////////////////////////////////////////////////

POLYLIB_STAT
	PolygonGroup::add_triangles(
	std::vector<PrivateTriangle*> *tri_list
	)
{
#define DEBUG
#ifdef DEBUG
	PL_DBGOSH << "PolygonGroup::add_triangles() in. " << std::endl;
#endif
	if( tri_list==NULL || tri_list->size()==0 ) {
		return PLSTAT_OK;
	}

#ifdef DEBUG
	PL_DBGOSH << "PolygonGroup::add_triangles() in. " << std::endl;
#endif

	m_polygons->add( tri_list );
#ifdef DEBUG
	PL_DBGOSH << "PolygonGroup::add_triangles() end. " << std::endl;
#endif


	// KD木要再構築フラグを立てる
	m_need_rebuild = true;

	return PLSTAT_OK;
#undef DEBUG
}

// public /////////////////////////////////////////////////////////////////////

POLYLIB_STAT
	PolygonGroup::rebuild_polygons()
{
#ifdef DEBUG
	PL_DBGOSH << "PolygonGroup::rebuild_polygons() in. " << std::endl;
#endif
	// 不要な再構築は行わない
	if( !m_need_rebuild ) {
#ifdef DEBUG
		PL_DBGOSH << "PolygonGroup::rebuild_polygons() didnot need rebuild." << std::endl;
#endif
		return PLSTAT_OK;
	}

	POLYLIB_STAT ret = build_polygon_tree();
	m_need_rebuild = false;
	return ret;
}

// public /////////////////////////////////////////////////////////////////////

POLYLIB_STAT PolygonGroup::show_group_info(
	int irank
	) {
		std::ostringstream	oss;
		std::string			rank;

		if (irank < 0) {
			rank = "";
		}
		else{
			oss << std::setw(3) << std::setfill('0') << "rank(" << irank << "): ";
			rank = oss.str();
		}
		PL_DBGOSH << "PolygonGroup::show_group_info::rank:" << rank << std::endl;

		if (m_name.empty() == false) {
			PL_DBGOSH << "  polygon group name: " << m_name << std::endl;
		}
		else {
			PL_DBGOSH << "  polygon group name: empty." << std::endl;
		}

		if (m_parent_path.empty() == false) {
			PL_DBGOSH << "  parent polygon group name: " << m_parent_path << std::endl;
		}
		else {
			PL_DBGOSH << "  parent polygon group name: empty." << std::endl;
		}

		if (m_file_name.size() > 0) {
			std::map<std::string, std::string>::iterator it = m_file_name.begin();
			for (; it != m_file_name.end(); it++) 
				PL_DBGOSH << "  file name: " << (*it).first << std::endl;
		}
		else {
			PL_DBGOSH << "  file name: empty." << std::endl;
		}

		if (m_polygons == NULL) {
			PL_ERROSH << "[ERROR]PolygonGroup::show_group_info():Polygon is nothing:"
				<< std::endl;
			return PLSTAT_POLYGON_NOT_EXIST;
		}
		if (m_polygons->get_tri_list() == 0) {
			PL_ERROSH << "[ERROR]PolygonGroup::show_group_info():Triangle is nothing:"
				<< std::endl;
			return PLSTAT_TRIANGLE_NOT_EXIST;
		}

		std::vector<PrivateTriangle*>* tmp_list = m_polygons->get_tri_list();

		PL_DBGOSH << "  triangle list size: " << tmp_list->size() << std::endl;
		PL_DBGOSH << "  vertex vector list: " << std::endl;
		std::vector<PrivateTriangle*>::iterator it;
		for (it = tmp_list->begin(); it != tmp_list->end(); it++) {
			//Vec3<REAL_TYPE>* vtx[3] = (*it)->get_vertex();
			Vertex** vtx = (*it)->get_vertex();
			for (int i=0; i<3; i++) {
				//	    PL_DBGOSH << "    id:" << i		   << " x:" << (*vtx)[i][0]
				//<< " y:"	  << (*vtx)[i][1] << " z:" << (*vtx)[i][2] << std::endl;
				PL_DBGOSH << "    id:" << i		   << " x:" << (*(vtx[i]))[0]
				<< " y:"	  << (*(vtx[i]))[1] << " z:" << (*(vtx[i]))[2] << std::endl;

			}

			// Vec3<REAL_TYPE> *vtx = (*it)->get_vertex();
			// for (int i=0; i<3; i++) {
			//   PL_DBGOSH << "    id:" << i		   << " x:" << vtx[i][0]
			// 	    << " y:"	  << vtx[i][1] << " z:" << vtx[i][2] << std::endl;
			// }
		}

		PL_DBGOSH << "  normal vector list: " << std::endl;
		for (it = tmp_list->begin(); it != tmp_list->end(); it++) {
			Vec3<REAL_TYPE> vtx = (*it)->get_normal();
			PL_DBGOSH << "    x:" << vtx[0] << " y:" << vtx[1] << " z:" << vtx[2] <<std::endl;
		}

		PL_DBGOSH << "  triangle area list: " << std::endl;
		for (it = tmp_list->begin(); it != tmp_list->end(); it++) {
			PL_DBGOSH << "    area:" << (*it)->get_area() << std::endl;
		}
		return PLSTAT_OK;
}

//// public? ///////////////////////
// add keno 20120331

REAL_TYPE PolygonGroup::get_group_area( void ) {

	REAL_TYPE m_area=0.0, a;

	std::vector<PrivateTriangle*>* tmp_list = m_polygons->get_tri_list();

	std::vector<PrivateTriangle*>::iterator it;

	for (it = tmp_list->begin(); it != tmp_list->end(); it++) {
		a = (*it)->get_area();
		m_area += a;
	}
	return m_area;
}
//// public? ///////////////////////

int PolygonGroup::get_group_num_tria( void ) {

	std::vector<PrivateTriangle*>* tmp_list = m_polygons->get_tri_list();

	return (int)tmp_list->size();
}// add keno 20120331

//// public? ///////////////////////

POLYLIB_STAT PolygonGroup::rescale_polygons( REAL_TYPE scale )
{
	std::vector<PrivateTriangle*>* tmp_list = m_polygons->get_tri_list();
	std::vector<PrivateTriangle*>::iterator it;
	for (it = tmp_list->begin(); it != tmp_list->end(); it++) {

		Vertex** original = (*it)->get_vertex();
		Vec3<REAL_TYPE> org[3];
		org[0]=*(original[0]);
		org[1]=*(original[1]);
		org[2]=*(original[2]);

		Vec3<REAL_TYPE>  scaled[3];
		scaled[0][0] = org[0][0] * scale;
		scaled[0][1] = org[0][1] * scale;
		scaled[0][2] = org[0][2] * scale;
		scaled[1][0] = org[1][0] * scale;
		scaled[1][1] = org[1][1] * scale;
		scaled[1][2] = org[1][2] * scale;
		scaled[2][0] = org[2][0] * scale;
		scaled[2][1] = org[2][1] * scale;
		scaled[2][2] = org[2][2] * scale;
		//MOD S fuchi error C2664: 'PolylibNS::Triangle::set_vertexes' : 1 番目の引数を 'PolylibNS::Vec3<T> [3]' から 'PolylibNS::Vertex *[]' に変換できません。(
		//(*it)->set_vertexes( scaled, true, true );
		Vertex* vertex_ptr[3];
		for(int i=0;i<3;i++){
			vertex_ptr[i] = new Vertex(scaled[i]);
		}
		(*it)->set_vertexes( vertex_ptr, true, true );
		//MOD E 
		// Vec3<REAL_TYPE>* org = (*it)->get_vertex();
		// Vec3<REAL_TYPE>  scaled[3];
		// scaled[0][0] = org[0][0] * scale;
		// scaled[0][1] = org[0][1] * scale;
		// scaled[0][2] = org[0][2] * scale;
		// scaled[1][0] = org[1][0] * scale;
		// scaled[1][1] = org[1][1] * scale;
		// scaled[1][2] = org[1][2] * scale;
		// scaled[2][0] = org[2][0] * scale;
		// scaled[2][1] = org[2][1] * scale;
		// scaled[2][2] = org[2][2] * scale;
		// (*it)->set_vertexes( scaled, true, true );
	}
	m_need_rebuild = true;
	return rebuild_polygons();
}

// public /////////////////////////////////////////////////////////////////////

POLYLIB_STAT PolygonGroup::set_all_exid_of_trias( int id )
{
	m_id = id;           // keno 2013-07-20
	m_id_defined = true; // keno 2013-07-20
	return m_polygons->set_all_exid( id );
}

// public /////////////////////////////////////////////////////////////////////

const PrivateTriangle* PolygonGroup::search_nearest(
	const Vec3<REAL_TYPE>&    pos
	) const {
		return m_polygons->search_nearest(pos);
}

// TextParser Version
// protected //////////////////////////////////////////////////////////////////

POLYLIB_STAT PolygonGroup::setup_attribute (
	Polylib					*polylib,
	PolygonGroup			*parent, 
	TextParser* tp
	) {
		//#define DEBUG
#ifdef DEBUG
		PL_DBGOS << __func__ << " in"  <<std::endl;
#endif
		TextParserError tp_error = TP_NO_ERROR;
		int ierror;


		//  vector<std::string> nodes,leaves;
		std::vector<std::string> leaves;
		//  tp_error=tp->getNodes(nodes);
		tp_error=tp->getLeaves(leaves,1);


		//  class_name search first
		std::vector<std::string>::iterator leaf_iter = find(leaves.begin(),leaves.end(),ATT_NAME_CLASS);

		//  PL_DBGOS << __func__ << " # of nodes: " << nodes.size()
		//<< " # of leaves: " << leaves.size() << std::endl;

		std::string class_name = "";
		if(leaf_iter!=leaves.end()) {
			tp_error=tp->getValue((*leaf_iter),class_name);
			//PL_DBGOS << __func__ << " there is a class_name "<< class_name<<std::endl;
		}

		// id 
		std::string id_string = "";
		leaf_iter = find(leaves.begin(),leaves.end(),ATT_NAME_ID);

		if(leaf_iter!=leaves.end()) {
			tp_error=tp->getValue((*leaf_iter),id_string);
			//PL_DBGOS << __func__ << " there is a id number. "<< id_string
			//<<std::endl;
		}

		// label (2012.08.31 追加)
		std::string label_string = "";
		leaf_iter = find(leaves.begin(),leaves.end(),ATT_NAME_LABEL);

		if(leaf_iter!=leaves.end()) {
			tp_error=tp->getValue((*leaf_iter),label_string);
			//PL_DBGOS << __func__ << " there is a label. "<< label_string
			//<<std::endl;
		}

		// type (2013.07.17 追加)
		std::string type_string = "";
		leaf_iter = find(leaves.begin(),leaves.end(),ATT_NAME_TYPE);

		if(leaf_iter!=leaves.end()) {
			tp_error=tp->getValue((*leaf_iter),type_string);
			//PL_DBGOS << __func__ << " there is a type. "<< type_string
			//<<std::endl;
		}

		// moveメソッドにより移動するグループか?
		if (this->whoami() == this->get_class_name()) {
			// 基本クラスの場合はmovableの設定は不要
		}
		else {
			std::string movable_string;
			leaf_iter = find(leaves.begin(),leaves.end(),ATT_NAME_MOVABLE);

			if(leaf_iter!=leaves.end()) {
				tp_error=tp->getValue((*leaf_iter),movable_string);

				m_movable = tp->convertBool(movable_string,&ierror);		
				//PL_DBGOS << __func__ << " is movavle ? true or false  "
				//<< m_movable <<std::endl;
			}
		}

		// グループ名が重複していないか確認
		// for tp
		std::string current_node;
		tp->currentNode(current_node);

		//cout << __func__ << " current_node = "  << current_node <<std::endl;
		std::string pg_name = current_node;

		std::string	parent_path = "";
		if (parent != NULL)		parent_path = parent->acq_fullpath();
		POLYLIB_STAT ret = polylib->check_group_name(pg_name, parent_path);
		if (ret != PLSTAT_OK)		return ret;

		std::string fname = "";

		leaf_iter = find(leaves.begin(),leaves.end(),"filepath[0]");
		if(leaf_iter != leaves.end()){
			//filepath が複数の場合

			int index=0;

			while(leaf_iter != leaves.end()){ //end　にいかなければ。
				std::stringstream ss;
				std::string tmpstring=ATT_NAME_PATH;

				ss << tmpstring <<"["<<index<<"]";
				ss >> tmpstring;
#ifdef DEBUG      
				PL_DBGOS << __func__<< " multi stl files "<< tmpstring << " "<<*leaf_iter<<std::endl;
#endif //DEBUG      

				leaf_iter = find(leaf_iter,leaves.end(),tmpstring);
				if(leaf_iter == leaves.end()) break;
				tp_error=tp->getValue((*leaf_iter),fname);

#ifdef DEBUG      
				PL_DBGOS << __func__ << " STLfiles " << index <<" " << fname <<std::endl;
#endif //DEBUG      

				std::string format = TriMeshIO::input_file_format(fname);
				if (format.empty()) {
					PL_ERROSH << "[ERROR]PolygonGroup::setup_attribute():Unknown"
						<< "extention: fname[]=" << fname << std::endl;
					return PLSTAT_UNKNOWN_STL_FORMAT;
				}             

				m_file_name.insert(std::map<std::string, std::string>::value_type(fname, format));
				index++;
				leaf_iter++;
			}

		} else { //filepath が単数の場合
			leaf_iter = find(leaves.begin(),leaves.end(),ATT_NAME_PATH);
			if(leaf_iter!=leaves.end()) {
				tp_error=tp->getValue((*leaf_iter),fname);

#ifdef DEBUG
				PL_DBGOS << __func__ << " STLfile "  << fname <<std::endl;
#endif // DEBUG
				std::string format = TriMeshIO::input_file_format(fname);
				if (format.empty()) {
					PL_ERROSH << "[ERROR]PolygonGroup::setup_attribute():Unknown"
						<< "extention: fname=" << fname << std::endl;
					return PLSTAT_UNKNOWN_STL_FORMAT;
				}             

				m_file_name.insert(std::map<std::string, std::string>::value_type(fname, format));
			}
		}


		// 親を設定
		if (parent != NULL)	{
			m_parent		= parent;
			m_parent_path	= parent->acq_fullpath();
			parent->add_children(this);
		} else {
#ifdef DEBUG	  
			PL_DBGOSH << "parent is root."<<pg_name<<std::endl;
#endif // DEBUG	  
		}

		// その他の属性を設定
		// for tp
		m_name = pg_name;

		m_internal_id = create_global_id();

		// ユーザ定義ID追加 2010.10.20
		// m_id_defined追加 2013.06.17
		if (id_string == "") {
			m_id = 0;
			m_id_defined = false;
		}
		else {
			m_id = tp->convertInt(id_string,&ierror);
			m_id_defined = true;
		}

		// ユーザ定義ラベル追加 2012.08.31
		m_label = label_string;

		// ユーザ定義タイプ追加 2013.07.17
		m_type = type_string;

		return PLSTAT_OK;
		//#undef DEBUG
}

// protected //////////////////////////////////////////////////////////////////

POLYLIB_STAT
	PolygonGroup::init_check_leaped()
{
#ifdef DEBUG
	PL_DBGOSH << "PolygonGroup::init_check_leaped() in. " << std::endl;
#endif

	std::vector<PrivateTriangle*>* p_trias = get_triangles();

	// 動かないポリゴングループならば何もしないで終了
	if( !m_movable || p_trias==NULL || p_trias->size()==0 ) return PLSTAT_OK;

	// move後と比較するために三角形ポリゴンリストのディープコピーを保存
	m_trias_before_move = new std::vector<PrivateTriangle*>;
	for( unsigned int i=0; i<p_trias->size(); i++ ) {
		m_trias_before_move->push_back( new PrivateTriangle(*(p_trias->at(i))) );
	}
	return PLSTAT_OK;
}

// protected //////////////////////////////////////////////////////////////////

POLYLIB_STAT
	PolygonGroup::check_leaped(
	Vec3<REAL_TYPE> origin,
	Vec3<REAL_TYPE> cell_size
	)
{
#ifdef DEBUG
	PL_DBGOSH << "PolygonGroup::check_leaped() in. " << std::endl;
#endif
	unsigned int i, j;
	std::vector<PrivateTriangle*>* p_trias = get_triangles();

	// 動かないポリゴングループならば何もしないで終了
	if( !m_movable || p_trias==NULL || p_trias->size()==0 ) return PLSTAT_OK;

	// move前の三角形と座標を比較。
	for( i=0; i<p_trias->size(); i++ ) {

		for( j=0; j<3; j++ ) {
			// 隣接セルより遠方へ移動してたらcerrにメッセージを出力。

			//MOD S fuchi		 error C2664: 'PolylibNS::PolygonGroup::is_far' : 3 番目の引数を 'PolylibNS::Vertex *' から 'PolylibNS::Vec3<T>' に変換できません。	
			/*	
			if( is_far( origin, cell_size, p_trias->at(i)->get_vertex()[j],
			m_trias_before_move->at(i)->get_vertex()[j]        ) ) {
			PL_ERROSH << "[ERROR]PolygonGroup::check_leaped():Leaped Vertex"
			<< " Detected. GroupID:" << m_internal_id
			<< " TriaID:" << p_trias->at(i)->get_id()
			<< " before:(" << m_trias_before_move->at(i)->get_vertex()[j]
			<< ") after:(" << p_trias->at(i)->get_vertex()[j]
			<< ")" << std::endl;
			}
			*/
			if( is_far( origin, cell_size,
			            (*(*p_trias->at(i)->get_vertex()))[j],
			            (*(*m_trias_before_move->at(i)->get_vertex()))[j] )
			) {
				PL_ERROSH << "[ERROR]PolygonGroup::check_leaped():Leaped Vertex"
						<< " Detected. GroupID:" << m_internal_id
						<< " TriaID:" << p_trias->at(i)->get_id()
						<< " before:(" << m_trias_before_move->at(i)->get_vertex()[j]
					<< ") after:(" << p_trias->at(i)->get_vertex()[j]
					<< ")" << std::endl;
			}
			//MOD E 
		}
		// 移動前三角形インスタンスはもう不要なので削除
		delete m_trias_before_move->at(i);
	}

	// あとしまつ
	delete m_trias_before_move;

	return PLSTAT_OK;
}

// protected //////////////////////////////////////////////////////////////////

bool
	PolygonGroup::is_far(
	Vec3<REAL_TYPE> origin,
	Vec3<REAL_TYPE> cell_size,
	Vec3<REAL_TYPE> pos1,
	Vec3<REAL_TYPE> pos2
	)
{
	for( int i=0; i<3; i++ ) {
		// pos1所属ボクセルの起点座標を求める
		REAL_TYPE p;
		REAL_TYPE dist = pos1[i] - origin[i];
		if( dist >= 0 ) {
			p = origin[i] + ((int(dist / cell_size[i])) * cell_size[i]);
		}
		else {
			if( fmodf(dist,cell_size[i]) == 0 ) {
				p = origin[i] + ((int(dist / cell_size[i])) * cell_size[i]);
			} else {
				p = origin[i] + ((int(dist / cell_size[i]) - 1) * cell_size[i]);
			}
		}

		// 隣接ボクセルまで含んだ領域のmin/max
		REAL_TYPE min = p - cell_size[i];
		REAL_TYPE max = p + cell_size[i] * 2;

		// pos2がmin-max間に含まれなければ真
		if( pos2[i] < min || pos2[i] > max ) return true;
	}
	return false;
}
// protected //////////////////////////////////////////////////////////////////

// protected //////////////////////////////////////////////////////////////////

char *PolygonGroup::mk_stl_fname(
	std::string		rank_no,
	std::string		extend,
	std::string		format
	) {
		char		fname1[1024];
		std::string		prefix;
		static char	fname2[1024];

		// グループ名のフルパスを取得して、/を_に置き換え
		strcpy(fname1, acq_fullpath().c_str());

		//cout << __func__ << " acq_fullpath() " <<acq_fullpath()<<std::endl;

		for (int i = 0; i < (int)strlen(fname1); i++) {
			if (fname1[i] == '/')	fname1[i] = '_';
		}

#ifdef DEBUG
		PL_DBGOS << __func__ << " fname1 " <<fname1<<std::endl;
#endif //  DEBUG
		if (format == TriMeshIO::FMT_STL_A || format == TriMeshIO::FMT_STL_AA) {
			//		prefix = "stla";
			prefix = "stl";
		}
		else if (format == TriMeshIO::FMT_OBJ_A || format == TriMeshIO::FMT_OBJ_AA) {
			//		prefix = "stla";
			prefix = "obj";
		}
		else if (format == TriMeshIO::FMT_OBJ_B || format == TriMeshIO::FMT_OBJ_BB) {
			//		prefix = "stla";
			prefix = "obj";
		}

		else {
			prefix = "stl";
		}

		if (rank_no == "") {
			sprintf(fname2, "%s_%s.%s", fname1, extend.c_str(), prefix.c_str());
		}
		else {
			sprintf(fname2, "%s_%s_%s.%s", fname1, rank_no.c_str(), extend.c_str(), 
				prefix.c_str());
		}
		//#define DEBUG
#ifdef DEBUG
		PL_DBGOS << __func__ << " prefix " <<prefix<<std::endl;
#endif //  DEBUG
		//#undef DEBUG

#ifdef DEBUG
		PL_DBGOS << __func__ << " fname2 " <<fname2<<std::endl;
#endif //DEBUG

		return fname2;
}

// protected //////////////////////////////////////////////////////////////////

char *PolygonGroup::mk_stl_fname(
	std::string		rank_no,
	std::string		extend,
	std::string		*format,
	std::map<std::string,std::string>& stl_fname_map
	) {
		char		fname1[1024];
		std::string		prefix;
		static char	fname2[1024];

		// グループ名のフルパスを取得して、/を_に置き換え
		strcpy(fname1, acq_fullpath().c_str());

		// std::cout <<__func__ <<"else" <<TriMeshIO::FMT_VTK_A <<" "<<*format 
		// 	    << " " << ( TriMeshIO::FMT_VTK_A == *format) 
		// 	    <<std::endl;



		//cout << __func__ << " acq_fullpath() " <<acq_fullpath()<<std::endl;

		for (int i = 0; i < (int)strlen(fname1); i++) {
			if (fname1[i] == '/')	fname1[i] = '_';
		}

		//cout << __func__ << " fname1 " <<fname1<<std::endl;

		if (*format == TriMeshIO::FMT_STL_A || *format == TriMeshIO::FMT_STL_AA) {
			prefix = "stl";
		}
		else if (*format == TriMeshIO::FMT_OBJ_A || *format == TriMeshIO::FMT_OBJ_AA) {
			prefix = "obj";
		}
		else if (*format == TriMeshIO::FMT_OBJ_B || *format == TriMeshIO::FMT_OBJ_BB) {
			prefix = "obj";
		}

		else if (*format == TriMeshIO::FMT_VTK_A || *format == TriMeshIO::FMT_VTK_B) {
			//	  std::cout <<__func__ <<"vtk"<<std::endl;
			prefix = "vtk";
			// //if(m_polygons->hasDVertex()){
			// 		prefix = "vtk";
			// } else {
			//   *format=TriMeshIO::FMT_STL_A;
			//   prefix = "stl";
			// }

		}


		else {
			/* std::cout <<__func__ <<"else|" <<TriMeshIO::FMT_VTK_A <<" "<<*format  */
			/* 	    << " " << ( TriMeshIO::FMT_VTK_A == *format)  */
			/* 	    <<std::endl; */
			prefix = "stl";
		}

		if (rank_no == "") {
			sprintf(fname2, "%s_%s.%s", fname1, extend.c_str(), prefix.c_str());
		}
		else {
			sprintf(fname2, "%s_%s_%s.%s", fname1, rank_no.c_str(), extend.c_str(), 
				prefix.c_str());
		}


		//#define DEBUG
#ifdef DEBUG
		PL_DBGOS << __func__ << " prefix " <<prefix <<" format "<<*format <<std::endl;
#endif //  DEBUG


#ifdef DEBUG
		PL_DBGOS << __func__ << " fname2 " <<fname2<<std::endl;
		PL_DBGOS << __func__ << " acq_fullpath() " <<acq_fullpath()<<std::endl;
#endif //  DEBUG
		//#undef DEBUG
		std::string tmp_fname = fname2;
		stl_fname_map.insert(std::map<std::string,std::string>::value_type(acq_fullpath(),tmp_fname));

		return fname2;
}

// protected //////////////////////////////////////////////////////////////////

char* PolygonGroup::mk_id_fname(
	std::string		rank_no,
	std::string		extend
	) {
		char		fname1[1024];
		static char	fname2[1024];

		// グループ名のフルパスを取得して、/を_に置き換え
		strcpy(fname1, acq_fullpath().c_str());
		for (int i = 0; i < (int)strlen(fname1); i++) {
			if (fname1[i] == '/')	fname1[i] = '_';
		}

		if (rank_no == "") {
			sprintf(fname2, "%s_%s.id", fname1, extend.c_str());
		}
		else {
			sprintf(fname2, "%s_%s_%s.id", fname1, rank_no.c_str(), extend.c_str());
		}
		return fname2;
}

// protected //////////////////////////////////////////////////////////////////

int PolygonGroup::create_global_id() {
	static int global_id = 0;
	return global_id++;
}


// public //

POLYLIB_STAT PolygonGroup::replace_DVertex(int nscalar,int nvector){
	return m_polygons->replace_DVertex(nscalar,nvector);
}
// public //

POLYLIB_STAT PolygonGroup::prepare_DVertex(int nscalar,int nvector){
	//#define DEBUG
#ifdef DEBUG
	PL_DBGOSH << "PolygonGroup::"<< __func__ << " this "<< this <<std::endl;
	PL_DBGOSH << "name"<< get_name()  <<std::endl;
	PL_DBGOSH << "parent path "<< get_parent_path()  <<std::endl;
	PL_DBGOSH << "polygon_ptr "<< m_polygons <<std::endl;
#endif
	return m_polygons->prepare_DVertex(nscalar,nvector);
	//#undef DEBUG
}
// public //


DVertexTriangle* PolygonGroup::add_DVertex_Triangle(Vec3<REAL_TYPE>* v){
	//#define DEBUG
#ifdef DEBUG
	PL_DBGOSH << "PolygonGroup::"<< __func__ << " this "<< this <<std::endl;
	PL_DBGOSH << "name "<< get_name()  <<std::endl;
	PL_DBGOSH << "parent path "<< get_parent_path()  <<std::endl;
	PL_DBGOSH << "polygon_ptr "<< m_polygons <<std::endl;
#endif

#ifdef DEBUG
	PL_DBGOSH << "PolygonGroup::"<< __func__
		<<" v0 "<<v[0]
	<<" v1 "<<v[1]
	<<" v2 "<<v[2]
	<< std::endl;
#endif

	return m_polygons->add_DVertex_Triangle(v);
	//#undef DEBUG
}


void PolygonGroup::finalize_DVertex(){
	m_polygons->finalize_DVertex();
}

} //namespace PolylibNS

