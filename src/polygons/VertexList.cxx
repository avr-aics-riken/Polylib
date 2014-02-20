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


#include "polygons/VertexList.h"
#include "polygons/VertKDT.h"

#include <vector>
#include <map>
#include <iostream>
#include <string>

#include "common/Vec3.h"
#include "common/BBox.h"
#include "common/PolylibCommon.h"
#include "polygons/Vertex.h"


//#define DEBUG
//#define VertexListDEBUG

namespace PolylibNS{

VertexList::VertexList(REAL_TYPE tolerance)
{ 
}

/// コンストラクタ
///
VertexList::VertexList(VertKDT* vkdt,REAL_TYPE tolerance){
	m_vkdt = vkdt;
	m_tolerance = tolerance;
	m_tolerance_2=m_tolerance*m_tolerance;
	m_bbox.init();
	m_vertex_list= new std::vector<Vertex*>;
	m_num_map=NULL;

#ifdef  VertexListDEBUG
	m_pointer_count.clear();
#endif

}

/// デストラクタ
///
/// Vertex  は削除。削除する場合は、vtx_clearを呼ぶ。
/// VertKDT  は削除しない。削除する場合は、 外部から

VertexList::~VertexList()
{
	//    delete m_vkdt;
	vtx_clear();
	delete m_vertex_list;
	m_vertex_list=NULL;
	index_map_clear();
}


///Vertex 用KD木のセット
///
/// @param[in]  vkdt 利用するKD木のポインタ
void VertexList::setKDT(VertKDT* vkdt)
{
	m_vkdt = vkdt;
}

/// Vertex  用KD木
VertKDT* VertexList::getKDT()
{
	return m_vkdt;
}


///  Vertexの格納場所へのポインタ
const std::vector<Vertex*>* VertexList::get_vertex_lists()
{
	return m_vertex_list;
}

//!<  Vertexの格納場所へのポインタ
std::vector<Vertex*>* VertexList::get_vertex_lists_mod()
{
	return m_vertex_list;
}

Vertex* VertexList::ith(int i) {
	return (*m_vertex_list)[i];
} //!< i番目Vertexを取り出す。

Vertex* VertexList::ith(long i) {
	return (*m_vertex_list)[i];
} //!< i番目Vertexを取り出す。

// int ith(Vertex* vtx_ptr) const { 
//   int i;
//   for(i=0;i<this->m_vertex_list->size();++i ){
//     if(vtx_ptr==this->m_vertex_list[i]) return i;
//   }

//   PL_ERROSH << " " << __func__ <<" something wrong with Vertex id"<<std::endl;
//   return -1;
// } //!< i番目Vertexを取り出す。

long VertexList::ith(Vertex* vtx_ptr) const 
{ 
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
std::vector<Vertex*>::size_type VertexList::size()
{
	return       this->m_vertex_list->size();
}

/// bbox をVertKDTへ設定
void VertexList::set_bbox() 
{ 
	//std::cout << __func__ <<std::endl;
	m_vkdt->set_root_bbox(m_bbox);
	//1std::cout << __func__ <<std::endl;
}

/// bbox をVertKDTを取得
BBox VertexList::get_bbox() const {
	return m_bbox;
}

/// 三角形ポリゴンの面出力時、頂点の番号を取得する場合に、番号のテーブルを準備する。
/// 
///  後始末は index_map_clear()で行う。
void VertexList::prepare_num_out()
{
	m_num_map=new std::map<Vertex*, std::vector<Vertex*>::size_type>;

	std::vector<Vertex*>::size_type index=0;
	for(std::vector<Vertex*>::iterator iter=m_vertex_list->begin();
		iter!=m_vertex_list->end();
		++iter)
	{ // 0番から インデクシング
		(*m_num_map)[ (*iter) ]=index;
		++index;
	}
}
/// 三角形ポリゴンの面出力時、頂点の番号を取得する
///
/// @param[in] Vertex のポインタ
/// @return 頂点番号
/// @attention prepare_num_out() を呼び出す。
std::vector<Vertex*>::size_type VertexList::vtx_index(Vertex* v)
{
	if(m_num_map==NULL){
		prepare_num_out();
	}
	return (*m_num_map)[v];
}

/// 三角形ポリゴンの面出力時、頂点の番号を取得する場合の、番号のテーブルを削除する。
void VertexList::index_map_clear(){
	// clear index map
	if(m_num_map!=NULL) {
		delete m_num_map;
		m_num_map=NULL;
	}
}


/// 基準値を設定する
void VertexList::set_tolerance(const REAL_TYPE tol){
	m_tolerance=tol;
}

/// 基準
REAL_TYPE VertexList::tolerance() const {
	return m_tolerance;
}

///  プリント
void VertexList::print() const 
{
	for(int i=0;i<this->m_vertex_list->size();++i){

		Vertex& test=*(m_vertex_list->at(i));

		PL_DBGOS << "VertexList "<<i 
			<< " "<<test[AXIS_X] 
		<< " "<<test[AXIS_Y] 
		<< " "<<test[AXIS_Z] <<std::endl;;
	}
}

// setter and getter  for tolerance  


/// コンストラクタ　基準値
VertexList::VertexList():m_tolerance(1.0e-10){
	m_bbox.init();
}
/// コンストラクタ　基準値
// VertexList(REAL_TYPE tol):m_tolerance(tol){
//   m_bbox.init();
// }
/// 基準値の取得
REAL_TYPE VertexList::get_tolerance(){
	return m_tolerance;
}


//  private:
/// Vertex の解放

void VertexList::vtx_clear(){
	if(m_vertex_list->size()!=0){
		for(std::vector<Vertex*>::iterator iter=this->m_vertex_list->begin(); 
			iter!=this->m_vertex_list->end(); 
			++iter)
		{
			//std::cout <<"deleting"<<std::endl;
			delete *iter;
		}
		this->m_vertex_list->clear();
	}
}

void VertexList::vtx_add_nocheck(Vertex* v)
{

#ifdef  VertexListDEBUG
	if(m_pointer_count.count(v)==0){
		PL_DBGOSH << "VertexList::"<<__func__ << " new pointer " << v <<std::endl;    
		m_pointer_count[v]=1;
	} else {
		PL_DBGOSH << "VertexList::"<<__func__ << " same pointer " << v << " Why?"<<std::endl;    
		m_pointer_count[v]+=1; //why?
	}
#endif

	//PL_DBGOSH << "VertexList::"<<__func__ << " new pointer " << v <<std::endl;    

	m_bbox.add( *v);

	// PL_DBGOSH << "VertexList::"<<__func__ << " new pointer " 
	// 	      << (*v)[0]<< " "
	// 	      << (*v)[1]<< " "
	// 	      << (*v)[2]<< " "
	// 	      <<std::endl;    
	m_vertex_list->push_back( v);
	//    PL_DBGOSH << "VertexList::"<<__func__ << " new pointer " << v <<std::endl;    

}



int VertexList::vtx_add_i(Vertex* v)
{
	//search same vertex point in the list.

	//REAL_TYPE min_distance = 
	// tempolary uses __DBL_MAX__
	//    REAL_TYPE min_distance=__DBL_MAX__;

	REAL_TYPE min_distance= (sizeof(REAL_TYPE)==4)?FLT_MAX:DBL_MAX ;	

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

	for(std::vector<Vertex*>::iterator
		iter=this->m_vertex_list->begin();   
		iter!=this->m_vertex_list->end();
	++iter){
#ifdef DEBUG
		std::cout<< "comp ("<< (*(*iter))[AXIS_X] << ", " 
			<< (*(*iter))[AXIS_Y] << ", " 
			<< (*(*iter))[AXIS_Z] << ")" <<std::endl;
#endif // DEBUG

		REAL_TYPE d=(*iter)->distance(*v);
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

Vertex* VertexList::vtx_add(Vertex* v)
{
	//search same vertex point in the list.

	int index= vtx_add_i(v);

	return m_vertex_list->at(index);
 
} 

Vertex* VertexList::vtx_add_KDT(Vertex* v)
{
	//search same vertex point in the list.

	//REAL_TYPE min_distance = 
	// tempolary uses __DBL_MAX__
	//    REAL_TYPE min_distance=__DBL_MAX__;
	//REAL_TYPE min_distance=1.0e107;

	//    std::cout <<__func__<<" start "<< *v <<std::endl;

	int index=0;
	int isave=-1;
	bool same_point_find=false;
	//REAL_TYPE m_tolerance_2=m_tolerance*m_tolerance;

	Vec3<REAL_TYPE> vtmp=(Vec3<REAL_TYPE>) *v;

	// first check bbox
	BBox bbox;
	if (this->m_vertex_list->size()>1){
		bbox=m_vkdt->get_root_bbox();
	} else {
		BBox tmp(0.,0.,0.,0.,0.,0.);
		bbox=tmp;
	}

	// Vec3<REAL_TYPE> min=bbox.getPoint(0);
	// Vec3<REAL_TYPE> max=bbox.getPoint(7);
	// std::cout << __func__ << " min " <<min<<std::endl;
	// std::cout << __func__ << " max " <<max<<std::endl;

	if( !bbox.contain(vtmp) ){ 
		//std::cout << __func__ << " out of bbox " <<max<<std::endl;
		// if vertex is out of BBox create KD tree again.
		m_vertex_list->push_back(v);
		BBox newbbox;
		for(int i=0; i<this->m_vertex_list->size();i++){
			newbbox.add( (Vec3<REAL_TYPE>) *(*m_vertex_list)[i] );
		}
		m_vkdt->create(newbbox,m_vertex_list );
		return v;	   
	} else {
		//std::cout << __func__ << " in the bbox " <<max<<std::endl;
		const Vertex* nearest=this->m_vkdt->search_nearest(vtmp);
		if(nearest==0){
			//std::cout << __func__ << " there is no vertex in the list. " <<max<<std::endl;
			if(m_vkdt->add(v)==PLSTAT_OK){
				//std::cout << __func__ << " add vertex to kdt. " <<max<<std::endl;
				this->m_vertex_list->push_back(v);
				return v;
			} else {
				//std::cout << __func__ << " add vertex to kdt, failed. " <<max<<std::endl;
				PL_ERROSH 
					<< "[ERROR]VertList::vtx_add_KDT():Can't find appropriate node" 
					<< std::endl;
				return 0; 
			}
		} else {
			//std::cout << __func__ << " nearest vertex found. " <<max<<std::endl;
			REAL_TYPE distance = ((Vec3<REAL_TYPE>) (*nearest) - (Vec3<REAL_TYPE>) (*v)).lengthSquared();
			// std::cout << "nearest "<< *nearest  << " add " << *v <<std::endl;
			// std::cout << __func__ << " check distance "<<distance 
			// 	      <<" "<< m_tolerance <<std::endl;
			if (distance< m_tolerance_2){
				//std::cout << __func__ << " same vertex. " <<max<<std::endl;
				return  const_cast<Vertex*>(nearest);
			} else {
				//std::cout << __func__ << " different vertex. " <<max<<std::endl;
				if(m_vkdt->add(v)==PLSTAT_OK){
					//std::cout << __func__ << " add vertex to kdt. " <<max<<std::endl;
					this->m_vertex_list->push_back(v);
					return v;
				} else {
					//std::cout << __func__ << " add vertex to kdt, failed. " <<max<<std::endl;
					PL_ERROSH 
						<< "[ERROR]VertList::vtx_add_KDT():Can't find appropriate node" 
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

POLYLIB_STAT 
	VertexList::vertex_compaction(
	std::map<Vertex*,Vertex*> *vertex_map
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


	std::vector<Vertex*>* remove_vtx_list=new std::vector<Vertex*>;
	std::vector<Vertex*>* save_vtx_list=new std::vector<Vertex*>;

	std::vector<Vertex*>::iterator vtx_itr;

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
						<< "[ERROR]VertList::vertex_compaction():Can't find appropriate node" 
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

				const Vertex* nearest=m_vkdt->search_nearest( *(*vtx_itr) );
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
							<< "[ERROR]VertList::vertex_compaction():Can't find appropriate node" 
							<< std::endl;
						return PLSTAT_NG;
					}

				} else {


#ifdef DEBUG
					PL_DBGOSH<< __func__<< " nearest point is found."<<std::endl;
#endif  
					//	PL_DBGOSH<< __func__<< " nearest point is found. "<< nearest <<std::endl;

					REAL_TYPE distance2 = ((Vec3<REAL_TYPE>) (*nearest) - (Vec3<REAL_TYPE>) *(*vtx_itr)).lengthSquared();
					if (distance2< m_tolerance_2){ // 同一頂点
						remove_vtx_list->push_back( *vtx_itr);
						(*vertex_map)[(*vtx_itr)] = const_cast<Vertex*>(nearest);
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
								<< "[ERROR]VertList::vertex_compaction():Can't find appropriate node" 
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

	for(std::vector<Vertex*>::iterator itr=remove_vtx_list->begin();
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
	for(std::map<Vertex*, int>::iterator
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

