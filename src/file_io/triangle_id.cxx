/* -- Mode: c++ --*/
/*
* Polylib - Polygon Management Library
*
* Copyright (c) 2010-2011 VCAD System Research Program, RIKEN.
* All rights reserved.
*
* Copyright (c) 2012-2015 Advanced Institute for Computational Science, RIKEN.
* All rights reserved.
*
*/

#include "file_io/triangle_id.h"
#include <stdlib.h>

namespace PolylibNS {



#define AS_BINARY	0
#define AS_ASCII	1
//////////////////////////////////////////////////////////////////////////////
// 変更:ポリゴンIDのバイナリ入力対応 2010.10.19

POLYLIB_STAT load_id(
	std::vector<PrivateTriangle*>* tri_list, 
	std::string fname,
	ID_FORMAT id_format
	) {
		std::ifstream is;

		if (id_format == ID_BIN) is.open(fname.c_str(), std::ios::binary);
		else		is.open(fname.c_str());

		if (is.fail()) {
			PL_ERROSH << "[ERROR]triangle_id:load_id():Can't open " << fname << std::endl;
			return PLSTAT_STL_IO_ERROR;
		}

		std::vector<PrivateTriangle*>::iterator itr = tri_list->begin();

		if (id_format == ID_BIN) {
			int		id;
			while (is.read((char*)&id, sizeof(int)) && !is.eof()) {
				// ポリゴン数とIDファイルの行数が一致しているか?
				if (itr == tri_list->end()) {
					PL_ERROSH << "[ERROR]triangle_id::load_id():Triangle number "
						<< "is short:" << fname << std::endl;
					return PLSTAT_STL_IO_ERROR;
				}
				(*itr)->set_id(id);
				itr++;
			}
		}
		else {
			std::string	id;
			while (is >> id && !is.eof()) {
				// ポリゴン数とIDファイルの行数が一致しているか?
				if (itr == tri_list->end()) {
					PL_ERROSH << "[ERROR]triangle_id::load_id():Triangle number "
						<< "is short:" << fname << std::endl;
					return PLSTAT_STL_IO_ERROR;
				}
				(*itr)->set_id(atoi(id.c_str()));
				itr++;
			}
		}

		// ポリゴン数とIDファイルの行数が一致しているか?
		if (itr != tri_list->end()) {
			PL_ERROSH << "[ERROR]triangle_id::load_id():ID number is short:" 
				<< fname << std::endl;
			return PLSTAT_STL_IO_ERROR;
		}

		if (!is.eof() && is.fail()) {
			PL_ERROSH << "[ERROR]triangle_id:load_id():Error in loading:" 
				<< fname << std::endl;
			return PLSTAT_STL_IO_ERROR;
		}

		return PLSTAT_OK;
}

//////////////////////////////////////////////////////////////////////////////
// 変更:ポリゴンIDのバイナリ出力対応 2010.10.19

POLYLIB_STAT save_id(
	std::vector<PrivateTriangle*>	*tri_list, 
	std::string 				fname,
	ID_FORMAT			id_format
	) {
		std::ofstream os;

		if (id_format == ID_BIN)	os.open(fname.c_str(), std::ios::binary);
		else						os.open(fname.c_str());

		if (os.fail()) {
			PL_ERROSH << "[ERROR]triangle_id:save_id():Can't open " << fname << std::endl;
			return PLSTAT_STL_IO_ERROR;
		}

		std::vector<PrivateTriangle*>::iterator itr;
		if (id_format == ID_BIN) {
			for (itr = tri_list->begin(); itr != tri_list->end(); itr++) {
				int		id = (*itr)->get_id();
				os.write((char *)&id, sizeof(int));
			}
		}
		else {
			for (itr = tri_list->begin(); itr != tri_list->end(); itr++) {
				os	<< (*itr)->get_id() << std::endl;
			}
		}

		if (!os.eof() && os.fail()) {
			PL_ERROSH << "[ERROR]triangle_id:save_id():Error in saving:" 
				<< fname << std::endl;
			return PLSTAT_STL_IO_ERROR;
		}

		return PLSTAT_OK;
}





} //namespace PolylibNS


