# Polylib - Polygon Management Library

## REVISION HISTORY

---
- 2017-08-25  Version 3.6.8
  - intel_F_TCS


---
- 2017-07-06  Version 3.6.7
  - display "TextParser support"


---
- 2017-07-06  Version 3.6.6
  - set(CMAKE_FIND_ROOT_PATH   /opt/FJSVtclang/1.2.0) in Toolchain_K.cmake

---
- 2017-03-08  Version 3.6.5
  - modify implementation of 'real_type'


---
- 2017-03-07  Version 3.6.4
  - add fx100 entry


---
- 2017-03-04  Version 3.6.3
  - test gcc >> test_DVertex.cpp faied.
  - Tested.

|Compiler|Serial|Ex.|MPI |Ex.|
|:--|:--:|:--:|:--:|:--:|
|Intel 17.0.1 |ok|ok|ok|0%|
|GNU 6.2.0    |ok|x|||
|fx10         ||||–|
|K            ||||–|


---
- 2017-03-02  Version 3.6.2
  - modify structure of include files


---
- 2017-03-01  Version 3.6.1
  - add `#include <cstring>` into src/file_io/obj.cxx for SuSE linux


---
- 2017-03-01  Version 3.6.0
  - cmake Version
  - All examples for MPI failed.


---
- 2015-11-28  Version 3.5.4
  - r64 --with-comp becomes essential
  - change configure.ac, INSTALL, and NEWS


---
- 2015-07-07  Version 3.5.3
  - r63 add altenative API for load_rank0


---
- 2015-06-14  Version 3.5.2
  - r62 bug fix include headers
  - Making invalid the example option


---
- 2015-06-10  Version 3.5.1
  - r61 clean package


---
- 2015-06-09  Version 3.5.0
  - r60 introduce BUILD_DIR to keep source directory clean
  - Change to run configure
  - Change configure.ac


---
- 2015-03-14  Version 3.4.9
  - r59 update year
  - add mpiicc, mpiicpc
  - update description of INSTALL >> ${FFV_HOME}


---
- 2015-02-21  Version 3.4.8
  -  r58 change cal. procedure of calc_area()
  - change cal. procedure in calc_area(), float > double, length() >> lengthsquared()
  - change comparison in if-branch statement of get_area from int to float if ( get_area()==0 ) >> if ( get_area()==0.0 )


---
- 2014-05-19
  - r57 remove to_string() from source code to avoid compile error on Marverics
  - comment out all to_string()
  - MPIPolylib.cxx L739
  - remove #define to_string( s ) from MPIPolylib.h


---
- 2014-05-12  Version 3.4.6
  -  r56 update & bug fix
  - bug
    - Polylib::save_with_rankno for maxrank=0
    - Add exid copy in TriMesh::init(trias)
  - add a method
    - search_polygons()


---
- 2014-05-02  Version 3.4.5
  - r55 suppress extra debug message at pack_tria_ids() in MPIPolylib.cxx


---
- 2014-04-23  Version 3.4.4
  -  r54 update Vec3.h [ver 3.4.4]
  - add typedef vec3r


---
- 2014-03-23  Version 3.4.3
  -  r53 update polylib-config
  - add include/common


---
- 2014-03-23  Version 3.4.2
  - r52 update Vec3.h
  - change notation : t[3] >> x,y,z


---
- 2014-03-22  Version 3.4.1
  - r51 suppress debug write
  - recieve_polygon_from_rank0()


---
- 2014-03-19  Version 3.4.0
  - r50 suppress to generate shared library
  - comment out by SO


---
- 2014-03-17  Version 3.3.9
  -  r49 introduce common namespace of Vec3class
  - include axis.h in Vec3.h
  - expire Vec3_func.h and Vec3f_func.h


---
- 2014-03-08  Version 3.3.8
  -  r48 remove AC_FUNC_MALLOC for FX/K
  - In examples, if you compile these programs on K/FX, remove -lstdc++ from LDADD lines
  - change library name from MPIPOLY to POLYmpi


