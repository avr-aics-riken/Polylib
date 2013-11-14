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


} //namespace PolylibNS
