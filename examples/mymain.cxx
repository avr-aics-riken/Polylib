/*
* Polylib - Polygon Management Library
*
* Copyright (c) 2010-2011 VCAD System Research Program, RIKEN.
* All rights reserved.
*
* Copyright (c) 2012-2015 Advanced Institute for Computational Science, RIKEN.
* All rights reserved.
*
*/
#ifdef WIN32
//１８ケース
extern int main_test();//1
extern int main_test2();//2
extern int main_test_mpi(int argc, char** argv );//3
extern int main_test_mpi2(int argc, char** argv );
extern int main_test_mpi3(int argc, char** argv );
extern int main_test_mpi_DVertex(int argc, char** argv );
extern int main_test_mpi_xyzrgb1(int argc, char** argv );
extern int main_test_mpi_xyzrgb2(int argc, char** argv );
extern int main_test_mpi_xyzrgb4(int argc, char** argv );
extern int main_test_mpi_xyzrgb8(int argc, char** argv );
extern int main_test_obj();
extern int main_test_obj2();
extern int main_test_obj3();
extern int main_test_obj4();
extern int main_test_read();
extern int main_test_vtx();
extern int main_vtx_float();
extern int main_test_xyzrgb_statuette_stl();
extern int main_test_move(int argc, char** argv );//3
#include <stdlib.h>

int main(int argc, char** argv ){
	/*
	if(argc!=2){
	return -1;
	}
	int ret = 0;
	int id = ::atoi( argv[1]);
	*/
	int ret = 0;
	int id = 1;
	if(id == 1){
		ret = main_test();
	}else if(id == 2){
		ret = main_test2();
	}else if(id == 3){
		ret = main_test_mpi(argc,argv);
	}else if(id == 4){
		ret = main_test_mpi2(argc,argv);
	}else if(id == 5){
		ret = main_test_mpi3(argc,argv);
	}else if(id == 6){
		ret = main_test_mpi_DVertex(argc,argv);
	}else if(id == 7){
		ret = main_test_mpi_xyzrgb1(argc,argv);
	}else if(id == 8){
		ret = main_test_mpi_xyzrgb2(argc,argv);
	}else if(id == 9){
		ret = main_test_mpi_xyzrgb4(argc,argv);
	}else if(id == 10){
		ret = main_test_mpi_xyzrgb8(argc,argv);
	}else if(id == 11){
		ret = main_test_obj();
	}else if(id == 12){
		ret = main_test_obj2();
	}else if(id == 13){
		ret = main_test_obj3();
	}else if(id == 14){
		ret = main_test_obj4();
	}else if(id == 15){
		ret = main_test_read();
	}else if(id == 16){
		ret = main_test_vtx();
	}else if(id == 17){
		ret = main_vtx_float();
	}else if(id == 18){
		ret = main_test_xyzrgb_statuette_stl();
	}else if(id == 19){
		ret = main_test_move(argc,argv);
	}
	return ret;
}

#endif
