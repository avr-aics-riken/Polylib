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
#include "polygons/Vertex.h"



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



/// コンストラクタ  
///
///
Vertex::Vertex(){}

/// コンストラクタ  
///
/// @param[in] vec 頂点ベクトル

Vertex::Vertex(const Vec3<REAL_TYPE>& vec){
	this->t[0]=vec.t[0];
	this->t[1]=vec.t[1];
	this->t[2]=vec.t[2];
}


/// コンストラクタ  
///
/// @param[in] x 座標
/// @param[in] y 座標
/// @param[in] z 座標

Vertex::Vertex(REAL_TYPE x,REAL_TYPE y,REAL_TYPE z){
	this->t[0]=x;
	this->t[1]=y;
	this->t[2]=z;

}

///  index アクセス
///

REAL_TYPE& Vertex::operator [](const AxisEnum& axis) { 
	return this->t[axis];
}
///
///  index アクセス
///

const REAL_TYPE& Vertex::operator [](const AxisEnum& axis) const {
	return this->t[axis];
}


// virtual   REAL_TYPE x() const{
//     return this->t[AXIS_X];
//   }

///
///  2乗距離
///

REAL_TYPE Vertex::distanceSquared(Vertex v)
{
	Vec3<REAL_TYPE> vdis(this->t[0]-v.t[0],this->t[1]-v.t[1],this->t[2]-v.t[2]);

	#ifdef DEBUG
	std::cout << "a("<<this->t[0]<<","
		  <<this->t[1]<<","
		  <<this->t[2]<<")"
		  << " b("<<v.t[0]<<","
		  <<v.t[1]<<","
		  <<v.t[2]<<")"
		  << " c("<<vdis.t[0]<<","
		  <<vdis.t[1]<<","
		  <<vdis.t[2]<<")"
		 <<std::endl;
	#endif // DEBUG


	return vdis.lengthSquared();
}

///距離

REAL_TYPE Vertex::distance(Vertex v){
	return sqrt(distanceSquared(v));
}


}