---
- 2014-03-07  Version 3.3.7
  -  r47 suppress `__func__` for FX in PolylibCommon.h


---
- 2014-03-05  Version 3.3.6
  - r46 suppress debug write


---
- 2014-03-04  Version 3.3.5
  - r45 version format
  - change output format for poly-config --version


---
- 2014-03-01  Version 3.3.4
  - r44 suppress message by keno-2014-03-01


---
- 2014-03-01  Version 3.3.3
  - r43 change filename Version.h.in to polyVersion.h.in to avoid conflict with other version file


---
- 2014-02-21  Version 3.3.2
  - r42 change implementation PL_TYPE


---
- 2014-02-21  Version 3.3.1
  - r41 change REAL_TYPE Macro to avoid conflict with FFVC


---
- 2014-02-21  Version 3.3.0
  - r40 Merge branch version3 to master


---
- 2014-02-11  Version 3.2.0
  - r39 add CC compiler wrapper script in configure.ac for NERSC Hopper


---
- 2013-11-16  Version 3.1.0


---
- 2013-09-16  Version 3.0.0


---
- 2013-11-02    Version 2.7.3
  - r38 fix undistributed Triangle::m_exid at MPI


---
- 2013-11-02  Version 2.7.2
  - r37 cleanup configure.ac
  - expire MPICH version
  - introduce subdir-objects in Makefile.am


---
- 2013-10-11  Version 2.7.1
  - r36 improve precision of calculating normal vector
  - calculate Triangle::calc_normal() in double precision
  - add Polylib::get_leaf_groups()


---
- 2013-10-02  Version 2.7.0
  - r35 modify to include string.h for strcpy in CMPIPolylib.cxx


---
- 2013-10-02  Version 2.6.9
  - r34 modify for intel mpi
  - include mpi.h before stdio.h to suppress error message #error "SEEK_SET is #defined but must not be for the C++ binding of MPI"
  - There are some commit operations from r30 to r33


---
- 2013-09-16
  - r30 Branch - ver 3.0.0
  - Package 3.0.0
  - update a user guide


---
- 2013-07-20  Version 2.6.8
  - r29 update PolygonGroup::set_all_exid_of_trias()
    - set value to m_id, m_id_defined


---
- 2013-07-20  Version 2.6.7
  - r28 Introduce getVersionInfo()
  - add Version.h.in


---
- 2013-07-17  Version 2.6.6
  - r27 Merge
  - add PolygonGroup::m_type. and  PolygonGroup::set_all_exid_of_trias()


---
- 2013-07-15  Version 2.6.5
  - r26 update release style
  - with-example=no (default)


---
- 2013-07-04
  - r25 update INSTALL & README


---
- 2013-06-27  Version 2.6.4
  - r24 update configure


---
- 2013-06-27  Version 2.6.3
  - r23 update configure option


---
- 2013-06-27
  - r22 update INSTALL again
  - r21 update INSTALL


---
- 2013-06-26  Version 2.6.2
  - r20 bug fix configure.ac
  - r19 change configure option


---
- 2013-06-25  Version 2.6.1
  - r18 update README
  - r17 ver 2.6.1
  - r16 update README


---
- 2013-06-24  Version 2.6
  - r15 ver 2.6 autotools version
  - r14 make libMPIPOLY.a and libPOLY.a


---
- 2013-06-24
  - r13 ver 2.6 autotool version but only MPIPOLY


---
- 2013-06-23
  - r12 prepare autotools 2
  - r11 prepare autotools


---
- 2013-06-22
  - r10 update AUTHORS


---
- 2013-06-17  Version 2.5


---
- 2013-05-08  Version 2.4


---
- 2013-03-26  Version 2.3


---
- 2012-11-27  Version 2.2


---
- 2012-08-31  Version 2.1


---
- 2010-11-29  Version 2.0.3


---
- 2010-11-09  Version 2.0.2


---
- 2010-10-29  Version 2.0.1


---
- 2010-07-30  Version 2.0.0


---
- 2010-02-26  Version 1.0.0
