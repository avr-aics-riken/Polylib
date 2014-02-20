// -*- Mode: c++ -*- 
/*
* Polylib - Polygon Management Library
*
* Copyright (c) 2010-2011 VCAD System Research Program, RIKEN.
* All rights reserved.
*
* Copyright (c) 2012-2014 Advanced Institute for Computational Science, RIKEN.
* All rights reserved.
*
*/

#ifndef polylib_obj_h
#define polylib_obj_h

#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <iomanip>

#include "common/PolylibCommon.h"
#include "polygons/Triangle.h"
#include "polygons/Vertex.h"
#include "polygons/VertexList.h"
#include "file_io/stl.h"
#include "common/tt.h"

#if HAVE_STRING_H 
#include <string.h>
#else
// write some code here
#endif

namespace PolylibNS {

///  ファイルの一部を読み込み、ascii / binary を判定する。
/// 
/// @param[in] path ファイルパス
bool is_obj_a(std::string path);

/// 
/// ASCII モードのOBJファイルを読み込み、VertexList, tri_listに三角形ポリゴン情報を設定する。
/// 
///
///  @param[in,out] vertex_list 頂点リストの領域。
///  @param[in,out] tri_list	三角形ポリゴンリストの領域。
///  @param[in]		fname		STLファイル名。
///  @param[in,out] total		ポリゴンIDの通番。
///  @return	POLYLIB_STATで定義される値が返る。
///
///  エラーについて
///  1. ファイルが開けないとき
///  2. face のリストがまだ読み込まれていない頂点IDを使った場合
///  
///  注意事項
///  faceはすべて三角形だとして読み込む。
///  情報として取り込むのは、v と f のみで、他の情報は破棄される。
///  v と fの情報から、normalを計算する。


POLYLIB_STAT obj_a_load(
	VertexList* vertex_list, 
	std::vector<PrivateTriangle*> *tri_list,
	std::string fname,
	int	*total,
	REAL_TYPE	scale=1.0
	);

/// 
/// OBJ_BIN形式のファイルを読み込み、
/// VertexList, tri_listに三角形ポリゴン情報を設定する。
/// 頂点法線が記録されているかどうかを判別して読み取る。
/// 頂点法線は記録されていても、この時点で情報を捨てる。
/// 
///
///  @param[in,out] vertex_list 頂点リストの領域。
///  @param[in,out] tri_list	三角形ポリゴンリストの領域。
///  @param[in]		fname		STLファイル名。
///  @param[in,out] total		ポリゴンIDの通番。
///  @return	POLYLIB_STATで定義される値が返る。
///
///  エラーについて
///  1. ファイルが開けないとき
///  2. face のリストがまだ読み込まれていない頂点IDを使った場合
///  
///  注意事項
///  faceはすべて三角形だとして読み込む。
///  情報として取り込むのは、v と f のみで、他の情報は破棄される。
///  v と fの情報から、normalを計算する。


POLYLIB_STAT obj_b_load(
	VertexList* vertex_list, 
	std::vector<PrivateTriangle*> *tri_list,
	std::string fname,
	int	*total,
	REAL_TYPE	scale=1.0
	);

/// 
/// VertexList, tri_listにから OBJアスキー形式に出力する。
//  尚、頂点法線を計算して出力する。
/// 
///
///  @param[in] vertex_list 頂点リストの領域。
///  @param[in] tri_list　三角形ポリゴンリストの領域。
///  @param[in]	fname	　ファイル名。
///
///  エラーについて
///  
///  注意事項
///  情報として書き出すのは、v, vn と f のみで、他の情報は破棄される。
///  すべての面を持たない頂点の頂点法線は、正しく計算されない可能性があるので、注意すること。


POLYLIB_STAT obj_a_save(
	VertexList* vertex_list, 
	std::vector<PrivateTriangle*> *tri_list,
	std::string fname
	);

/// 
/// VertexList, tri_listにから OBJバイナリ形式に出力する。
//  尚、頂点法線を計算して出力する。
///
///  @param[in] vertex_list 頂点リストの領域。
///  @param[in] tri_list　三角形ポリゴンリストの領域。
///  @param[in]	fname	　ファイル名。
///
///  エラーについて
///  
///  注意事項
///  情報として書き出すのは、v と f のみで、他の情報は破棄される。



POLYLIB_STAT obj_b_save(
	VertexList* vertex_list, 
	std::vector<PrivateTriangle*> *tri_list,
	std::string fname
	);


/// 
/// VertexList, tri_listにから OBJバイナリ形式に出力する。
//  尚、頂点法線を計算して出力する。
///
///  @param[in] vertex_list 頂点リストの領域。
///  @param[in] tri_list　三角形ポリゴンリストの領域。
///  @param[in]	fname	　ファイル名。
///
///  エラーについて
///  
///  注意事項
///  情報として書き出すのは、v, vn と f のみで、他の情報は破棄される。
///  すべての面を持たない頂点の頂点法線は、正しく計算されない可能性があるので、注意すること。


POLYLIB_STAT obj_bb_save(
	VertexList* vertex_list, 
	std::vector<PrivateTriangle*> *tri_list,
	std::string fname
	);


//////////////////////////////////////////////////////////////////////////////
// POLYLIB_STAT obj_a_load VertexList version.
//////////////////////////////////////////////////////////////////////////////

POLYLIB_STAT obj_a_load(VertexList* vertex_list, 
	std::vector<PrivateTriangle*>*tri_list, 
	std::string	fname,
	int	*total,
	REAL_TYPE scale );

/////////////////////////////////////////////////////

POLYLIB_STAT obj_a_save(
	VertexList* vertex_list, 
	std::vector<PrivateTriangle*> *tri_list,
	std::string fname
	);
///////////////////////////////////////////////////////////////

POLYLIB_STAT obj_b_save(
	VertexList* vertex_list, 
	std::vector<PrivateTriangle*> *tri_list,
	std::string fname);
///////////////////////////////////////////////////////////////

POLYLIB_STAT obj_bb_save(
	VertexList* vertex_list, 
	std::vector<PrivateTriangle*> *tri_list,
	std::string fname);
/////////////////////////////////////////////////
////////////////////////////


POLYLIB_STAT obj_b_load(
	VertexList *vertex_list,
	std::vector<PrivateTriangle*> *tri_list, 
	std::string fname,
	int	*total,
	REAL_TYPE	scale
	);

}// end of namespace PolylibNS

#endif //obj_h
