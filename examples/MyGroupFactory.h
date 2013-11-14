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
template<typename T>
class MyGroupFactory:public PolygonGroupFactory<T>{
 public:
 PolygonGroup<T>* create_instance(std::string class_name){
    if(class_name == CarGroup<T>::get_class_name()){
      //  cout << "CarGroup() called!" <<endl;
      return new CarGroup<T>();
    }

    //  if(class_name == BladeGroup::get_class_name()){
    //    return new BladeGroup();
    //  }

    //default
    return PolygonGroupFactory<T>::create_instance(class_name);

  }
};
#endif // MYGROUPFACTORY_H
