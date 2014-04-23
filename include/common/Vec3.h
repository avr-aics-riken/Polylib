#ifndef _VEC3_H_
#define _VEC3_H_

//##################################################################################
//
// vec3 class
//
// Copyright (c) 2007-2011 VCAD System Research Program, RIKEN.
// All rights reserved.
//
// Copyright (c) 2011-2014 Institute of Industrial Science, The University of Tokyo.
// All rights reserved.
//
// Copyright (c) 2012-2014 Advanced Institute for Computational Science, RIKEN.
// All rights reserved.
//
//##################################################################################

/**
 * @file   Vec3.h
 * @brief  version 1.1 2014-04-23
 * @author aics
 */

//##################################################################################
//
// ATTENTION : If you want modify this header, please consult with organizer.
//             Since this header class is being used for several libraries,
//             we need to take care fo consistency between them.
//
//##################################################################################

#include <iostream>
#include <math.h>
//#include <stdexcept>

namespace Vec3class {
  
// precision
#ifdef _REAL_IS_DOUBLE_
#define REAL_TYPE double
#else
  /** 実数型の指定
   * - デフォルトでは、REAL_TYPE=float
   * - コンパイル時オプション-D_REAL_IS_DOUBLE_を付与することで
   *   REAL_TYPE=doubleになる
   */
#define REAL_TYPE float
#endif
  
typedef enum {
	AXIS_X = 0,
	AXIS_Y,
	AXIS_Z,
	AXIS_ERROR
} AxisEnum;

//=========================================================================
// class Vec3<T>
//=========================================================================

template<typename T>
class Vec3 {
public:
  
  T x, y, z;
  
	Vec3(T v = 0)			      { x = y = z = v; }
	Vec3(T _x, T _y, T _z)	{ x=_x;  y=_y;  z=_z; }
	Vec3(const T v[3])		  { x = v[0];  y = v[1];  z = v[2]; }
  Vec3(const Vec3& v) : x(v.x), y(v.y), z(v.z) {}

	Vec3<T>& assign(T _x, T _y, T _z) { 
		x=_x; y=_y; z=_z; 
		return *this; 
	}

	operator       T*()       { return &x; }
	operator const T*() const { return &x; }
	T* ptr()       { return &x; }
	const T* ptr() const { return &x; }



	T& operator [](const AxisEnum& axis) {
    return (&x)[axis];
    /*
    switch (axis) {
      case 0: return x;
      case 1: return y;
      case 2: return z;
      default: throw std::out_of_range("Vec3 index must be 0, 1, or 2");
    }*/
	}
	const T& operator [](const AxisEnum& axis) const {
    return (&x)[axis];
    /*
    switch (axis) {
      case 0: return x;
      case 1: return y;
      case 2: return z;
      default: throw std::out_of_range("Vec3 index must be 0, 1, or 2");
    }*/
	}


	Vec3<T>& operator+=(const Vec3<T>& v) {
		x += v.x; y += v.y; z += v.z; 
		return *this;
	}

	Vec3<T>& operator-=(const Vec3<T>& v) {
		x -= v.x; y -= v.y; z -= v.z; 
		return *this;
	}

	Vec3<T>& operator*=(const Vec3<T>& v) {
		x *= v.x; y *= v.y; z *= v.z; 
		return *this;
	}

	Vec3<T>& operator/=(const Vec3<T>& v) {
		x /= v.x; y/= v.y; z /= v.z; 
		return *this;
	}

	Vec3<T>& operator*=(T s) {
		x *= s; y *= s; z *= s; 
		return *this;
	}

	Vec3<T>& operator/=(T s) {
		T inv = 1./s;
		x *= inv; y *= inv; z *= inv; 
		return *this;
	}

	Vec3<T> operator+(const Vec3<T>& v) const {
		return Vec3<T>(x + v.x, y + v.y, z + v.z);
	}

	Vec3<T> operator-(const Vec3<T>& v) const {
		return Vec3<T>(x - v.x, y - v.y, z - v.z);
	}

