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

  CarGroup(){
    this->m_parent	= 0;
    this->m_polygons	= new TriMesh<T>();
    this->m_movable	= false;
    this->m_need_rebuild = false;
    this->m_trias_before_move = NULL;
  }
  CarGroup(T tolerance){
    this->m_tolerance=tolerance;
    this->m_parent	= 0;
    this->m_polygons	= new TriMesh<T>(tolerance);
    this->m_movable	= false;
    this->m_need_rebuild = false;
    this->m_trias_before_move = NULL;
    this->m_tolerance=tolerance;
  }



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

  //cout << whoami() <<" "<<__FUNCTION__<< " "<<  m_velocity << endl;
  PL_DBGOSH << whoami() <<" "<<__FUNCTION__<< " "<<  m_velocity << endl;

  POLYLIB_STAT stat = PolygonGroup<T>::build_group_tree(polylib,parent,tp);

  return stat;
}

template <typename T>
POLYLIB_STAT CarGroup<T>::move(PolylibMoveParams& params){

  //   write code here
  
  
  //init_check_leaped();

  //VertexList<T> * m_polygons->m_get_vtx_list
  //#define DEBUG
#ifdef DEBUG
  PL_DBGOSH << "CarGroup::move() started."<<std::endl;
#endif // DEBUG


  // 引数チェック
  if (params.m_current_step == params.m_next_step) return PLSTAT_OK;
  if (params.m_current_step > params.m_next_step) return PLSTAT_NG;
    if (params.m_delta_t <= 0.0) return PLSTAT_NG;




// 移動量
// X 軸方向に 1step あたり params.m_delta_t × m_velocity だけ移動するものとする.
//    PL_REAL move_pos = (params.m_next_step - params.m_current_step) *
    T move_pos = (params.m_next_step - params.m_current_step) *
     this->m_velocity * params.m_delta_t;

#ifdef DEBUG
  PL_DBGOSH << "CarGroup::move() next step."<< params.m_next_step <<std::endl;
  PL_DBGOSH << "CarGroup::move() current step."<< params.m_current_step <<std::endl;
  PL_DBGOSH << "CarGroup::move() deltat."<< params.m_delta_t <<std::endl;
  PL_DBGOSH << "CarGroup::move() velocity."<< this->m_velocity <<std::endl;
  PL_DBGOSH << "CarGroup::move() velocity."<< m_velocity <<std::endl;
  PL_DBGOSH << "CarGroup::move() move_pos."<< move_pos <<std::endl;
#endif // DEBUG
    
    std::vector<Vertex<T>*>* vertexlist;
    vertexlist=this->m_polygons->get_vtx_list()->get_vertex_lists_mod();

#ifdef DEBUG
    PL_DBGOSH << "CarGroup::move() Vertexpointer. size="<< vertexlist->size()<<std::endl;
#endif // DEBUG

  for (int i =0;i<vertexlist->size();++i) {
    Vertex<T>* v = (*vertexlist)[i];
    PL_DBGOSH << "CarGroup::move() "<< i <<" "<< vertexlist->size() 
	      << " point "<< *v <<std::endl;
    /* PL_DBGOSH << "CarGroup::move() x "<< (*v)[0] <<std::endl; */
    /* PL_DBGOSH << "CarGroup::move() y "<< (*v)[1] <<std::endl; */
    /* PL_DBGOSH << "CarGroup::move() z "<< (*v)[2] <<std::endl; */

    (*v)[1]-=move_pos;

    /* PL_DBGOSH << "CarGroup::move() x "<< (*v)[0] <<std::endl; */
    /* PL_DBGOSH << "CarGroup::move() y "<< (*v)[1] <<std::endl; */
    /* PL_DBGOSH << "CarGroup::move() z "<< (*v)[2] <<std::endl; */

    PL_DBGOSH << "CarGroup::move() after "<< i <<" "<< vertexlist->size() 
	      << " point "<< *v <<std::endl;

    //#undef DEBUG
  }

    



// 三角形リストを取得
 /*    std::vector<PrivateTriangle<T>*>* tria_list = this->m_polygons->get_tri_list(); */
/*     typename std::vector<PrivateTriangle<T>*>::iterator it; */
/*     // 三角形リスト内の全ての三角形について頂点座標を更新 */
/*     for (it=tria_list->begin(); it!=tria_list->end(); it++) { */
/*       PrivateTriangle<T> *tria = (*it); */
/* f      //Vec3<T> **last_vtx = tria->get_vertex(); */
/*       Vertex<T> **last_vtx = tria->get_vertex(); */
/*       Vec3<T> moved_vtx[3]; */
/*       // X 座標 (t[0]) のみ更新 */
/*       moved_vtx[0].t[0] = last_vtx[0].t[0] + move_pos; */
/*       moved_vtx[1].t[0] = last_vtx[1].t[0] + move_pos; */
/*       moved_vtx[2].t[0] = last_vtx[2].t[0] + move_pos; */
/*       moved_vtx[0].t[1] = last_vtx[0].t[1]; */
/*       moved_vtx[1].t[1] = last_vtx[1].t[1]; */
/*       moved_vtx[2].t[1] = last_vtx[2].t[1]; */
/*       moved_vtx[0].t[2] = last_vtx[0].t[2]; */
/*       moved_vtx[1].t[2] = last_vtx[1].t[2]; */
/*       moved_vtx[2].t[2] = last_vtx[2].t[2]; */

/*       // 移動後の頂点座標を設定.法線ベクトルも再計算 */
/*       tria->set_vertexes( moved_vtx, true, false ); */

/*     } */
/*     // 頂点座標が移動したことにより,KD 木の再構築が必要. */
/*     // 要再構築フラグを立てる. */
    this->m_need_rebuild = true;

#ifdef DEBUG
    PL_DBGOSH << "CarGroup::move() end."<<std::endl;
#endif // DEBUG
#undef DEBUG

  return PLSTAT_OK;
}

template <typename T>
POLYLIB_STAT CarGroup<T>::mk_param_tag(
				    TextParser* tp,
				    string rank_no,
				    string extend,
				    string format
) {

#ifdef DEBUG
   cout << "CarGroup::"<<__FUNCTION__ << endl;
#endif
  stringstream ss;
  POLYLIB_STAT	stat;
  //goto root
  tp->changeNode("/Polylib");
  tp->changeNode(this->acq_fullpath());
  //this is test. change m_velocity forcely
  //  m_velocity = -500.;
  ss<<m_velocity;
  string value;
  ss>>value;
  tp->updateValue("velocity",value);

  return PolygonGroup<T>::mk_param_tag(tp,rank_no,extend,format);
}






#endif //CARGROUP_H
