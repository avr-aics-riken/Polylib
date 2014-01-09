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

#include "MyGroupFactory.h"
 


PolygonGroup* MyGroupFactory::create_instance(std::string class_name)
{
    if(class_name == CarGroup::get_class_name()){
      //  cout << "CarGroup() called!" <<endl;
      return new CarGroup();
    }

    //  if(class_name == BladeGroup::get_class_name()){
    //    return new BladeGroup();
    //  }

    //default
	//MOD S fuchi  error C2660: 'PolylibNS::PolygonGroupFactory::create_instance' : 関数に 1 個の引数を指定できません。
    //return PolygonGroupFactory::create_instance(class_name);
	REAL_TYPE tolerance = 0;
	return PolygonGroupFactory::create_instance(class_name,tolerance);
	//MOD E 

}
