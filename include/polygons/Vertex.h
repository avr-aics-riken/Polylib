// -*- Mode:c++ -*-
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

#ifndef polylib_vertex_h
#define polylib_vertex_h

#include "common/PolylibDefine.h"
#include "common/Vec3.h"

namespace PolylibNS{

////////////////////////////////////////////////////////////////////////////
///
/// クラス:vertex
///   polygon の頂点クラス。
/// 
///
////////////////////////////////////////////////////////////////////////////

class Vertex :public Vec3<REAL_TYPE>{

public:

	/// コンストラクタ  
	///
	///
	Vertex();

	/// コンストラクタ  
	///
	/// @param[in] vec 頂点ベクトル

	Vertex(const Vec3<REAL_TYPE>& vec);


	/// コンストラクタ  
	///
	/// @param[in] x 座標
	/// @param[in] y 座標
	/// @param[in] z 座標

	Vertex(REAL_TYPE x,REAL_TYPE y,REAL_TYPE z);

	///  index アクセス
	///

	virtual  REAL_TYPE& operator [](const AxisEnum& axis);
	///
	///  index アクセス
	///

	virtual   const REAL_TYPE& operator [](const AxisEnum& axis) const;


	// virtual   REAL_TYPE x() const{
	//     return this->t[AXIS_X];
	//   }

	///
	///  2乗距離
	///

	virtual  REAL_TYPE distanceSquared(Vertex v);

	///距離

	virtual   REAL_TYPE distance(Vertex v);


};


}

#endif // polylib_vertex_h
