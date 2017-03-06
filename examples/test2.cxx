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
# Copyright (c) 2016-2017 Research Institute for Information Technology (RIIT), Kyushu University.
# All rights reserved.
#
###################################################################################
*/

#include "Polylib.h"

using namespace std;
using namespace PolylibNS;

#ifdef WIN32
int main_test2(){
#else
int main(int argc, char** argv ){
#endif


	Polylib* pl_instance = Polylib::get_instance();

	//  pl_instance->load();
	pl_instance->load("config/polylib_config-multi.tp");
	pl_instance->show_group_hierarchy();
	//  pl_instance->show_group_info("car"); // not working??
	string fname="";
	string stl="stl_a";
	string extend="";
	pl_instance->save(&fname,stl,extend);

	return 0;


}
