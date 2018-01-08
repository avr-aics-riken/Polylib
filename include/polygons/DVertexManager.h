/*
###################################################################################
#
# Polylib - Polygon Management Library
#
# Copyright (c) 2010-2011 VCAD System Research Program, RIKEN.
# All rights reserved.
#
# Copyright (c) 2012-2015 Advanced Institute for Computational Science (AICS), RIKEN.
# All rights reserved.
#
# Copyright (c) 2016-2018 Research Institute for Information Technology (RIIT), Kyushu University.
# All rights reserved.
#
###################################################################################
*/

#ifndef polylib_dvertex_manager_h
#define polylib_dvertex_manager_h

#include "common/Vec3.h"

using namespace Vec3class;

namespace PolylibNS{

////////////////////////////////////////////////////////////////////////////
///
/// クラス:DVertexManager
///   DVertex  のユーザー定義データ形式を保持します。
///
///
////////////////////////////////////////////////////////////////////////////
class DVertexManager {

private:
	int m_nscalar;
	int m_nvector;
	//    int m_size;
public:
	//    DVertexManager():m_nscalar(0),m_nvector(0),m_size(8){};
	DVertexManager():m_nscalar(0),m_nvector(0){};
	// DVertexManager(int nscalar,int nvector,int size){
	//   m_nscalar=nscalar;
	//   m_nvector=nvector;
	//   m_size=size;
	// }
	DVertexManager(int nscalar,int nvector){
		m_nscalar=nscalar;
		m_nvector=nvector;
	}
	int nscalar(){return m_nscalar;}
	int nvector(){return m_nvector;}
	//int size(){return m_size;}

};

}

#endif // polylib_dvertex_manager_h
