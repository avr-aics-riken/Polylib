/*
 * Polylib - Polygon Management Library.
 *
 * Copyright (c) RIKEN, Japan. All right reserved. 2010-
 *
 */
#include <fstream>
#include <vector>
#include <iomanip>
#include "polygons/Triangle.h"
#include "file_io/triangle_id.h"

namespace PolylibNS {

using namespace std;

//////////////////////////////////////////////////////////////////////////////
// 変更:ポリゴンIDのバイナリ入力対応 2010.10.19
POLYLIB_STAT load_id(
	vector<PrivateTriangle*>	*tri_list, 
	string 						fname
) {
#ifdef SAVE_ID_ASCII
	ifstream is(fname.c_str());
#else
	ifstream is(fname.c_str(), ios::binary);
#endif

fprintf(stderr, "triangle_id:load_id():%s\n", fname.c_str());
	if (is.fail()) {
		PL_ERROSH << "[ERROR]triangle_id:load_id():Can't open " << fname << endl;
		return PLSTAT_STL_IO_ERROR;
	}

	vector<PrivateTriangle*>::iterator itr = tri_list->begin();
#ifdef SAVE_ID_ASCII
	string	id;
	while (is >> id && !is.eof()) {
#else
	int		id;
	while (is.read((char*)&id, sizeof(int)) && !is.eof()) {
#endif
		// ポリゴン数とIDファイルの行数が一致しているか?
		if (itr == tri_list->end()) {
			PL_ERROSH << "[ERROR]triangle_id::load_id():Triangle number is short:"
					  << fname << endl;
			return PLSTAT_STL_IO_ERROR;
		}
#ifdef SAVE_ID_ASCII
		(*itr)->set_id(atoi(id.c_str()));
#else
		(*itr)->set_id(id);
#endif
		itr++;
	}
	// ポリゴン数とIDファイルの行数が一致しているか?
	if (itr != tri_list->end()) {
		PL_ERROSH << "[ERROR]triangle_id::load_id():ID number is short:" 
				  << fname << endl;
		return PLSTAT_STL_IO_ERROR;
	}

	if (!is.eof() && is.fail()) {
		PL_ERROSH << "[ERROR]triangle_id:load_id():Error in loading:" 
				  << fname << endl;
		return PLSTAT_STL_IO_ERROR;
	}

	return PLSTAT_OK;
}

//////////////////////////////////////////////////////////////////////////////
// 変更:ポリゴンIDのバイナリ出力対応 2010.10.19
POLYLIB_STAT save_id(
	vector<PrivateTriangle*>	*tri_list, 
	string 						fname
) {
#ifndef SAVE_ID_ASCII
	ofstream os(fname.c_str());
#else
	ofstream os(fname.c_str(), ios::binary);
#endif

	if (os.fail()) {
		PL_ERROSH << "[ERROR]triangle_id:save_id():Can't open " << fname << endl;
		return PLSTAT_STL_IO_ERROR;
	}

	vector<PrivateTriangle*>::iterator itr;
	for (itr = tri_list->begin(); itr != tri_list->end(); itr++) {
#ifndef SAVE_ID_ASCII
		os	<< (*itr)->get_id() << endl;
#else
		int		id = (*itr)->get_id();
		os.write((char *)&id, sizeof(int));
#endif
	}

	if (!os.eof() && os.fail()) {
		PL_ERROSH << "[ERROR]triangle_id:save_id():Error in saving:" 
				  << fname << endl;
		return PLSTAT_STL_IO_ERROR;
	}

	return PLSTAT_OK;
}

} //namespace PolylibNS
