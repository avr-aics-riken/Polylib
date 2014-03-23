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

using namespace Vec3class;

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
	this->x=vec.x;
	this->y=vec.y;
	this->z=vec.z;
}


/// コンストラクタ  
///
/// @param[in] x 座標
/// @param[in] y 座標
/// @param[in] z 座標

Vertex::Vertex(PL_REAL x,PL_REAL y,PL_REAL z){
	this->x=x;
	this->y=y;
	this->z=z;

}

///  index アクセス
///

PL_REAL& Vertex::operator [](const AxisEnum& axis) { 
	//return this->t[axis]; // keno 2014-03-23
  return (&x)[axis];
}

///
///  index アクセス
///

const PL_REAL& Vertex::operator [](const AxisEnum& axis) const {
	//return this->t[axis]; // keno 2014-03-23
  return (&x)[axis];
}


// virtual   PL_REAL x() const{
//     return this->t[AXIS_X];
//   }

///
///  2乗距離
///

PL_REAL Vertex::distanceSquared(Vertex v)
{
	Vec3<PL_REAL> vdis(this->x-v.x,this->y-v.y,this->z-v.z);

#ifdef DEBUG
	std::cout << "a("<<this->x<<","
		<<this->y<<","
		<<this->z<<")"
		<< " b("<<v.x<<","
		<<v.y<<","
		<<v.z<<")"
		<< " c("<<vdis.x<<","
		<<vdis.y<<","
		<<vdis.z<<")"
		<<std::endl;
#endif // DEBUG


	return vdis.lengthSquared();
}

///距離

PL_REAL Vertex::distance(Vertex v){
	return sqrt(distanceSquared(v));
}


}


