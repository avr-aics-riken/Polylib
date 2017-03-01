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


#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#if HAVE_STRING_H
#include <string.h>
#else
//alternative codes here
#endif

#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
#include "common/tt.h"
#include "polygons/Triangle.h"
#include "file_io/TriMeshIO.h"
#include "file_io/obj.h"
#include "file_io/stl.h"

namespace PolylibNS {


//////////////////////////////////////////////////////////////////////////////
bool is_obj_a(std::string path)
{
	const char  dcs[] = "\n\r";
	char		buff[STL_BUFF_LEN];
	size_t		i = 0;
	char		c;

	std::ifstream ifs(path.c_str());
	if (!ifs)				   return false;

	// ファイル内容の一部を読み込み
	memset(buff, 0, STL_BUFF_LEN);
	while (i < STL_BUFF_LEN - 1 && ifs.get(c) && !strchr(dcs, c)) {
		buff[i++] = c;
	}

	int icheck, icheck2;
	icheck = strncmp( buff, "OBJ_BIN TRIA V_NORMAL COND_ID",29);
	if (icheck ==0) return false ;
	icheck2 = strncmp( buff, "OBJ_BIN TRIA COND_ID",20);
	if (icheck2 ==0) return false ;

	if(buff[0]=='#') return true;
	if(buff[0]=='v') return true;

	return false;
}

//////////////////////////////////////////////////////////////////////////////
// POLYLIB_STAT obj_a_load VertexList version.
//////////////////////////////////////////////////////////////////////////////

POLYLIB_STAT obj_a_load(VertexList* vertex_list,
	std::vector<PrivateTriangle*>*tri_list,
	std::string	fname,
	int	*total,
	PL_REAL scale )
{
	// PL_DBGOSH << "fname " <<fname<<std::endl;
	std::ifstream is(fname.c_str());
	if (is.fail()) {
		PL_ERROSH << "[ERROR]obj:obj_a_load():Can't open " << fname << std::endl;
		return PLSTAT_OBJ_IO_ERROR;
	}

	int n_tri = *total;		// 通番の初期値をセット
	int n_vtx = 0;
	int ivtx=0;
	int n_zero_area_tri=0;
	std::string token;
	Vec3<PL_REAL> nml;
	Vec3<PL_REAL> vtx[3];

	while (is >> token && !is.eof()) {
		if (token[0] == '#') { //comment through end of the line.
			//if (token == "#") { //comment through end of the line.
			std::string s;
			getline(is,s);
			//	is >> s;
			//PL_DBGOSH << "comment" <<s<<std::endl;
		}
		else if (token == "v") { //geometric vertices
			//Vec3<PL_REAL> v;
			Vertex* v=new Vertex();
			is >> *v;
			//vertex_list->vtx_add(v);
			//vertex_list->vtx_add_KDT(v);
			vertex_list->vtx_add_nocheck(v);
			//PL_DBGOSH << "vertex " <<*v<<std::endl;
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
					PL_ERROSH << "Face uses bigger vertex id than the size of vertex list. " << i << " " <<ii[i]<< std::endl;

					return PLSTAT_OBJ_IO_ERROR;
				}


			}
			Vertex* tmpvertlist[3];

			tmpvertlist[0]=vertex_list->ith(ii[0]-1);
			tmpvertlist[1]=vertex_list->ith(ii[1]-1);
			tmpvertlist[2]=vertex_list->ith(ii[2]-1);


			PrivateTriangle* tri = new PrivateTriangle(tmpvertlist, n_tri);
			if(tri->get_area()==0.0){

				PL_DBGOSH << __func__
					<< " Warning :  obj file contains a triangle of the area is zero." << std::endl;
				PL_DBGOSH <<  "vertex0 ("<< *(tmpvertlist[0]) <<")"<<std::endl;
				PL_DBGOSH <<  "vertex1 ("<< *(tmpvertlist[1]) <<")"<<std::endl;
				PL_DBGOSH <<  "vertex2 ("<< *(tmpvertlist[2]) <<")"<<std::endl;
				n_zero_area_tri++;

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



	if(n_zero_area_tri!=0){
		PL_DBGOSH <<  "# of zero area Triangles "<< n_zero_area_tri <<std::endl;
	}


	if (!is.eof() && is.fail()) {
		PL_ERROSH << "[ERROR]obj:obj_a_load():Error in loading: " << fname << std::endl;
		return PLSTAT_OBJ_IO_ERROR;
	}

	*total = n_tri;		// 更新した通番をセット

#ifdef DEBUG
	PL_DBGOSH <<  "obj_a_load total=" << *total << std::endl;
#endif

	// vertex_list->print();
	// for(std::vector<PrivateTriangle*>::iterator iter=tri_list->begin();
	//     iter!=tri_list->end();
	//     iter++
	//     ){
	//   Vertex** tmp=(*iter)->get_vertex();
	//   std::cout << "last 0 "<<*(tmp[0])<<std::endl;
	//   std::cout << "last 1 "<<*(tmp[1])<<std::endl;
	//   std::cout << "last 2 "<<*(tmp[2])<<std::endl;
	// }

	return PLSTAT_OK;
}



/////////////////////////////////////////////////////

POLYLIB_STAT obj_a_save(
	VertexList* vertex_list,
	std::vector<PrivateTriangle*> *tri_list,
	std::string fname
	){

		std::ofstream os(fname.c_str());
		if (os.fail()) {
			PL_ERROSH << "[ERROR]obj:obj_a_save():Can't open " << fname << std::endl;
			return PLSTAT_OBJ_IO_ERROR;
		}

		// 頂点 v 出力
		const std::vector<Vertex*>* vlistout=vertex_list->get_vertex_lists();
		for(int i=0;i<vlistout->size();++i){
#if SCIENTIFIC_OUT
			os << "v " << std::setprecision(6)<< scientific <<*((*vlistout)[i])<<std::endl;
#else
			os << "v " << std::setprecision(6) <<*((*vlistout)[i])<<std::endl;
#endif
		}

		// 法線　vn 出力
		std::map<Vertex*,Vec3<PL_REAL> > vnmap;
		std::map<Vertex*,Vec3<PL_REAL> >::iterator vnmap_iter;
		std::vector<PrivateTriangle*>::iterator itr;

		for (itr = tri_list->begin(); itr != tri_list->end(); itr++) {
			Vertex** tmpvtx=(*itr)->get_vertex();
			Vec3<PL_REAL> normal=(*itr)->get_normal();
			for(int i=0;i<3;++i){
				// Vertex*  をkeyにして法線ベクトル（計算途中）を探し、無ければ加える。
				vnmap_iter=vnmap.find(tmpvtx[i]);
				if(vnmap_iter==vnmap.end()) {
					Vec3<PL_REAL> newnormal(0,0,0);
					vnmap[tmpvtx[i]]=newnormal;
				}
				// 頂点ごとに面法線の合計
				vnmap[tmpvtx[i]]+=normal;
			}
		}

		for(vnmap_iter=vnmap.begin();
			vnmap_iter!=vnmap.end();
			vnmap_iter++){

				Vec3<PL_REAL> out = (vnmap_iter->second).normalize();

#if SCIENTIFIC_OUT
				os << "vn " << std::setprecision(6)<< scientific << out <<std::endl;
#else
				os << "vn " << std::setprecision(6) << out <<std::endl;
#endif
		}

		// 面 f 出力 すべての面を持っていない頂点（境界付近）は、
		// 頂点法線が正しくない可能性があるので注意すること.

		for (itr = tri_list->begin(); itr != tri_list->end(); itr++) {
			Vertex** tmpvtx=(*itr)->get_vertex();
			int i0,i1,i2;
			/* i0=vertex_list->ith(tmpvtx[0]); */
			/* i1=vertex_list->ith(tmpvtx[1]); */
			/* i2=vertex_list->ith(tmpvtx[2]); */
			i0=vertex_list->vtx_index(tmpvtx[0]);
			i1=vertex_list->vtx_index(tmpvtx[1]);
			i2=vertex_list->vtx_index(tmpvtx[2]);


			if(i0==-1 || i1==-1 || i2==-1) {
				PL_ERROSH << "[ERROR]obj:obj_a_save():wrong vertex id "
					<< i0 << " "<< i1 << " "<< i2 << " "<< std::endl;
			}
			os << "f "
				<< (i0+1)<<"//"<< (i0+1) << " "
				<< (i1+1)<<"//"<< (i1+1) << " "
				<< (i2+1)<<"//"<< (i2+1) << std::endl;
		}

		vertex_list->index_map_clear();

		return PLSTAT_OK;
}
///////////////////////////////////////////////////////////////

POLYLIB_STAT obj_b_save(
	VertexList* vertex_list,
	std::vector<PrivateTriangle*> *tri_list,
	std::string fname){

		std::ofstream ofs(fname.c_str(), std::ios::out | std::ios::binary);
		if (ofs.fail()) {
			PL_ERROSH << "[ERROR]obj:obj_b_save():Can't open " << fname << std::endl;
			return PLSTAT_STL_IO_ERROR;
		}
		int inv = tt_check_machine_endian() == TT_LITTLE_ENDIAN ? 0 : 1;
#ifdef DEBUG
#undef DEBUG
#endif
		//#define DEBUG
#ifdef DEBUG
		PL_DBGOSH << __func__ << " in. "<<fname <<std::endl;
#endif

		char buf[STL_HEAD];
		for (int i = 0; i < STL_HEAD; i++) buf[i] = 0;
		strcpy(buf, "OBJ_BIN TRIA COND_ID");
		tt_write(ofs, buf, 1, STL_HEAD, inv);

#ifdef DEBUG
		PL_DBGOSH << __func__ << " write header"<< buf <<std::endl;
#endif


		ulong element_vertex = vertex_list->size();
#ifdef DEBUG
		PL_DBGOSH << __func__ << " write vertex size " << element_vertex<<std::endl;
#endif

		tt_write(ofs, &element_vertex, sizeof(ulong), 1, inv);


		ulong element = tri_list->size();
#ifdef DEBUG
		PL_DBGOSH << __func__ << " write triangle size " << element<<std::endl;
#endif

		tt_write(ofs, &element, sizeof(ulong), 1, inv);


#ifdef DEBUG
		PL_DBGOSH << __func__ << " header:"<< buf <<std::endl;
		PL_DBGOSH << __func__ << " element_vertex:"<< element_vertex <<std::endl;
		PL_DBGOSH << __func__ << " element:"<< element <<std::endl;
#endif

		// 頂点 v 出力
		const std::vector<Vertex*>* vlistout=vertex_list->get_vertex_lists();
		for(int i=0;i< vlistout->size();++i){
			Vertex*  vert_tmp=(*vlistout)[i];
			float float_vertex[3];
			float_vertex[0]=(*vert_tmp)[0];
			float_vertex[1]=(*vert_tmp)[1];
			float_vertex[2]=(*vert_tmp)[2];
			tt_write(ofs, float_vertex, sizeof(float), 3, inv);
#ifdef DEBUG
			/* PL_DBGOSH << __func__ << " write vertecies " */
			/* 	    <<float_vertex[0] << " " */
			/* 	    <<float_vertex[1] << " " */
			/* 	    <<float_vertex[2] << " " */
			/* 	    <<std::endl; */
#endif


		}

#ifdef DEBUG
		PL_DBGOSH << __func__ << " write vertecies "<<std::endl;
#endif

		vertex_list->prepare_num_out();
		// 面 f 出力
		std::vector<PrivateTriangle*>::iterator itr;
		for (itr = tri_list->begin(); itr != tri_list->end(); itr++) {
			Vertex** tmpvtx=(*itr)->get_vertex();
			long index[3];
			// index[0]=vertex_list->ith(tmpvtx[0]);
			// index[1]=vertex_list->ith(tmpvtx[1]);
			// index[2]=vertex_list->ith(tmpvtx[2]);

			index[0]=vertex_list->vtx_index(tmpvtx[0]);
			index[1]=vertex_list->vtx_index(tmpvtx[1]);
			index[2]=vertex_list->vtx_index(tmpvtx[2]);

#ifdef DEBUG
			// PL_DBGOSH<< __func__ <<" "<<index[0]
			// 	      <<" "<<index[1]
			// 	      <<" "<<index[2]
			// 	      <<std::endl;
#endif


			if(index[0]==-1 || index[1]==-1 || index[2]==-1) {
				PL_ERROSH << "[ERROR]obj:obj_b_save():wrong vertex id "
					<< index[0] << " "<< index[1] << " "<< index[2] << " "<< std::endl;
			}
			// index starts from 1
			++index[0]; ++index[1]; ++index[2];
			tt_write(ofs, index , sizeof(long), 3, inv);

			/* PL_DBGOSH << __func__ << " write face " */
			/* 	    <<index[0] << " " */
			/* 	    <<index[1] << " " */
			/* 	    <<index[2] << " " */
			/* 	    <<std::endl; */


			int exid = (*itr)->get_exid();
			tt_write(ofs, &exid, sizeof(ushort), 1, inv);

		}

		vertex_list->index_map_clear();

#ifdef DEBUG
		PL_DBGOSH << __func__ << " write faces and exid "<<std::endl;
#endif


		if (!ofs.eof() && ofs.fail()) {
			PL_ERROSH << "[ERROR]obj:obj_b_save():Error in saving: " << fname << std::endl;
			return PLSTAT_OBJ_IO_ERROR;
		}

		return PLSTAT_OK;

		///#undef DEBUG
}

///////////////////////////////////////////////////////////////

POLYLIB_STAT obj_bb_save(
	VertexList* vertex_list,
	std::vector<PrivateTriangle*> *tri_list,
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
		long element_vertex = vertex_list->size();
		tt_write(ofs, &element_vertex, sizeof(long), 1, inv);
		//  uint element = tri_list->size();
		//tt_write(ofs, &element, sizeof(uint), 1, inv);
		long element = tri_list->size();
		tt_write(ofs, &element, sizeof(long), 1, inv);

		// 頂点 v 出力
		const std::vector<Vertex*>* vlistout=vertex_list->get_vertex_lists();
		for(int i=0;i<vlistout->size();++i){
			Vertex*  vert_tmp=(*vlistout)[i];
			float float_vertex[3];
			float_vertex[0]=(*vert_tmp)[0];
			float_vertex[1]=(*vert_tmp)[1];
			float_vertex[2]=(*vert_tmp)[2];
			tt_write(ofs, float_vertex, sizeof(float), 3, inv);
		}


		// 法線　vn 出力
		std::map<Vertex*,Vec3<PL_REAL> > vnmap;
		std::map<Vertex*,Vec3<PL_REAL> >::iterator vnmap_iter;
		std::vector<PrivateTriangle*>::iterator itr;

		for (itr = tri_list->begin(); itr != tri_list->end(); itr++) {
			Vertex** tmpvtx=(*itr)->get_vertex();
			Vec3<PL_REAL> normal=(*itr)->get_normal();
			for(int i=0;i<3;++i){
				// Vertex*  をkeyにして法線ベクトル（計算途中）を探し、無ければ加える。
				vnmap_iter=vnmap.find(tmpvtx[i]);
				if(vnmap_iter==vnmap.end()) {
					Vec3<PL_REAL> newnormal(0,0,0);
					vnmap[tmpvtx[i]]=newnormal;
				}
				// 頂点ごとに面法線の合計
				vnmap[tmpvtx[i]]+=normal;
			}
		}

		for(vnmap_iter=vnmap.begin();
			vnmap_iter!=vnmap.end();
			vnmap_iter++){

				Vec3<PL_REAL> out = (vnmap_iter->second).normalize();
				float tmp[3];
				tmp[0]=out[0];
				tmp[1]=out[1];
				tmp[2]=out[2];

				tt_write(ofs, tmp, sizeof(float), 3, inv);

		}

		// 面 f 出力
		//  std::vector<PrivateTriangle*>::iterator itr;
		for (itr = tri_list->begin(); itr != tri_list->end(); itr++) {
			Vertex** tmpvtx=(*itr)->get_vertex();
			long index[3];
			// index[0]=vertex_list->ith(tmpvtx[0]);
			// index[1]=vertex_list->ith(tmpvtx[1]);
			// index[2]=vertex_list->ith(tmpvtx[2]);
			index[0]=vertex_list->vtx_index(tmpvtx[0]);
			index[1]=vertex_list->vtx_index(tmpvtx[1]);
			index[2]=vertex_list->vtx_index(tmpvtx[2]);

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
		// cleanup index table
		vertex_list->index_map_clear();
		if (!ofs.eof() && ofs.fail()) {
			PL_ERROSH << "[ERROR]obj:obj_bb_save():Error in saving: " << fname << std::endl;
			return PLSTAT_OBJ_IO_ERROR;
		}
		return PLSTAT_OK;
		//#undef DEBUG
}




/////////////////////////////////////////////////
////////////////////////////


POLYLIB_STAT obj_b_load(
	VertexList *vertex_list,
	std::vector<PrivateTriangle*> *tri_list,
	std::string fname,
	int	*total,
	PL_REAL	scale
	) {
		std::ifstream ifs(fname.c_str(), std::ios::in | std::ios::binary);
		if (ifs.fail()) {
			PL_ERROSH << "[ERROR]stl:stl_b_load():Can't open " << fname << std::endl;
			return PLSTAT_STL_IO_ERROR;
		}

		//#define DEBUG
#ifdef DEBUG
		PL_DBGOSH << __func__ << " in. "<<fname <<std::endl;
#endif
		int  n_zero_area_tri=0;

		int inv = tt_check_machine_endian() == TT_LITTLE_ENDIAN ? 0 : 1;

		int	n_tri = *total;		// 通番の初期値をセット

		char buf[STL_HEAD];
		for (int i = 0; i < STL_HEAD; i++) buf[i] = 0;
		tt_read(ifs, buf, sizeof(char), STL_HEAD, inv);

#ifdef DEBUG
		PL_DBGOSH << __func__ << " read header"<<std::endl;
#endif


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

#ifdef DEBUG
		PL_DBGOSH << __func__ << " withnormal"<< withnormal
			<< " " << icheck
			<< " " << icheck2
			<<std::endl;
#endif

		ulong element_vertex;
		ulong element;

		tt_read(ifs, &element_vertex, sizeof(ulong), 1, inv);
#ifdef DEBUG
		PL_DBGOSH << __func__ << " read vertex size"<< element_vertex<<std::endl;
#endif

		tt_read(ifs, &element, sizeof(ulong), 1, inv);
#ifdef DEBUG
		PL_DBGOSH << __func__ << " read element size"<< element<<std::endl;
#endif


		for(ulong i=0;i<element_vertex;++i){
			float float_vertex[3];
			tt_read(ifs, float_vertex, sizeof(float), 3, inv);
			Vertex* v = new Vertex();
			(*v)[0]=float_vertex[0];
			(*v)[1]=float_vertex[1];
			(*v)[2]=float_vertex[2];
			//
			//	  Vertex* tmp=vertex_list->vtx_add_KDT(v);
			//	  Vertex* tmp=vertex_list->vtx_add_nocheck(v);
			vertex_list->vtx_add_nocheck(v);
			//if(tmp!=v) delete v;

		}

#ifdef DEBUG
		PL_DBGOSH << __func__ << " read vertex"<< element_vertex<<std::endl;
#endif

		if(withnormal){ // 頂点法線は捨てる
			for(ulong i=0;i<element_vertex;++i){
				float float_vertex[3];
				tt_read(ifs, float_vertex, sizeof(float), 3, inv);
			}
		}


#ifdef DEBUG
		PL_DBGOSH << __func__ << " read vertex normal"<< element_vertex<<std::endl;
#endif

		for(ulong i=0;i<element;++i){
			long index[3];
			tt_read(ifs, index , sizeof(long), 3, inv);
			int exid;
			tt_read(ifs, &exid, sizeof(ushort), 1, inv);
			Vertex* tmpvlist[3];

			if(vertex_list->size()>index[0]){
				tmpvlist[0] = vertex_list->ith(index[0]-1);
			}
			if(vertex_list->size()>index[1]){
				tmpvlist[1] = vertex_list->ith(index[1]-1);
			}
			if(vertex_list->size()>index[2]){
				tmpvlist[2] = vertex_list->ith(index[2]-1);
			}
			PrivateTriangle* tri=new PrivateTriangle(tmpvlist,
				exid);

			if(tri->get_area()==0.0){
				PL_DBGOSH << __func__
					<< " Warning :  obj file contains a triangle of the area is zero." << std::endl;
				PL_DBGOSH <<  "vertex0 ("<< *(tmpvlist[0]) <<")"<<std::endl;
				PL_DBGOSH <<  "vertex1 ("<< *(tmpvlist[1]) <<")"<<std::endl;
				PL_DBGOSH <<  "vertex2 ("<< *(tmpvlist[2]) <<")"<<std::endl;
				n_zero_area_tri++;
			}
			tri_list->push_back(tri);
		}


		if(n_zero_area_tri!=0){
			PL_DBGOSH <<  "# of zero area Triangles "<< n_zero_area_tri <<std::endl;
		}


#ifdef DEBUG
		PL_DBGOSH << __func__ << " read face"<< element_vertex<<std::endl;
#endif
		//#undef DEBUG

		return PLSTAT_OK;
}


}// end of namespace PolylibNS
