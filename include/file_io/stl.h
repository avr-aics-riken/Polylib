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
# Copyright (c) 2016-2017 Research Institute for Information Technology (RIIT), Kyushu University.
# All rights reserved.
#
###################################################################################
*/

#ifndef stl_h
#define stl_h

#include <fstream>
#include <vector>
#include <iomanip>

#include "common/PolylibCommon.h"

#include "polygons/PrivateTriangle.h"
#include "polygons/Vertex.h"
#include "polygons/VertexList.h"



#if HAVE_STRING_H
#include <string.h>
#else
// write some code here
#endif


#define SCIENTIFIC_OUT		0
#define STL_HEAD		80		// header size for STL binary
#define STL_BUFF_LEN		256
#define TT_OTHER_ENDIAN		1
#define TT_LITTLE_ENDIAN	2
#define TT_BIG_ENDIAN		3

namespace PolylibNS {


// tempolary define uint to unsigned int
typedef unsigned int uint;
typedef unsigned short ushort;


void	tt_invert_byte_order(void* _mem, int size, int n);
int	tt_check_machine_endian();
void	tt_read(std::istream& is, void* _data, int size, int n, int inv);
void	tt_write(std::ostream& os, const void* _data, int size, int n, int inv);


class PrivateTriangle;


///
/// ASCIIモードのSTLファイルを読み込み、VertexList, tri_listに三角形ポリゴン情報を設定する。
///
///
///  @param[in,out] vertex_list 頂点リストの領域。
///  @param[in,out] tri_list	三角形ポリゴンリストの領域。
///  @param[in]		fname		STLファイル名。
///  @param[in,out] total		ポリゴンIDの通番。
///  @return	POLYLIB_STATで定義される値が返る。
///

POLYLIB_STAT stl_a_load(
	VertexList* vertex_list,
	std::vector<PrivateTriangle*>	*tri_list,
	std::string 					fname,
	int								*total,
	PL_REAL							scale=1.0
	);

///
/// 三角形ポリゴン情報をASCIIモードでSTLファイルに書き出す。
///
///  @param[in] tri_list	三角形ポリゴン情報。
///  @param[in] fname		STLファイル名。
///  @return	POLYLIB_STATで定義される値が返る。
///

POLYLIB_STAT stl_a_save(
	std::vector<PrivateTriangle*>	*tri_list,
	std::string 					fname
	);

///
/// バイナリモードのSTLファイルを読み込み、tri_listに三角形ポリゴン情報を設定
/// する。
///
///  @param[in,out] tri_list	三角形ポリゴンリストの領域。
///  @param[in]		fname		ファイル名。
///  @param[in,out] total		ポリゴンIDの通番。
///  @return	POLYLIB_STATで定義される値が返る。
///

POLYLIB_STAT stl_b_load(
	VertexList *vertex_list,
	std::vector<PrivateTriangle*>	*tri_list,
	std::string   fname,
	int	*total,
	PL_REAL	scale=1.0
	);

///
/// 三角形ポリゴン情報をバイナリモードでSTLファイルに書き出す。
///
///  @param[in] tri_list	三角形ポリゴン情報。
///  @param[in] fname		STLファイル名。
///  @return	POLYLIB_STATで定義される値が返る。
///

POLYLIB_STAT stl_b_save(
	std::vector<PrivateTriangle*> *tri_list,
	std::string fname
	);

///
/// STLファイルを読み込みバイナリかアスキーかを判定する。
///
///  @param[in] STLファイルのフルパス名。
///  @return	true:アスキー形式 / false:バイナリ形式。
///
bool is_stl_a(
	std::string		path
	);

///
/// STLファイル名から名称(拡張子を除いた部分)を取得する。
///
///  @param[in] STLファイルのフルパス名。
///  @return	拡張子を除いた名称。
///  @attention	戻り値のchar *は解放不要。
///
char *stl_get_fname(
	std::string		path
	);

///
/// STLファイル名から拡張子のみを取得する。
///
///  @param[in] STLファイルのフルパス名。
///  @return	拡張子。
///  @attention	戻り値のchar *は解放不要。
///
char *stl_get_ext(
	std::string		path
	);



} //namespace PolylibNS

#endif  // stl_h
