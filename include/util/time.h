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

#ifndef polylib_time_h
#define polylib_time_h

namespace PolylibNS {
	bool getrusage_sec(
		double	*usr_time, 
		double	*sys_time, 
		double	*total
		);
}

#endif //polylib_time_h
