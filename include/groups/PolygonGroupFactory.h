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

#ifndef polylib_polygongroupfactory_h
#define polylib_polygongroupfactory_h

#include <vector>

namespace PolylibNS {
template <typename T> class PolygonGroup;

////////////////////////////////////////////////////////////////////////////
///
/// クラス:PolygonGroupFactory
///  
////////////////////////////////////////////////////////////////////////////
template <typename T> 
class PolygonGroupFactory {
public:
	///
 	/// コンストラクタ。
	///
	PolygonGroupFactory() {};

	///
 	/// デストラクタ。
	///
	virtual ~PolygonGroupFactory() {};

	///
	/// インスタンス作成。
	///
	///  @param[in]	class_name	作成するクラス名。
	///  @return	作成に失敗した場合はNULLが返る。
	///
	//	virtual PolygonGroup<T>* create_instance(std::string class_name) {
	virtual PolygonGroup<T>* create_instance(std::string class_name,T tolerance) {
		if (class_name == PolygonGroup<T>::get_class_name()) {
		  //		  return new PolygonGroup<T>;
		  		  return new PolygonGroup<T>(tolerance);
		}
		else {
			return NULL;
		}
	};
};

} //namespace PolylibNS

#endif //polylib_polygongroup_h
