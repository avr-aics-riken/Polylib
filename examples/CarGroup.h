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



class CarGroup:public PolygonGroup{

 public:
  static string get_class_name() ;
  virtual string whoami();

 protected:
  double m_velocity;
  POLYLIB_STAT move(PolylibMoveParams& params);
  POLYLIB_STAT build_group_tree(Polylib* polylib,PolygonGroup* parent,TextParser* tp);


  POLYLIB_STAT mk_param_tag(
				      TextParser* tp,
				      string rank_no,
				      string extend,
				      string format);

};

#endif //CARGROUP_H
