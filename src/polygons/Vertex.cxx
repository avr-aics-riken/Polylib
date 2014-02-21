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

Vertex::Vertex(const Vec3<PL_REAL>& vec){
	this->t[0]=vec.t[0];
	this->t[1]=vec.t[1];
	this->t[2]=vec.t[2];
}


/// コンストラクタ  
///
/// @param[in] x 座標
/// @param[in] y 座標
/// @param[in] z 座標

Vertex::Vertex(PL_REAL x,PL_REAL y,PL_REAL z){
	this->t[0]=x;
	this->t[1]=y;
	this->t[2]=z;

}

///  index アクセス
///

PL_REAL& Vertex::operator [](const AxisEnum& axis) { 
	return this->t[axis];
}
///
///  index アクセス
///

const PL_REAL& Vertex::operator [](const AxisEnum& axis) const {
	return this->t[axis];
}


// virtual   PL_REAL x() const{
//     return this->t[AXIS_X];
//   }

///
///  2乗距離
///

PL_REAL Vertex::distanceSquared(Vertex v)
{
	Vec3<PL_REAL> vdis(this->t[0]-v.t[0],this->t[1]-v.t[1],this->t[2]-v.t[2]);

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

PL_REAL Vertex::distance(Vertex v){
	return sqrt(distanceSquared(v));
}


}


