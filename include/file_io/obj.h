/* -- Mode: c++ --*/
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

#ifndef polylib_obj_h
#define polylib_obj_h

#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <iomanip>

#include "common/PolylibCommon.h"
#include "polygons/Triangle.h"
#include "polygons/Vertex.h"
#include "polygons/VertexList.h"

#include "file_io/stl.h"

#if HAVE_STRING_H 
#include <string.h>
#else
// write some code here
#endif

namespace PolylibNS {

  ///  ファイルの一部を読み込み、ascii / binary を判定する。
  bool is_obj_a(std::string path);

/// 
/// ASCII モードのOBJファイルを読み込み、VertexList, tri_listに三角形ポリゴン情報を設定する。
/// 
///
///  @param[in,out] vertex_list 頂点リストの領域。
///  @param[in,out] tri_list	三角形ポリゴンリストの領域。
///  @param[in]		fname		STLファイル名。
///  @param[in,out] total		ポリゴンIDの通番。
///  @return	POLYLIB_STATで定義される値が返る。
///
///  エラーについて
///  1. ファイルが開けないとき
///  2. face のリストがまだ読み込まれていない頂点IDを使った場合
///  
///  注意事項
///  faceはすべて三角形だとして読み込む。
///  情報として取り込むのは、v と f のみで、他の情報は破棄される。
///  v と fの情報から、normalを計算する。

template <typename T>
POLYLIB_STAT obj_a_load(
	VertexList<T>* vertex_list, 
	std::vector<PrivateTriangle<T>*> *tri_list,
	std::string fname,
	int	*total,
	T	scale=1.0
);

/// 
/// ASCII モードのOBJファイルを読み込み、VertexList, tri_listに三角形ポリゴン情報を設定する。
/// 
///
///  @param[in,out] vertex_list 頂点リストの領域。
///  @param[in,out] tri_list	三角形ポリゴンリストの領域。
///  @param[in]		fname		STLファイル名。
///  @param[in,out] total		ポリゴンIDの通番。
///  @return	POLYLIB_STATで定義される値が返る。
///
///  エラーについて
///  1. ファイルが開けないとき
///  2. face のリストがまだ読み込まれていない頂点IDを使った場合
///  
///  注意事項
///  faceはすべて三角形だとして読み込む。
///  情報として取り込むのは、v と f のみで、他の情報は破棄される。
///  v と fの情報から、normalを計算する。

template <typename T>
POLYLIB_STAT obj_b_load(
	VertexList<T>* vertex_list, 
	std::vector<PrivateTriangle<T>*> *tri_list,
	std::string fname,
	int	*total,
	T	scale=1.0
);

/// 
/// VertexList, tri_listにから OBJアスキー形式に出力する。
//  尚、頂点法線を計算して出力する。
/// 
///
///  @param[in] vertex_list 頂点リストの領域。
///  @param[in] tri_list　三角形ポリゴンリストの領域。
///  @param[in]	fname	　ファイル名。
///
///  エラーについて
///  
///  注意事項
///  情報として書き出すのは、v, vn と f のみで、他の情報は破棄される。
///  すべての面を持たない頂点の頂点法線は、正しく計算されない可能性があるので、注意すること。

template <typename T>
POLYLIB_STAT obj_a_save(
	VertexList<T>* vertex_list, 
	std::vector<PrivateTriangle<T>*> *tri_list,
	std::string fname
);

/// 
/// VertexList, tri_listにから OBJバイナリ形式に出力する。
//  尚、頂点法線を計算して出力する。
///
///  @param[in] vertex_list 頂点リストの領域。
///  @param[in] tri_list　三角形ポリゴンリストの領域。
///  @param[in]	fname	　ファイル名。
///
///  エラーについて
///  
///  注意事項
///  情報として書き出すのは、v と f のみで、他の情報は破棄される。


template <typename T>
POLYLIB_STAT obj_b_save(
	VertexList<T>* vertex_list, 
	std::vector<PrivateTriangle<T>*> *tri_list,
	std::string fname
);
 

/// 
/// VertexList, tri_listにから OBJバイナリ形式に出力する。
//  尚、頂点法線を計算して出力する。
///
///  @param[in] vertex_list 頂点リストの領域。
///  @param[in] tri_list　三角形ポリゴンリストの領域。
///  @param[in]	fname	　ファイル名。
///
///  エラーについて
///  
///  注意事項
///  情報として書き出すのは、v, vn と f のみで、他の情報は破棄される。
///  すべての面を持たない頂点の頂点法線は、正しく計算されない可能性があるので、注意すること。

template <typename T>
POLYLIB_STAT obj_bb_save(
	VertexList<T>* vertex_list, 
	std::vector<PrivateTriangle<T>*> *tri_list,
	std::string fname
);
 

//////////////////////////////////////////////////////////////////////////////
// POLYLIB_STAT obj_a_load VertexList version.
//////////////////////////////////////////////////////////////////////////////
 template <typename T>
 POLYLIB_STAT obj_a_load(VertexList<T>* vertex_list, 
			 std::vector<PrivateTriangle<T>*>*tri_list, 
			 std::string	fname,
			 int	*total,
			 T scale ) 
{
  PL_DBGOSH << "fname " <<fname<<std::endl;
   std::ifstream is(fname.c_str());
   if (is.fail()) {
     PL_ERROSH << "[ERROR]obj:obj_a_load():Can't open " << fname << std::endl;
     return PLSTAT_OBJ_IO_ERROR;
   }

    int n_tri = *total;		// 通番の初期値をセット
    int n_vtx = 0;
    int ivtx=0;

    std::string token;
    Vec3<T> nml;
    Vec3<T> vtx[3];

    while (is >> token && !is.eof()) {
      if (token[0] == '#') { //comment through end of the line.
	//if (token == "#") { //comment through end of the line.
	std::string s;
	getline(is,s);
	  //	is >> s;
	//PL_DBGOSH << "comment" <<s<<std::endl;
      }
      else if (token == "v") { //geometric vertices
	//Vec3<T> v;
	Vertex<T>* v=new Vertex<T>;
	is >> *v;
	//vertex_list->vtx_add(v);
	vertex_list->vtx_add_KDT(v);
	PL_DBGOSH << "vertex " <<*v<<std::endl;
      }
      else if (token == "f") {	  // face 
	int ii[3];
	for(int i=0;i<3;i++){
	  std::stringstream ss;
	  std::string s;
	  is>>s;

	  std::string::size_type pos = s.find("/");
	  if(pos!=std::string::npos) {
	    std::string tmp(s,0,pos);
	    s=tmp;
	  }
	  //PL_DBGOSH << "face |" <<s << "| pos "<< pos <<std::endl;
	  
	  ss <<s ;
	  ss>>ii[i];
	  if (vertex_list->size() < ii[i]){
	    PL_ERROSH << "[ERROR]obj:obj_a_load():error reading file " << fname << std::endl;
	    PL_ERROSH << "Face uses bigger vertex id than the size of vertex list." << std::endl;  
	    return PLSTAT_OBJ_IO_ERROR;
	  }

	
	}
	Vertex<T>* tmpvertlist[3];

	tmpvertlist[0]=vertex_list->ith(ii[0]-1);
	tmpvertlist[1]=vertex_list->ith(ii[1]-1);
	tmpvertlist[2]=vertex_list->ith(ii[2]-1);

	PrivateTriangle<T>* tri = new PrivateTriangle<T>(tmpvertlist, n_tri);
	if(tri->get_area()==0){
	  PL_DBGOSH << __func__ 
		    << " Warning :  obj file contains a triangle of the area is zero." << std::endl;
	  PL_DBGOSH <<  "vertex0 ("<< *(tmpvertlist[0]) <<")"<<std::endl;
	  PL_DBGOSH <<  "vertex1 ("<< *(tmpvertlist[1]) <<")"<<std::endl;	
	  PL_DBGOSH <<  "vertex2 ("<< *(tmpvertlist[2]) <<")"<<std::endl;
	  }
	
	tri_list->push_back(tri);
	n_tri++;

      }
      else if (token == "g") {	//groupname 
	std::string s;
	is >> s;
      }	else if (token == "vn") { 
	std::string s;
	getline(is,s);
	  //	is >> s;
	//PL_DBGOSH << "vn " <<s<<std::endl;

      }	else if (token == "vp") { 
	std::string s;
	getline(is,s);
	  //	is >> s;
	//PL_DBGOSH << "vp " <<s<<std::endl;

      }	else if (token == "vt") { 
	std::string s;
	getline(is,s);
	  //	is >> s;
	//PL_DBGOSH << "vt " <<s<<std::endl;

      } else {
	// I must go to the end of line.
	std::string s;
	getline(is,s);
	PL_DBGOSH <<  " why I am here? |"<< token << "|　|"<<s<<"|="<< std::endl;
      }
    } // end of reading file.


    if (!is.eof() && is.fail()) {
      PL_ERROSH << "[ERROR]obj:obj_a_load():Error in loading: " << fname << std::endl;
      return PLSTAT_OBJ_IO_ERROR;
    }

    *total = n_tri;		// 更新した通番をセット

#ifdef DEBUG
    PL_DBGOSH <<  "obj_a_load total=" << *total << std::endl;
#endif

    // vertex_list->print();
    // for(typename std::vector<PrivateTriangle<T>*>::iterator iter=tri_list->begin();
    //     iter!=tri_list->end();
    //     iter++
    //     ){
    //   Vertex<T>** tmp=(*iter)->get_vertex();
    //   std::cout << "last 0 "<<*(tmp[0])<<std::endl;
    //   std::cout << "last 1 "<<*(tmp[1])<<std::endl;
    //   std::cout << "last 2 "<<*(tmp[2])<<std::endl;
    // }

    return PLSTAT_OK;
  }



