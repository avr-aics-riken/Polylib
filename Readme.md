# Polylib - Polygon Management Library

## OUTLINE
Polylib is a C++ class library to keep and to manage polygon data, and has following functions.

- load and save polygon data. The supported file formats are STL and OBJ.
- management of polygon data on distributed parallel environment.
- search and retrieve polygon data
- grouping by input parameter file (text parser format).
- data movement and migration between subdomains.


## SOFTWARE REQUIREMENT
- Cmake
- MPI library (option)
- Text parsing library


## INGREDIENTS

ChangeLog.md   History of development
CMakeLists.txt Makefile of cmake
License.txt    License of Polylib
Readme.md      This document
cmake/         cmake modules
doc/           Documents
examples/      Example sources
include/       Header files
src/           Source files





## HOW TO BUILD

### Build

~~~
$ export CC=c_compiler CXX=c++_compiler F90=fortran_compiler FC=fortran_compiler
$ export PL_HOME=/hogehoge
$ mkdir BUILD
$ cd BUILD
$ cmake [options] ..
$ make
$ make test  // if test option is specified.
$ sudo make install
~~~

### Options

`-D INSTALL_DIR=` *Install_directory*

>  Specify the directory that this library will be installed. Built library is
   installed at `install_directory/lib` and the header files are placed at
   `install_directory/include`.

`-D real_type=` {float | double}

>  Specify the type of floating point. If this option is omitted, the default is float.


`-D with_MPI=` {no | yes}

>  If you use an MPI library, specify `with_MPI=yes`, otherwise no or you can omit.

`-D with_example=` {no | yes}

>  Specify building example. The default is 'no'.

`-D with_TP=` *Installed_directory*

>  Specify the directory path that TextParser library is installed.


The default compiler options are described in `cmake/CompilerOptionSelector.cmake` file. See BUILD OPTION section in CMakeLists.txt in detail.



## Configure Examples

### INTEL/GNU/PGI compiler

~~~
$ cmake -DINSTALL_DIR=${PL_HOME}/Polylib -Dreal_type=float -Dwith_MPI=yes -Dwith_example=yes -Dwith_TP=${PL_HOME}/TextParser ..
~~~


### FUJITSU compiler / FX10, FX100, K on login nodes (Cross compilation)

~~~
$ cmake -DINSTALL_DIR=${PL_HOME}/Polylib -DCMAKE_TOOLCHAIN_FILE=../cmake/Toolchain_fx10.cmake -Dreal_type=float -Dwith_MPI=yes -Dwith_example=yes -Dwith_TP=${PL_HOME}/TextParser ..

$ cmake -DINSTALL_DIR=${PL_HOME}/Polylib -DCMAKE_TOOLCHAIN_FILE=../cmake/Toolchain_fx100.cmake -Dreal_type=float -Dwith_MPI=yes -Dwith_example=yes -Dwith_TP=${PL_HOME}/TextParser ..

$ cmake -DINSTALL_DIR=${PL_HOME}/Polylib -DCMAKE_TOOLCHAIN_FILE=../cmake/Toolchain_K.cmake -Dreal_type=float -Dwith_MPI=yes -Dwith_example=yes -Dwith_TP=${PL_HOME}/TextParser ..
~~~


##### Note
- On Fujitsu machines(fx10, K, fx100), confirm appropriate directrory path for compiler environment.
- Before building, execute following command for clean. `$ make distclean`



## TEST
* If you specify the test option by `-Dwith_example=yes`, you can
execute the intrinsic tests by;

	`$ make test` or `$ ctest`

* The detailed results are written in `BUILD/Testing/Temporary/LastTest.log` file.
Meanwhile, the summary is displayed for stdout.



## CONTRIBUTORS

* Kenji    Ono        *keno@{cc.kyushu-u.ac, riken, iis.u-tokyo.ac}.jp*
* Tomohiro  Kawanabe   tkawanab@iis.u-tokyo.ac.jp
* Takehiro  Tawara
* Takashi   Suzuki
