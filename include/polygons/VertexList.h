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
#include <map>
#include <iostream>

#include "common/Vec3.h"
#include "common/BBox.h"
#include "common/PolylibCommon.h"
#include "polygons/Vertex.h"


//#define DEBUG
//#define VertexListDEBUG

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
    std::vector<Vertex<T>*>* m_vertex_list; 
    /// 同一性チェックの基準値
    T m_tolerance; 
    /// 同一性チェックの基準値の2乗
    T m_tolerance_2;
    /// Vertex  用KD木
    VertKDT<T>* m_vkdt; 

    /// KD木生成用BBox
    BBox<T> m_bbox;
    
    //番号検索用 map
    std::map<Vertex<T>*, typename std::vector<Vertex<T>*>::size_type>* m_num_map;

#ifdef  VertexListDEBUG
    std::map<Vertex<T>*, int> m_pointer_count;
#endif

  public:
    VertexList(T tolerance){
      m_vkdt = NULL;
      m_tolerance = tolerance;
      m_tolerance_2=m_tolerance*m_tolerance;
      m_bbox.init();
      m_vertex_list= new std::vector<Vertex<T>*>;
      m_num_map=NULL;

#ifdef  VertexListDEBUG
      m_pointer_count.clear();
#endif
    }



    /// コンストラクタ
    ///
    VertexList(VertKDT<T>* vkdt,T tolerance){
      m_vkdt = vkdt;
      m_tolerance = tolerance;
      m_tolerance_2=m_tolerance*m_tolerance;
      m_bbox.init();
      m_vertex_list= new std::vector<Vertex<T>*>;
      m_num_map=NULL;

#ifdef  VertexListDEBUG
      m_pointer_count.clear();
#endif

    }
  
    /// デストラクタ
    ///
    /// Vertex  は削除。削除する場合は、vtx_clearを呼ぶ。
    /// VertKDT  は削除しない。削除する場合は、 外部から

    ~VertexList(){
      //    delete m_vkdt;
      vtx_clear();
      delete m_vertex_list;
      m_vertex_list=NULL;
      index_map_clear();
    }


    ///Vertex 用KD木のセット
    ///
    /// @param[in]  vkdt 利用するKD木のポインタ
    void setKDT(VertKDT<T>* vkdt){
      m_vkdt = vkdt;
    }

    /// Vertex  用KD木
    VertKDT<T>* getKDT(){return m_vkdt;}


    ///  Vertexの格納場所へのポインタ
    const std::vector<Vertex<T>*>* get_vertex_lists(){return m_vertex_list;}
    /// Vertex の追加 同一性チェック無し。
    void vtx_add_nocheck(Vertex<T>* v);

    int vtx_add_i(Vertex<T>* v); //!< Vertexの追加、m_vertex_listのindexを返す。同一性チェック済み。
    Vertex<T>* vtx_add(Vertex<T>* v); //!< Vertexの追加、その頂点のポインタを示す。同一性チェック済み。
    Vertex<T>* vtx_add_KDT(Vertex<T>* v); //!< Vertexの追加、その頂点のポインタを示す。同一性チェック済み。

    Vertex<T>* ith(int i) {return (*m_vertex_list)[i];} //!< i番目Vertexを取り出す。
    Vertex<T>* ith(long i) {return (*m_vertex_list)[i];} //!< i番目Vertexを取り出す。

    // int ith(Vertex<T>* vtx_ptr) const { 
    //   int i;
    //   for(i=0;i<this->m_vertex_list->size();++i ){
    //     if(vtx_ptr==this->m_vertex_list[i]) return i;
    //   }
    
    //   PL_ERROSH << " " << __func__ <<" something wrong with Vertex id"<<std::endl;
    //   return -1;
    // } //!< i番目Vertexを取り出す。

    long ith(Vertex<T>* vtx_ptr) const { 
      long i;
      for(i=0;i<this->m_vertex_list->size();++i ){
	if(vtx_ptr== (*m_vertex_list)[i]) return i;
      }
    
      PL_ERROSH << " " << __func__ <<" something wrong with Vertex id"<<std::endl;
      return -1;
    } //!< i番目Vertexを取り出す。




    /// 頂点の数
    ///
    /// @return 頂点の数
    typename std::vector<Vertex<T>*>::size_type size(){
      return       this->m_vertex_list->size();
    }

    /// bbox をVertKDTへ設定
    void set_bbox() { 
      //std::cout << __func__ <<std::endl;
      m_vkdt->set_root_bbox(m_bbox);
      //1std::cout << __func__ <<std::endl;
    }
    /// bbox をVertKDTを取得
    BBox<T> get_bbox() const {return m_bbox;}

    /// 三角形ポリゴンの面出力時、頂点の番号を取得する場合に、番号のテーブルを準備する。
    /// 
    ///  後始末は index_map_clear()で行う。
    void prepare_num_out(){
      m_num_map=new std::map<Vertex<T>*, typename std::vector<Vertex<T>*>::size_type>;

      typename std::vector<Vertex<T>*>::size_type index=0;
      for(typename std::vector<Vertex<T>*>::iterator iter=m_vertex_list->begin();
	  iter!=m_vertex_list->end();
	  ++iter)
	{ // 0番から インデクシング
	  (*m_num_map)[ (*iter) ]=index;
	  ++index;
	}
    }
    /// 三角形ポリゴンの面出力時、頂点の番号を取得する
    ///
    /// @param[in] Vertex<T> のポインタ
    /// @return 頂点番号
    /// @attention prepare_num_out() を呼び出す。
    typename std::vector<Vertex<T>*>::size_type vtx_index(Vertex<T>* v)
    {
      if(m_num_map==NULL){
	prepare_num_out();
      }
      return (*m_num_map)[v];
    }

    /// 三角形ポリゴンの面出力時、頂点の番号を取得する場合の、番号のテーブルを削除する。
    void index_map_clear(){
      // clear index map
      if(m_num_map!=NULL) {
	delete m_num_map;
	m_num_map=NULL;
      }
    }


    /// 基準値を設定する
    void set_tolerance(const T tol){m_tolerance=tol;}

    /// 基準
    T tolerance() const {return m_tolerance;}

    ///  プリント
    void print() const {
    

      for(int i=0;i<this->m_vertex_list->size();++i){
	Vertex<T> test=*(this->m_vertex_list[i]);
      
	PL_DBGOS << "VertexList "<<i 
		 << " "<<test[AXIS_X] 
		 << " "<<test[AXIS_Y] 
		 << " "<<test[AXIS_Z] <<std::endl;;
      }
    }

    // setter and getter  for tolerance  

  
    /// コンストラクタ　基準値
    VertexList():m_tolerance(1.0e-10){
      m_bbox.init();
    }
    /// コンストラクタ　基準値
    // VertexList(T tol):m_tolerance(tol){
    //   m_bbox.init();
    // }
    /// 基準値の取得
    T get_tolerance(){return m_tolerance;}

    /// 重複頂点の削除
    POLYLIB_STAT vertex_compaction(std::map<Vertex<T>*,Vertex<T>*>* vertex_map);


    //  private:
    /// Vertex の解放

    void vtx_clear(){
      if(m_vertex_list->size()!=0){
	for(typename std::vector<Vertex<T>*>::iterator iter=this->m_vertex_list->begin(); 
	    iter!=this->m_vertex_list->end(); 
	    ++iter){
	  //std::cout <<"deleting"<<std::endl;
	  delete *iter;
	}
	this->m_vertex_list->clear();
      }
    }


  };

  template <typename T> void VertexList<T>::vtx_add_nocheck(Vertex<T>* v){

#ifdef  VertexListDEBUG
    if(m_pointer_count.count(v)==0){
      PL_DBGOSH << "VertexList<T>::"<<__func__ << " new pointer " << v <<std::endl;    
      m_pointer_count[v]=1;
    } else {
      PL_DBGOSH << "VertexList<T>::"<<__func__ << " same pointer " << v << " Why?"<<std::endl;    
      m_pointer_count[v]+=1; //why?
    }
#endif

    //PL_DBGOSH << "VertexList<T>::"<<__func__ << " new pointer " << v <<std::endl;    

    m_bbox.add( *v);

    // PL_DBGOSH << "VertexList<T>::"<<__func__ << " new pointer " 
    // 	      << (*v)[0]<< " "
    // 	      << (*v)[1]<< " "
    // 	      << (*v)[2]<< " "
    // 	      <<std::endl;    
    m_vertex_list->push_back( v);
    //    PL_DBGOSH << "VertexList<T>::"<<__func__ << " new pointer " << v <<std::endl;    


  }


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
             << " size of list "  << this->m_vertex_list->size()
	     <<std::endl;
