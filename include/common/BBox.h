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

#ifndef bbox_h
#define bbox_h

#include <algorithm>
#include <list>
#include "PolylibCommon.h"
#include "common/Vec2.h"
#include "common/Vec3.h"
#include "common/axis.h"

namespace PolylibNS {

////////////////////////////////////////////////////////////////////////////
///
/// クラス:BBox
/// Bounding Boxを管理するクラス
///
////////////////////////////////////////////////////////////////////////////

class BBox {
public:
	BBox();

	BBox(REAL_TYPE _minx, REAL_TYPE _miny, REAL_TYPE _minz, REAL_TYPE _maxx, REAL_TYPE _maxy, REAL_TYPE _maxz) ;

	BBox(REAL_TYPE _min[3], REAL_TYPE _max[3]);

	BBox(const Vec3<REAL_TYPE>& _min, const Vec3<REAL_TYPE>& _max);

	void init();

	void setMinMax(const Vec3<REAL_TYPE>& _min, const Vec3<REAL_TYPE>& _max);

	void add(const Vec3<REAL_TYPE>& v) ;

	Vec3<REAL_TYPE> getPoint(int idx) const ;

	Vec3<REAL_TYPE> center() const;

	Vec3<REAL_TYPE> size() const;

	REAL_TYPE xsize() const ;
	REAL_TYPE ysize() const;
	REAL_TYPE zsize() const;
	REAL_TYPE length(const AxisEnum& axis) const;
	REAL_TYPE diameter() const ;
	AxisEnum getMaxAxis(REAL_TYPE& length) const ;

	///
	/// 引数で与えられた点が、このBBoxに含まれるかを判定する。
	/// @param[in] pos 試行する点
	/// @return 含まれる場合はtrue。他はfalse。
	///
	bool contain(const Vec3<REAL_TYPE>& pos) const;

	///
	/// BBoxとBBoxの交差判定を行う。
	/// KD-Treeの交差判定と同じ。
	/// @param[in] bbox 試行するBBox
	/// @return 交差する場合はtrue。他はfalse。
	///
	bool crossed(const BBox& bbox) const ;

	///
	/// BBoxとBBoxの重複領域の抽出を行う。
	/// 自身の面と他方の辺との交差判定を行う。
	/// @param[in] other_bbox 試行するBBox
	/// @return 交差する場合はtrue。他はfalse。
	///
	BBox getCrossedRegion(BBox& other_bbox) const;
	///
	/// 引数axis_id(0=x,1=y,z=2)に垂直な成分を詰めて返す。
	///
	Vec2<REAL_TYPE> vec3to2(int axis_id, Vec3<REAL_TYPE>& v3) const;
	///
	/// 引数axis_id(0=x,1=y,z=2)に垂直な、このBBoxの面の対角点を返す。
	/// @param[in] axis_id 軸番号。0=x軸、1=y軸、2=z軸。
	/// @param[in] face BBoxの面の中で、軸に垂直な面の対角点。
	///
	// get 2 faces perpendicular to the axis_id
	void getFace(int axis_id, Vec3<REAL_TYPE> face[2][2]) const;
	///
	/// 引数axis_id(0=x,1=y,z=2)に平行な、このBBoxの辺の端点を返す。
	/// @param[in] axis_id 軸番号。0=x軸、1=y軸、2=z軸。
	/// @param[in] side BBoxの辺の中で、軸に平行な辺の端点。
	///
	void getSide(int axis_id, Vec3<REAL_TYPE> side[4][2]) const;

	Vec3<REAL_TYPE> min;
	Vec3<REAL_TYPE> max;

};

} // namespace PolylibNS

#endif  // bbox_h

