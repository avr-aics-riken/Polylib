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
#include <string>
#include <iostream>
#include "Polylib.h"

using namespace std;
using namespace PolylibNS;

#define PL_REAL double
//#define PL_REAL float
int main(){

  cout<< "test 0" << endl;

  //Polylib<double>* pl_instance = Polylib<double>::get_instance();
  Polylib<PL_REAL>* pl_instance = Polylib<PL_REAL>::get_instance();

  cout<< "test 1" << endl;

  //  pl_instance->load();

  PL_REAL scale=1.0;
  std::string file="polylib_config_objfile.tp";

  pl_instance->load(file,scale);

  cout<< "test 2" << endl;

  pl_instance->show_group_hierarchy();
 
  cout<< "test 3" << endl;


  // //  pl_instance->show_group_info("car"); // not working??
  string fname="";
  string obj="obj_bb";
  string extend="";

  pl_instance->save(&fname,obj,extend);

  cout<< "test 4" << endl;

  return 0;


}
