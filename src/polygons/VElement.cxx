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

#include "polygons/VElement.h"
#include "polygons/PrivateTriangle.h"

//#define DEBUG_VTREE
namespace PolylibNS {


////////////////////////////////////////////////////////////////////////////
///
/// クラス:VElement
/// KD木構造の要素クラスです。
///
////////////////////////////////////////////////////////////////////////////
///
/// Bounding box of this triangle
///
BBox VElement::get_bbox() const {
	return m_bbox;
}

VElement::VElement()
{
}
///
/// デストラクタ。
///
VElement::~VElement()
{
}

// VElement 
// public /////////////////////////////////////////////////////////////////////

VElement::VElement(
	PrivateTriangle* tri
	) {
		m_tri = tri;
		Vertex** tmp=tri->get_vertex();
		for(int i=0; i<3; i++){
			m_bbox.add( (Vec3<PL_REAL>) (*(tmp[i])) );
		}
		m_pos = m_bbox.center();
}

//=======================================================================
// Setter/Getter
//=======================================================================
///
/// triangle。
///
PrivateTriangle* VElement::get_triangle() 
{
	return m_tri;
}

///
/// Center position of bbox on triangle.
/// 
Vec3<PL_REAL> VElement::get_pos() const {
	return m_pos;
}



} //namespace PolylibNS


