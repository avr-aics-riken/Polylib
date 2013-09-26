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


#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdio.h>
#include <string.h>
#include <vector>
#include <iostream>
#include "common/PolylibCommon.h"
#include "common/PolylibStat.h"
#include "Polylib.h"
#include "c_lang/CPolylib.h"


///
/// Ｃ言語用Polylib-API
///

using namespace std;
using namespace PolylibNS;


// load
POLYLIB_STAT
polylib_load( char *config_name )
{
	if( config_name == NULL ) {
	  return (Polylib<PL_REAL>::get_instance())->load();
	}
	string fname = config_name;
	return (Polylib<PL_REAL>::get_instance())->load( fname );
}

// save
POLYLIB_STAT
polylib_save(
	char	**p_fname,
	char	*format,
	char	*extend
)
{
	string s_fname;
	string s_format = format;
	string s_extend;
	POLYLIB_STAT stat;

	if( extend ) s_extend = extend;

	if( extend==NULL ) {
	  stat = (Polylib<PL_REAL>::get_instance())->save( &s_fname, s_format );
	}
	else {
	  stat = (Polylib<PL_REAL>::get_instance())->save( &s_fname, s_format, s_extend );
	}
	*p_fname = (char*)malloc( s_fname.size()+1 );
	if(p_fname == NULL){
		fprintf(stderr,"polylib_save: Can not allocate memory.\n");
		return PLSTAT_MEMORY_NOT_ALLOC;
	}
	strcpy( *p_fname, s_fname.c_str() );
	return stat;
}


// search_polygons
TriangleStruct**
polylib_search_polygons(
	char* group_name,
	PL_REAL min_pos[3],
	PL_REAL max_pos[3],
	int every,
	int *num_tri,
	POLYLIB_STAT *err
)
{
  //	PL_VEC3 c_min_pos,c_max_pos;
  Vec3<PL_REAL> c_min_pos,c_max_pos;
	string c_group_name(group_name);

	for(int i=0; i<3; i++){
		c_min_pos[i] = min_pos[i];
		c_max_pos[i] = max_pos[i];
	}

	bool b_every;
	if(every == POLYLIB_TRUE) b_every = true;
	else b_every = false;

	//Polylibから三角形リストを抽出
	std::vector<Triangle<PL_REAL>*>*  tri_list =
	  (Polylib<PL_REAL>::get_instance())->search_polygons(
			c_group_name, c_min_pos, c_max_pos, b_every);
	*num_tri  = tri_list->size();

	//三角形リストのポインタ配列の確保
	TriangleStruct **p_tri =
			(TriangleStruct**)malloc( sizeof(TriangleStruct*)*(*num_tri) );
	if(p_tri == NULL){
		fprintf(stderr,"polylib_serch_polygons: Can not allocate memory.\n");
		*err = PLSTAT_MEMORY_NOT_ALLOC;
		return NULL;
	}

	std::vector<Triangle<PL_REAL>*>::iterator itr;
	int num = 0;
	for(itr=tri_list->begin(); itr!=tri_list->end(); itr++){
		//TriangleクラスインスタンスをTriangleStruct*にキャストする
		p_tri[num] = (TriangleStruct*)( *itr );
		num++;
	}
	delete(tri_list);
	*err = PLSTAT_OK;
	return p_tri;
}

// show_group_hierarchy
void 
polylib_show_group_hierarchy()
{
  (Polylib<PL_REAL>::get_instance())->show_group_hierarchy();
}

// show_group_info
POLYLIB_STAT
polylib_show_group_info(char* group_name)
{
	string c_group_name(group_name);

	return (Polylib<PL_REAL>::get_instance())->show_group_info(c_group_name);
}

// used_memory_size
unsigned int
polylib_used_memory_size()
{
  return (Polylib<PL_REAL>::get_instance())->used_memory_size();
}


// eof
