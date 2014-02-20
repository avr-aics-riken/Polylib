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

#include "groups/PolygonGroupFactory.h"
#include "groups/PolygonGroup.h"

namespace PolylibNS {

///
/// コンストラクタ。
///
PolygonGroupFactory::PolygonGroupFactory() 
{
}

///
/// デストラクタ。
///
PolygonGroupFactory::~PolygonGroupFactory() 
{
}

///
/// インスタンス作成。
///
///  @param[in]	class_name	作成するクラス名。
///  @return	作成に失敗した場合はNULLが返る。
///
//	virtual PolygonGroup* create_instance(std::string class_name) {
PolygonGroup* PolygonGroupFactory::create_instance(std::string class_name,REAL_TYPE tolerance) 
{

#ifdef DEBUG
	PL_DBGOSH << "PolygonGroup<T>::create_instance class_name " 
		<<class_name<<std::endl;
#endif
	if (class_name == PolygonGroup::get_class_name()) {
		//		  return new PolygonGroup;
		return new PolygonGroup(tolerance);
	}
	else {
		return NULL;
	}

}

} //namespace PolylibNS

