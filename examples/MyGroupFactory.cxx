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

#include "MyGroupFactory.h"



PolygonGroup* MyGroupFactory::create_instance(std::string class_name,PL_REAL tolerance)
{
#ifdef DEBUG
	PL_DBGOSH<< "in MyGroupFactory::create_instance called. "<<class_name<<std::endl;
	PL_DBGOSH<< "in MyGroupFactory::create_instance called. "
		<<CarGroup::get_class_name<<std::endl;
#endif
	if(class_name == CarGroup::get_class_name()){
#ifdef DEBUG
		PL_DBGOSH<< "in MyGroupFactory::create_instance create CarGroup."<<std::endl;
#endif
		return new CarGroup(tolerance);
	}

	//default
#ifdef DEBUG
	PL_DBGOSH<< "in MyGroupFactory::create_instance create OtherGroup."<<std::endl;
#endif //DEBUG
	return PolygonGroupFactory::create_instance(class_name,tolerance);
}
