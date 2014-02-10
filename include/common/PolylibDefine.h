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

#ifndef polylib_define_h
#define polylib_define_h

#include <float.h>


//PL_REALはgcc -Dで定義済み   -Dの中に定義されている値は見えないので,configure等に例REAL_TYPE_FLOATなど  float/doubleを分けるヒントになる別のdefineを用意する必要がある
//#ifdef PL_REAL
#define REAL_TYPE float
#define REAL_VAL_MAX FLT_MAX;
/*
#else	
#define REAL_TYPE double
#define REAL_VAL_MAX DBL_MAX;
#endif
*/

#endif // polylib_define_h
