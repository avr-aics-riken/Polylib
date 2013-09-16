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


// // MPI通信用メッセージタグ
// #define	MPITAG_NUM_CONFIG			1
// #define	MPITAG_CONFIG				2
// #define	MPITAG_NUM_TRIAS			3
// #define MPITAG_TRIA_IDS				4
// #define MPITAG_TRIAS				5


#include <iostream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <map>
//#include "groups/PolygonGroup.h"
//#include "c_lang/CMPIPolylib.h"
#include "mpi.h"

//#define PL_MPI_REAL MPI_DOUBLE
#if PL_REAL==float
#define PL_MPI_REAL MPI_FLOAT
#else
#define PL_MPI_REAL MPI_DOUBLE
#endif

#include "MPIPolylib.h"


using namespace std;
using namespace PolylibNS;

////////////////////////////////////////////////////////////////////////////
/// 
/// クラス:MPIPolylib
/// Polylib継承したポリゴンを管理する為の並列版クラスライブラリです。
/// 
////////////////////////////////////////////////////////////////////////////



// eof
