// -*- Mode:c++ -*-
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

#ifndef polylib_vertex_h
#define polylib_vertex_h

#include "common/Vec3.h"

namespace PolylibNS{

////////////////////////////////////////////////////////////////////////////
///
/// クラス:vertex
///   polygon の頂点クラス。
/// 
///
////////////////////////////////////////////////////////////////////////////
template <typename T>
class Vertex :public Vec3<T>{

public:

  /// コンストラクタ  
  ///
  ///
 Vertex(){};

  /// コンストラクタ  
  ///
  /// @param[in] vec 頂点ベクトル

  Vertex(const Vec3<T>& vec){
    this->t[0]=vec.t[0];
    this->t[1]=vec.t[1];
    this->t[2]=vec.t[2];
  };


  /// コンストラクタ  
  ///
  /// @param[in] x 座標
  /// @param[in] y 座標
  /// @param[in] z 座標

 Vertex(T x,T y,T z){
    this->t[0]=x;
    this->t[1]=y;
    this->t[2]=z;
    
  };

  ///  index アクセス
  ///

virtual  T& operator [](const AxisEnum& axis) { 
    return this->t[axis];
  }
  ///
  ///  index アクセス
  ///

virtual   const T& operator [](const AxisEnum& axis) const {
    return this->t[axis];
  }


// virtual   T x() const{
//     return this->t[AXIS_X];
//   }

  ///
  ///  2乗距離
  ///

virtual  T distanceSquared(Vertex v){

    Vec3<T> vdis(this->t[0]-v.t[0],this->t[1]-v.t[1],this->t[2]-v.t[2]);

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

virtual   T distance(Vertex v){
    return sqrt(distanceSquared(v));
  }



 };


}

#endif // polylib_vertex_h
