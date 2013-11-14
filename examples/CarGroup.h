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

#ifndef CARGROUP_H
#define CARGROUP_H

#include "common/BBox.h"
#include "polygons/VTree.h"
#include "polygons/Polygons.h"
#include "groups/PolygonGroup.h"
#include <vector>

#include "Polylib.h"
using namespace std;
using namespace PolylibNS;

//#define PL_REAL double
//#define PL_REAL float
template <typename T>
class CarGroup:public PolygonGroup<T>{

 public:
  static string get_class_name() {return "CarGroup";}
  virtual string whoami(){return get_class_name();}

 protected:
  double m_velocity;
  POLYLIB_STAT move(PolylibMoveParams& params);
  POLYLIB_STAT build_group_tree(Polylib<T>* polylib,PolygonGroup<T>* parent,TextParser* tp);


  POLYLIB_STAT mk_param_tag(
				      TextParser* tp,
				      string rank_no,
				      string extend,
				      string format);

};

using namespace std;
template <typename T>
POLYLIB_STAT CarGroup<T>::build_group_tree(Polylib<T>* polylib,
					PolygonGroup<T>* parent,
					TextParser* tp)
{
  TextParserError status = TP_NO_ERROR;
  m_velocity = 0;
  vector<string> leaves;
  tp->getLabels(leaves);
  vector<string>::iterator leaf_iter=find(leaves.begin(),leaves.end(),"velocity");

  if(leaf_iter!=leaves.end()){
    string value;
    status=tp->getValue(*leaf_iter,value);
    if(status!=TP_NO_ERROR){
      tp->TextParserErrorHandler(status," can not read velocity.");
      return PLSTAT_CONFIG_ERROR;
    }else{
      int error;
      m_velocity = tp->convertDouble(value,&error);
    }
  }

  cout << whoami() <<" "<<__FUNCTION__<< " "<<  m_velocity << endl;

  POLYLIB_STAT stat = PolygonGroup<T>::build_group_tree(polylib,parent,tp);

  return stat;
}

template <typename T>
POLYLIB_STAT CarGroup<T>::move(PolylibMoveParams& params){

  //   write code here
  
  
  //init_check_leaped();

  //VertexList<T> * m_polygons->m_get_vtx_list


  return PLSTAT_OK;
}

template <typename T>
POLYLIB_STAT CarGroup<T>::mk_param_tag(
				    TextParser* tp,
				    string rank_no,
				    string extend,
				    string format
) {

  cout << "CarGroup::"<<__FUNCTION__ << endl;
  stringstream ss;
  POLYLIB_STAT	stat;
  //goto root
  tp->changeNode("/Polylib");
  tp->changeNode(this->acq_fullpath());
  //this is test. change m_velocity forcely
  m_velocity = -500.;
  ss<<m_velocity;
  string value;
  ss>>value;
  tp->updateValue("velocity",value);

  return PolygonGroup<T>::mk_param_tag(tp,rank_no,extend,format);
}



#endif //CARGROUP_H
