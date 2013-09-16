// -*- Mode:c++ -*-
/*
 * Polylib - Polygon Management Library
 *
 * Copyright (c) 2012-2013 Advanced Institute for Computational Science, RIKEN.
 * All rights reserved.
 *
 */

#ifndef polylib_vertexlist_h
#define polylib_vertexlist_h

#include <vector>
#include <iostream>

#include "common/Vec3.h"
#include "common/BBox.h"
#include "common/PolylibCommon.h"
#include "polygons/Vertex.h"


//#define DEBUG


namespace PolylibNS{

  template <typename T> class VertKDT;

////////////////////////////////////////////////////////////////////////////
///
/// クラス:vertex_list
///   polygon の頂点クラスVertex  を収めるクラス。
/// 
///
////////////////////////////////////////////////////////////////////////////

template <typename T>
class VertexList{
 private:
  /// Vertex の格納場所
  std::vector<Vertex<T>*> m_vertex_list; 
  /// 同一性チェックの基準値
  T m_tolerance; 
  /// Vertex  用KD木
  VertKDT<T>* m_vkdt; 

public:

   /// コンストラクタ
   ///
  VertexList(VertKDT<T>* vkdt,T tolerance){
    m_vkdt = vkdt;
    m_tolerance = tolerance;
  }

  
   /// デストラクタ
   ///
  /// Vertex  は削除しない。削除する場合は、vtx_clearを呼ぶ。
  /// VertKDT  は削除しない。削除する場合は、 外部から

  ~VertexList(){
    //    delete m_vkdt;
    //    delete m_vertex_list;
    //    m_vertex_list=NULL;
}
  /// Vertex の解放

  void vtx_clear(){
    if(m_vertex_list.size()!=0){
      for(typename std::vector<Vertex<T>*>::iterator iter=this->m_vertex_list.begin(); 
	  iter!=this->m_vertex_list.end(); 
	  ++iter){
	//std::cout <<"deleting"<<std::endl;
	delete *iter;
      }
    this->m_vertex_list.clear();
    }
  }

  ///  Vertexの格納場所へのポインタ
  const std::vector<Vertex<T>*>* get_vertex_lists(){return &(this->m_vertex_list);}

  //  int vtx_add(const Vertex<T>* v);
  int vtx_add_i(Vertex<T>* v); //!< Vertexの追加、m_vertex_listのindexを返す。同一性チェック済み。
  
  Vertex<T>* vtx_add(Vertex<T>* v); //!< Vertexの追加、その頂点のポインタを示す。同一性チェック済み。

  Vertex<T>* vtx_add_KDT(Vertex<T>* v); //!< Vertexの追加、その頂点のポインタを示す。同一性チェック済み。

  Vertex<T>* ith(int i) {return this->m_vertex_list[i];} //!< i番目Vertexを取り出す。
  Vertex<T>* ith(long i) {return this->m_vertex_list[i];} //!< i番目Vertexを取り出す。
  

  // int ith(Vertex<T>* vtx_ptr) const { 
  //   int i;
  //   for(i=0;i<this->m_vertex_list.size();++i ){
  //     if(vtx_ptr==this->m_vertex_list[i]) return i;
  //   }
    
  //   PL_ERROSH << " " << __func__ <<" something wrong with Vertex id"<<std::endl;
  //   return -1;
  // } //!< i番目Vertexを取り出す。

  long ith(Vertex<T>* vtx_ptr) const { 
    long i;
    for(i=0;i<this->m_vertex_list.size();++i ){
      if(vtx_ptr==this->m_vertex_list[i]) return i;
    }
    
    PL_ERROSH << " " << __func__ <<" something wrong with Vertex id"<<std::endl;
    return -1;
  } //!< i番目Vertexを取り出す。




  /// 頂点の数
  ///
  /// @return 頂点の数
  typename std::vector<Vertex<T>*>::size_type size(){
    return       this->m_vertex_list.size();
  }


  /// 基準の設定
  void set_tolerance(const T tol){m_tolerance=tol;}
  /// 基準
  T tolerance() const {return m_tolerance;}

  ///  プリント
  void print() const {
    

    for(int i=0;i<this->m_vertex_list.size();++i){
      Vertex<T> test=*(this->m_vertex_list[i]);
      
      PL_DBGOS << "VertexList "<<i 
	       << " "<<test[AXIS_X] 
	       << " "<<test[AXIS_Y] 
	       << " "<<test[AXIS_Z] <<std::endl;;
    }
  }

  // setter and getter  for tolerance  

  
  VertexList():m_tolerance(1.0e-10){
  }

  VertexList(T tol):m_tolerance(tol){
  }

  T get_tolerance(){return m_tolerance;}

 };

