##############################################################################
#
# Polylib - Polygon Management Library
#
# Copyright (c) 2010-2011 VCAD System Research Program, RIKEN. 
# All rights reserved.
# 
# Copyright (c) 2012-2014 Advanced Institute for Computational Science, RIKEN.
# All rights reserved.
#
##############################################################################


SUBDIRS = @MAKE_SUB_DIRS@

bin_SCRIPTS    = polylib-config
CLEANFILES     = $(bin_SCRIPTS)
EXTRA_DIST     = polylib-config.in LICENSE Makefile_hand doc/doxygen/Doxyfile
dist_data_DATA = AUTHORS COPYING ChangeLog README LICENSE

#ACLOCAL_AMFLAGS = -I m4 // SO

DISTCLEANFILES = *.tar.gz

CXXFLAGS = @REAL_OPT@

# to check libtool is upto date // SO
#LIBTOOL_DEPS = @LIBTOOL_DEPS@
#libtool:$(LIBTOOL_DEPS)
#	$(SHELL) ./config.status --recheck

#clean-local: // SO
#	find ./ -name *.exe | xargs rm -f
#	find ./ -name *.lib | xargs rm -f


# clean-local:
	find ./ -name *.obj | xargs rm -f
	find ./ -name *.exe | xargs rm -f
	find ./ -name *.lib | xargs rm -f

distclean-local: clean-local
	rm -f ./bin/*
	rm -f ./lib/*
