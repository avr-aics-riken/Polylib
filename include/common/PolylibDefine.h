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

#ifndef polylib_define_h
#define polylib_define_h

#include <float.h>

/** 実数型の指定
 * - デフォルトでは、PL_REAL=float
 * - コンパイル時オプション-D_REAL_IS_DOUBLE_を付与することで
 *   PL_REAL=doubleになる
 */
#ifdef _REAL_IS_DOUBLE_
#define PL_REAL double
#else
#define PL_REAL float
#endif

#endif // polylib_define_h