  template <typename T> int VertexList<T>::vtx_add_i(Vertex<T>* v){
    //search same vertex point in the list.

    //T min_distance = 
    // tempolary uses __DBL_MAX__
    //    T min_distance=__DBL_MAX__;
    T min_distance=1.0e107;

    int index=0;
    int isave=-1;
    bool same_point_find=false;


#ifdef DEBUG
    std::cout<< "vertex v=("<< (*v)[AXIS_X] << ", " 
	     << (*v)[AXIS_Y] << ", " 
	     << (*v)[AXIS_Z] << ")" 
             << " size of list "  << this->m_vertex_list.size()
	     <<std::endl;
#endif // DEBUG

    for(typename std::vector<Vertex<T>*>::iterator
	  iter=this->m_vertex_list.begin();   
	iter!=this->m_vertex_list.end();
	++iter){
#ifdef DEBUG
      std::cout<< "comp ("<< (*(*iter))[AXIS_X] << ", " 
	     << (*(*iter))[AXIS_Y] << ", " 
	       << (*(*iter))[AXIS_Z] << ")" <<std::endl;
#endif // DEBUG

      T d=(*iter)->distance(*v);
	//d=v->distance( *(*iter) );
#ifdef DEBUG
      std::cout<<min_distance <<" distance "<< d <<std::endl;
#endif // DEBUG
      if(min_distance>d){
#ifdef DEBUG
	std::cout<< "distance changed "<< d <<std::endl;
#endif // DEBUG
	min_distance=d;
	isave=index;
       
	if (min_distance<m_tolerance){
#ifdef DEBUG
	  std::cout << "going to break" <<std::endl;
#endif // DEBUG
	  same_point_find=true;
	  delete v;
	  break;
	}

      }
      ++index;

    }

    //if we can not found same point that's the new one.
    if(!same_point_find){
#ifdef DEBUG
      std::cout << "no same point " <<std::endl;
#endif // DEBUG
      isave=this->m_vertex_list.size();
      this->m_vertex_list.push_back(v);
    }   
    return isave;
  }

template <typename T> Vertex<T>* VertexList<T>::vtx_add(Vertex<T>* v){

    //search same vertex point in the list.

    int index= vtx_add_i(v);
    return m_vertex_list[index];
  } 



  template <typename T> Vertex<T>* VertexList<T>::vtx_add_KDT(Vertex<T>* v){
    //search same vertex point in the list.

    //T min_distance = 
    // tempolary uses __DBL_MAX__
    //    T min_distance=__DBL_MAX__;
    //T min_distance=1.0e107;

    //    std::cout <<__func__<<" start "<< *v <<std::endl;

    int index=0;
    int isave=-1;
    bool same_point_find=false;

    Vec3<T> vtmp=(Vec3<T>) *v;

    // first check bbox
    BBox<T> bbox;

    if (this->m_vertex_list.size()>1){
      bbox=m_vkdt->get_root_bbox();
    } else {
      BBox<T> tmp(0.,0.,0.,0.,0.,0.);
      bbox=tmp;
    }

    Vec3<T> min=bbox.getPoint(0);
    Vec3<T> max=bbox.getPoint(7);
    // std::cout << __func__ << " min " <<min<<std::endl;
    // std::cout << __func__ << " max " <<max<<std::endl;

    if( !bbox.contain(vtmp) ){ 
      //std::cout << __func__ << " out of bbox " <<max<<std::endl;
      // if vertex is out of BBox create VTree again.
      m_vertex_list.push_back(v);
      BBox<T> newbbox;
      for(int i=0; i<this->m_vertex_list.size();i++){
	newbbox.add( (Vec3<T>) *((this->m_vertex_list)[i]) );
	}
      m_vkdt->create(newbbox,&(this->m_vertex_list) );
      return v;	    
    } else {
      //std::cout << __func__ << " in the bbox " <<max<<std::endl;
      const Vertex<T>* nearest=this->m_vkdt->search_nearest(vtmp);
	  if(nearest==0){
	    //std::cout << __func__ << " there is no vertex in the list. " <<max<<std::endl;
	    if(m_vkdt->add(v)==PLSTAT_OK){
	      //std::cout << __func__ << " add vertex to kdt. " <<max<<std::endl;
	      this->m_vertex_list.push_back(v);
	      return v;
	    } else {
	      //std::cout << __func__ << " add vertex to kdt, failed. " <<max<<std::endl;
	      PL_ERROSH 
		<< "[ERROR]VertList<T>::vtx_add_KDT():Can't find appropriate node" 
		<< std::endl;
	      return 0; 
	    }
	  } else {
	    //std::cout << __func__ << " nearest vertex found. " <<max<<std::endl;
	    T distance = ((Vec3<T>) (*nearest) - (Vec3<T>) (*v)).length();
	    // std::cout << "nearest "<< *nearest  << " add " << *v <<std::endl;
	    // std::cout << __func__ << " check distance "<<distance 
	    // 	      <<" "<< m_tolerance <<std::endl;
	    if (distance<m_tolerance){
	      //std::cout << __func__ << " same vertex. " <<max<<std::endl;
	      return  const_cast<Vertex<T>*>(nearest);
	    } else {
	      //std::cout << __func__ << " different vertex. " <<max<<std::endl;
	      if(m_vkdt->add(v)==PLSTAT_OK){
		//std::cout << __func__ << " add vertex to kdt. " <<max<<std::endl;
		this->m_vertex_list.push_back(v);
		return v;
	      } else {
		//std::cout << __func__ << " add vertex to kdt, failed. " <<max<<std::endl;
		PL_ERROSH 
		  << "[ERROR]VertList<T>::vtx_add_KDT():Can't find appropriate node" 
		  << std::endl;

		return 0; 
	      }
	    }
	  }
	
    }
    std::cout <<__func__<<" end "<< *v <<std::endl;
	    // never happens...
	    return 0;   
	    
  }
	  } //end of namespace PolylibNS




//#undef DEBUG
#endif // polylib_vertexlist_h
