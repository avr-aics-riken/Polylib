/*
###################################################################################
#
# Polylib - Polygon Management Library
#
# Copyright (c) 2010-2011 VCAD System Research Program, RIKEN.
# All rights reserved.
#
# Copyright (c) 2012-2015 Advanced Institute for Computational Science (AICS), RIKEN.
# All rights reserved.
#
# Copyright (c) 2016-2018 Research Institute for Information Technology (RIIT), Kyushu University.
# All rights reserved.
#
###################################################################################
*/

#include <string.h>
#include <string>
#include "polygons/Triangle.h"
#include "file_io/TriMeshIO.h"
#include "file_io/stl.h"

namespace PolylibNS {

using namespace std;

const string TriMeshIO::FMT_STL_A  = "stl_a";
const string TriMeshIO::FMT_STL_AA = "stl_aa";
const string TriMeshIO::FMT_STL_B  = "stl_b";
const string TriMeshIO::FMT_STL_BB = "stl_bb";
const string TriMeshIO::FMT_OBJ_A  = "obj_a";
const string TriMeshIO::FMT_OBJ_AA = "obj_aa";
const string TriMeshIO::FMT_OBJ_B  = "obj_b";
const string TriMeshIO::FMT_OBJ_BB = "obj_bb";
const string TriMeshIO::FMT_VTK_A  = "vtk_a";
const string TriMeshIO::FMT_VTK_B  = "vtk_b";
const string TriMeshIO::DEFAULT_FMT = TriMeshIO::FMT_STL_B;



// public /////////////////////////////////////////////////////////////////////
string TriMeshIO::input_file_format(
	const string &filename
	)
{


	//書式の決定
	char	*ext = stl_get_ext(filename);
	//#define DEBUG
#ifdef DEBUG
	PL_DBGOS << __func__ << " file " << filename <<" ext " << ext <<std::endl;
#endif
	//#undef DEBUG


	if (!strcmp(ext, "stla") || !strcmp(ext, "STLA")) {
		return FMT_STL_A;
	}
	else if (!strcmp(ext, "stlb") || !strcmp(ext, "STLB")) {
		return FMT_STL_B;
	}

	else if (!strcmp(ext, "obja") || !strcmp(ext, "OBJA")) {
		return FMT_STL_A;
	}
	else if (!strcmp(ext, "objb") || !strcmp(ext, "OBJB")) {
		return FMT_STL_B;
	}

	else if (!strcmp(ext, "stl") || !strcmp(ext, "STL")) {
		//読み込んで書式を判定する
		if(is_stl_a(filename) == true)	return FMT_STL_A;
		else	return FMT_STL_B;
	}
	else if (!strcmp(ext, "obj") || !strcmp(ext, "OBJ")) {
		if(is_obj_a(filename) == true)	  return FMT_OBJ_A;
		else	return FMT_OBJ_B;

	}



	return "";
}


POLYLIB_STAT TriMeshIO::load(
	VertexList*	vertex_list,
	std::vector<PrivateTriangle*>	*tri_list,
	const std::map<std::string, std::string>	&fmap,
	PL_REAL scale
	) {


		//   PL_DBGOSH<< "TriMeshIO::"<<__func__<<std::endl;
		std::map<std::string, std::string>::const_iterator	it;
		int					total;
		POLYLIB_STAT				ret = PLSTAT_OK;

		if (tri_list == NULL) {
			PL_ERROSH << "[ERROR]TriMeshIO::load():tri_list is NULL." << std::endl;
			return PLSTAT_NG;
		}
		if (vertex_list == NULL) {
			PL_ERROSH << "[ERROR]TriMeshIO::load():vertex_list is NULL." << std::endl;
			return PLSTAT_NG;
		}

		total = 0;	// 通算番号に初期値をセット
		for (it = fmap.begin(); it != fmap.end(); it++) {
			std::string fname = it->first;
			std::string fmt = it->second;
#ifdef DEBUG
			PL_DBGOSH<< __func__<<" " << fname << " "<< fmt << std::endl; // keno-2014-03-01
#endif
			if (fmt == "") {
				PL_ERROSH << "[ERROR]:TTriMeshIO::load():Unknown stl format." << std::endl;
				ret = PLSTAT_NG;
			}
			else if (fmt == FMT_STL_A || fmt == FMT_STL_AA) {
				//PL_DBGOSH<< __func__<<" stl_a_load "<< fmt << std::endl;
				ret = stl_a_load(vertex_list,
					tri_list, fname, &total, scale);

			}
			else if (fmt == FMT_STL_B || fmt == FMT_STL_BB) {
				//		  PL_DBGOSH<< __func__<<" stl_b_load "<< fmt << std::endl;
				ret = stl_b_load(vertex_list,tri_list, fname, &total, scale);
				//ret = stl_b_load(tri_list, fname, &total, scale);
			}
			else if (fmt == FMT_OBJ_A || fmt == FMT_OBJ_AA) {
				//		  PL_DBGOSH<< __func__<<" obj_a_load "<< fmt << std::endl;
				ret = obj_a_load(vertex_list,tri_list, fname, &total, scale);
				//ret = stl_b_load(vertex_list,tri_list, fname, &total, scale);
				//ret = stl_b_load(tri_list, fname, &total, scale);
			}
			else if (fmt == FMT_OBJ_B || fmt == FMT_OBJ_BB) {
				//PL_DBGOSH<< __func__<<" obj_b_load "<< fmt << std::endl;
				ret = obj_b_load(vertex_list,tri_list, fname, &total, scale);
			} else {
				//PL_DBGOSH<< __func__<<" failed!!! "<< fmt << std::endl;
				return PLSTAT_UNKNOWN_STL_FORMAT;
			}
			// 一ファイルでも読み込みに失敗したら戻る
			if (ret != PLSTAT_OK)		return ret;
		}


		//PL_DBGOSH<< __func__ <<" end"<<std::endl;

		//	return vtx_compaction(vertex_list,tri_list);
		return ret;
}

// public /////////////////////////////////////////////////////////////////////

POLYLIB_STAT TriMeshIO::save(
	VertexList* vertex_list,
	std::vector<PrivateTriangle*>	*tri_list,
	std::string	fname,
	std::string	fmt
	) {
		//#define DEBUG
#ifdef DEBUG
		PL_DBGOS<<"TriMeshIO::"<<__FUNCTION__ << " saving file..."<<std::endl;
		PL_DBGOS<<__FUNCTION__ << " fname fmt "<<fname <<" "<<fmt <<std::endl;
#endif
		if (tri_list == NULL) {
			PL_ERROSH << "[ERROR]:TriMeshIO::save():tri_list is NULL." << std::endl;
			return PLSTAT_NG;
		}

		if (fmt == FMT_STL_A || fmt == FMT_STL_AA) {
			return stl_a_save(tri_list, fname);
		}
		else if (fmt == FMT_STL_B || fmt == FMT_STL_BB) {
			return stl_b_save(tri_list, fname);
		}
		else if (fmt == FMT_OBJ_A || fmt == FMT_OBJ_AA) {
#ifdef DEBUG
			PL_DBGOS<<__FUNCTION__ << " saving file. ascii obj file to be saved."<<std::endl;
#endif

			return obj_a_save(vertex_list,tri_list, fname);
		}
		else if (fmt == FMT_OBJ_B ) {
			return obj_b_save(vertex_list,tri_list, fname);
		}
		else if (fmt == FMT_OBJ_BB) {
			return obj_bb_save(vertex_list,tri_list, fname);
		}
		else if (fmt == FMT_VTK_A) {
			return vtk_a_save(vertex_list,tri_list, fname);
		}
		else if (fmt == FMT_VTK_B) {
			return vtk_b_save(vertex_list,tri_list, fname);
		}
		else{
			return PLSTAT_UNKNOWN_STL_FORMAT;
		}

		//#undef DEBUG
}



} //namespace PolylibNS
