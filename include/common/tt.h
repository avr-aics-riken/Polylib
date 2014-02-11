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

#ifndef polylib_tt_h
#define polylib_tt_h

typedef unsigned char  uchar;
typedef unsigned short ushort;
typedef unsigned int   uint;
typedef unsigned long  ulong;

#ifndef M_PI
# define M_PI		3.14159265358979323846	/* pi */
#endif

#ifndef __GNUC__
# include <math.h>
# define sqrtf(x) (float)sqrt((double)(x))
#endif

#define GL_GLEXT_PROTOTYPES 1

#endif  // polylib_tt_h

