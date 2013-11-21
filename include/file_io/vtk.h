// -*- Mode: c++ -*- 
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

#ifndef polylib_vtk_h
#define polylib_vtk_h

#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <iomanip>

#include "common/PolylibCommon.h"
#include "polygons/Triangle.h"
#include "polygons/Vertex.h"
#include "polygons/DVertexManager.h"
#include "polygons/DVertex.h"
#include "polygons/VertexList.h"

#include "file_io/stl.h"

#if HAVE_STRING_H 
#include <string.h>
#else
// write some code here
#endif

namespace PolylibNS {

/// 
/// VertexList, tri_listにから vtkアスキー形式に出力する。
/// 
///
///  @param[in] vertex_list 頂点リストの領域。
///  @param[in] tri_list　三角形ポリゴンリストの領域。
///  @param[in]	fname	　ファイル名。
///
///  T は、頂点データの実数型


template <typename T>
POLYLIB_STAT vtk_a_save(
	VertexList<T>* vertex_list, 
	std::vector<PrivateTriangle<T>*> *tri_list,
	std::string fname
);

/// 
/// VertexList, tri_listにから vtkバイナリ形式に出力する。
///
///  @param[in] vertex_list 頂点リストの領域。
///  @param[in] tri_list　三角形ポリゴンリストの領域。
///  @param[in]	fname	　ファイル名。
///
///  T は、頂点データの実数型
///  DT は、頂点データに付随するスカラーデータの型
///  


  template <typename T>
POLYLIB_STAT vtk_b_save(
	VertexList<T>* vertex_list, 
	std::vector<PrivateTriangle<T>*> *tri_list,
	std::string fname
);
 

