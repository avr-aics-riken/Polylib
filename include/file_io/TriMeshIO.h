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

#ifndef mesh_io_h
#define mesh_io_h

#include <vector>
#include <map>
#include "common/PolylibStat.h"
#include "common/PolylibCommon.h"
#include "file_io/stl.h"
#include "file_io/obj.h"
#include "file_io/vtk.h"
#include "polygons/Vertex.h"
#include "polygons/VertexList.h"
#include "polygons/Triangle.h"

namespace PolylibNS {

////////////////////////////////////////////////////////////////////////////
///
/// クラス:TriMeshIO
/// 三角形ポリゴン入出力管理。
///
////////////////////////////////////////////////////////////////////////////
class TriMeshIO {
public:

	///
	/// STL /OBJファイルを読み込み、tri_listにセットする。
	///
	///  @param[in,out] vertex_list	頂点リストの領域。
	///  @param[in,out] tri_list	三角形ポリゴンリストの領域。
	///  @param[in]		fmap		ファイル名、ファイルフォーマットのセット。
	///  @return	POLYLIB_STATで定義される値が返る。
	///
  template <typename T>
	static POLYLIB_STAT load(
		VertexList<T>*	vertex_list,
		std::vector<PrivateTriangle<T>*>	*tri_list,
		const std::map<std::string, std::string>	&fmap,
		T scale = 1.0
	);


	///
	/// tri_listの内容をSTL形式でファイルへ保存。
	///
	///  @param[in] tri_list	三角形ポリゴンのリスト(出力内容)。
	///  @param[in] fname		ファイル名。
	///  @param[in] fmt	ファイルフォーマット。
	///  @return	POLYLIB_STATで定義される値が返る。
	///
  template <typename T>
  static POLYLIB_STAT save(
			   VertexList<T>* vertex_list,
			   std::vector<PrivateTriangle<T>*>	*tri_list,
			   std::string				fname, 
			   std::string 				fmt = ""
	);

	///
	/// ファイル名を元に入力ファイルのフォーマットを取得する。
	///
	///  @param[in] filename		入力ファイル名。
	///  @return	判定したファイルフォーマット。
	///  @attention	ファイル拡張子が"stl"の場合、ファイルを読み込んで判定する。
	///
	static std::string input_file_format(
		const std::string &filename
	);

	/// STLファイルのフォーマット種別
	///
	///  @attention STLファイルの拡張子とは異なるので注意すること。
	///
	static const std::string FMT_STL_A;		///< アスキーファイル
	static const std::string FMT_STL_AA;	///< アスキーファイル
	static const std::string FMT_STL_B;		///< バイナリファイル
	static const std::string FMT_STL_BB;	///< バイナリファイル
	static const std::string FMT_OBJ_A;		///< ascii
	static const std::string FMT_OBJ_AA;	///< ascii 
	static const std::string FMT_OBJ_B;		///< binary
	static const std::string FMT_OBJ_BB;	///< binary
	static const std::string FMT_VTK_A;	///< vtk ascii
	static const std::string FMT_VTK_B;	///< vtk binary
	static const std::string DEFAULT_FMT;	///< TrimeshIO.cxxで定義している値

};

// public /////////////////////////////////////////////////////////////////////
 template <typename T>
POLYLIB_STAT TriMeshIO::load(
     VertexList<T>*	vertex_list, 
     std::vector<PrivateTriangle<T>*>	*tri_list, 
     const std::map<std::string, std::string>	&fmap,
	T scale
) {


   //   PL_DBGOSH<< "TriMeshIO::"<<__func__<<std::endl;
   typename std::map<std::string, std::string>::const_iterator	it;
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

	  PL_DBGOSH<< __func__<<" " << fname << " "<< fmt << std::endl;

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
 template <typename T>
POLYLIB_STAT TriMeshIO::save(
			     VertexList<T>* vertex_list,
			     std::vector<PrivateTriangle<T>*>	*tri_list, 
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

#endif  // mesh_io_h
