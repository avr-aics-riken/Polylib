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

#include "Polylib.h"

using namespace std;
using namespace PolylibNS;

#define PL_REAL float
//#define PL_REAL double
int main(){

  Polylib<PL_REAL>* pl_instance = Polylib<PL_REAL>::get_instance();

  //  pl_instance->load();
  pl_instance->load("polylib_config-multi.tp");
  pl_instance->show_group_hierarchy();
  //  pl_instance->show_group_info("car"); // not working??
  string fname="";
  string stl="stl_a";
  string extend="";
  pl_instance->save(&fname,stl,extend);
  
  return 0;


}
