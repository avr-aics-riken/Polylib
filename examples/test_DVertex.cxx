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

#include <string>
#include <typeinfo>
#include <iostream>
#include "Polylib.h"

using namespace std;
using namespace PolylibNS;

int main(){

  cout<< "test 0" << endl;

  Polylib* pl_instance = Polylib::get_instance();

  cout<< "test 1" << endl;

  //  pl_instance->load();

  PL_REAL scale=1.0;
  //  std::string file="polylib_config_vtxtest.tp";
  //

  //    std::string file="polylib_config.tp";

  //    pl_instance->load(file,scale);

  cout<< "test 2" << endl;

  std::string pgname="DV/byhand";
  pl_instance->make_DVertex_PolygonGroup(pgname,4,3);

  pgname="DV/layer/byhand2";
  pl_instance->make_DVertex_PolygonGroup(pgname,4,3);

  pgname="sphere";
  pl_instance->make_DVertex_PolygonGroup(pgname,4,3);


  pl_instance->show_group_hierarchy();
  pgname="DV/byhand";
  Vec3<PL_REAL> vlist[3];

  for (int itri=0;itri<2;itri++){
    if(itri==0){
    vlist[0].assign(0.,0.,0.);
    vlist[1].assign(0.,1.,0.);
    vlist[2].assign(0.,0.,1.);
    } else {

    vlist[0].assign(1.,1.,0.);
    vlist[1].assign(0.,1.,0.);
    vlist[2].assign(0.,0.,1.);

    }


  DVertexTriangle* tri=
    pl_instance->add_DVertex_Triangle(pgname,vlist);

  DVertex** vec_list= tri->get_DVertex();
  std::cout << "back from get_DVertex " <<vec_list <<std::endl;
    DVertex* DVp=NULL;
    for(int ii=0;ii<3;++ii){
    DVp=vec_list[ii];

  DVp->set_scalar(0,0.0+ii);

  DVp->set_scalar(1,1.0+ii);
  DVp->set_scalar(2,2.0+ii);
  DVp->set_scalar(3,3.0+ii);
  std::cout <<pgname<<std::endl;
  std::cout<<  "0 "
	   << DVp->get_scalar(0)<<" "
	   <<  "1 "
	   << DVp->get_scalar(1)<<" "
	   <<  "2 "
	   << DVp->get_scalar(2)<<" "
	   <<  "3 "
	   << DVp->get_scalar(3)<<std::endl;

  std::cout << "set_vector" <<std::endl;

  Vec3 a(10,20+ii,30+ii);
  Vec3 b(13,23+ii,33+ii);

  DVp->set_vector(0,a);
  DVp->set_vector(2,b);

  Vec3 c,d;
  DVp->get_vector(0,&c);

  DVp->get_vector(2,&d);

  std::cout<<  "vector 0 (10,20,30) "<< c << std::endl;
  std::cout<<  "vector 2 (13,23,33) "<< d << std::endl;
    }

  }


pl_instance->finalize_DVertex(pgname);

  cout<< "test 3" << endl;


  string fname="";
  string obj="vtk_a";
  string extend="";


  pl_instance->save(&fname,obj,extend);



  return 0;


}
