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

#ifndef MYGROUP_FACTORY_H
#define MYGROUP_FACTORY_H

#include <string.h>

#include "common/BBox.h"
#include "polygons/VTree.h"
#include "polygons/Polygons.h"

#include "groups/PolygonGroup.h"
#include "groups/PolygonGroupFactory.h"
#include "CarGroup.h"
#include "Polylib.h"
//#include "BladeGroup.h"
#include <string>
using namespace std;
using namespace PolylibNS;

//class PolygonGroup;
#define PL_REAL double
//#define PL_REAL float 

/// MyGroupFactory 
///
///
///


template<typename T>
class MyGroupFactory:public PolygonGroupFactory<T>{
 public:
  ///  create_instance  をオーバーライドして、
  ///
  /// @param[in] class_name
  /// @param[in] tolerance

  PolygonGroup<T>* create_instance(std::string class_name,T tolerance){
#ifdef DEBUG
   PL_DBGOSH<< "in MyGroupFactory::create_instance called. "<<class_name<<std::endl;
   PL_DBGOSH<< "in MyGroupFactory::create_instance called. "
	   <<CarGroup<T>::get_class_name<<std::endl;
#endif
    if(class_name == CarGroup<T>::get_class_name()){
#ifdef DEBUG
      PL_DBGOSH<< "in MyGroupFactory::create_instance create CarGroup."<<std::endl;
#endif
      return new CarGroup<T>(tolerance);
    }

    //default
#ifdef DEBUG
    PL_DBGOSH<< "in MyGroupFactory::create_instance create OtherGroup."<<std::endl;
#endif //DEBUG
    return PolygonGroupFactory<T>::create_instance(class_name,tolerance);

  }
};
#endif // MYGROUPFACTORY_H
