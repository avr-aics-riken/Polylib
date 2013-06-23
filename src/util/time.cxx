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

#include<time.h>
#include<sys/time.h>
#include<sys/resource.h>
#include "util/time.h"

namespace PolylibNS {

bool getrusage_sec(double *usr_time, double *sys_time, double *total)
{
	struct rusage	t;
	struct timeval	st, ut, tt;
	bool			ret;
	int				iret;

	iret = getrusage(RUSAGE_SELF, &t);
	if (iret == 0)		ret = true;
	else				ret = false;

	iret = gettimeofday(&tt, NULL);
	if (iret == 0)		ret = true;
	else				ret = false;

	ut = t.ru_utime;
	st = t.ru_stime;
	*usr_time = ut.tv_sec + (double)ut.tv_usec*1.e-6;
	*sys_time = st.tv_sec + (double)st.tv_usec*1.e-6;
	*total    = tt.tv_sec + (double)tt.tv_usec*1.e-6;
	return ret;
}

} //namespace PolylibNS
