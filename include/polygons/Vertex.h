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

#ifndef polylib_vertex_h
#define polylib_vertex_h

#include "common/PolylibDefine.h"
#include "common/Vec3.h"

using namespace Vec3class;

namespace PolylibNS{

////////////////////////////////////////////////////////////////////////////
///
/// クラス:vertex
///   polygon の頂点クラス。
///
///
////////////////////////////////////////////////////////////////////////////

class Vertex :public Vec3<PL_REAL>{

public:

	/// コンストラクタ
	///
	///
	Vertex();

	/// コンストラクタ
	///
	/// @param[in] vec 頂点ベクトル

	Vertex(const Vec3<PL_REAL>& vec);


	/// コンストラクタ
	///
	/// @param[in] x 座標
	/// @param[in] y 座標
	/// @param[in] z 座標

	Vertex(PL_REAL x,PL_REAL y,PL_REAL z);

	///  index アクセス
	///

	virtual  PL_REAL& operator [](const AxisEnum& axis);
	///
	///  index アクセス
	///

	virtual   const PL_REAL& operator [](const AxisEnum& axis) const;


	// virtual   PL_REAL x() const{
	//     return this->t[AXIS_X];
	//   }

	///
	///  2乗距離
	///

	virtual  PL_REAL distanceSquared(Vertex v);

	///距離

	virtual   PL_REAL distance(Vertex v);


};


}

#endif // polylib_vertex_h
