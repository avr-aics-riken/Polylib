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

#ifndef polylib_VertKDTElem_h
#define polylib_VertKDTElem_h
#include <vector>
#include "common/Vec3.h"
#include "common/BBox.h"
#include "common/PolylibCommon.h"
#include "common/PolylibStat.h"
#include "polygons/Vertex.h"
#include "polygons/VertexList.h"

using namespace Vec3class;

namespace PolylibNS{

////////////////////////////////////////////////////////////////////////////
///
///  クラス:VertKDTElem
///  VertexKD木の要素クラスです。
///
////////////////////////////////////////////////////////////////////////////


class VertKDTElem{
public:
	///
	/// コンストラクタ。
	///
	/// @param[in]
	/// @attention ポインタを格納するが、参照のみ。deleteは行わない。
	///
	VertKDTElem(Vertex* vertex);

	//setter/getter
	///
	/// return vertex.
	///
	Vertex* get_vertex();
	//setter/getter

	///
	/// return vertex.
	///
	Vec3<PL_REAL>* get_pos() const;

private:
	///
	/// vertex
	///
	Vertex* m_vertex;


};// end of class

}// end of namespace PolylibNS
#endif //polylib_VertKDTElem_h
