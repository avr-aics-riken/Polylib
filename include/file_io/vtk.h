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

#ifndef polylib_vtk_h
#define polylib_vtk_h

#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <iomanip>

#include "common/PolylibCommon.h"
#include "polygons/Triangle.h"
#include "polygons/Vertex.h"
#include "polygons/DVertexManager.h"
#include "polygons/DVertex.h"
#include "polygons/VertexList.h"

#include "file_io/stl.h"

#if HAVE_STRING_H 
#include <string.h>
#else
// write some code here
#endif

namespace PolylibNS {

/// 
/// VertexList, tri_listにから vtkアスキー形式に出力する。
/// 
///
///  @param[in] vertex_list 頂点リストの領域。
///  @param[in] tri_list　三角形ポリゴンリストの領域。
///  @param[in]	fname	　ファイル名。
///
///  REAL_TYPE は、頂点データの実数型



POLYLIB_STAT vtk_a_save(
	VertexList* vertex_list, 
	std::vector<PrivateTriangle*> *tri_list,
	std::string fname
	);

/// 
/// VertexList, tri_listにから vtkバイナリ形式に出力する。
///
///  @param[in] vertex_list 頂点リストの領域。
///  @param[in] tri_list　三角形ポリゴンリストの領域。
///  @param[in]	fname	　ファイル名。
///
///  REAL_TYPE は、頂点データの実数型
///  DT は、頂点データに付随するスカラーデータの型
///  


POLYLIB_STAT vtk_b_save(
	VertexList* vertex_list, 
	std::vector<PrivateTriangle*> *tri_list,
	std::string fname
	);

} // end of namespace

#endif //vtk_h