 /////////////////////////////////////////////////////
template <typename T>
POLYLIB_STAT obj_a_save(
			VertexList<T>* vertex_list, 
			std::vector<PrivateTriangle<T>*> *tri_list,
			std::string fname
			){

  std::ofstream os(fname.c_str());
  if (os.fail()) {
    PL_ERROSH << "[ERROR]obj:obj_a_save():Can't open " << fname << std::endl;
    return PLSTAT_OBJ_IO_ERROR;
  }

  // 頂点 v 出力
 const std::vector<Vertex<T>*>* vlistout=vertex_list->get_vertex_lists();
  for(int i=0;i<vlistout->size();++i){
#if SCIENTIFIC_OUT
    os << "v " << std::setprecision(6)<< scientific <<*((*vlistout)[i])<<std::endl;
#else
      os << "v " << std::setprecision(6) <<*((*vlistout)[i])<<std::endl;
#endif
  }

  // 法線　vn 出力
  std::map<Vertex<T>*,Vec3<T> > vnmap;
  typename std::map<Vertex<T>*,Vec3<T> >::iterator vnmap_iter;
  typename std::vector<PrivateTriangle<T>*>::iterator itr;

  for (itr = tri_list->begin(); itr != tri_list->end(); itr++) {
    Vertex<T>** tmpvtx=(*itr)->get_vertex();
    Vec3<T> normal=(*itr)->get_normal();
    for(int i=0;i<3;++i){
      // Vertex*  をkeyにして法線ベクトル（計算途中）を探し、無ければ加える。
      vnmap_iter=vnmap.find(tmpvtx[i]);
      if(vnmap_iter==vnmap.end()) {
  	Vec3<T> newnormal(0,0,0);
  	vnmap[tmpvtx[i]]=newnormal;
      }
      // 頂点ごとに面法線の合計
      vnmap[tmpvtx[i]]+=normal;
    }
  }

  for(vnmap_iter=vnmap.begin();
      vnmap_iter!=vnmap.end();
      vnmap_iter++){

    Vec3<T> out = (vnmap_iter->second).normalize();

#if SCIENTIFIC_OUT
    os << "vn " << std::setprecision(6)<< scientific << out <<std::endl;
#else
      os << "vn " << std::setprecision(6) << out <<std::endl;
#endif
  }

  // 面 f 出力 すべての面を持っていない頂点（境界付近）は、
  // 頂点法線が正しくない可能性があるので注意すること.

  for (itr = tri_list->begin(); itr != tri_list->end(); itr++) {
    Vertex<T>** tmpvtx=(*itr)->get_vertex();
    int i0,i1,i2;
    i0=vertex_list->ith(tmpvtx[0]);
    i1=vertex_list->ith(tmpvtx[1]);
    i2=vertex_list->ith(tmpvtx[2]);
    if(i0==-1 || i1==-1 || i2==-1) {
      PL_ERROSH << "[ERROR]obj:obj_a_save():wrong vertex id " 
		<< i0 << " "<< i1 << " "<< i2 << " "<< std::endl;
    }
    os << "f " 
       << (i0+1)<<"//"<< (i0+1) << " " 
       << (i1+1)<<"//"<< (i1+1) << " " 
       << (i2+1)<<"//"<< (i2+1) << std::endl;
  }


}
///////////////////////////////////////////////////////////////
template <typename T>
POLYLIB_STAT obj_b_save(
	VertexList<T>* vertex_list, 
	std::vector<PrivateTriangle<T>*> *tri_list,
	std::string fname){

  std::ofstream ofs(fname.c_str(), std::ios::out | std::ios::binary);
  if (ofs.fail()) {
    PL_ERROSH << "[ERROR]obj:obj_b_save():Can't open " << fname << std::endl;
    return PLSTAT_STL_IO_ERROR;
  }
  int inv = tt_check_machine_endian() == TT_LITTLE_ENDIAN ? 0 : 1;

  char buf[STL_HEAD];
  for (int i = 0; i < STL_HEAD; i++) buf[i] = 0;
  strcpy(buf, "OBJ_BIN TRIA COND_ID");
  tt_write(ofs, buf, 1, STL_HEAD, inv);

  //uint element_vertex = vertex_list->size();
  //tt_write(ofs, &element_vertex, sizeof(uint), 1, inv);
  ulong element_vertex = vertex_list->size();
  tt_write(ofs, &element_vertex, sizeof(ulong), 1, inv);
  //  uint element = tri_list->size();
  //tt_write(ofs, &element, sizeof(uint), 1, inv);
  ulong element = tri_list->size();
  tt_write(ofs, &element, sizeof(ulong), 1, inv);

  // 頂点 v 出力
 const std::vector<Vertex<T>*>* vlistout=vertex_list->get_vertex_lists();
  for(int i=0;i<vlistout->size();++i){
    Vertex<T>*  vert_tmp=(*vlistout)[i];
    float float_vertex[3];
    float_vertex[0]=(*vert_tmp)[0];
    float_vertex[1]=(*vert_tmp)[1];
    float_vertex[2]=(*vert_tmp)[2];
    tt_write(ofs, float_vertex, sizeof(float), 3, inv);
  }

  // 面 f 出力 
  typename std::vector<PrivateTriangle<T>*>::iterator itr;
  for (itr = tri_list->begin(); itr != tri_list->end(); itr++) {
    Vertex<T>** tmpvtx=(*itr)->get_vertex();
    long index[3];
    index[0]=vertex_list->ith(tmpvtx[0]);
    index[1]=vertex_list->ith(tmpvtx[1]);
    index[2]=vertex_list->ith(tmpvtx[2]);
    if(index[0]==-1 || index[1]==-1 || index[2]==-1) {
      PL_ERROSH << "[ERROR]obj:obj_b_save():wrong vertex id " 
		<< index[0] << " "<< index[1] << " "<< index[2] << " "<< std::endl;
    }
    // index starts from 1
    ++index[0]; ++index[1]; ++index[2];
    tt_write(ofs, index , sizeof(long), 3, inv);

    int exid = (*itr)->get_exid();
    tt_write(ofs, &exid, sizeof(ushort), 1, inv);

  }


  if (!ofs.eof() && ofs.fail()) {
    PL_ERROSH << "[ERROR]obj:obj_b_load():Error in saving: " << fname << std::endl;
    return PLSTAT_OBJ_IO_ERROR;
  }

    return PLSTAT_OK;

}

///////////////////////////////////////////////////////////////
template <typename T>
POLYLIB_STAT obj_bb_save(
	VertexList<T>* vertex_list, 
	std::vector<PrivateTriangle<T>*> *tri_list,
	std::string fname){

  std::ofstream ofs(fname.c_str(), std::ios::out | std::ios::binary);
  if (ofs.fail()) {
    PL_ERROSH << "[ERROR]obj:obj_b_save():Can't open " << fname << std::endl;
    return PLSTAT_STL_IO_ERROR;
  }
  int inv = tt_check_machine_endian() == TT_LITTLE_ENDIAN ? 0 : 1;

  char buf[STL_HEAD];
  for (int i = 0; i < STL_HEAD; i++) buf[i] = 0;
  strcpy(buf, "OBJ_BIN TRIA V_NORMAL COND_ID");
  tt_write(ofs, buf, 1, STL_HEAD, inv);

  //uint element_vertex = vertex_list->size();
  //tt_write(ofs, &element_vertex, sizeof(uint), 1, inv);
  ulong element_vertex = vertex_list->size();
  tt_write(ofs, &element_vertex, sizeof(ulong), 1, inv);
  //  uint element = tri_list->size();
  //tt_write(ofs, &element, sizeof(uint), 1, inv);
  ulong element = tri_list->size();
  tt_write(ofs, &element, sizeof(ulong), 1, inv);

  // 頂点 v 出力
 const std::vector<Vertex<T>*>* vlistout=vertex_list->get_vertex_lists();
  for(int i=0;i<vlistout->size();++i){
    Vertex<T>*  vert_tmp=(*vlistout)[i];
    float float_vertex[3];
    float_vertex[0]=(*vert_tmp)[0];
    float_vertex[1]=(*vert_tmp)[1];
    float_vertex[2]=(*vert_tmp)[2];
    tt_write(ofs, float_vertex, sizeof(float), 3, inv);
  }


  // 法線　vn 出力
  std::map<Vertex<T>*,Vec3<T> > vnmap;
  typename std::map<Vertex<T>*,Vec3<T> >::iterator vnmap_iter;
  typename std::vector<PrivateTriangle<T>*>::iterator itr;

  for (itr = tri_list->begin(); itr != tri_list->end(); itr++) {
    Vertex<T>** tmpvtx=(*itr)->get_vertex();
    Vec3<T> normal=(*itr)->get_normal();
    for(int i=0;i<3;++i){
      // Vertex*  をkeyにして法線ベクトル（計算途中）を探し、無ければ加える。
      vnmap_iter=vnmap.find(tmpvtx[i]);
      if(vnmap_iter==vnmap.end()) {
  	Vec3<T> newnormal(0,0,0);
  	vnmap[tmpvtx[i]]=newnormal;
      }
      // 頂点ごとに面法線の合計
      vnmap[tmpvtx[i]]+=normal;
    }
  }

  for(vnmap_iter=vnmap.begin();
      vnmap_iter!=vnmap.end();
      vnmap_iter++){

    Vec3<T> out = (vnmap_iter->second).normalize();
    float tmp[3];
    tmp[0]=out[0];
    tmp[1]=out[1];
    tmp[2]=out[2];

    tt_write(ofs, tmp, sizeof(float), 3, inv);

  }

  // 面 f 出力 
  //  typename std::vector<PrivateTriangle<T>*>::iterator itr;
  for (itr = tri_list->begin(); itr != tri_list->end(); itr++) {
    Vertex<T>** tmpvtx=(*itr)->get_vertex();
    long index[3];
    index[0]=vertex_list->ith(tmpvtx[0]);
    index[1]=vertex_list->ith(tmpvtx[1]);
    index[2]=vertex_list->ith(tmpvtx[2]);
    if(index[0]==-1 || index[1]==-1 || index[2]==-1) {
      PL_ERROSH << "[ERROR]obj:obj_bb_save():wrong vertex id " 
		<< index[0] << " "<< index[1] << " "<< index[2] << " "<< std::endl;
    }
    // index starts from 1
    ++index[0]; ++index[1]; ++index[2];
    tt_write(ofs, index , sizeof(long), 3, inv);

    int exid = (*itr)->get_exid();
    tt_write(ofs, &exid, sizeof(ushort), 1, inv);

  }


  if (!ofs.eof() && ofs.fail()) {
    PL_ERROSH << "[ERROR]obj:obj_bb_save():Error in saving: " << fname << std::endl;
    return PLSTAT_OBJ_IO_ERROR;
  }
    return PLSTAT_OK;
}




/////////////////////////////////////////////////
////////////////////////////

template <typename T>
POLYLIB_STAT obj_b_load(
			VertexList<T> *vertex_list,
			std::vector<PrivateTriangle<T>*> *tri_list, 
			std::string fname,
			int	*total,
			T	scale
) {
 std::ifstream ifs(fname.c_str(), std::ios::in | std::ios::binary);
	if (ifs.fail()) {
		PL_ERROSH << "[ERROR]stl:stl_b_load():Can't open " << fname << std::endl;
		return PLSTAT_STL_IO_ERROR;
	}

	int inv = tt_check_machine_endian() == TT_LITTLE_ENDIAN ? 0 : 1;

	int	n_tri = *total;		// 通番の初期値をセット

	char buf[STL_HEAD];
	for (int i = 0; i < STL_HEAD; i++) buf[i] = 0;
	tt_read(ifs, buf, sizeof(char), STL_HEAD, inv);

	bool withnormal=false;
	int icheck, icheck2;
	icheck = strncmp( buf, "OBJ_BIN TRIA V_NORMAL COND_ID",29);

	if(icheck!=0){
	  icheck2 = strncmp( buf, "OBJ_BIN TRIA COND_ID",20);
	  if(icheck2!=0){
	    PL_ERROSH << "[ERROR]obj:obj_b_load():Error in loading: " << fname 
		      <<" file header error!!" << std::endl;
	        return PLSTAT_OBJ_IO_ERROR; 
	  }
	} else {
	  withnormal=true;
	}
	ulong element_vertex;
	ulong element;
	
	tt_read(ifs, &element_vertex, sizeof(ulong), 1, inv);
	tt_read(ifs, &element, sizeof(ulong), 1, inv);

	for(ulong i=0;i<element_vertex;++i){
	  float float_vertex[3];
	  tt_read(ifs, float_vertex, sizeof(float), 3, inv);
	  Vertex<T>* v = new Vertex<T>;
	  (*v)[0]=float_vertex[0];
	  (*v)[1]=float_vertex[1];
	  (*v)[2]=float_vertex[2];
	  // 
	  Vertex<T>* tmp=vertex_list->vtx_add_KDT(v);
	    //if(tmp!=v) delete v;

	}
	if(withnormal){ // 頂点法線は捨てる
	  for(ulong i=0;i<element_vertex;++i){
	    float float_vertex[3];
	    tt_read(ifs, float_vertex, sizeof(float), 3, inv);
	  }	  
	}
	for(ulong i=0;i<element;++i){
	    long index[3];
	    tt_read(ifs, index , sizeof(long), 3, inv);
	    int exid;
	    tt_read(ifs, &exid, sizeof(ushort), 1, inv);
	    Vertex<T>* tmpvlist[3];
	    tmpvlist[0] = vertex_list->ith(index[0]);
	    tmpvlist[1] = vertex_list->ith(index[1]);
	    tmpvlist[2] = vertex_list->ith(index[2]);

	    PrivateTriangle<T>* tri=new PrivateTriangle<T>(vertex_list,
							   exid);
	      
	    tri_list->push_back(tri);
	  }

	  
	  
	  

    return PLSTAT_OK;
 }


}// end of namespace PolylibNS





#endif //obj_h