	Vec3<T> operator*(const Vec3<T>& v) const {
		return Vec3<T>(x * v.x, y * v.y, z * v.z);
	}

	Vec3<T> operator/(const Vec3<T>& v) const {
		return Vec3<T>(x / v.x, y / v.y, z / v.z);
	}

	Vec3<T> operator*(T s) const {
		return Vec3<T>(x * s, y * s, z * s);
	}

	Vec3<T> operator/(T s) const {
		T inv = 1./s;
		return Vec3<T>(x * inv, y * inv, z * inv);
	}

	Vec3<T> operator-() const {
		return Vec3<T>(-x, -y, -z);
	}

	bool operator==(const Vec3<T>& v) const {
		return x == v.x && y == v.y && z == v.z;
	}

	bool operator!=(const Vec3<T>& v) const {
		return !(*this == v);
	}

	static Vec3<T> xaxis() { return Vec3<T>(1, 0, 0); }
	static Vec3<T> yaxis() { return Vec3<T>(0, 1, 0); }
	static Vec3<T> zaxis() { return Vec3<T>(0, 0, 1); }

	T lengthSquared() const { 
		return x*x + y*y + z*z;
	}

	T length() const { return sqrt(lengthSquared()); }

	Vec3<T>& normalize() {
		T len = length();
		if (len != 0)
			return *this /= len;
		else
			return *this;
	}

	Vec3<T>& normalize(T* len) {
		*len = length();
		if (*len != 0)
			return *this /= *len;
		else
			return *this;
	}

	T average() const { return (x + y + z)/3.f; }
};

//=========================================================================
// typedef
//=========================================================================

typedef Vec3<unsigned char> Vec3uc;
typedef Vec3<int>           Vec3i;
typedef Vec3<float>         Vec3f;
typedef Vec3<double>        Vec3d;
typedef Vec3<REAL_TYPE>     Vec3r;

//=========================================================================
// inline
//=========================================================================  

template <typename T>
inline Vec3<T> operator*(T s, const Vec3<T>& v) {
	return Vec3<T>(s*v.x, s*v.y, s*v.z);
}
  
template <typename T>
inline Vec3<T> multi(const Vec3<T>& a, const Vec3<T>& b) {
	return a * b;
}
  
template <typename T>
inline T dot(const Vec3<T>& a, const Vec3<T>& b) {
	return a.x * b.x + a.y * b.y + a.z * b.z;
}
  
template <typename T>
inline Vec3<T> cross(const Vec3<T>& a, const Vec3<T>& b) {
	return Vec3<T>(a.y * b.z - a.z * b.y,
		a.z * b.x - a.x * b.z,
		a.x * b.y - a.y * b.x);
}
  
template <typename T>
inline T distanceSquared(const Vec3<T>& a, const Vec3<T>& b) {
	return (a - b).lengthSquared();
}
  
template <typename T>
inline T distance(const Vec3<T>& a, const Vec3<T>& b) {
	return (a - b).length();
}

// @brief compare length between a and b, if a<b return true
inline bool lessVec3f(const Vec3f& a, const Vec3f& b) 
{
	return (a.lengthSquared() < b.lengthSquared()) ? true : false;
}

//=============================


template<typename T>
inline std::istream& operator>>(std::istream& is, Vec3<T>& v) 
{
	return is >> v[0] >> v[1] >> v[2];
}

template<typename T>
inline std::ostream& operator<<(std::ostream& os, const Vec3<T>& v) 
{
	return os << v[0] << " " << v[1] << " " << v[2];
}



inline std::istream& operator>>(std::istream& is, Vec3uc& v) 
{
	int t[3];
	is >> t[0] >> t[1] >> t[2];
	v[0]=t[0]; v[1]=t[1]; v[2]=t[2];
	return is;
}

inline std::ostream& operator<<(std::ostream& os, const Vec3uc& v) 
{
	int t[3];
	t[0]=v[0]; t[1]=v[1]; t[2]=v[2];
	os << t[0] << " " << t[1] << " " << t[2];
	return os;
}

} // namespace Vec3

#endif  // _VEC3_H_
