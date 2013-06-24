##############################################################################
#
# Polylib - Polygon Management Library
#
# Copyright (c) 2010-2011 VCAD System Research Program, RIKEN. 
# All rights reserved.
# 
# Copyright (c) 2012-2013 Advanced Institute for Computational Science, RIKEN.
# All rights reserved.
#
##############################################################################
#
#  At first, edit MACRO, then make


AR          = ar cru
RANLIB      = ranlib
RM          = \rm -f
MPI_DIR	    = /opt/openmpi
TP_DIR      = /usr/local/TextParser
OMP_FLAGS   =
CXX         = icpc
CXXFLAGS    = -O3 $(OMP_FLAGS) -Wall -fno-strict-aliasing
UDEF_INC_PATH = -I$(TP_DIR)/include -I$(MPI_DIR)/include



all:
	( \
	cd src; \
	make \
		CXX='$(CXX)' \
		CXXFLAGS='$(CXXFLAGS)' \
		AR='$(AR)' \
		RANLIB='$(RANLIB)' \
		RM='$(RM)' \
		UDEF_INC_PATH='$(UDEF_INC_PATH)' \
	)

clean:
	(cd src; make clean)

depend:
	(cd src; make depend)
