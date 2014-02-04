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


#include "Polylib.h"

#include <fstream>
#include <map>
#include <string.h>
#include <vector>
#include <iostream>
#include "polygons/Polygons.h"
#include "polygons/TriMesh.h"
#include "polygons/Triangle.h"
#include "polygons/DVertexTriangle.h"
#include "groups/PolygonGroup.h"
#include "groups/PolygonGroupFactory.h"
#include "common/PolylibStat.h"
#include "common/PolylibCommon.h"
#include "common/BBox.h"
#include "common/Vec3.h"
#include "polygons/VertexList.h"
#include "polygons/VertKDT.h"
#include "polygons/VTree.h"



#include "TextParser.h"
#include "Version.h"

using namespace std;
using namespace PolylibNS;

/// デバッグ用ランク番号グローバル文字列
std::string PolylibNS::gs_rankno = "";


namespace PolylibNS {


/**
* @brief バージョン番号の文字列を返す
*/
std::string Polylib::getVersionInfo()
{
	std::string str(PL_VERSION_NO);
	return str;
}




// public /////////////////////////////////////////////////////////////////////

Polylib* Polylib::get_instance() {
	// この方法ならば、デストラクタを呼び出さなくてもクラスインスタンスの領域
	// が解放できるし、もし本関数が複数回呼び出されても、クラスインスタンスが
	// 複数作成されることはない(=singletonになる)
	static Polylib m_instance;
	return &m_instance;
}

// public /////////////////////////////////////////////////////////////////////

void Polylib::set_factory(
	PolygonGroupFactory		*factory
	) {
#ifdef DEBUG
		PL_DBGOSH<<"Polylib<T>::set_factory calld. "<<factory<<std::endl;
#endif // DEBUG
		if (factory == NULL) return;
#ifdef DEBUG
		PL_DBGOSH<<"Polylib<T>::set_factory new factory is set. "<<factory<<std::endl;
#endif // DEBUG

		// 明示的に指定された場合は、そのFactoryクラスを使用する
		delete m_factory;
		m_factory = factory;

}

/// TextParser
// public /////////////////////////////////////////////////////////////////////
// 

POLYLIB_STAT Polylib::load(
	std::string	config_name,
	REAL_TYPE	scale
	) {
		//#define DEBUG
#ifdef DEBUG
		PL_DBGOSH << "Polylib::load() in." << std::endl;
#endif

		// 設定ファイル読み込み
		try {
			//PolylibConfig base(config_name);

			tp->read(config_name);

#ifdef DEBUG
			PL_DBGOSH << "Polylib::load() reading tp tile done." << std::endl;
#endif

			//  tp->write("tmp.tpp");
			// グループツリー作成
			POLYLIB_STAT stat = make_group_tree(tp);

#ifdef DEBUG
			PL_DBGOSH << "Polylib::load() make_group_tree done." << std::endl;
#endif


			if (stat != PLSTAT_OK)	return stat;

			// STLファイル読み込み (三角形IDファイルは不要なので、第二引数はダミー)
			return load_polygons(false, ID_BIN, scale);

#ifdef DEBUG
			PL_DBGOSH << "Polylib::load() load_polygons done." << std::endl;
#endif

		}
		catch (POLYLIB_STAT e) {
			return e;
		}

#ifdef DEBUG
		PL_DBGOSH << "Polylib::load() done." << std::endl;
#endif


}

// public /////////////////////////////////////////////////////////////////////
//TextParser 版

POLYLIB_STAT Polylib::save(
	std::string	*p_config_name,
	std::string	stl_format,
	std::string	extend
	) {
		//#define DEBUG
#ifdef DEBUG
		PL_DBGOSH << "Polylib::save() in." << std::endl;
#endif
		char	my_extend[128];
		POLYLIB_STAT stat=PLSTAT_OK;


		// 拡張文字列がカラであれば、現在時刻から作成
		if (extend == "") {
			time_t timer = time(NULL);
			struct tm	*date = localtime(&timer);
			sprintf(my_extend, "%04d%02d%02d%02d%02d%02d",
				date->tm_year+1900,date->tm_mon+1,date->tm_mday,
				date->tm_hour,date->tm_min,date->tm_sec);
		}
		else {
			sprintf(my_extend, "%s", extend.c_str());
		}

#ifdef DEBUG
		PL_DBGOSH << __FUNCTION__ << " extend "<< my_extend << std::endl;
#endif
		std::map<std::string,std::string> stl_fname_map;
		std::vector<PolygonGroup*>::iterator it;
		for (it = m_pg_list.begin(); it != m_pg_list.end(); it++) {
			//リーフのみがポリゴン情報を持っている
			if ((*it)->get_children().empty() == false)	continue;

			// ポリゴン数が0ならばファイル出力不要 2010.10.19
			if ((*it)->get_triangles()->size() == 0)	continue;

			// STLファイル保存 (第一引数のランク番号は不要)
			stat = (*it)->save_stl_file("", my_extend, stl_format,
				stl_fname_map);

#ifdef DEBUG
			PL_DBGOSH << __FUNCTION__ << " save_stl_file called "<< my_extend << std::endl;
#endif
			if (stat != PLSTAT_OK) std::cout <<"failed"<<std::endl;
			if (stat != PLSTAT_OK) return stat;

			stat = (*it)->mk_param_tag(tp, "", "", "");
#ifdef DEBUG
			PL_DBGOSH << __FUNCTION__ << " make_param_tag called "<< my_extend << std::endl;
#endif


			if (stat != PLSTAT_OK) return stat;

		}


		// before clearpath // 暫定措置
		if(stl_format=="vtk_a"|| stl_format=="vtk_b")	{
			//create_tp_structure(stl_fname_map);
		} else {



			// update stl filepath 
			// clear file path first
			stat=clearfilepath(tp);

#ifdef DEBUG
			PL_DBGOSH << __FUNCTION__ << " clearfilepath called "<< my_extend << std::endl;
#endif


			//tp->write("tmp2.tpp");
			// set filepath

			stat=setfilepath(stl_fname_map);

#ifdef DEBUG
			PL_DBGOSH << __FUNCTION__ << " setfilepath called"<<  std::endl;
#endif

		}
		//	std::string tmp_extend = my_extend;
		//	char	*config_name = save_config_file("", tmp_extend, stl_format);

		char	*config_name = save_config_file("", my_extend, stl_format);


#ifdef DEBUG
		PL_DBGOSH << __FUNCTION__ << " save_config_file "<<  config_name <<std::endl;
#endif

		if (config_name == NULL)	return PLSTAT_NG;
		else	*p_config_name = std::string(config_name);
		return PLSTAT_OK;
		//#undef DEBUG
}


// public /////////////////////////////////////////////////////////////////////

POLYLIB_STAT Polylib::move(
	PolylibMoveParams	&params
	) {
#ifdef DEBUG
		PL_DBGOSH << "Polylib::move() in." << std::endl;
#endif
		POLYLIB_STAT ret;
		std::vector<PolygonGroup*>::iterator it;

		for (it = m_pg_list.begin(); it != m_pg_list.end(); it++) {

			// リーフグループで、movableフラグONのポリゴンを移動
			if ((*it)->get_children().empty() == true && (*it)->get_movable() ) {
				ret = (*it)->move(params);
				if (ret != PLSTAT_OK)	return ret;

				// 座標移動したのでKD木の再構築
				ret = (*it)->rebuild_polygons();
				if( ret != PLSTAT_OK ) return ret;
			}

		}
		return PLSTAT_OK;
}

// public /////////////////////////////////////////////////////////////////////

std::vector<PolygonGroup *> *Polylib::get_root_groups() const {
#ifdef DEBUG
	PL_DBGOSH << "Polylib::get_root_groups() in." << std::endl;
#endif
	std::vector<PolygonGroup*>	*root = new std::vector<PolygonGroup*>;
	std::vector<PolygonGroup*>::const_iterator	it;

	for (it = m_pg_list.begin(); it != m_pg_list.end(); it++) {
		if (((*it)->get_parent()) == NULL) {
			root->push_back(*it);
		}
	}
	return root;
}

// public /////////////////////////////////////////////////////////////////////

std::vector<PolygonGroup *> *Polylib::get_leaf_groups() const {
#ifdef DEBUG
	PL_DBGOSH << "Polylib::get_leaf_groups() in." << endl;
#endif
	std::vector<PolygonGroup*> *leaf = new std::vector<PolygonGroup*>;
	std::vector<PolygonGroup*>::const_iterator it;

	for (it = m_pg_list.begin(); it != m_pg_list.end(); it++) {
		if (((*it)->get_children()).size() == 0) {
			leaf->push_back(*it);
		}
	}
	return leaf;
}

// public /////////////////////////////////////////////////////////////////////

std::vector<Triangle*>* Polylib::search_polygons(
	std::string		group_name, 
	Vec3<REAL_TYPE>		min_pos, 
	Vec3<REAL_TYPE>		max_pos, 
	bool		every
	) const {
#ifdef DEBUG
		PL_DBGOSH << "Polylib::search_polygons() in." << std::endl;
#endif
		POLYLIB_STAT ret;
		return (std::vector<Triangle*>*)
			search_polygons(group_name, min_pos, max_pos,every, false, &ret);
}

// public /////////////////////////////////////////////////////////////////////

POLYLIB_STAT Polylib::check_group_name(
	const std::string	&name, 
	const std::string	&path
	) {
#ifdef DEBUG
		PL_DBGOSH << "Polylib::check_group_name() in." << std::endl;
#endif
		if (name.empty() == true) {
			PL_ERROSH << "[ERROR]Polylib::check_group_name():Group name is empty." 
				<< std::endl;
			return PLSTAT_GROUP_NAME_EMPTY;
		}

		std::vector<PolygonGroup*>::iterator it;
		for (it = m_pg_list.begin(); it != m_pg_list.end(); it++) {
			if ((*it)->get_name() == name && (*it)->get_parent_path() == path) {
				PL_ERROSH << "[ERROR]Polylib::check_group_name():Group name is "
					<< "duplicate:name:" << name << "," << "path:" << path 
					<< std::endl;
				return PLSTAT_GROUP_NAME_DUP;
			}
		}
		return PLSTAT_OK;
}

// public /////////////////////////////////////////////////////////////////////

PolygonGroup *Polylib::create_polygon_group(std::string class_name,REAL_TYPE tolerance)
{
	//#define DEBUG
#ifdef DEBUG
	PL_DBGOSH << "Polylib::create_polygon_group() in." << std::endl;
#endif
	return m_factory->create_instance(class_name,tolerance);
	//#undef DEBUG

}

// public /////////////////////////////////////////////////////////////////////

void Polylib::add_pg_list(PolygonGroup *pg)
{
#ifdef DEBUG
	PL_DBGOSH << "Polylib::add_pg_list() in." << std::endl;
#endif
	m_pg_list.push_back(pg);
}

// public /////////////////////////////////////////////////////////////////////
/// 2010.10.20 引数FILE *追加。

void Polylib::show_group_hierarchy(FILE	*fp)
{

#ifdef DEBUG
	PL_DBGOSH << "Polylib::show_group_hierarchy() in." << std::endl;
#endif
	std::string		tab;
	std::vector<PolygonGroup*>::iterator	it;
	for (it = m_pg_list.begin(); it != m_pg_list.end(); it++) {
#ifdef DEBUG
		PL_DBGOSH << "Polylib::show_group_hierarchy() "<< (*it) << std::endl;
#endif
		if ((*it)->get_parent() != NULL) {
			// Not Use
		}
		else{
			show_group_name(*it, tab, fp);
		}
	}
}

// public /////////////////////////////////////////////////////////////////////

POLYLIB_STAT Polylib::show_group_info(std::string group_name)
{
#ifdef DEBUG
	PL_DBGOSH << "Polylib::show_group_info() in." << std::endl;
#endif
	PolygonGroup *p = get_group(group_name);
	if (p == NULL) {
		PL_ERROSH << "[ERROR]Polylib::show_group_info():Group not found:"
			<< group_name << std::endl;
		return PLSTAT_GROUP_NOT_FOUND;
	}

	return p->show_group_info();
}

// public /////////////////////////////////////////////////////////////////////

unsigned int Polylib::used_memory_size()
{
	unsigned int size;
	std::vector<PolygonGroup*>::iterator pg;
	std::vector<PrivateTriangle*>::iterator pt;

	// 自クラスとFactoryクラス
	size = sizeof(Polylib) + sizeof(PolygonGroupFactory);

	// ポリゴングループ
#ifdef DEBUG
	PL_DBGOSH << "Polylib::used_memory_size:PolygonGroup num=" << m_pg_list.size() << std::endl;
#endif
	for (pg = m_pg_list.begin(); pg != m_pg_list.end(); pg++) {

		// PolygonGroupクラス
		size += sizeof(PolygonGroup);

		// TriMeshクラス
		size += sizeof(TriMesh);

		// 三角形移動前一時リスト
		size += (*pg)->get_num_of_trias_before_move() * sizeof(std::vector<void> *);

		// リーフにはポリゴンがある
		if ((*pg)->get_children().empty()) {

			// 三角形ポリゴン
			std::vector<PrivateTriangle*>	*tri_list = (*pg)->get_triangles();
#ifdef DEBUG
			PL_DBGOSH << "Polylib::used_memory_size:PrivateTriangle num=" << tri_list->size() << std::endl;
#endif
			size += tri_list->size() * 
				(sizeof(PrivateTriangle)+sizeof(PrivateTriangle*));
			VertexList* vertexlist=(*pg)->get_vertexlist();
			size += vertexlist->size() * (sizeof(Vertex)+sizeof(Vertex*));
			VertKDT* vertkdt=(*pg)->get_vertkdt();
			size+= vertkdt->memory_size();

			// KD木
			VTree *vtree = (*pg)->get_vtree();
			size += vtree->memory_size();
		}

	}

	return size;
}

// public /////////////////////////////////////////////////////////////////////

PolygonGroup* Polylib::get_group(std::string name) const
{
#ifdef DEBUG
	PL_DBGOSH << "Polylib::get_group(" << name << ") in." << std::endl;
#endif
	std::vector<PolygonGroup*>::const_iterator it;
	for (it = m_pg_list.begin(); it != m_pg_list.end(); it++) {
		if (name == (*it)->acq_fullpath()) {
#ifdef DEBUG
			PL_DBGOS	<< "get_group: " << (*it)->get_parent_path()
				<< " name: " << (*it)->get_name()
				<< " size: " << (*it)->get_children().size() << std::endl;
#endif
			return *it;
		}
	}
	for (it = m_pg_list.begin(); it != m_pg_list.end(); it++) {
		if (name == (*it)->get_name()) {
			return *it;
		}
	}
#ifdef DEBUG
	PL_DBGOS << "Polylib::get_group(" << name << ") returns NULL" << std::endl;
#endif
	return NULL;
}

// public /////////////////////////////////////////////////////////////////////

const Triangle* Polylib::search_nearest_polygon(
	std::string	 group_name, 
	const Vec3<REAL_TYPE>&	pos
	) const {

		PolygonGroup* pg = get_group(group_name);
		if (pg == 0) {
			PL_ERROSH << "[ERROR]Polylib::search_polygons():Group not found: " 
				<< group_name << std::endl;
			return 0;
		}

		std::vector<PolygonGroup*>* pg_list2 = new std::vector<PolygonGroup*>;

		//子孫を検索
		search_group(pg, pg_list2);

		//自身を追加
		pg_list2->push_back(pg);

		const PrivateTriangle* tri_min = 0;
		REAL_TYPE dist2_min = 0.0;

		//対象ポリゴングループ毎に検索
		std::vector<PolygonGroup*>::iterator it;
		for (it = pg_list2->begin(); it != pg_list2->end(); it++) {
			//リーフポリゴングループからのみ検索を行う
			if ((*it)->get_children().size()==0) {
				const PrivateTriangle* tri = (*it)->search_nearest(pos);
				if (tri) {
					// MOD	S fuchi error C2440: '初期化中' : 'PolylibNS::Vertex **' から 'PolylibNS::Vec3<T> *' に変換できません。
					//Vec3<REAL_TYPE>* v = tri->get_vertex();

					Vec3<REAL_TYPE>* v = *(tri->get_vertex());
					// MOD	E 					
					Vec3<REAL_TYPE> c((v[0][0]+v[1][0]+v[2][0])/3.0,
						(v[0][1]+v[1][1]+v[2][1])/3.0,
						(v[0][2]+v[1][2]+v[2][2])/3.0);
					REAL_TYPE dist2 = (c - pos).lengthSquared();
					if (tri_min == 0 || dist2 < dist2_min) {
						tri_min = tri;
						dist2_min = dist2;
					}
				}
			}
		}

		delete pg_list2;

		return (const Triangle*)tri_min;
}

// protected //////////////////////////////////////////////////////////////////

Polylib::Polylib()
{
	gs_rankno = "";
	// デフォルトのファクトリークラスを登録する 2010.08.16
	m_factory = new PolygonGroupFactory();


	//Polylib にTextParser クラスを持たせる。
	tp = new TextParser;

	//同一頂点かどうかの判定基準
	m_distance_tolerance=1.0e-10;

	//PL_DBGOS<< __FUNCTION__ <<" m_factory "<< m_factory << " tp " << tp<<std::std::endl;

}

// protected //////////////////////////////////////////////////////////////////

Polylib::~Polylib()
{

	//#define DEBUG
	std::vector<PolygonGroup*>::iterator it;

	if(m_pg_list.size()!=0){

#ifdef DEBUG
		PL_DBGOSH << "~Polylib():there is at least one PolygonGroup.";
#endif

	} else {

#ifdef DEBUG
		PL_DBGOSH << "~Polylib():there is no PolygonGroup.";
#endif
	}

	for (it = m_pg_list.begin(); it != m_pg_list.end();) {
#ifdef DEBUG
		PL_DBGOSH << "~Polylib():" << (*it)->get_name() << std::endl;
#endif
		(*it)->get_children().clear();
		delete *it;
		it = m_pg_list.erase(it);
	}
	if(tp !=0) delete tp;

	//#undef DEBUG

}

// protected //////////////////////////////////////////////////////////////////
// TextParser 版

POLYLIB_STAT Polylib::make_group_tree(
	TextParser* tp
	) {

		//#define DEBUG
#ifdef DEBUG
		PL_DBGOSH << "Polylib::make_group_tree(TextParser) in." << std::endl;
#endif
		TextParserError status = TP_NO_ERROR;

		// 念のため階層構造の最上位へ
		std::string cur;	
		tp->currentNode(cur);
		//std::cout << __func__<< " 1 "<< cur <<std::endl;
		if(cur != "/Polylib") {
			status=tp->changeNode("/Polylib");
			tp->currentNode(cur);
			//std::cout << __func__<<" 2 "<< cur <<std::endl;
			if(status!=TP_NO_ERROR){
				PL_ERROSH << 
					"[ERROR]Polylib::make_group_tree(TextParser):Root node not found."
					<< std::endl;
				return PLSTAT_CONFIG_ERROR;
			}

		}

		// ノードとリーフのリストを取る
		//toleranceの為にリーフを取る
		std::vector<std::string> leaves;
		tp->getLeaves(leaves);
		if(leaves.size()!=0){
			std::vector<std::string>::iterator 
				leaf_iter=find(leaves.begin(),
				leaves.end(),
				PolygonGroup::ATT_NAME_TOLERANCE);
			if(leaf_iter != leaves.end()){
				std::string value;
				status=tp->getValue((*leaf_iter),value);
				int ierror;
				this->m_distance_tolerance=tp->convertDouble(value,&ierror);
			}
		}

		//ノードを取得
		std::vector<std::string> nodes;
		tp->getNodes(nodes);
		std::string current_node;

		// loop over node recurcively.
		PolygonGroup *pg;
		for(std::vector<std::string>::iterator nodes_iter = nodes.begin(); 
			nodes_iter != nodes.end();
			nodes_iter++){

				status = tp->changeNode(*nodes_iter);
				tp->currentNode(current_node);
#ifdef DEBUG	  
				PL_DBGOSH<<"current_node "<< current_node <<  std::endl;
#endif // DEBUG	  
				std::vector<std::string> nodes;
				std::vector<std::string> leaves;

				tp->getNodes(nodes);
				tp->getLabels(leaves);

				// comment out by T. Suzuki Hulinks Inc. 
				// To fix examples/test_mpi2. Please check it later. 2013/7/31
				//if(nodes.size()==0 && leaves.size()==0){ return PLSTAT_CONFIG_ERROR;}

				std::string class_name = "PolygonGroup"; //default


				REAL_TYPE tolerance = this->m_distance_tolerance;
				if(leaves.size()!=0){
					//class name 
					std::vector<std::string>::iterator leaf_iter=find(leaves.begin(),
						leaves.end(),
						PolygonGroup::ATT_NAME_CLASS);
					if(leaf_iter != leaves.end()){
						//     class_name = *leaf_iter;
						std::string value;
						status=tp->getValue((*leaf_iter),value);
						class_name=value;
					}

					//TOLERANCE

					leaf_iter=find(leaves.begin(),
						leaves.end(),
						PolygonGroup::ATT_NAME_TOLERANCE);

					if(leaf_iter != leaves.end()){
						//     class_name = *leaf_iter;
						std::string value;
						status=tp->getValue((*leaf_iter),value);
						int ierror;
						tolerance=tp->convertDouble(value,&ierror);
					}

				}
#ifdef DEBUG
				PL_DBGOSH << "m_factory->create_instance calling" <<std::endl;
#endif
				pg = m_factory->create_instance(class_name,tolerance);
				add_pg_list(pg);
				if (pg == NULL) {
					PL_ERROSH << "[ERROR]Polylib::make_group_tree():Class name not found. "

						<< class_name
						<< std::endl;
					return PLSTAT_CONFIG_ERROR;
				}

				// 配下のタグを取得して、PolygonGroupツリーを作成
				//	  POLYLIB_STAT res = pg->build_group_tree(this, NULL, elem);
				//  
				POLYLIB_STAT res = pg->build_group_tree(this, NULL, tp);
				if (res != PLSTAT_OK) return res;
				status = tp->changeNode("..");

		} 

		return PLSTAT_OK;
		//#undef DEBUG
}


/// textparser 版
// protected //////////////////////////////////////////////////////////////////

POLYLIB_STAT Polylib::load_with_idfile(
	std::string		config_name,
	ID_FORMAT	id_format,
	REAL_TYPE		scale
	) {
#ifdef DEBUG
		PL_DBGOSH << "Polylib::load_with_idfile() in." << std::endl;
#endif
		// 設定ファイル読み込み
		tp->read_local(config_name);

		// グループツリー作成

		POLYLIB_STAT stat = make_group_tree(tp);
		if (stat != PLSTAT_OK)	return stat;

		// STLファイルとIDファイル読み込み
		return load_polygons(true, id_format, scale);
}


// protected //////////////////////////////////////////////////////////////////

POLYLIB_STAT Polylib::load_polygons(
	bool		with_id_file,
	ID_FORMAT	id_format,
	REAL_TYPE		scale
	)
{

#ifdef TIME_MEASURE
	time_t start;
	time(&start);
#endif

	//#define DEBUG
#ifdef DEBUG
	PL_DBGOSH << "Polylib::load_polygons() in." << std::endl;
#endif
	std::vector<PolygonGroup*>::iterator it;
	for (it = m_pg_list.begin(); it != m_pg_list.end(); it++) {
		// リーフの場合
		if ((*it)->get_children().empty() == true) {

			//STLファイルを読み込む

#ifdef DEBUG
			PL_DBGOSH << "Polylib::load_polygons() load_stl_file in." << std::endl;
#endif

			POLYLIB_STAT ret = (*it)->load_stl_file(scale);

#ifdef DEBUG
			PL_DBGOSH << "Polylib::load_polygons() load_stl_file out." << std::endl;
#endif

			if (ret != PLSTAT_OK)		return ret;

			// 必要であればIDファイルを読み込んでm_idを設定
			if (with_id_file == true) {
				POLYLIB_STAT ret = (*it)->load_id_file(id_format);
				if (ret != PLSTAT_OK)		return ret;
			}
		}
	}

#ifdef TIME_MEASURE
	time_t end;
	time(&end);
	printf("start time %s\n",ctime(&start));
	printf("end time %s\n",ctime(&end));
	int a;
	std::cin >> a;
#endif

	return PLSTAT_OK;

	//#undef DEBUG
}


// protected //////////////////////////////////////////////////////////////////
//TextParser 版

char *Polylib::save_config_file(
	std::string	rank_no,
	std::string	extend,
	std::string	format
	){
		//#define DEBUG
#ifdef DEBUG
		PL_DBGOSH << "Polylib::save_config_file() in. " << std::endl;
#endif

		std::vector<PolygonGroup*>::iterator	it;
		//POLYLIB_STAT					stat;

		// ファイル出力
		char	*config_name;

		if (rank_no == "") {
			// ランク番号不要
			config_name = polylib_config_save_file("", extend);
			//config_name = PolylibConfig::save_file(doc, "", extend);
		}
		else {
			// 要ランク番号(MPI版)
			//	config_name = PolylibConfig::save_file(doc, rank_no, extend);
			config_name = polylib_config_save_file(rank_no, extend);
		}

#ifdef DEBUG
		PL_DBGOSH << "save_config_file(): " << config_name << std::endl;
#endif
		//	xmlFreeDoc(doc);

		return config_name;
		//#undef DEBUG
}

/////////////////////////////////////////　　
// 追加　２０１２ー０８
//////////////////////////////////

char* Polylib::polylib_config_save_file(
	std::string rank_no,
	std::string extend){

#define POLYLIB_CONFIG_NAME		"polylib_config"  
#define POLYLIB_CONFIG_EXT		"tp"

#ifdef DEBUG
		std::cout <<__FUNCTION__<<" in "<< rank_no <<" "<< extend << std::endl;
#endif

		static char fname[1024];
		if (rank_no == ""){
			sprintf(fname,"%s_%s.%s",POLYLIB_CONFIG_NAME,extend.c_str(),POLYLIB_CONFIG_EXT);
		} else {
			sprintf(fname, "%s_%s_%s.%s", POLYLIB_CONFIG_NAME, rank_no.c_str(),
				extend.c_str(), POLYLIB_CONFIG_EXT);
		}
		std::string fname_string = fname;

		//config_file の書き込み

		tp->write(fname_string,1);

#ifdef DEBUG
		std::cout <<__FUNCTION__<<" end "<< fname << std::endl;
#endif
		return fname;

}


//TextParser version 
// protected //////////////////////////////////////////////////////////////////

POLYLIB_STAT Polylib::save_with_rankno(
	std::string		*p_config_name,
	int			myrank,
	int 		maxrank,
	std::string		extend,
	std::string		stl_format,
	ID_FORMAT	id_format
	){
		//#define DEBUG
#ifdef DEBUG
		PL_DBGOSH << "Polylib::save_with_rankno() in. " << std::endl;
#endif
		char	my_extend[128];

		// 拡張文字列がカラであれば、現在時刻から作成
		if (extend == "") {
			time_t		timer = time(NULL);
			struct tm	*date = localtime(&timer);
			/* PL_DBGOSH<<"save_with_rank0 "<<date->tm_year+1900 */
			/* 	 <<" "<< date->tm_mon+1 */
			/* 	 << " " << date->tm_mday */
			/* 	 << " " <<date->tm_hour */
			/* 	 << " " <<date->tm_min */
			/* 	 << " " <<date->tm_sec<<std::endl; */
			sprintf(my_extend, "%04d%02d%02d%02d%02d%02d",
				date->tm_year+1900, date->tm_mon+1, date->tm_mday,
				date->tm_hour,      date->tm_min,   date->tm_sec);
		}
		else {
			sprintf(my_extend, "%s", extend.c_str());
		}

		// ランク番号の整形
		char	rank_no[16];
		int		fig = (int)log10((double)maxrank) + 1;
		sprintf(rank_no, "%0*d", fig, myrank);

#ifdef DEBUG
		PL_DBGOSH << "Polylib::save_with_rankno() rank_no" << rank_no<< std::endl;
#endif

		std::map<std::string,std::string> stl_fname_map;
		// STLファイルとIDファイルの保存
		std::vector<PolygonGroup*>::iterator	it;
		for (it = m_pg_list.begin(); it != m_pg_list.end(); it++) {
			POLYLIB_STAT	stat;
			//リーフのみがポリゴン情報を持っている
			if ((*it)->get_children().empty() == false) continue;

			// ポリゴン数が0ならばファイル出力不要 2010.10.19
			if ((*it)->get_triangles()->size() == 0) continue;

			//stat = (*it)->save_stl_file(rank_no, my_extend, stl_format);
			stat = (*it)->save_stl_file(rank_no, my_extend, stl_format,stl_fname_map);
			if (stat != PLSTAT_OK)	return stat;
			stat = (*it)->save_id_file(rank_no, my_extend, id_format);
			if (stat != PLSTAT_OK)	return stat;
			std::string rank_string,my_extend_string;
			rank_string=rank_no;
			my_extend_string = my_extend;
			stat = (*it)->mk_param_tag(tp, "", "", "");
			if (stat != PLSTAT_OK) return stat;

		}
		tp->changeNode("/"); //
		//	cout << "before cleanfilepath" <<std::endl;
		clearfilepath(tp); //clear whole file path
		//	cout << "after cleanfilepath" <<std::endl;

		//	cout << "before setfilepath" <<std::endl;
		setfilepath(stl_fname_map);
		//cout << "after setfilepath" <<std::endl;

		// 定義ファイルの保存	


		char	*config_name = save_config_file(rank_no, my_extend, stl_format);
#ifdef DEBUG	
		PL_DBGOSH << __FUNCTION__ << " config_name "<< config_name << std::endl;
#endif 
#ifdef DEBUG
		PL_DBGOSH << "Polylib::save_with_rankno() config_name " << config_name << std::endl;
#endif
		if (config_name == NULL)	return PLSTAT_NG;
		else	*p_config_name = std::string(config_name);
		return PLSTAT_OK;

}

/////////////////////////////////////////　　
// 追加　２０１２ー０８
//////////////////////////////////

POLYLIB_STAT Polylib::setfilepath(std::map<std::string,std::string>& stl_fname_map){

	//#define DEBUG
#ifdef DEBUG
	PL_DBGOS << "stl_map size " <<  stl_fname_map.size()<<std::endl;
#endif //DEBUG
	//  tp->changeNode("/");
	tp->changeNode("/Polylib");
	for(std::map<std::string,std::string>::iterator map_iter=stl_fname_map.begin();
		map_iter != stl_fname_map.end();
		map_iter++){

#ifdef DEBUG
			PL_DBGOS << "stl_map " <<  map_iter->first <<" "<< map_iter->second<<std::endl;
#endif // DEBUG


			tp->changeNode(map_iter->first); //
			std::string cur;
			tp->currentNode(cur);



			//    cout << __FUNCTION__ << " " <<  cur << std::endl;
			std::string value = "\"" + map_iter->second + "\"";
			//    cout << "before leaf createion" <<std::endl;
			tp->createLeaf("filepath",value);
			//    cout << "after leaf createion" <<std::endl;
			//check
			//    std::vector<std::string> leaves;
			//   tp->getLabels(leaves);  //label　の取り直し
			//    for(std::vector<std::string>::iterator leaf_iter=leaves.begin();
			//	leaf_iter!=leaves.end();
			//	leaf_iter++){
			//      cout << __FUNCTION__ << " "  << *leaf_iter << std::endl;
			//    }
			//  	  std::string value;
			//    tp->getValue("filepath",value);
			//    cout << __FUNCTION__ << " " << cur << " "<< value <<std::endl;

			//    tp->changeNode("/");

			tp->changeNode("/Polylib");
	}
	return PLSTAT_OK;
	//#undef DEBUG
}

/////////////////////////////////////////　　
// 追加　２０１２ー０８
//////////////////////////////////

POLYLIB_STAT Polylib::clearfilepath(TextParser* tp_ptr){

	// recursive にするため、TextParserのポインタを引数に取る。

	std::vector<std::string> leaves;
	tp_ptr->getLabels(leaves,1);

	std::vector<std::string>::iterator leaf_iter=find(leaves.begin(),leaves.end(),"filepath");
	if(leaf_iter!=leaves.end()){ // 見つかったら
		tp_ptr->deleteLeaf("filepath");
	} 
	leaves.clear();
	tp_ptr->getLabels(leaves);


	leaf_iter=find(leaves.begin(),leaves.end(),"filepath[0]");
	if(leaf_iter!=leaves.end()){ // 見つかったら

		int index=0;
		leaf_iter = leaves.begin();
		while(leaf_iter != leaves.end()){
			std::stringstream ss;
			std::string tmpstring="filepath";
			ss << tmpstring <<"["<<index<<"]";
			ss >> tmpstring;
			leaf_iter = find(leaf_iter,leaves.end(),tmpstring);
			if(leaf_iter == leaves.end()) break;
			TextParserError tp_error=tp_ptr -> deleteLeaf(tmpstring);
			if (tp_error!=TP_NO_ERROR) {
				PL_ERROSH << "[ERROR]Polylib::save() "
					<< "can not remove leaf = " << tmpstring << std::endl;
				return PLSTAT_NG;
			}
			index++;
			leaf_iter++;
		}
	}

	std::vector<std::string> nodes;
	tp_ptr->getNodes(nodes);
	for( std::vector<std::string>::iterator node_iter=nodes.begin();
		node_iter!=nodes.end();
		node_iter++){
			tp_ptr->changeNode(*node_iter);
			clearfilepath(tp_ptr);
			tp_ptr->changeNode("..");
	}

	return PLSTAT_OK;

}

void Polylib::create_tp_structure(std::map<std::string,std::string> &stl_fname_map){

	tp->changeNode("/Polylib"); //
	for(std::map<std::string,std::string>::iterator map_iter=stl_fname_map.begin();
		map_iter != stl_fname_map.end();
		map_iter++){

#ifdef DEBUG
			PL_DBGOS << "stl_map " <<  map_iter->first <<" "<< map_iter->second<<std::endl;
#endif // DEBUG

			std::string a=map_iter->first+"/filepath";
			std::string b="\""+map_iter->second+"\"";
			tp->createLeaf(a,b);

	}


}

// protected //////////////////////////////////////////////////////////////////
// 2010.10.20 引数FILE *追加。

void Polylib::show_group_name(
	PolygonGroup	*pg, 
	std::string			tab,
	FILE			*fp
	){
		std::vector<PolygonGroup*>::iterator it;
		// ユーザ定義id出力 2010.10.20
		// ユーザ定義ラベル出力 2012.08.31
		// ユーザ定義タイプ出力 2013.07.17
		if (fp == NULL) {
			PL_DBGOSH << "Polylib::show_group_name: " << tab ;
			if (pg->get_parent_path().empty() == true)  PL_DBGOS << "+";
			PL_DBGOS << pg->get_name() << ":" << pg->acq_file_name() << ":"
				<< pg->get_id() << ":" << pg->get_label() << ":"
				<< pg->get_type()  <<std::endl;

			//pg->show_bbox();
			// to print vertex for checking read file.
			//if(pg->acq_file_name()!="") pg->print_vertex();

		}
		else {
			// 出力先が指定されていれば、そちらに出力
			fprintf(fp, "%sPolylib::show_group_name:%s%s%s:%s:%d:%s:%s\n", 
				gs_rankno.c_str(),		tab.c_str(),
				(pg->get_parent_path().empty() == true) ? "+" : "",
				pg->get_name().c_str(),	pg->acq_file_name().c_str(),
				pg->get_id(), pg->get_label().c_str(), pg->get_type().c_str());
		}

		tab = tab + "    ";
		for (it = pg->get_children().begin(); it != pg->get_children().end(); it++){
			show_group_name(*it, tab, fp);
		}
}

// protected //////////////////////////////////////////////////////////////////

PolygonGroup* Polylib::get_group(int internal_id) const
{
	//#define DEBUG
#ifdef DEBUG
	PL_DBGOSH << "Polylib::get_group(" << internal_id << ") in." << std::endl;
#endif
	std::vector<PolygonGroup*>::const_iterator it;
	for (it = m_pg_list.begin(); it != m_pg_list.end(); it++) {
		if (internal_id == (*it)->get_internal_id()) {
			return *it;
		}
	}
#ifdef DEBUG
	PL_DBGOS << "Polylib::get_group(" << internal_id << ") returns NULL" << std::endl;
#endif
	return NULL;
	//#undef  DEBUG
}

// private ////////////////////////////////////////////////////////////////////

std::vector<PrivateTriangle*>* Polylib::search_polygons(
	std::string			group_name, 
	Vec3<REAL_TYPE>			min_pos, 
	Vec3<REAL_TYPE>			max_pos,
	bool			every,
	bool			linear, 
	POLYLIB_STAT	*ret
	) const {
#ifdef DEBUG
		PL_DBGOSH << "Polylib::search_polygons() in." << std::endl;
#endif
		std::vector<PrivateTriangle*>* tri_list = new std::vector<PrivateTriangle*>;
		PolygonGroup* pg = get_group(group_name);

		if (pg == 0) {
			PL_ERROSH << "[ERROR]Polylib::search_polygons():Group not found: " 
				<< group_name << std::endl;
			*ret = PLSTAT_GROUP_NOT_FOUND;
			return tri_list;
		}
		std::vector<PolygonGroup*>* pg_list2 = new std::vector<PolygonGroup*>;

#ifdef BENCHMARK
		double st1, st2, ut1, ut2, tt1, tt2;
		bool ret1, ret2;
		ret1 = getrusage_sec(&ut1, &st1, &tt1);
#endif

		//子孫を検索
		search_group(pg, pg_list2);

		//自身を追加
		pg_list2->push_back(pg);

		// 検索範囲
		BBox bbox;
		bbox.init();
		bbox.add(min_pos);
		bbox.add(max_pos);

		//全ポリゴングループを検索
		std::vector<PolygonGroup*>::iterator it;
		for (it = pg_list2->begin(); it != pg_list2->end(); it++) {

			//リーフ構造からのみ検索を行う
			if ((*it)->get_children().size()==0) {
				POLYLIB_STAT ret2;
#ifdef DEBUG
				if (linear == false) {
#endif
					ret2 = (*it)->search (&bbox,every,tri_list);
					if (ret2 != PLSTAT_OK) {
						*ret = ret2;
						return tri_list;
					}
#ifdef DEBUG
				}
				else{
					ret2 = (*it)->linear_search (&bbox,every,tri_list);
					if (ret2 != PLSTAT_OK) {
						*ret = ret2;
						return tri_list;
					}
				}
#endif
			}
		}
#ifdef BENCHMARK
		ret2 = getrusage_sec(&ut2,&st2,&tt2);
		if (ret1 == false || ret2 == false) {
			PL_DBGOSH << "Search SYS   Time Error" << std::endl;
			PL_DBGOSH << "Search CPU   Time Error" << std::endl;
			PL_DBGOSH << "Search Total Time Error" << std::endl;
		}
		else{
			cout.setf(ios::scientific, ios::floatfield);
			PL_DBGOSH << "Search SYS   Time:" << st2 - st1 << std::endl;
			PL_DBGOSH << "Search CPU   Time:" << ut2 - ut1 << std::endl;
			PL_DBGOSH << "Search Total Time:" << tt2 - tt1 << std::endl;
			std::cout.unsetf(ios::scientific);
		}
#endif

		delete pg_list2;
		*ret = PLSTAT_OK;
		return tri_list;
}

// private ////////////////////////////////////////////////////////////////////

void Polylib::search_group(
	PolygonGroup			*p, 
	std::vector<PolygonGroup*>	*pg
	) const {
#ifdef DEBUG
		PL_DBGOSH << "Polylib::search_group() in." << std::endl;
#endif
		std::vector<PolygonGroup*>::iterator it;
		for (it = p->get_children().begin(); it != p->get_children().end(); it++) {
			pg->push_back(*it);
			search_group(*it, pg);
		}
}



// public/////

void Polylib::make_DVertex_PolygonGroup(
	std::string group_name,
	int nscalar,
	int nvector){

		//#define DEBUG
#ifdef DEBUG
		PL_DBGOSH << "Polylib::"<< __func__ <<" in." << std::endl;
#endif

		std::string parent_name=group_name;
		std::string daughter_name;
		//  std::string::size_type index=daughter_name.find("/");  
		std::string::size_type index=parent_name.rfind("/");  
		std::vector<std::string> path_list;

		//path_list.push_back(daughter_name);
		path_list.push_back(parent_name);
		//PL_DBGOSH << "list "<<daughter_name <<std::endl;
#ifdef DEBUG
		PL_DBGOSH << "list "<<parent_name << " "<< index <<std::endl;
#endif
		while( index !=std::string::npos){
			daughter_name=parent_name.substr(index+1);
			parent_name=parent_name.substr(0,index);
			path_list.push_back(parent_name);
			//    PL_DBGOSH << "list "<<parent_name << " "<< path_list.size() <<std::endl;
			index=parent_name.find("/");  
		}

		//  PL_DBGOSH << "list check"<<std::endl;

		for(int i=0;i<path_list.size();++i){
			//    PL_DBGOSH << "list check"<<std::endl;
			PolygonGroup* pg_instance = get_group(path_list[i]);
			// if(pg_instance==NULL)   PL_DBGOSH << "We do not have ";
			// else   PL_DBGOSH << "We have ";
			// PL_DBGOSH<< path_list[i]<<std::endl;
		}

		PolygonGroup* pg_instance;
		PolygonGroup* parent=NULL;
		int i=0;
		while( i<path_list.size() ){
			pg_instance=get_group(path_list[i]);
#ifdef DEBUG
			PL_DBGOSH << "while "<<i<<" "<<pg_instance <<std::endl;
#endif
			if(pg_instance!=NULL) break;
			i++;
		}


		if(i<path_list.size()){
			parent = pg_instance;
			parent_name=path_list[i];
			daughter_name= group_name;
			index = group_name.find_first_not_of(parent_name);
			if(index!=std::string::npos){
				daughter_name= group_name.substr(index+1);
			}

			PL_DBGOSH << "list check "<< parent_name<< " "<<daughter_name<< " "<<index<<std::endl;

		} else {

		}

		//PolygonGroup* pg_instance = get_group(group_name);
		//  PolygonGroup* pg_instance = get_group(path_list[i]);

		pg_instance = get_group(group_name);

		std::string class_name="PolygonGroup";

		if(pg_instance==NULL){
#ifdef DEBUG
			PL_DBGOSH << "Polylib::"<< __func__ <<" polygon group " 
				<< group_name << " is not found."<< std::endl;
#endif
			pg_instance=
				m_factory->create_instance(class_name,
				this->m_distance_tolerance);
			add_pg_list(pg_instance);

			if (pg_instance == NULL) {
				PL_ERROSH << "[ERROR]Polylib::"<< __func__ <<" Class name not found."
					<< class_name
					<< std::endl;
				//return PLSTAT_CONFIG_ERROR;
			}


			if(parent ==NULL){
				pg_instance->build_group_tree(this, parent, group_name);
			}    else {
				pg_instance->build_group_tree(this, parent, daughter_name);
			}
			//get right pointer again
			pg_instance = get_group(group_name);
			pg_instance->prepare_DVertex(nscalar,nvector);
#ifdef DEBUG
			PL_DBGOSH << "Polylib::"<< __func__ 
				<<" gp  " << group_name <<" end " <<get_group(group_name)<<std::endl;
#endif


		} else {
#ifdef DEBUG
			PL_DBGOSH << "Polylib::"<< __func__ <<" polygon group " 
				<< group_name << " is found."<< std::endl;
#endif
			pg_instance->replace_DVertex(nscalar,nvector);



		}


		//#undef DEBUG
}

////////////////////////////////////////////////

DVertexTriangle* 
	Polylib::add_DVertex_Triangle(std::string name,
	Vec3<REAL_TYPE>* v){

		//#define DEBUG
#ifdef DEBUG
		PL_DBGOSH << "Polylib::"<< __func__ <<" in." << std::endl;
#endif


		DVertexTriangle* ret=0;
		PolygonGroup* pg_instance = get_group(name);

#ifdef DEBUG
		PL_DBGOSH << "Polylib::"<< __func__ <<" name "<<name <<" "<<pg_instance << std::endl;
#endif

		if( pg_instance!=NULL)  ret = pg_instance->add_DVertex_Triangle(v);

#ifdef DEBUG
		PL_DBGOSH << "Polylib::"<< __func__ 
			<<" v0 "<<v[0]
		<<" v1 "<<v[1]
		<<" v2 "<<v[2]
		<<" "<<pg_instance << std::endl;
#endif


		return ret;
		//#undef DEBUG
}



void Polylib::finalize_DVertex(std::string name){
	PolygonGroup* pg_instance = get_group(name);
	pg_instance->finalize_DVertex();

}


} //namespace PolylibNS

