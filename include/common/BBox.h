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

using namespace Vec3class;

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

	BBox(PL_REAL _minx, PL_REAL _miny, PL_REAL _minz, PL_REAL _maxx, PL_REAL _maxy, PL_REAL _maxz) ;

	BBox(PL_REAL _min[3], PL_REAL _max[3]);

	BBox(const Vec3<PL_REAL>& _min, const Vec3<PL_REAL>& _max);

	void init();

	void setMinMax(const Vec3<PL_REAL>& _min, const Vec3<PL_REAL>& _max);

	void add(const Vec3<PL_REAL>& v) ;

	Vec3<PL_REAL> getPoint(int idx) const ;

	Vec3<PL_REAL> center() const;

	Vec3<PL_REAL> size() const;

	PL_REAL xsize() const ;
	PL_REAL ysize() const;
	PL_REAL zsize() const;
	PL_REAL length(const AxisEnum& axis) const;
	PL_REAL diameter() const ;
	AxisEnum getMaxAxis(PL_REAL& length) const ;

	///
	/// 引数で与えられた点が、このBBoxに含まれるかを判定する。
	/// @param[in] pos 試行する点
	/// @return 含まれる場合はtrue。他はfalse。
	///
	bool contain(const Vec3<PL_REAL>& pos) const;

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
	Vec2<PL_REAL> vec3to2(int axis_id, Vec3<PL_REAL>& v3) const;
	///
	/// 引数axis_id(0=x,1=y,z=2)に垂直な、このBBoxの面の対角点を返す。
	/// @param[in] axis_id 軸番号。0=x軸、1=y軸、2=z軸。
	/// @param[in] face BBoxの面の中で、軸に垂直な面の対角点。
	///
	// get 2 faces perpendicular to the axis_id
	void getFace(int axis_id, Vec3<PL_REAL> face[2][2]) const;
	///
	/// 引数axis_id(0=x,1=y,z=2)に平行な、このBBoxの辺の端点を返す。
	/// @param[in] axis_id 軸番号。0=x軸、1=y軸、2=z軸。
	/// @param[in] side BBoxの辺の中で、軸に平行な辺の端点。
	///
	void getSide(int axis_id, Vec3<PL_REAL> side[4][2]) const;

	Vec3<PL_REAL> min;
	Vec3<PL_REAL> max;

};

} // namespace PolylibNS

#endif  // bbox_h