#endif // DEBUG

    for(typename std::vector<Vertex<T>*>::iterator
	  iter=this->m_vertex_list->begin();   
	iter!=this->m_vertex_list->end();
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
    PL_DBGOSH << "no same point " <<std::endl;
#endif // DEBUG
      isave=m_vertex_list->size();
      m_vertex_list->push_back(v);
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
    //T m_tolerance_2=m_tolerance*m_tolerance;

    Vec3<T> vtmp=(Vec3<T>) *v;

    // first check bbox
    BBox<T> bbox;
    if (this->m_vertex_list->size()>1){
      bbox=m_vkdt->get_root_bbox();
    } else {
      BBox<T> tmp(0.,0.,0.,0.,0.,0.);
      bbox=tmp;
    }

    // Vec3<T> min=bbox.getPoint(0);
    // Vec3<T> max=bbox.getPoint(7);
    // std::cout << __func__ << " min " <<min<<std::endl;
    // std::cout << __func__ << " max " <<max<<std::endl;

    if( !bbox.contain(vtmp) ){ 
      //std::cout << __func__ << " out of bbox " <<max<<std::endl;
      // if vertex is out of BBox create KD tree again.
      m_vertex_list->push_back(v);
      BBox<T> newbbox;
      for(int i=0; i<this->m_vertex_list->size();i++){
  	newbbox.add( (Vec3<T>) *(*m_vertex_list)[i] );
  	}
      m_vkdt->create(newbbox,m_vertex_list );
      return v;	   
    } else {
      //std::cout << __func__ << " in the bbox " <<max<<std::endl;
      const Vertex<T>* nearest=this->m_vkdt->search_nearest(vtmp);
  	  if(nearest==0){
  	    //std::cout << __func__ << " there is no vertex in the list. " <<max<<std::endl;
  	    if(m_vkdt->add(v)==PLSTAT_OK){
  	      //std::cout << __func__ << " add vertex to kdt. " <<max<<std::endl;
  	      this->m_vertex_list->push_back(v);
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
  	    T distance = ((Vec3<T>) (*nearest) - (Vec3<T>) (*v)).lengthSquared();
  	    // std::cout << "nearest "<< *nearest  << " add " << *v <<std::endl;
  	    // std::cout << __func__ << " check distance "<<distance 
  	    // 	      <<" "<< m_tolerance <<std::endl;
  	    if (distance< m_tolerance_2){
  	      //std::cout << __func__ << " same vertex. " <<max<<std::endl;
  	      return  const_cast<Vertex<T>*>(nearest);
  	    } else {
  	      //std::cout << __func__ << " different vertex. " <<max<<std::endl;
  	      if(m_vkdt->add(v)==PLSTAT_OK){
  		//std::cout << __func__ << " add vertex to kdt. " <<max<<std::endl;
  		this->m_vertex_list->push_back(v);
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

  //// public //////////////////////////////////////

  template <typename T>  
  POLYLIB_STAT 
  VertexList<T>::vertex_compaction(
				   std::map<Vertex<T>*,Vertex<T>*> *vertex_map
				   )
{

  //#define DEBUG
#ifdef DEBUG
	PL_DBGOSH<< __func__<< " start" <<std::endl;
#endif
	set_bbox();

#ifdef DEBUG
  PL_DBGOSH<< __func__<< " set bbox min "<<m_bbox.min
	   << " max "<< m_bbox.max << std::endl;
#endif
  // PL_DBGOSH<< __func__<< " set bbox min "<<m_bbox.min
  // 	   << " max "<< m_bbox.max << std::endl;
  //forcely initialize VertKDT


  std::vector<Vertex<T>*>* remove_vtx_list=new std::vector<Vertex<T>*>;
  std::vector<Vertex<T>*>* save_vtx_list=new std::vector<Vertex<T>*>;
  
  typename std::vector<Vertex<T>*>::iterator vtx_itr;

  for(vtx_itr = m_vertex_list->begin();
      vtx_itr != m_vertex_list->end();
      ++vtx_itr){

#ifdef DEBUG
	PL_DBGOSH<< __func__<< " loop" <<std::endl;
#endif
    

#ifdef DEBUG
	PL_DBGOSH<< __func__<< " vertex "<< *(*vtx_itr)<< std::endl;

	// if( ( (*(*vtx_itr))[0] > 1000) || ( (*(*vtx_itr))[1] > 1000) ||( (*(*vtx_itr))[2] > 1000) ){
	   PL_DBGOSH<< __func__<< " vertex error "<< *(*vtx_itr)<< std::endl;
	   PL_DBGOSH<< __func__<< " vertex error 2 "
	 	   << (*(*vtx_itr))[0]<< " "
	 	   << (*(*vtx_itr))[1]<< " "
	 	   << (*(*vtx_itr))[2]<< std::endl;
	// }
#endif

    if( save_vtx_list->size()==0){
#ifdef DEBUG
      PL_DBGOSH<< __func__<< " the first time "<<(*vtx_itr) <<  std::endl;
#endif
	save_vtx_list->push_back(*vtx_itr);
	(*vertex_map)[*vtx_itr]=*vtx_itr;
	if(m_vkdt !=NULL &&  m_vkdt->add(*vtx_itr)==PLSTAT_OK){ 
#ifdef DEBUG
      PL_DBGOSH<< __func__<< " the first time add pointer to KD tree. "<<(*vtx_itr) <<  std::endl;
#endif
	  
	} else {
	  PL_ERROSH 
	    << "[ERROR]VertList<T>::vertex_compaction():Can't find appropriate node" 
	    << std::endl;
	  return PLSTAT_NG;
	}
    } else {

#ifdef DEBUG
	PL_DBGOSH<< __func__<< " not the first time "<<  std::endl;
#endif

#ifdef DEBUG
	PL_DBGOSH<< __func__<< " nearest search start"<< std::endl;
#endif

    const Vertex<T>* nearest=m_vkdt->search_nearest( *(*vtx_itr) );
#ifdef DEBUG
	PL_DBGOSH<< __func__<< " nearest "<< (*nearest)<< std::endl;
#endif


    if(nearest==0){

      if(m_vkdt->add(*vtx_itr)==PLSTAT_OK){ 
	//nearest point not found. buggy??

#ifdef DEBUG
	PL_DBGOSH<< __func__<< " nearest point not found?"<<std::endl;
#endif
	PL_DBGOSH<< __func__<< " nearest point not found? never happened!!!"<<std::endl;
	save_vtx_list->push_back(*vtx_itr);
	(*vertex_map)[*vtx_itr]=*vtx_itr;
      } else {
	PL_ERROSH 
	  << "[ERROR]VertList<T>::vertex_compaction():Can't find appropriate node" 
	  << std::endl;
	return PLSTAT_NG;
      }

    } else {


#ifdef DEBUG
	PL_DBGOSH<< __func__<< " nearest point is found."<<std::endl;
#endif  
	//	PL_DBGOSH<< __func__<< " nearest point is found. "<< nearest <<std::endl;
	
      T distance2 = ((Vec3<T>) (*nearest) - (Vec3<T>) *(*vtx_itr)).lengthSquared();
      if (distance2< m_tolerance_2){ // 同一頂点
	remove_vtx_list->push_back( *vtx_itr);
	(*vertex_map)[(*vtx_itr)] = const_cast<Vertex<T>*>(nearest);
#ifdef DEBUG
	PL_DBGOSH<< __func__<< " same point "<<distance2  <<std::endl;
	PL_DBGOSH<< __func__ << " nearest "<< (*nearest) <<std::endl;
	PL_DBGOSH<< __func__  << " current "<< *(*vtx_itr)  <<std::endl;
	PL_DBGOSH<< __func__  << " pointer "<< (*vtx_itr)  <<std::endl;
	PL_DBGOSH<< __func__  << " mapped "<< (*vertex_map)[(*vtx_itr)]  <<std::endl;
#endif


      } else { // 同一頂点ではない。

#ifdef DEBUG
	PL_DBGOSH<< __func__<< "not same point "<<distance2 <<std::endl;
#endif

	save_vtx_list->push_back(*vtx_itr);
	(*vertex_map)[*vtx_itr]=(*vtx_itr);

	if(m_vkdt->add(*vtx_itr)!=PLSTAT_OK){
	  PL_ERROSH 
	    << "[ERROR]VertList<T>::vertex_compaction():Can't find appropriate node" 
	    << std::endl;
	  return PLSTAT_NG;
	}
      }
      
    }

    }
  }



#ifdef DEBUG
  PL_DBGOSH<< __func__<< " size "<< m_vertex_list->size() 
	   << " to "<< save_vtx_list->size() 
 	   << " delete "<< remove_vtx_list->size()  <<std::endl;
#endif


  // cleanup old and define new list
  delete m_vertex_list;
#ifdef DEBUG
  PL_DBGOSH<< __func__<< " delete 1"<< std::endl;
#endif
  
  m_vertex_list=save_vtx_list;
  
  for(typename std::vector<Vertex<T>*>::iterator itr=remove_vtx_list->begin();
       itr!=remove_vtx_list->end();
       ++itr){

#ifdef DEBUG
    //    PL_DBGOSH<< __func__<< " delete 2 "<< std::endl;
#endif

#ifdef  VertexListDEBUG
    if(m_pointer_count.count( *itr )==0){
      m_pointer_count[*itr]=-1;
    } else {
      m_pointer_count[*itr]-=1;
    }
#endif
    //tempolary off
    delete (*itr);
  }
  remove_vtx_list->clear();
  delete remove_vtx_list;

#ifdef  VertexListDEBUG
  for(typename std::map<Vertex<T>*, int>::iterator
	imap =  m_pointer_count.begin();
      imap!=m_pointer_count.end();
      imap){
    if( (*imap).second<0 ) {
      PL_DBGOSH<< __func__<< " delete 2 times "<< (*imap).first
	       << " " << (*imap).second 
	       << std::endl;
    } else if((*imap).second==0 ) {
      delete ((*imap).first);
    }
  
  } 
#endif
//#undef  VertexListDEBUG

#ifdef DEBUG
  PL_DBGOSH<< __func__<< " delete 3"<< std::endl;
#endif

  return PLSTAT_OK;
  //#undef DEBUG
}



} //end of namespace PolylibNS




//#undef DEBUG
#endif // polylib_vertexlist_h