 /////////////////////////////////////////////////////
  template <typename T>
POLYLIB_STAT vtk_a_save(
			VertexList<T>* vertex_list, 
			std::vector<PrivateTriangle<T>*> *tri_list,
			std::string fname
			){

    //#define DEBUG
#ifdef DEBUG
    PL_DBGOSH << __func__ <<fname <<std::endl;
#endif // DEBUG
  std::ofstream os(fname.c_str());
  if (os.fail()) {
    PL_ERROSH << "[ERROR]vtk:vtk_a_save():Can't open " << fname << std::endl;
    return PLSTAT_VTK_IO_ERROR;
  }


#ifdef DEBUG
    PL_DBGOSH << __func__ <<"Header" <<std::endl;
#endif // DEBUG
    
  //header 

  os << "# vtk DataFile Version 2.0" <<std::endl;
  os << " title "<<std::endl;
  os << "ASCII"<<std::endl;
  os << "DATASET UNSTRUCTURED_GRID"<<std::endl;

#ifdef DEBUG
    PL_DBGOSH << __func__ <<"POINTS" <<std::endl;
#endif // DEBUG

  os << "POINTS " << vertex_list->size();

  if(sizeof(T)==4){
    os <<" float"<<std::endl;
  } else if(sizeof(T)==8) {
    os << " double"<<std::endl;
  } else {
    PL_ERROSH << "[ERROR]vtk:vtk_a_save():wrong data type sizeof(T)="<<sizeof(T) << std::endl;
    return PLSTAT_VTK_IO_ERROR;
  }




  // 頂点 v 出力
  const std::vector<Vertex<T>*>* vlistout=vertex_list->get_vertex_lists();
  for(int i=0;i<vlistout->size();++i){
#if SCIENTIFIC_OUT
    os <<  std::setprecision(6)<< scientific <<*((*vlistout)[i])<<std::endl;
#else
      os << std::setprecision(6) <<*((*vlistout)[i])<<std::endl;
#endif
  }


#ifdef DEBUG
    PL_DBGOSH << __func__ <<"CELLS" <<std::endl;
#endif // DEBUG


  // CELLS
  os << "CELLS " << tri_list->size() << " "<< 4*tri_list->size()<<std::endl;
  typename std::vector<PrivateTriangle<T>*>::size_type index=0;  
  typename std::vector<PrivateTriangle<T>*>::iterator itr;
  for (itr = tri_list->begin(); itr != tri_list->end(); itr++) {
    Vertex<T>** tmpvtx=(*itr)->get_vertex();
    int i0,i1,i2;
    /* i0=vertex_list->ith(tmpvtx[0]); */
    /* i1=vertex_list->ith(tmpvtx[1]); */
    /* i2=vertex_list->ith(tmpvtx[2]); */
    i0=vertex_list->vtx_index(tmpvtx[0]);
    i1=vertex_list->vtx_index(tmpvtx[1]);
    i2=vertex_list->vtx_index(tmpvtx[2]);


    if(i0==-1 || i1==-1 || i2==-1) {
      PL_ERROSH << "[ERROR]vtk:vtk_a_save():wrong vertex id " 
		<< i0 << " "<< i1 << " "<< i2 << " "<< std::endl;
    }
    // vertex indexes are start from 0
    int n_points=3;
    //os << index << " " << i0 << " "<< i1 << " "<< i2 << std::endl;
    os << n_points << " " << i0 << " "<< i1 << " "<< i2 << std::endl;
    index++;
  }

  //clear indexmap
  vertex_list->index_map_clear();


#ifdef DEBUG
    PL_DBGOSH << __func__ <<"CELLS_TYPES" <<std::endl;
#endif // DEBUG

  os << "CELL_TYPES " << tri_list->size() <<std::endl;  
  
  for(typename std::vector<PrivateTriangle<T>*>::size_type i=0;
      i<tri_list->size();i++){
    os<< "5";
    if ( (i+1)%10==0){
      os<< std::endl;  
    } else {
      os<<" ";
    }

  }
  os <<std::endl;  


#ifdef DEBUG
    PL_DBGOSH << __func__ <<"SCALAR" <<std::endl;
#endif // DEBUG

  //SCALAR_DATA;
  //  const std::vector<Vertex<T>*>* vlistout=vertex_list->get_vertex_lists();
  
  // should be a DVertex
  DVertex<T>* dv=dynamic_cast<DVertex<T>*>(vlistout->at(0));
  DVertexManager* dvm=dv->DVM();

    os << "POINT_DATA " << vertex_list->size() <<std::endl;  
#ifdef DEBUG
    PL_DBGOSH << __func__ <<"SCALAR "<<dvm->nscalar() <<std::endl;
#endif // DEBUG

  for(int i=0;i<dvm->nscalar();++i){

    os << "SCALARS polylib_Scalar_data"<<i;

    if(sizeof(T)==4){
      os <<" float"<<std::endl;
     } else if(sizeof(T)==8) {
      os << " double"<<std::endl;
    } else {
      PL_ERROSH << "[ERROR]vtk:vtk_a_save():wrong data type with sizeof(T)="<<sizeof(T) << std::endl;
      return PLSTAT_VTK_IO_ERROR;
    }
    os<<"LOOKUP_TABLE default"<<std::endl;


    for(int j=0;j<vlistout->size();++j){
      DVertex<T> *dv=dynamic_cast<DVertex<T>*>( (*vlistout)[j] );
#if SCIENTIFIC_OUT
      os <<  std::setprecision(6)<< scientific << dv->get_scalar(i) <<std::endl;
#else
      os << std::setprecision(6) << dv->get_scalar(i)<<std::endl;
#endif
    }
  }


#ifdef DEBUG
    PL_DBGOSH << __func__ <<"VECTOR" <<std::endl;
#endif // DEBUG


  for(int i=0;i<dvm->nvector();++i){

#ifdef DEBUG
    PL_DBGOSH << __func__ <<"VECTOR "<<i << " " << dvm->nvector()<<std::endl;
#endif // DEBUG

    os << "VECTORS polylib_Vector_data"<<i; 

    if(sizeof(T)==4){
      os <<" float"<<std::endl;
    } else if(sizeof(T)==8) {
      os << " double"<<std::endl;
    } else {
      PL_ERROSH << "[ERROR]vtk:vtk_a_save():wrong data type with sizeof(T)="<<sizeof(T) << std::endl;
      return PLSTAT_VTK_IO_ERROR;
    }

    //    os<<"LOOKUP_TABLE default"<<std::endl;
    //vlistout=vertex_list->get_vertex_lists();
    for(int j=0;j<vlistout->size();++j){

#ifdef DEBUG
    PL_DBGOSH << __func__ <<"VECTOR "<<i << " " << dvm->nvector()<<std::endl;
#endif // DEBUG

      DVertex<T> *dv=dynamic_cast<DVertex<T>*>((*vlistout)[j]);

#ifdef DEBUG
    PL_DBGOSH << __func__ <<"VECTOR "<<i << " " << dvm->nvector()<<std::endl;
#endif // DEBUG

      Vec3<T> v;

#ifdef DEBUG
    PL_DBGOSH << __func__ <<"VECTOR "<<i << " " << dvm->nvector()<<std::endl;
#endif // DEBUG

      dv->get_vector(i,&v);

#ifdef DEBUG
    PL_DBGOSH << __func__ <<"VECTOR "<<i << " " << v<<std::endl;
#endif // DEBUG

#if SCIENTIFIC_OUT  
      os <<  std::setprecision(6)<< scientific << v <<std::endl;
#else
      os <<  std::setprecision(6)<< v <<std::endl;
#endif

#ifdef DEBUG
    PL_DBGOSH << __func__ <<"VECTOR end"<<i <<std::endl;
#endif // DEBUG

    }
  }


#ifdef DEBUG
    PL_DBGOSH << __func__ <<"FINISH" <<std::endl;
#endif // DEBUG

  return PLSTAT_OK;

  //#undef DEBUG
}
///////////////////////////////////////////////////////////////
  template <typename T>
POLYLIB_STAT vtk_b_save(
	VertexList<T>* vertex_list, 
	std::vector<PrivateTriangle<T>*> *tri_list,
	std::string fname){

#define DEBUG

#ifdef DEBUG
    PL_DBGOSH << __func__ << " start"<<std::endl;
#endif


  std::ofstream ofs(fname.c_str(), std::ios::out | std::ios::binary);
  if (ofs.fail()) {
    PL_ERROSH << "[ERROR]vtk:vtk_b_save():Can't open " << fname << std::endl;
    return PLSTAT_STL_IO_ERROR;
  }
  int inv = tt_check_machine_endian() == TT_LITTLE_ENDIAN ? 0 : 1;

  // header 
  

  ofs << "# vtk DataFile Version 2.0" <<std::endl;
  ofs << " title "<<std::endl;
  ofs << "BINARY"<<std::endl;
  ofs << "DATASET UNSTRUCTURED_GRID"<<std::endl;

#ifdef DEBUG
    PL_DBGOSH << __func__ << " end HEADER"<<std::endl;
#endif


  //points

  ofs << "POINTS " << vertex_list->size();

  if(sizeof(T)==4){
    ofs <<" float"<<std::endl;
  } else if(sizeof(T)==8) {
    ofs << " double"<<std::endl;
  } else {
    PL_ERROSH << "[ERROR]vtk:vtk_a_save():wrong data type sizeof(T)="<<sizeof(T) << std::endl;
    return PLSTAT_VTK_IO_ERROR;
  }

  // 頂点 v 出力
  const std::vector<Vertex<T>*>* vlistout=vertex_list->get_vertex_lists();
  for(int i=0;i<vlistout->size();++i){
    T buffer[3];
    buffer[0]= (*((*vlistout)[i]))[0];
    buffer[1]= (*((*vlistout)[i]))[1];
    buffer[2]= (*((*vlistout)[i]))[2];

    tt_write(ofs, buffer, sizeof(T), 3, inv);

  }
  ofs << std::endl;

#ifdef DEBUG
    PL_DBGOSH << __func__ << " end POINTS"<<std::endl;
#endif


  //cells

  ofs << "CELLS " << tri_list->size() << " "<< 4*tri_list->size()<<std::endl;
  typename std::vector<PrivateTriangle<T>*>::size_type index=0;  
  typename std::vector<PrivateTriangle<T>*>::iterator itr;
  for (itr = tri_list->begin(); itr != tri_list->end(); itr++) {
    Vertex<T>** tmpvtx=(*itr)->get_vertex();
    int i0,i1,i2;
    /* i0=vertex_list->ith(tmpvtx[0]); */
    /* i1=vertex_list->ith(tmpvtx[1]); */
    /* i2=vertex_list->ith(tmpvtx[2]); */
    i0=vertex_list->vtx_index(tmpvtx[0]);
    i1=vertex_list->vtx_index(tmpvtx[1]);
    i2=vertex_list->vtx_index(tmpvtx[2]);


    if(i0==-1 || i1==-1 || i2==-1) {
      PL_ERROSH << "[ERROR]vtk:vtk_a_save():wrong vertex id " 
		<< i0 << " "<< i1 << " "<< i2 << " "<< std::endl;
    }
    // vertex indexes are start from 0

    typename std::vector<PrivateTriangle<T>*>::size_type buff[4];
    //    buff[0]=index;
    buff[0]=3;
    buff[1]=i0;
    buff[2]=i1;
    buff[3]=i2;

    tt_write(ofs,buff,
	     sizeof(typename std::vector<PrivateTriangle<T>*>::size_type),
	     4, inv);
    

    index++;    
  }
  ofs <<std::endl;  


#ifdef DEBUG
    PL_DBGOSH << __func__ << " end CELLS"<<std::endl;
#endif


  //clear indexmap
  vertex_list->index_map_clear();

  //celltypes
  ofs << "CELL_TYPES " << tri_list->size() <<std::endl;  

  for(typename std::vector<PrivateTriangle<T>*>::size_type i=0;
      i<tri_list->size();i++){
    int tmp=5;
    tt_write(ofs,&tmp,sizeof(int),1,inv);
    //tt_write(ofs,tmp,sizeof(tmp),1,inv);
  }
  ofs <<std::endl;  



#ifdef DEBUG
    PL_DBGOSH << __func__ << " end CELL_TYPES"<<std::endl;
#endif

  //pointdata
  ofs << "POINT_DATA " << vertex_list->size() <<std::endl;  
  DVertex<T>* dv=dynamic_cast<DVertex<T>*>(vlistout->at(0));
  DVertexManager* dvm=dv->DVM();

  for(int i=0;i<dvm->nscalar();++i){

    ofs << "SCALARS polylib_Scalar_data "<<i;

    if(sizeof(T)==4){
      ofs <<" float"<<std::endl;
    } else if(sizeof(T)==8) {
      ofs << " double"<<std::endl;
    } else {
      PL_ERROSH << "[ERROR]vtk:vtk_a_save():wrong data type with sizeof(T)="<<sizeof(T) << std::endl;
      return PLSTAT_VTK_IO_ERROR;
    }
    ofs<<"LOOKUP_TABLE default"<<std::endl;
    vlistout=vertex_list->get_vertex_lists();
    for(int j=0;j<vlistout->size();++j){
      DVertex<T> *dv=dynamic_cast<DVertex<T>*>( (*vlistout)[j] );
      //      T tmp=dv->get_scalar(i);
      T tmp=dv->get_scalar(i);

#ifdef DEBUG
      PL_DBGOSH << __func__ << " i,j "<<i<<" "<<j<<" "<<tmp <<std::endl;
#endif

      tt_write(ofs, &tmp,sizeof(T),1,inv);
    }
    ofs <<std::endl;  
  }

#ifdef DEBUG
    PL_DBGOSH << __func__ << " end SCALARS"<<std::endl;
#endif



    for(int i=0;i<dvm->nvector();++i){

      ofs << "VECTORS polylib_Vector_data"<<i;

      if(sizeof(T)==4){
	ofs <<" float"<<std::endl;
      } else if(sizeof(T)==8) {
	ofs << " double"<<std::endl;
      } else {
	PL_ERROSH << "[ERROR]vtk:vtk_a_save():wrong data type with sizeof(T)="<<sizeof(T) << std::endl;
	return PLSTAT_VTK_IO_ERROR;
      }
      //  ofs<<"LOOKUP_TABLE default"<<std::endl;
      vlistout=vertex_list->get_vertex_lists();
      for(int j=0;j<vlistout->size();++j){

	DVertex<T> *dv=dynamic_cast<DVertex<T>*>( (*vlistout)[j] );
	Vec3<T> vec3 ;
	dv->get_vector(i,&vec3);
#ifdef DEBUG
	PL_DBGOSH << __func__ << " i,j "<<i<<" "<<j<<" "<< vec3<<std::endl;
#endif

	T ovector[3];
	ovector[0]=vec3[0];
	ovector[1]=vec3[1];
	ovector[2]=vec3[2];
	tt_write(ofs, ovector,sizeof(T),3,inv);
      }
      ofs <<std::endl;  
    }


#ifdef DEBUG
    PL_DBGOSH << __func__ << " end VECTORS"<<std::endl;
#endif

#undef DEBUG

  return PLSTAT_OK;
}



} // end of namespace

#endif //vtk_h
