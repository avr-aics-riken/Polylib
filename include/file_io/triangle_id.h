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

#ifndef triangle_id_h
#define triangle_id_h


#include <vector>
#include <string>
#include <fstream>
#include "polygons/PrivateTriangle.h"
#include "common/PolylibStat.h"
#include "common/PolylibCommon.h"

namespace PolylibNS {

//////////////////////////////////////////////////////////////////////////////
// 変更:ポリゴンIDのバイナリ入力対応 2010.10.19

POLYLIB_STAT load_id(
	std::vector<PrivateTriangle*>* tri_list,
	std::string fname,
	ID_FORMAT id_format
	);
//////////////////////////////////////////////////////////////////////////////
// 変更:ポリゴンIDのバイナリ出力対応 2010.10.19

POLYLIB_STAT save_id(
	std::vector<PrivateTriangle*>	*tri_list,
	std::string 				fname,
	ID_FORMAT			id_format
	);


} //namespace PolylibNS

#endif  // stl_h
