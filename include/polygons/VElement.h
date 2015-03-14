/*
* Polylib - Polygon Management Library
*
* Copyright (c) 2010-2011 VCAD System Research Program, RIKEN.
* All rights reserved.
*
* Copyright (c) 2012-2015 Advanced Institute for Computational Science, RIKEN.
* All rights reserved.
*
*/

#ifndef polylib_velement_h
#define polylib_velement_h

#include "common/BBox.h"
//#include "common/PolylibStat.h"
//#include "common/PolylibCommon.h"
//#include "polygons/Vertex.h"
//#include "polygons/Triangle.h"

#include <vector>
//#define DEBUG_VTREE
namespace PolylibNS {

////////////////////////////////////////////////////////////////////////////
///
/// クラス:VElement
/// KD木構造の要素クラスです。
///
////////////////////////////////////////////////////////////////////////////


class PrivateTriangle;

class VElement {
public:
	///
	/// コンストラクタ。
	///
	/// @param[in] tri ポリゴン情報のポインタ。
	/// @attention ポインタを格納するが、参照のみ。deleteは行わない。
	///
	VElement(
		PrivateTriangle* tri
		);


	VElement();
	///
	/// デストラクタ。
	///
	virtual ~VElement();


	//=======================================================================
	// Setter/Getter
	//=======================================================================
	///
	/// triangle。
	///
	PrivateTriangle* get_triangle();

	///
	/// Center position of bbox on triangle.
	/// 
	Vec3<PL_REAL> get_pos() const;

	///
	/// Bounding box of this triangle
	///
	BBox get_bbox() const ;

private:
	//=======================================================================
	// クラス変数
	//=======================================================================
	/// triangle
	PrivateTriangle	*m_tri;

	/// Center position of bbox on triangle.
	Vec3<PL_REAL>			m_pos;

	/// Bounding box of this triangle
	BBox			m_bbox;
};

}

#endif  // polylib_velement_h

