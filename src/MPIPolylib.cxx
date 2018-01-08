/*
###################################################################################
#
# Polylib - Polygon Management Library
#
# Copyright (c) 2010-2011 VCAD System Research Program, RIKEN.
# All rights reserved.
#
# Copyright (c) 2012-2015 Advanced Institute for Computational Science (AICS), RIKEN.
# All rights reserved.
#
# Copyright (c) 2016-2018 Research Institute for Information Technology (RIIT), Kyushu University.
# All rights reserved.
#
###################################################################################
*/

#include "MPIPolylib.h"


namespace PolylibNS {





MPIPolylib*
	MPIPolylib::get_instance() {
		static MPIPolylib instance;
		return &instance;
}


// public /////////////////////////////////////////////////////////////////////

POLYLIB_STAT
	MPIPolylib::init_parallel_info(
	MPI_Comm comm,
	PL_REAL bpos[3],
	unsigned int bbsize[3],
	unsigned int gcsize[3],
	PL_REAL dx[3]
)
{
	//#define DEBUG
	//#undef DEBUG

#ifdef DEBUG
	PL_DBGOSH << "MPIPolylib::init_parallel_info() in. " << std::endl;
#endif
	int i;

	// MPI情報の設定
	m_mycomm = comm;
	MPI_Comm_rank(comm, &m_myrank);
	MPI_Comm_size(comm, &m_numproc);

	// デバッグ出力用ランク番号文字列を設定
	std::ostringstream ostr;
	ostr << m_myrank;
	gs_rankno = "(rk:";
	gs_rankno += ostr.str();
	gs_rankno += ")";

#ifdef DEBUG
	PL_DBGOSH << "m_myrank: " << m_myrank << " m_numproc: " << m_numproc << std::endl;
#endif

	PL_REAL bbsize_f[3], gcsize_f[3];
	for (i = 0; i < 3; i++) {
		bbsize_f[i] = (PL_REAL)bbsize[i];
		gcsize_f[i] = (PL_REAL)gcsize[i];
	}

	Vec3<PL_REAL> v_bbsize(bbsize_f[0],bbsize_f[1],bbsize_f[2]);
	Vec3<PL_REAL> v_gcsize(gcsize_f[0],gcsize_f[1],gcsize_f[2]);
	Vec3<PL_REAL> v_bpos(bpos[0],bpos[1],bpos[2]);
	Vec3<PL_REAL> v_dx(dx[0],dx[1],dx[2]);

	// 自PE領域情報を設定
	m_myproc.m_comm = comm;
	m_myproc.m_rank = m_myrank;
	m_myproc.m_area.m_bpos = v_bpos;
	m_myproc.m_area.m_bbsize = v_bbsize;
	m_myproc.m_area.m_gcsize = v_gcsize;
	m_myproc.m_area.m_dx = dx;
	m_myproc.m_area.m_gcell_min = v_bpos-( v_gcsize )*v_dx;
	m_myproc.m_area.m_gcell_max = v_bpos+( v_bbsize+v_gcsize )*v_dx;
	m_myproc.m_area.m_gcell_bbox.add(m_myproc.m_area.m_gcell_min);
	m_myproc.m_area.m_gcell_bbox.add(m_myproc.m_area.m_gcell_max);

#ifdef DEBUG
	PL_DBGOSH << "(my_rank:" << m_myrank << "):" <<"bpos      :" << v_bpos  << std::endl;
	PL_DBGOSH << "(my_rank:" << m_myrank << "):" <<"bbsize    :" << v_bbsize << std::endl;
	PL_DBGOSH << "(my_rank:" << m_myrank << "):" <<"gcsize    :" << v_gcsize << std::endl;
	PL_DBGOSH << "(my_rank:" << m_myrank << "):" <<"dx        :" << v_dx << std::endl;
	PL_DBGOSH << "(my_rank:" << m_myrank << "):" <<"gcell_min :"
		 << m_myproc.m_area.m_gcell_min << std::endl;
	PL_DBGOSH << "(my_rank:" << m_myrank << "):" <<"gcell_max :"
	 	 << m_myproc.m_area.m_gcell_max << std::endl;
#endif

	// 送信データ作成
	PL_REAL send_buf[12];
	for (i = 0; i < 3; i++) {
		send_buf[i] = v_bpos[i];
	}
	for (i = 0; i < 3; i++) {
		send_buf[3+i] = v_bbsize[i];
	}
	for (i = 0; i < 3; i++) {
		send_buf[6+i] = v_gcsize[i];
	}
	for (i = 0; i < 3; i++) {
		send_buf[9+i] = v_dx[i];
	}

	// 受信領域確保
	PL_REAL* recv_buf = new PL_REAL[12 * m_numproc];

	// Allgather通信を行う
	//	if (MPI_Allgather(send_buf, 12, PL_MPI_REAL, recv_buf, 12, PL_MPI_REAL, comm) != MPI_SUCCESS) {
	//if (MPI_Allgather(send_buf, 12, PL_MPI_REAL, recv_buf, 12, PL_MPI_REAL, comm) != MPI_SUCCESS) {
	/* #ifdef PL_REAL */
	/* 	PL_DBGOSH << __func__ << " PL_REAL is defined  " << to_string((PL_REAL)) << std::endl; */
	/* #else */
	/* 	PL_DBGOSH << __func__ << " PL_REAL is not defined " << to_string((PL_REAL)) << std::endl; */
	/* #endif */
	/* #ifndef PL_REAL */
	/* 	PL_DBGOSH << __func__ << " PL_REAL is undefined " << to_string((PL_REAL)) << std::endl; */
	/* #else */
	/* 	PL_DBGOSH << __func__ << " PL_REAL is not undefined  " << to_string((PL_REAL)) << std::endl; */
	/* #endif */

	// #ifdef PL_REAL
	// #if PL_REAL==float
	// 	PL_DBGOSH << __func__ << " float " << to_string((PL_REAL)) << std::endl;
	// 	if (MPI_Allgather(send_buf, 12, MPI_FLOAT, recv_buf, 12, MPI_FLOAT, comm) != MPI_SUCCESS) {
	// #else
	// 	  PL_DBGOSH << __func__ << " double 1 " << to_string((PL_REAL)) << std::endl;
	// 	if (MPI_Allgather(send_buf, 12, MPI_DOUBLE, recv_buf, 12, MPI_DOUBLE, comm) != MPI_SUCCESS) {
	// #endif
	// #else
	// 	  PL_DBGOSH << __func__ << " double 2 " << to_string((PL_REAL)) << std::endl;
	// 	if (MPI_Allgather(send_buf, 12, MPI_DOUBLE, recv_buf, 12, MPI_DOUBLE, comm) != MPI_SUCCESS) {
	// #endif
	//   PL_ERROSH << "[ERROR]MPIPolylib::init_parallel_info():MPI_Allgather "
	// 		<< "faild." << std::endl;
	//   return PLSTAT_MPI_ERROR;
	// }

	if(sizeof(PL_REAL)==4){
		//PL_DBGOSH << __func__ << " float " << to_string((PL_REAL)) << std::endl;
		if (MPI_Allgather(send_buf, 12, MPI_FLOAT, recv_buf, 12, MPI_FLOAT, comm) != MPI_SUCCESS) {
			PL_ERROSH << "[ERROR]MPIPolylib::init_parallel_info():MPI_Allgather "
				<< "faild." << std::endl;
			return PLSTAT_MPI_ERROR;
		}
	} else if (sizeof(PL_REAL)==8){
		//PL_DBGOSH << __func__ << " double  " << to_string((PL_REAL)) << std::endl;
		if (MPI_Allgather(send_buf, 12, MPI_DOUBLE, recv_buf, 12, MPI_DOUBLE, comm) != MPI_SUCCESS) {
			PL_ERROSH << "[ERROR]MPIPolylib::init_parallel_info():MPI_Allgather "
				<< "faild." << std::endl;

		}

	} else {
		PL_ERROSH << "[ERROR]MPIPolylib::init_parallel_info():MPI_Allgather "
			<< "real type is not double nor float"<<std::endl;
		PL_ERROSH << __func__ <<"sizeof(PL_REAL)" <<sizeof(PL_REAL) << std::endl;
		return PLSTAT_MPI_ERROR;
	}

#ifdef DEBUG
	PL_DBGOSH << __func__ <<m_myrank << std::endl;
	PL_DBGOSH << __func__ <<"sizeof(PL_REAL)" <<sizeof(PL_REAL) << std::endl;
	PL_DBGOSH << __func__ <<"m_numproc" <<m_numproc << std::endl;

	PL_DBGOSH << __func__ <<"recv_buf (" << std::endl;
	for(int inode=0;inode<m_numproc;++inode){
		for(int iline=0;iline<4;++iline){
			for(int ielem=0;ielem<3;++ielem){
				PL_DBGOSH <<  recv_buf[ielem+iline*3+inode*12] <<", ";
			}
			PL_DBGOSH <<  ")"<< std::endl;
		}
	}
#endif //DEBUG


	// 受信データの展開
	for (int irank = 0; irank < m_numproc; irank++) {
		// 自PE領域情報はスキップ
		if( irank == m_myrank ) continue;
		for (i = 0; i < 3; i++) {
			v_bpos[i] = recv_buf[i + 12*irank];
		}
		for (i = 0; i < 3; i++) {
			v_bbsize[i] = recv_buf[3 + i + 12*irank];
		}
		for (i = 0; i < 3; i++) {
			v_gcsize[i] = recv_buf[6 + i + 12*irank];
		}
		for (i = 0; i < 3; i++) {
			v_dx[i]	= recv_buf[9 + i + 12*irank];
		}
#ifdef DEBUG
		PL_DBGOSH << "(rank:" << irank << "):" <<"bpos  :" << v_bpos  << std::endl;
		PL_DBGOSH << "(rank:" << irank << "):" <<"bbsize:" << v_bbsize << std::endl;
		PL_DBGOSH << "(rank:" << irank << "):" <<"gcsize:" << v_gcsize << std::endl;
		PL_DBGOSH << "(rank:" << irank << "):" <<"dx    :" << v_dx << std::endl;
#endif
		ParallelInfo* proc = new (ParallelInfo);
		proc->m_comm = comm;
		proc->m_rank = irank;
		proc->m_area.m_bpos = v_bpos;
		proc->m_area.m_bbsize = v_bbsize;
		proc->m_area.m_gcsize = v_gcsize;
		proc->m_area.m_dx = v_dx;
		proc->m_area.m_gcell_min = v_bpos-( v_gcsize )*v_dx;
		proc->m_area.m_gcell_max = v_bpos+( v_bbsize+v_gcsize )*v_dx;
		proc->m_area.m_gcell_bbox.add(proc->m_area.m_gcell_min);
		proc->m_area.m_gcell_bbox.add(proc->m_area.m_gcell_max);

		// 全PE領域情報リストに追加
		m_other_procs.push_back(proc);

		// 自PE領域と隣接するPE領域情報はm_neibour_procsにも追加
		if( m_myproc.m_area.m_gcell_bbox.crossed(proc->m_area.m_gcell_bbox) ) {
			m_neibour_procs.push_back(proc);
#ifdef DEBUG
			PL_DBGOSH << m_myrank << ": " << "neighbour rank:" << proc->m_rank  << std::endl;
#endif
		}
	}
	// 受信領域あとしまつ
	delete[] recv_buf;

	return PLSTAT_OK;
	//#undef DEBUG
}


// new tp version  without  PolylibConfig
// public /////////////////////////////////////////////////////////////////////

POLYLIB_STAT
	MPIPolylib::load_rank0(
	std::string config_filename,
	PL_REAL scale
	)
{
	//#define TIME_MEASURE
#ifdef TIME_MEASURE
	double time1=0;
	double time2=0;
	double time3=0;
	double time4=0;
	time1=MPI_Wtime();

#endif // TIME_MEASURE

	//#define DEBUG
#ifdef DEBUG
	PL_DBGOSH << m_myrank << ": " << "MPIPolylib::load_rank0() in. " << std::endl;
#endif
	POLYLIB_STAT ret;
	std::string       config_contents;

	// for tp
	try {
		//PolylibConfig base(config_filename);
		this->tp->read(config_filename);

#ifdef DEBUG
		PL_DBGOSH << m_myrank << ": " << "MPIPolylib::load_rank0() tp->read end. " << std::endl;
#endif


		//ret = make_group_tree(&base);
		// only on rank0 ?
		ret = this->make_group_tree(this->tp);

#ifdef DEBUG
		PL_DBGOSH << m_myrank << ": " << "MPIPolylib::load_rank0() make_group_tree end. " << std::endl;
#endif

		if( ret != PLSTAT_OK ) return ret;
	}
	catch( POLYLIB_STAT e ){
		return e;
	}

	if( m_myrank == 0 ) {

		// ポリゴン情報を構築 (三角形IDファイルは不要なので、第二引数はダミー)

#ifdef DEBUG
		PL_DBGOSH << m_myrank << ": " << "MPIPolylib::load_rank0() load_polygons start. " << std::endl;
#endif

		if( (ret = this->load_polygons(false, ID_BIN, scale)) != PLSTAT_OK ) {
			PL_ERROSH << "[ERROR]MPIPolylib::load_rank0():load_polygons() faild."
				<<" returns:" << PolylibStat2::String(ret) << std::endl;
			return ret;
		}


#ifdef TIME_MEASURE
		time2=MPI_Wtime();
#endif // TIME_MEASURE

#ifdef DEBUG
		PL_DBGOSH << m_myrank << ": " << "MPIPolylib::load_rank0() load_polygons end. " << std::endl;
#endif


		// ポリゴン情報を他PEへ配信する。
		if( (ret = send_polygons_to_all()) != PLSTAT_OK ) {
			PL_ERROSH << "[ERROR]MPIPolylib::load_rank0():send_polygons_to_all()"
				<<" faild. returns:" << PolylibStat2::String(ret) << std::endl;
			return ret;
		}

#ifdef TIME_MEASURE
		time3=MPI_Wtime();
#endif // TIME_MEASURE

#ifdef DEBUG
		PL_DBGOSH << m_myrank << ": " << "MPIPolylib::load_rank0() send_polyons_to_all. " << std::endl;
#endif

		// 他PE領域ポリゴン情報を削除して自領域分のみでデータ構造再構築
		if( (ret = erase_outbounded_polygons()) != PLSTAT_OK ) {
#ifdef DEBUG
			PL_DBGOSH << m_myrank << ": " << "MPIPolylib::load_rank0() erase_outbounded_polygons.  failed" << std::endl;
#endif

			PL_ERROSH << "[ERROR]MPIPolylib::load_rank0():erase_outbounded_polygons()"
				<<" faild. returns:" << PolylibStat2::String(ret) << std::endl;
			return ret;
		}

#ifdef DEBUG
		PL_DBGOSH << m_myrank << ": " << "MPIPolylib::load_rank0() erase_outbounded_polygons. " << std::endl;
#endif

	} else { //for other rank

		// ポリゴン情報をrank0から受信する。
		if( (ret = receive_polygons_from_rank0()) != PLSTAT_OK ) {
			PL_ERROSH << "[ERROR]MPIPolylib::load_rank0():receive_polygons_from_rank0()"
				<<" faild. returns:" << PolylibStat2::String(ret) << std::endl;
			return ret;
		}
	}

#ifdef TIME_MEASURE
	time4=MPI_Wtime();
	PL_DBGOSH << m_myrank << ": " << "MPIPolylib::load_rank0() time measure" << std::endl;
	PL_DBGOSH << m_myrank << ":time1 " << time1 << std::endl;
	PL_DBGOSH << m_myrank << ":time2 " << time2-time1 << std::endl;
	PL_DBGOSH << m_myrank << ":time3 " << time3-time1 << std::endl;
	PL_DBGOSH << m_myrank << ":time4 " << time4-time1 << std::endl;

#endif // TIME_MEASURE

	//#undef TIME_MEASURE

	return PLSTAT_OK;

	//#undef DEBUG
}

//////////////////////////////////////////////
POLYLIB_STAT
    MPIPolylib::load_only_in_rank0(
    std::string config_filename,
    PL_REAL scale
    )
{
    //#define TIME_MEASURE
#ifdef TIME_MEASURE
    double time1=0;
    double time2=0;
    time1=MPI_Wtime();

#endif // TIME_MEASURE

    //#define DEBUG
#ifdef DEBUG
    PL_DBGOSH << m_myrank << ": " << "MPIPolylib::load_only_in_rank0() in. " << std::endl;
#endif
    POLYLIB_STAT ret;
    std::string       config_contents;

    // for tp
    try {
        //PolylibConfig base(config_filename);
        this->tp->read(config_filename);

#ifdef DEBUG
        PL_DBGOSH << m_myrank << ": " << "MPIPolylib::load_only_in_rank0() tp->read end. " << std::endl;
#endif


        //ret = make_group_tree(&base);
        // only on rank0 ?
        ret = this->make_group_tree(this->tp);

#ifdef DEBUG
        PL_DBGOSH << m_myrank << ": " << "MPIPolylib::load_only_in_rank0() make_group_tree end. " << std::endl;
#endif

        if( ret != PLSTAT_OK ) return ret;
    }
    catch( POLYLIB_STAT e ){
        return e;
    }

    if( m_myrank == 0 ) {

        // ポリゴン情報を構築 (三角形IDファイルは不要なので、第二引数はダミー)

#ifdef DEBUG
        PL_DBGOSH << m_myrank << ": " << "MPIPolylib::load_only_in_rank0() load_polygons start. " << std::endl;
#endif

        if( (ret = this->load_polygons(false, ID_BIN, scale)) != PLSTAT_OK ) {
            PL_ERROSH << "[ERROR]MPIPolylib::load_only_in_rank0():load_polygons() faild."
                <<" returns:" << PolylibStat2::String(ret) << std::endl;
            return ret;
        }


#ifdef TIME_MEASURE
        time2=MPI_Wtime();
#endif // TIME_MEASURE

#ifdef DEBUG
        PL_DBGOSH << m_myrank << ": " << "MPIPolylib::load_only_in_rank0() load_polygons end. " << std::endl;
#endif
    }

#ifdef TIME_MEASURE
    PL_DBGOSH << m_myrank << ": " << "MPIPolylib::load_only_in_rank0() time measure" << std::endl;
    PL_DBGOSH << m_myrank << ":time1 " << time1 << std::endl;
    PL_DBGOSH << m_myrank << ":time2 " << time2-time1 << std::endl;
#endif // TIME_MEASURE
    return PLSTAT_OK;
}

POLYLIB_STAT
    MPIPolylib::distribute_only_from_rank0(
    )
{
    //#define TIME_MEASURE
#ifdef TIME_MEASURE
    double time3=0;
    double time4=0;
#endif // TIME_MEASURE

    POLYLIB_STAT ret;

    if( m_myrank == 0 ) {

        // ポリゴン情報を他PEへ配信する。
        if( (ret = send_polygons_to_all()) != PLSTAT_OK ) {
            PL_ERROSH << "[ERROR]MPIPolylib::distribute_only_from_rank0():send_polygons_to_all()"
                <<" faild. returns:" << PolylibStat2::String(ret) << std::endl;
            return ret;
        }

#ifdef TIME_MEASURE
        time3=MPI_Wtime();
#endif // TIME_MEASURE

#ifdef DEBUG
        PL_DBGOSH << m_myrank << ": " << "MPIPolylib::distribute_only_from_rank0() send_polyons_to_all. " << std::endl;
#endif

        // 他PE領域ポリゴン情報を削除して自領域分のみでデータ構造再構築
        if( (ret = erase_outbounded_polygons()) != PLSTAT_OK ) {
#ifdef DEBUG
            PL_DBGOSH << m_myrank << ": " << "MPIPolylib::distribute_only_from_rank0() erase_outbounded_polygons.  failed" << std::endl;
#endif

            PL_ERROSH << "[ERROR]MPIPolylib::distribute_only_from_rank0():erase_outbounded_polygons()"
                <<" faild. returns:" << PolylibStat2::String(ret) << std::endl;
            return ret;
        }

#ifdef DEBUG
        PL_DBGOSH << m_myrank << ": " << "MPIPolylib::distribute_only_from_rank0() erase_outbounded_polygons. " << std::endl;
#endif

    } else { //for other rank

        // ポリゴン情報をrank0から受信する。
        if( (ret = receive_polygons_from_rank0()) != PLSTAT_OK ) {
            PL_ERROSH << "[ERROR]MPIPolylib::distribute_only_from_rank0():receive_polygons_from_rank0()"
                <<" faild. returns:" << PolylibStat2::String(ret) << std::endl;
            return ret;
        }
    }

#ifdef TIME_MEASURE
    time4=MPI_Wtime();
    PL_DBGOSH << m_myrank << ": " << "MPIPolylib::distribute_only_from_rank0() time measure" << std::endl;
    PL_DBGOSH << m_myrank << ":time3 " << time3 << std::endl;
    PL_DBGOSH << m_myrank << ":time4 " << time4-time3 << std::endl;
#endif // TIME_MEASURE

    return PLSTAT_OK;
}

// public /////////////////////////////////////////////////////////////////////

POLYLIB_STAT
	MPIPolylib::load_parallel(
	std::string config_filename,
	ID_FORMAT	id_format
	)
{

#ifdef DEBUG
	PL_DBGOSH << "MPIPolylib::load_parallel() in. " << std::endl;
#endif
	POLYLIB_STAT ret;

	// 設定ファイル読み込み、グループ階層構造構築、ポリゴン情報構築。
	// MPIPolylib::save_parallel()で保存された設定ファイルであることが前提
	if( (ret = Polylib::load_with_idfile( config_filename, id_format )) != PLSTAT_OK ) {
		PL_ERROSH << "[ERROR]MPIPolylib::load_parallel():Polylib::load() faild. returns:" << PolylibStat2::String(ret) << std::endl;
		return ret;
	}

	return PLSTAT_OK;
}

// new version
// public /////////////////////////////////////////////////////////////////////

POLYLIB_STAT
	MPIPolylib::save_rank0(
	std::string *p_config_filename,
	std::string stl_format,
	std::string extend
	)
{
	//#define DEBUG
#ifdef DEBUG
	PL_DBGOSH << "MPIPolylib::save_rank0() in. " << std::endl;
#endif
	POLYLIB_STAT	ret;

	if( m_myrank == 0 ) {
		// 他rankからポリゴン情報を受信
		if(stl_format==TriMeshIO::FMT_VTK_A ||
			stl_format==TriMeshIO::FMT_VTK_B){ // VTK output

#ifdef DEBUG
				PL_DBGOSH << "MPIPolylib::save_rank0() VTK. |" << stl_format <<"| |"<<TriMeshIO::FMT_VTK_A<<"|" << std::endl;
#endif

				if( (ret = gather_polygons_vtk()) != PLSTAT_OK ) {
					PL_ERROSH << "[ERROR]MPIPolylib::save_rank0():gather_polygons_vtk() faild. returns:"
						<< PolylibStat2::String(ret) << std::endl;
					return ret;
				}


		} else { //other
#ifdef DEBUG
			PL_DBGOSH << "MPIPolylib::save_rank0() Normal. |" << stl_format <<"| |"<<TriMeshIO::FMT_VTK_A<<"|" << std::endl;
#endif

			if( (ret = gather_polygons()) != PLSTAT_OK ) {
				PL_ERROSH << "[ERROR]MPIPolylib::save_rank0():gather_polygons() faild. returns:"
					<< PolylibStat2::String(ret) << std::endl;
				return ret;
			}

		}
		// グループ階層構造、ポリゴン情報をファイルへ保存

		// for tp

#ifdef DEBUG
		PL_DBGOSH << "MPIPolylib::save_rank0() before save " << std::endl;
#endif

		if( (ret = Polylib::save( p_config_filename, stl_format, extend )) != PLSTAT_OK ) {
			PL_ERROSH << "[ERROR]MPIPolylib::save_rank0():Polylib::save() faild. returns:"
				<< PolylibStat2::String(ret) << std::endl;
			return ret;
		}

	}
	else {

		// rank0へポリゴン情報を送信
		if(stl_format==TriMeshIO::FMT_VTK_A ||
			stl_format==TriMeshIO::FMT_VTK_B){ // VTK output

				if( (ret = send_polygons_to_rank0_vtk()) != PLSTAT_OK ) {
					PL_ERROSH << "[ERROR]MPIPolylib::save_rank0():send_polygons_to_rank0() faild."
						<<" returns:"<< PolylibStat2::String(ret) << std::endl;
					return ret;
				}

		} else { // nomal

#ifdef DEBUG
			PL_DBGOSH << "MPIPolylib::save_rank0() send_polygons_to_rank0" << std::endl;
#endif

			if( (ret = send_polygons_to_rank0()) != PLSTAT_OK ) {
				PL_ERROSH << "[ERROR]MPIPolylib::save_rank0():send_polygons_to_rank0() faild."
					<<" returns:"<< PolylibStat2::String(ret) << std::endl;
				return ret;
			} else {
#ifdef DEBUG
				PL_DBGOSH << "MPIPolylib::save_rank0() send_polygons_to_rank0 end rank " << m_myrank<< std::endl;
#endif
			}
		}
	}
	return PLSTAT_OK;
	//#undef DEBUG
}


// public /////////////////////////////////////////////////////////////////////

POLYLIB_STAT
	MPIPolylib::save_parallel(
	std::string *p_config_filename,
	std::string stl_format,
	std::string extend,
	ID_FORMAT	id_format
	){

		//#define DEBUG
#ifdef DEBUG
		PL_DBGOSH << "MPIPolylib::save_parallel() in. " << std::endl;
#endif
		POLYLIB_STAT ret;

		// 各ランク毎に保存
		if( (ret = Polylib::save_with_rankno( p_config_filename, m_myrank, m_numproc-1, extend, stl_format, id_format)) != PLSTAT_OK ) {
			PL_ERROSH << "[ERROR]MPIPolylib::save_parallel():Polylib::save_with_rankno():failed. returns:" << PolylibStat2::String(ret) << std::endl;
			return ret;
		} else { //good
#ifdef DEBUG
			PL_DBGOSH << "MPIPolylib::save_parallel() ready to out. " << std::endl;
#endif
		}

		return PLSTAT_OK;
		//#undef DEBUG
}


// public ////////////////////////////////////////////////////////////////////

POLYLIB_STAT
	MPIPolylib::move(
	PolylibMoveParams &params
	)
{
#ifdef DEBUG
	PL_DBGOSH << "MPIPolylib::move() in. " << std::endl;
#endif
	POLYLIB_STAT ret;
	std::vector<PolygonGroup*>::iterator group_itr;
	PolygonGroup *p_pg;

	// 各ポリゴングループのmove()を実行
	// 全グループに対して
	for (group_itr = this->m_pg_list.begin(); group_itr != this->m_pg_list.end(); group_itr++) {
		p_pg = (*group_itr);

		// 移動する可能性のあるポリゴングループのみ対象
		if( p_pg->get_movable() ) {

			// move実行前から隣接PE領域に懸かっている三角形をmigrate対象除外リストに載せる
			if( (ret = select_excluded_trias( p_pg )) != PLSTAT_OK ) {
				PL_ERROSH << "[ERROR]MPIPolylib::move():select_exclude_trias() failed. returns:" << PolylibStat2::String(ret) << std::endl;
				return ret;
			}

			// move実行
			if( (ret = p_pg->move( params )) != PLSTAT_OK ) {
				PL_ERROSH << "[ERROR]MPIPolylib::move():(*group_itr)->move() failed. returns:" << PolylibStat2::String(ret) << std::endl;
				return ret;
			}

			// KD木を再構築 (三角形同士の位置関係が変化したため、再構築が必要)
			if( (ret = p_pg->rebuild_polygons()) != PLSTAT_OK ) {
				PL_ERROSH << "[ERROR]MPIPolylib::move():(*group_itr)->rebuild_polygons() failed. returns:" << PolylibStat2::String(ret) << std::endl;
				return ret;
			}
		}
	}
	return PLSTAT_OK;
}


// public /////////////////////////////////////////////////////////////////////

POLYLIB_STAT
	MPIPolylib::migrate(
	)
{
	//#define DEBUG
#ifdef DEBUG
	PL_DBGOSH << "MPIPolylib::migrate() in. " << std::endl;
#endif
	POLYLIB_STAT ret;
	unsigned int i, j;
	std::vector<PolygonGroup*>::iterator group_itr;
	PolygonGroup *p_pg;
	std::vector<ParallelInfo*>::iterator procs_itr;

	std::vector<PrivateTriangle*> const *p_trias;
	std::vector<int>   send_num_trias;
	int          *p_send_num_trias_array;
	std::vector<int>   send_tria_ids;
	int          *p_send_tria_ids_array;
	std::vector<int>   send_tria_exids;
	int          *p_send_tria_exids_array;
	std::vector<PL_REAL> send_trias;
	PL_REAL        *p_send_trias_array;

	// 送信バッファ領域をMPI_WaitAll()後に纏めてdeleteするための
	// 配列アドレス記憶用ベクタ
	std::vector<int*>  send_num_trias_bufs;
	std::vector<int*>  send_tria_ids_bufs;
	std::vector<int*>  send_tria_exids_bufs;
	std::vector<PL_REAL*> send_trias_bufs;

	// 送信用MPI_Reqeust配列を確保
	MPI_Request *mpi_reqs = new MPI_Request[ m_neibour_procs.size() * 4 ]; // 隣接PEごとに4回Isendする
	MPI_Status  *mpi_stats = new MPI_Status[ m_neibour_procs.size() * 4 ];
	int reqs_pos = 0;

	//隣接PEごとに移動三角形情報を送信
	for (procs_itr = m_neibour_procs.begin(); procs_itr != m_neibour_procs.end(); procs_itr++) {

		// 送信用一時データ初期化
		send_num_trias.clear();
		send_trias.clear();
		send_tria_ids.clear();
		send_tria_exids.clear();

		// 全ポリゴングループに対して
		for( group_itr=this->m_pg_list.begin(); group_itr!=this->m_pg_list.end(); group_itr++ ) {
			p_pg = (*group_itr);
			p_trias = NULL;

			// ポリゴン情報を持つグループだけ
			//if( p_pg->get_triangles() != NULL
			// && p_pg->get_triangles()->size() != 0 ) {

			// 移動する可能性のあるポリゴングループのみ対象
#ifdef DEBUG
			PL_DBGOSH << "pg_name:" << p_pg->get_name() << " movable:" << p_pg->get_movable() << std::endl;
#endif
			if( p_pg->get_movable() ) {

				// 当該隣接PE領域への移動除外三角形IDリストを取得
				std::map< int, std::vector<int> >::iterator const itr =
					(*procs_itr)->m_exclusion_map.find( p_pg->get_internal_id() );

				// 当該隣接PE領域内にある移動フラグONの三角形を取得
				p_trias = p_pg->search_outbounded(
					(*procs_itr)->m_area.m_gcell_bbox,
					&((*itr).second) );
			}

			// グループIDと当該グループの三角形数の対を送信データに追加
			pack_num_trias( &send_num_trias, p_pg->get_internal_id(), p_trias );

			// 三角形情報を送信データに追加
			pack_trias( &send_trias, p_trias );

			// 三角形ID情報を送信データに追加
			pack_tria_ids( &send_tria_ids, p_trias );

			// 三角形のユーザ定義ID情報を送信データに追加
			pack_tria_exids( &send_tria_exids, p_trias );

			// search結果の後始末
			if( p_trias ) delete p_trias;
		}

		//-----  送信データをシリアライズ
		// 送信データ初期化
		p_send_num_trias_array = NULL;
		p_send_tria_ids_array  = NULL;
		p_send_tria_exids_array  = NULL;
		p_send_trias_array     = NULL;

		// グループID,グループ毎三角形数リスト
		if( send_num_trias.size() > 0 ) {
			p_send_num_trias_array = new int[ send_num_trias.size() ];
		}
		for( i=0; i<send_num_trias.size(); i++ ) {
			p_send_num_trias_array[i] = send_num_trias[i];
		}
		// 三角形IDリスト
		if( send_tria_ids.size() > 0 ) {
			p_send_tria_ids_array  = new int[ send_tria_ids.size() ];
		}
		for( i=0; i<send_tria_ids.size(); i++ ) {
			p_send_tria_ids_array[i] = send_tria_ids[i];
		}
		// 三角形のユーザ定義IDリスト
		if( send_tria_exids.size() > 0 ) {
			p_send_tria_exids_array  = new int[ send_tria_exids.size() ];
		}
		for( i=0; i<send_tria_exids.size(); i++ ) {
			p_send_tria_exids_array[i] = send_tria_exids[i];
		}
		// 三角形頂点リスト
		if( send_trias.size() > 0 ) {
			p_send_trias_array = new PL_REAL[ send_trias.size() ];
		}
		for( i=0; i<send_trias.size(); i++ ) {
			p_send_trias_array[i] = send_trias[i];
		}

		// 送信データの先頭アドレスを記憶（MPI_Wait後にdeleteするため）
		send_num_trias_bufs.push_back( p_send_num_trias_array );
		send_tria_ids_bufs.push_back( p_send_tria_ids_array );
		send_tria_exids_bufs.push_back( p_send_tria_exids_array );
		send_trias_bufs.push_back( p_send_trias_array );

		// 当該PEへ非同期送信 (MPI_Wait()は後でまとめて行う)
#ifdef DEBUG
		PL_DBGOSH << "sending polygons rank:" << m_myrank <<  "->rank:"
			<< (*procs_itr)->m_rank << " ";
		for( i=0; i< send_num_trias.size(); i+=2 ) {
			PL_DBGOS << "(gid:" << send_num_trias[i]
			<< ",num_tria:" << send_num_trias[i+1] << ")";
			PL_DBGOS << " (gid:" << p_send_num_trias_array[i]
			<< ",num_tria:" << p_send_num_trias_array[i+1] << ")";

		}
		PL_DBGOS << std::endl;
#endif
		if (MPI_Isend( p_send_num_trias_array, send_num_trias.size(),
			MPI_INT, (*procs_itr)->m_rank, MPITAG_NUM_TRIAS,
			m_mycomm, &mpi_reqs[reqs_pos++] ) != MPI_SUCCESS) {
				PL_ERROSH << "[ERROR]MPIPolylib::migrate():MPI_Isend,"
					<< "MPITAG_NUM_TRIAS faild." << std::endl;
				return PLSTAT_MPI_ERROR;
		}

		if (MPI_Isend( p_send_tria_ids_array,  send_tria_ids.size(),
			MPI_INT, (*procs_itr)->m_rank, MPITAG_TRIA_IDS,
			m_mycomm, &mpi_reqs[reqs_pos++] ) != MPI_SUCCESS) {
				PL_ERROSH << "[ERROR]MPIPolylib::migrate():MPI_Isend,"
					<< " MPITAG_TRIA_IDS faild." << std::endl;
				return PLSTAT_MPI_ERROR;
		}

		if (MPI_Isend( p_send_tria_exids_array,  send_tria_exids.size(),
			MPI_INT, (*procs_itr)->m_rank, MPITAG_TRIA_EXIDS,
			m_mycomm, &mpi_reqs[reqs_pos++] ) != MPI_SUCCESS) {
				PL_ERROSH << "[ERROR]MPIPolylib::migrate():MPI_Isend,"
					<< " MPITAG_TRIA_EXIDS faild." << std::endl;
				return PLSTAT_MPI_ERROR;
		}

#ifdef DEBUG
		PL_DBGOSH << "sending data " <<std::endl;
		for(int ii=0;ii<send_trias.size();ii++){
			PL_DBGOSH << ii << " " << p_send_trias_array[ii]<<std::endl;
		}
		PL_DBGOSH << "sending data end" <<std::endl;
#endif

		if(sizeof(PL_REAL)==4){
			//PL_DBGOSH << __func__ << " float " << to_string((PL_REAL)) << std::endl;
			if (MPI_Isend( p_send_trias_array,     send_trias.size(),
				MPI_FLOAT, (*procs_itr)->m_rank, MPITAG_TRIAS,
				m_mycomm, &mpi_reqs[reqs_pos++] ) != MPI_SUCCESS) {
					PL_ERROSH << "[ERROR]MPIPolylib::init_parallel_info():MPI_Isend "
						<< "faild." << std::endl;
					return PLSTAT_MPI_ERROR;

			} else {
#ifdef DEBUG
				PL_DBGOSH << "sending data p_send_trias_array success" <<std::endl;
#endif
			}
		} else if (sizeof(PL_REAL)==8){
			if (MPI_Isend( p_send_trias_array,     send_trias.size(),
				MPI_DOUBLE, (*procs_itr)->m_rank, MPITAG_TRIAS,
				m_mycomm, &mpi_reqs[reqs_pos++] ) != MPI_SUCCESS) {

					PL_ERROSH << "[ERROR]MPIPolylib::migrate():MPI_Isend,"
						<< " MPITAG_TRIAS faild." << std::endl;
					return PLSTAT_MPI_ERROR;
			} else {
#ifdef DEBUG
				PL_DBGOSH << "sending data p_send_trias_array success" <<std::endl;
#endif
			}
		} else {
			PL_ERROSH << "[ERROR]MPIPolylib::init_parallel_info():MPI_Allgather "
				<< "real type is not double nor float"<<std::endl;
			PL_ERROSH << __func__ <<"sizeof(PL_REAL)" <<sizeof(PL_REAL) << std::endl;
			return PLSTAT_MPI_ERROR;
		}
	}


#ifdef DEBUG
	//     PL_DBGOSH << "sending data to "<<  (*procs_itr)->m_rank << " end" <<std::endl;
	PL_DBGOSH << "sending data to  end" <<std::endl;
#endif

	//隣接PEごとに移動三角形情報を受信
	for (procs_itr = m_neibour_procs.begin(); procs_itr != m_neibour_procs.end(); procs_itr++) {
		int pos_id, pos_exid, pos_tria;
		MPI_Request mpi_req;
		MPI_Status  mpi_stat;

		// グループIDとグループ毎三角形数の対を非同期受信
		// グループ情報は各rank共有しているのでグループ数は予め分かっている
		int *p_intarray = new int[ this->m_pg_list.size()*2 ];
		if (MPI_Irecv( p_intarray, this->m_pg_list.size()*2, MPI_INT, (*procs_itr)->m_rank,
			MPITAG_NUM_TRIAS, m_mycomm, &mpi_req ) != MPI_SUCCESS) {
				PL_ERROSH << "[ERROR]MPIPolylib::migrate():MPI_Irecv,"
					<< ",MPITAG_NUM_TRIAS faild." << std::endl;
				return PLSTAT_MPI_ERROR;
		}
		if (MPI_Wait( &mpi_req, &mpi_stat ) != MPI_SUCCESS) {
			PL_ERROSH << "[ERROR]MPIPolylib::migrate():MPI_Wait,"
				<< "MPITAG_NUM_TRIAS faild." << std::endl;
			return PLSTAT_MPI_ERROR;
		}
#ifdef DEBUG
		PL_DBGOSH << "receiving polygons rank:" << (*procs_itr)->m_rank
			<<  "->rank:" << m_myrank << " ";
		for( i=0; i< this->m_pg_list.size()*2-1; i+=2 ) {
			PL_DBGOS << "(gid:" << p_intarray[i]
			<< ",num_tria:" << p_intarray[i+1] << ")";
		}
		PL_DBGOS << std::endl;
#endif

		// 受信三角形数を算出
		int total_tria_num = 0;
		for( i=1; i<this->m_pg_list.size() * 2; i+=2 ){
			total_tria_num += p_intarray[i];
		}

		// 三角形IDリストを非同期受信
		int *p_idarray = new int[ total_tria_num ];
		if (MPI_Irecv( p_idarray,  total_tria_num, MPI_INT, (*procs_itr)->m_rank,
			MPITAG_TRIA_IDS, m_mycomm, &mpi_req ) != MPI_SUCCESS) {
				PL_ERROSH << "[ERROR]MPIPolylib::migrate():MPI_Irecv,"
					<< "MPI_INT faild." << std::endl;
				return PLSTAT_MPI_ERROR;
		} else {
#ifdef DEBUG
			PL_DBGOSH << "receive p_idarray" << std::endl;
#endif
		}
		if (MPI_Wait( &mpi_req, &mpi_stat ) != MPI_SUCCESS) {
			PL_ERROSH << "[ERROR]MPIPolylib::migrate():MPI_Wait,"
				<< "MPI_INT faild." << std::endl;
			return PLSTAT_MPI_ERROR;
		}  else {
#ifdef DEBUG
			PL_DBGOSH << "p_idarray ..." << std::endl;
			for(int i=0;i<total_tria_num ;++i){
				PL_DBGOSH << i<<" "<< p_idarray[i] << std::endl;
			}
#endif
		}

		// 三角形のユーザ定義IDリストを非同期受信
		int *p_exidarray = new int[ total_tria_num ];
		if (MPI_Irecv( p_exidarray,  total_tria_num, MPI_INT, (*procs_itr)->m_rank,
			MPITAG_TRIA_EXIDS, m_mycomm, &mpi_req ) != MPI_SUCCESS) {
				PL_ERROSH << "[ERROR]MPIPolylib::migrate():MPI_Irecv,"
					<< "MPI_INT faild." << std::endl;
				return PLSTAT_MPI_ERROR;
		} else {
#ifdef DEBUG
			PL_DBGOSH << "receive p_exidarray" << std::endl;
#endif
		}
		if (MPI_Wait( &mpi_req, &mpi_stat ) != MPI_SUCCESS) {
			PL_ERROSH << "[ERROR]MPIPolylib::migrate():MPI_Wait,"
				<< "MPI_INT faild." << std::endl;
			return PLSTAT_MPI_ERROR;
		}  else {
#ifdef DEBUG
			PL_DBGOSH << "p_exidarray ..." << std::endl;
			for(int i=0;i<total_tria_num ;++i){
				PL_DBGOSH << i<<" "<< p_exidarray[i] << std::endl;
			}
#endif
		}

		// 三角形リストを非同期受信
		PL_REAL *p_triaarray = new PL_REAL[ total_tria_num*3*3 ];

		if(sizeof(PL_REAL)==4){

			if (MPI_Irecv( p_triaarray, total_tria_num*3*3,
				MPI_FLOAT, (*procs_itr)->m_rank,
				MPITAG_TRIAS, m_mycomm, &mpi_req ) != MPI_SUCCESS) {
					PL_ERROSH << "[ERROR]MPIPolylib::migrate():MPI_Irecv,PL_MPI_REAL:"
						<< " faild." << std::endl;
					return PLSTAT_MPI_ERROR;
			} else {
				// #ifdef DEBUG
				// 	 PL_DBGOSH << "receive p_triaarray" << std::endl;
				// 	 for (int ii=0;ii<total_tria_num*3*3;++ii){
				// 	   PL_DBGOSH << ii<< " " << p_triaarray[ii]<<std::endl;
				//}
				// #endif
			}
		} else  if(sizeof(PL_REAL)==8){
			if (MPI_Irecv( p_triaarray, total_tria_num*3*3, MPI_DOUBLE, (*procs_itr)->m_rank,
				MPITAG_TRIAS, m_mycomm, &mpi_req ) != MPI_SUCCESS) {
					PL_ERROSH << "[ERROR]MPIPolylib::migrate():MPI_Irecv,PL_MPI_REAL:"
						<< " faild." << std::endl;
					return PLSTAT_MPI_ERROR;
			} else {
				// #ifdef DEBUG
				// 	 PL_DBGOSH << "receive p_triaarray" << std::endl;
				// 	 for (int ii=0;ii<total_tria_num*3*3;++ii){
				// 	   PL_DBGOSH << ii<< " " << p_triaarray[ii]<<std::endl;
				//	 }
				// #endif
			}
		} else {
			PL_ERROSH << "[ERROR]MPIPolylib::migrate():MPI_Irecv,PL_MPI_REAL:"
				<< " faild." << std::endl;
			return PLSTAT_MPI_ERROR;

		}



		if (MPI_Wait( &mpi_req, &mpi_stat ) != MPI_SUCCESS) {
			PL_ERROSH << "[ERROR]MPIPolylib::migrate():MPI_Wait,PL_MPI_REAL"
				<< " faild." << std::endl;
			return PLSTAT_MPI_ERROR;
		}  else {
#ifdef DEBUG
			PL_DBGOSH << "receive finish waiting ...2" << std::endl;

#endif
		}

#ifdef DEBUG
		PL_DBGOSH << "receive p_triaarray" << std::endl;
		for (int ii=0;ii<total_tria_num*3*3;++ii){
			PL_DBGOSH << ii<< " " << p_triaarray[ii]<<std::endl;
		}

#endif


		// 各ポリゴングループに対して三角形情報を追加
		pos_id = 0;
		pos_exid = 0;
		pos_tria = 0;
		for( i=0; i<this->m_pg_list.size()*2-1; i+=2 ){
#ifdef DEBUG
			PL_DBGOSH << "start adding trinagles to PolygonGroup"<<i << std::endl;
#endif
			// ポリゴングループID
			int pg_id = p_intarray[i];
#ifdef DEBUG
			PL_DBGOSH << "adding trinagles to PolygonGroup"
				<< i <<" polygon group ID " << pg_id << std::endl;
#endif
			// 当該ポリゴングループの三角形数
			unsigned int num_trias = p_intarray[i+1];
#ifdef DEBUG
			PL_DBGOSH << "adding trinagles to PolygonGroup"
				<< i <<" # of triangles. "<<num_trias << std::endl;
#endif
			// グループIDのポリゴングループインスタンス取得
			PolygonGroup* p_pg = this->get_group( pg_id );

			if( p_pg == NULL ) {
				PL_ERROSH << "[ERROR]MPIPolylib::migrate():invalid pg_id:"
					<< pg_id << std::endl;
				return PLSTAT_NG;
			}

#ifdef DEBUG
			PL_DBGOSH << "adding trinagles to PolygonGroup"
				<< i <<"  pg_id  "<< pg_id << std::endl;
#endif
			// PrivateTriangleのベクタ - 受信データ配列からベクタへの変換用
			std::vector<PrivateTriangle*> tria_vec;

			// ベクタに受信データ内容をコピー
			for( j=0; j<num_trias; j++ ) {
				//tria_vec.push_back(
				//	      new PrivateTriangle(&p_triaarray[pos_tria], p_idarray[pos_id]) );
#ifdef DEBUG
				PL_DBGOSH << "adding trinagles to PolygonGroup"
					<< i <<" copy data to tria_vec "<< j << std::endl;
				PL_DBGOSH << pos_id << " " << p_idarray[pos_id]<<std::endl;
				PL_DBGOSH << pos_tria   << " " << p_triaarray[pos_tria]   <<std::endl;
				PL_DBGOSH << pos_tria+1 << " " << p_triaarray[pos_tria+1] <<std::endl;
				PL_DBGOSH << pos_tria+2 << " " << p_triaarray[pos_tria+2] <<std::endl;
				PL_DBGOSH << pos_tria+3 << " " << p_triaarray[pos_tria+3] <<std::endl;
				PL_DBGOSH << pos_tria+4 << " " << p_triaarray[pos_tria+4] <<std::endl;
				PL_DBGOSH << pos_tria+5 << " " << p_triaarray[pos_tria+5] <<std::endl;
				PL_DBGOSH << pos_tria+6 << " " << p_triaarray[pos_tria+6] <<std::endl;
				PL_DBGOSH << pos_tria+7 << " " << p_triaarray[pos_tria+7] <<std::endl;
				PL_DBGOSH << pos_tria+8 << " " << p_triaarray[pos_tria+8] <<std::endl;
				PL_DBGOSH << &tria_vec << std::endl;
#endif //DEBUG
				// add vertex first
				// vertex compaction performed later.

				Vertex* vertex_ptr[3];
				vertex_ptr[0] = new Vertex(p_triaarray[pos_tria],
					p_triaarray[pos_tria+1],
					p_triaarray[pos_tria+2]);
				vertex_ptr[1] =  new Vertex(p_triaarray[pos_tria+3],
					p_triaarray[pos_tria+4],
					p_triaarray[pos_tria+5]);
				vertex_ptr[2] =  new Vertex(p_triaarray[pos_tria+6],
					p_triaarray[pos_tria+7],
					p_triaarray[pos_tria+8]);

				p_pg->get_vertexlist()->vtx_add_nocheck(vertex_ptr[0]);
				p_pg->get_vertexlist()->vtx_add_nocheck(vertex_ptr[1]);
				p_pg->get_vertexlist()->vtx_add_nocheck(vertex_ptr[2]);

				tria_vec.push_back( new PrivateTriangle(vertex_ptr,
					p_idarray[pos_id], p_exidarray[pos_exid]));
				//	   tria_vec.push_back(
				//new PrivateTriangle<T>(&p_triaarray[pos_tria],
				//p_idarray[pos_id]) );

#ifdef DEBUG
				PL_DBGOSH << "adding trinagles to tria_vec."<<std::endl;
#endif
				pos_id++;
				pos_exid++;
				pos_tria+=9;

			} // triangle loop



#ifdef DEBUG
			PL_DBGOSH << "adding trinagles to PolygonGroup"
				<< i <<" copy data to tria_vec" << std::endl;
#endif

			// ポリゴングループに三角形リストを追加
			if( (ret = p_pg->add_triangles( &tria_vec )) != PLSTAT_OK ) {
				PL_ERROSH << "[ERROR]MPIPolylib::migrate():p_pg->add_triangles() failed. returns:"
					<< PolylibStat2::String(ret) << std::endl;
				return ret;
			}

#ifdef DEBUG
			PL_DBGOSH << "adding trinagles to PolygonGroup"
				<< i <<" call add_triangles." << std::endl;
#endif
			// ベクタの内容あとしまつ
			for( j=0; j<num_trias; j++ ) {
				delete tria_vec.at(j);
			}
		}

		// 受信領域あとしまつ
		delete[] p_intarray;
		delete[] p_idarray;
		delete[] p_exidarray;
		delete[] p_triaarray;
	}

	// MPI_Isend()を纏めてアンロック
	if (MPI_Waitall( m_neibour_procs.size()*4, mpi_reqs, mpi_stats ) != MPI_SUCCESS) {
		PL_ERROSH << "[ERROR]MPIPolylib::migrate():MPI_Waitall failed." << std::endl;
		return PLSTAT_MPI_ERROR;
	}

	// 送信データ領域をdelete
	for( i=0; i<send_num_trias_bufs.size(); i++ ) {
		delete[] send_num_trias_bufs.at(i);
	}
	for( i=0; i<send_tria_ids_bufs.size(); i++ ) {
		delete[] send_tria_ids_bufs.at(i);
	}
	for( i=0; i<send_tria_exids_bufs.size(); i++ ) {
		delete[] send_tria_exids_bufs.at(i);
	}
	for( i=0; i<send_trias_bufs.size(); i++ ) {
		delete[] send_trias_bufs.at(i);
	}

	// 移動してきた三角形を含めたKD木を再構築
	for (group_itr = this->m_pg_list.begin(); group_itr != this->m_pg_list.end(); group_itr++) {
		p_pg = (*group_itr);

		// 移動可能グループだけ
		if( p_pg->get_movable() && p_pg->get_triangles() != NULL ) {

			// KD木を再構築
			if( (ret=p_pg->rebuild_polygons()) != PLSTAT_OK ) {
				PL_ERROSH << "[ERROR]MPIPolylib::migrate():p_pg->rebuild_polygons() failed. returns:"
					<< PolylibStat2::String(ret) << std::endl;
				return ret;
			}
		}
	}

	// 自PE領域外ポリゴン情報を消去
	if( erase_outbounded_polygons() != PLSTAT_OK ) {
		PL_ERROSH << "[ERROR]MPIPolylib::migrate():erace_outbounded_polygons() failed." << std::endl;
	}

	// 後始末 2010.08.24
	delete[] mpi_reqs;
	delete[] mpi_stats;

#ifdef DEBUG
	PL_DBGOSH << "MPIPolylib::migrate() out normaly." << std::endl;
#endif
	return PLSTAT_OK;
	//#undef DEBUG
}


// public /////////////////////////////////////////////////////////////////////

ParallelInfo* MPIPolylib::get_proc(int rank)
{
	std::vector<ParallelInfo*>::iterator itr;
	itr = m_other_procs.begin();
	for (; itr != m_other_procs.end(); itr++) {
		if ((*itr)->m_rank == rank) {
			return(*itr);
		}
	}
	return NULL;
}


// public /////////////////////////////////////////////////////////////////////

unsigned int MPIPolylib::used_memory_size()
{
	unsigned int								size;
	std::map< int, std::vector<int> >::iterator	ex;
	std::vector<ParallelInfo *>::iterator			pi;

	// Polylibクラスが管理している領域を取得
	size = Polylib::used_memory_size();

	// 自PE担当領域情報
	size += sizeof(ParallelInfo);
	size += m_myproc.m_exclusion_map.size() * (sizeof(int)+sizeof(std::vector<int>));
	for (ex = m_myproc.m_exclusion_map.begin();
		ex != m_myproc.m_exclusion_map.end(); ex++) {
			size += ex->second.size() * sizeof(int);
	}

	// 自PEを除く全PE担当領域情報リスト
	size += sizeof(std::vector<ParallelInfo *>);
	for (pi = m_other_procs.begin(); pi != m_other_procs.end(); pi++) {
		size += sizeof(ParallelInfo);
		size += (*pi)->m_exclusion_map.size() * (sizeof(int)+sizeof(std::vector<int>));
		for (ex = (*pi)->m_exclusion_map.begin();
			ex != (*pi)->m_exclusion_map.end(); ex++) {
				size += ex->second.size() * sizeof(int);
		}
	}

	// 隣接PE担当領域情報リスト
	size += sizeof(std::vector<ParallelInfo *>);
	size += m_neibour_procs.size() * sizeof(std::vector<ParallelInfo *>);

	// 自プロセスのランク数、全プロセス数
	size += sizeof(int) * 2;

	// 自プロセスが利用するコミュニケーター
	size += sizeof(MPI_Comm);

	return size;
}


// protected //////////////////////////////////////////////////////////////////

MPIPolylib::MPIPolylib() : Polylib()
{
}


// protected //////////////////////////////////////////////////////////////////

MPIPolylib::~MPIPolylib()
{
	std::vector<ParallelInfo*>::iterator itr;

	// 全PE領域情報リストを消去
	for ( itr=m_other_procs.begin(); itr != m_other_procs.end(); itr++ ) {
		delete *itr;
	}
	// 隣接PE領域情報リストについては、m_other_procsの内容と同じインスタンスを保持
	// するため、上記の処理で消去済。
}


// protected //////////////////////////////////////////////////////////////////

POLYLIB_STAT
	MPIPolylib::broadcast_config(
	std::string config_contents
	)
{
#ifdef DEBUG
	PL_DBGOSH << "MPIPolylib::broadcast_config() in. " << std::endl;
#endif

	// 文字数を送信(NULL文字も文字数にカウント）
	int data = config_contents.size()+1;
	if (MPI_Bcast( &data, 1, MPI_INT, 0, m_mycomm ) != MPI_SUCCESS) {
		PL_ERROSH << "[ERROR]MPIPolylib::broadcast_config():MPI_Bcast,"
			<< "MPI_INT faild." << std::endl;
		return PLSTAT_MPI_ERROR;
	}

	// ファイル内容文字列の送信
	if (MPI_Bcast( (void*)(config_contents.c_str()), config_contents.size()+1,
		MPI_CHAR, 0, m_mycomm ) != MPI_SUCCESS) {
			PL_ERROSH << "[ERROR]MPIPolylib::broadcast_config():MPI_Bcast,"
				<< "MPI_CHAR faild." << std::endl;
			return PLSTAT_MPI_ERROR;
	}

	return PLSTAT_OK;
}


// protected //////////////////////////////////////////////////////////////////

POLYLIB_STAT
	MPIPolylib::send_polygons_to_all(
	)
{
#ifdef DEBUG
	PL_DBGOSH << "MPIPolylib::send_polygons_to_all() in. " << std::endl;
#endif
	unsigned int i;
	std::vector<ParallelInfo*>::iterator	proc_itr;
	std::vector<PolygonGroup*>::iterator group_itr;
	PolygonGroup *p_pg;
	std::vector<PrivateTriangle*> const *p_trias;

	std::vector<int>   send_num_trias;
	int          *p_send_num_trias_array;
	std::vector<int>   send_tria_ids;
	int          *p_send_tria_ids_array;
	std::vector<int>   send_tria_exids;
	int          *p_send_tria_exids_array;
	std::vector<PL_REAL> send_trias;
	PL_REAL        *p_send_trias_array;

	// 全PEに対して
	for (proc_itr = m_other_procs.begin(); proc_itr != m_other_procs.end(); proc_itr++) {

		// 送信用一時データ初期化
		p_trias = NULL;
		send_num_trias.clear();
		send_trias.clear();
		send_tria_ids.clear();
		send_tria_exids.clear();

		// 全グループに対して
		for (group_itr = this->m_pg_list.begin(); group_itr != this->m_pg_list.end(); group_itr++) {
			p_pg = (*group_itr);
			p_trias = NULL;

			// ポリゴン情報を持つグループだけ
			if( p_pg->get_triangles() != NULL && p_pg->get_triangles()->size() != 0 ) {

				// 当該PE領域内に一部でも含まれるポリゴンを検索
				p_trias = p_pg->search( &((*proc_itr)->m_area.m_gcell_bbox), false );
			}

			// グループIDと当該グループの三角形数の対を送信データに追加
			pack_num_trias( &send_num_trias, p_pg->get_internal_id(), p_trias );

			// 三角形情報を送信データに追加
			pack_trias( &send_trias, p_trias );

			// 三角形ID情報を送信データに追加
			pack_tria_ids( &send_tria_ids, p_trias );

			// 三角形のユーザ定義ID情報を送信データに追加
			pack_tria_exids( &send_tria_exids, p_trias );

			// search結果の後始末
			if( p_trias ) delete p_trias;
		}

		//----  送信データをシリアライズ
		// 送信データ配列初期化
		p_send_num_trias_array = NULL;
		p_send_tria_ids_array  = NULL;
		p_send_tria_exids_array  = NULL;
		p_send_trias_array     = NULL;

		// グループID,グループ毎三角形数リスト
		if( send_num_trias.size() > 0 ) {
			p_send_num_trias_array = new int[ send_num_trias.size() ];
		}
		for( i=0; i<send_num_trias.size(); i++ ) {
			p_send_num_trias_array[i] = send_num_trias[i];
		}
		// 三角形IDリスト
		if( send_tria_ids.size() > 0 ) {
			p_send_tria_ids_array  = new int[ send_tria_ids.size() ];
		}
		for( i=0; i<send_tria_ids.size(); i++ ) {
			p_send_tria_ids_array[i] = send_tria_ids[i];
		}
		// 三角形のユーザ定義IDリスト
		if( send_tria_exids.size() > 0 ) {
			p_send_tria_exids_array  = new int[ send_tria_exids.size() ];
		}
		for( i=0; i<send_tria_exids.size(); i++ ) {
			p_send_tria_exids_array[i] = send_tria_exids[i];
		}
		// 三角形頂点座標リスト
		if( send_trias.size() > 0 ) {
			p_send_trias_array = new PL_REAL[ send_trias.size() ];
		}
		for( i=0; i<send_trias.size(); i++ ) {
			p_send_trias_array[i] = send_trias[i];
		}

		// 当該PEへ送信
#ifdef DEBUG
		PL_DBGOSH << "sending polygons rank:0->rank:" << (*proc_itr)->m_rank << " ";
		for( i=0; i< send_num_trias.size(); i+=2 ) {
			PL_DBGOS << "(gid:" << send_num_trias[i]
			<< ",num_tria:" << send_num_trias[i+1] << ")";
		}
		PL_DBGOS << std::endl;
#endif
		if (MPI_Send( p_send_num_trias_array, send_num_trias.size(), MPI_INT,
			(*proc_itr)->m_rank, MPITAG_NUM_TRIAS, m_mycomm ) != MPI_SUCCESS) {
				PL_ERROSH << "[ERROR]MPIPolylib::send_polygons_to_all():MPI_Send,"
					<< "MPITAG_NUM_TRIAS faild." << std::endl;
				return PLSTAT_MPI_ERROR;
		}
		if (MPI_Send( p_send_tria_ids_array,  send_tria_ids.size(),  MPI_INT,
			(*proc_itr)->m_rank, MPITAG_TRIA_IDS,  m_mycomm ) != MPI_SUCCESS) {
				PL_ERROSH << "[ERROR]MPIPolylib::send_polygons_to_all():MPI_Send,"
					<< "MPITAG_TRIA_IDS faild." << std::endl;
				return PLSTAT_MPI_ERROR;
		}
		if (MPI_Send( p_send_tria_exids_array,  send_tria_exids.size(),  MPI_INT,
			(*proc_itr)->m_rank, MPITAG_TRIA_EXIDS,  m_mycomm ) != MPI_SUCCESS) {
				PL_ERROSH << "[ERROR]MPIPolylib::send_polygons_to_all():MPI_Send,"
					<< "MPITAG_TRIA_EXIDS faild." << std::endl;
				return PLSTAT_MPI_ERROR;
		}

		if(sizeof(PL_REAL)==4){
			//PL_DBGOSH << "MPIPolylib::"<<__func__<<" float"<<std::endl;
			if (MPI_Send( p_send_trias_array, send_trias.size(), MPI_FLOAT,
				(*proc_itr)->m_rank, MPITAG_TRIAS, m_mycomm ) != MPI_SUCCESS) {

					PL_ERROSH << "[ERROR]MPIPolylib::send_polygons_to_all():MPI_Send,"
						<< "MPITAG_TRIAS faild." << std::endl;
					return PLSTAT_MPI_ERROR;
			}
		} else if(sizeof(PL_REAL)==8){
			//PL_DBGOSH << "MPIPolylib::"<<__func__<<" double 1"<<std::endl;
			if (MPI_Send( p_send_trias_array,     send_trias.size(),     MPI_DOUBLE,
				(*proc_itr)->m_rank, MPITAG_TRIAS,     m_mycomm ) != MPI_SUCCESS) {
					PL_ERROSH << "[ERROR]MPIPolylib::send_polygons_to_all():MPI_Send,"
						<< "MPITAG_TRIAS faild." << std::endl;
					return PLSTAT_MPI_ERROR;
			}
		} else {

			PL_ERROSH << "[ERROR]MPIPolylib::send_polygons_to_all():MPI_Send,"
				<< "MPITAG_TRIAS faild." << std::endl;
			return PLSTAT_MPI_ERROR;
		}





		// 送信データの後始末
		if( p_send_num_trias_array ) delete[] p_send_num_trias_array;
		if( p_send_tria_ids_array )  delete[] p_send_tria_ids_array;
		if( p_send_tria_exids_array )  delete[] p_send_tria_exids_array;
		if( p_send_trias_array )     delete[] p_send_trias_array;
	}
	return PLSTAT_OK;
}


// protected //////////////////////////////////////////////////////////////////

POLYLIB_STAT
	MPIPolylib::pack_num_trias(
	std::vector<int>* p_vec,
	int group_id,
	const std::vector<PrivateTriangle*>* p_trias
	)
{
#ifdef DEBUG
	PL_DBGOSH << "MPIPolylib::pack_num_trias() in. " << std::endl;
#endif
	// 出力配列に、グループID、グループ内三角形数、の順に追加
	int num = 0;
	if( p_trias ) num = p_trias->size();
	p_vec->push_back( group_id );
	p_vec->push_back( num );

	return PLSTAT_OK;
}


// protected //////////////////////////////////////////////////////////////////

POLYLIB_STAT
	MPIPolylib::pack_trias(
	std::vector<PL_REAL>* p_vec,
	const std::vector<PrivateTriangle*>* p_trias
	)
{
#ifdef DEBUG
	PL_DBGOSH << "MPIPolylib::pack_trias() in. " << std::endl;
#endif
	if( p_trias == NULL ) return PLSTAT_OK;

	// 出力配列に、三角形頂点座標を順に追加
	for( unsigned int i=0; i<p_trias->size(); i++ ) {
		Vertex** tmp_vlist=p_trias->at(i)->get_vertex();
		for( unsigned int j=0; j<3; j++ ) {
			Vertex* vtmp= tmp_vlist[j];
			for( unsigned int k=0; k<3; k++ ) {
				p_vec->push_back(  (*vtmp)[k] );
				//p_vec->push_back( p_trias->at(i)->get_vertex()[j][k] );
			}
		}
	}
	return PLSTAT_OK;
}

// protected ////////////////////////////////////////////////////////////////// //for DVertex

POLYLIB_STAT
	MPIPolylib::pack_tria_ndata(
	std::vector<int>* p_vec,
	const std::vector<PrivateTriangle*>* p_trias
	)
{
#ifdef DEBUG
	PL_DBGOSH << "MPIPolylib::pack_trias() in. " << std::endl;
#endif
	if( p_trias == NULL || p_trias->size()==0 ) {
		p_vec->push_back(0);
		p_vec->push_back(0);

		return PLSTAT_OK;
	}


	// 出力配列に、三角形頂点座標を順に追加


	PrivateTriangle* p_tria=p_trias->at(0);
	DVertexTriangle* p_dvtria=NULL;
	try{
		p_dvtria=dynamic_cast<DVertexTriangle*>(p_tria);
	}
	catch ( std::bad_cast ){
		std::cerr << "bad cast"<<std::endl;
	}

	DVertex** tmp_dvlist= p_dvtria->get_DVertex();
	DVertex* dvtmp= tmp_dvlist[0];
	p_vec->push_back(dvtmp->DVM()->nscalar());
	p_vec->push_back(dvtmp->DVM()->nvector());

	return PLSTAT_OK;
}


// protected ////////////////////////////////////////////////////////////////// //for DVertex

POLYLIB_STAT
	MPIPolylib::pack_tria_scalar_data(
	std::vector<PL_REAL>* p_vec,
	const std::vector<PrivateTriangle*>* p_trias
	)
{
#ifdef DEBUG
	PL_DBGOSH << "MPIPolylib::pack_trias() in. " << std::endl;
#endif
	if( p_trias == NULL ) return PLSTAT_OK;

	// 出力配列に、三角形頂点座標を順に追加
	for( unsigned int i=0; i<p_trias->size(); i++ ) {

		PrivateTriangle* p_tria=p_trias->at(i);
		DVertexTriangle* p_dvtria=NULL;
		try{
			p_dvtria=dynamic_cast<DVertexTriangle*>(p_tria);
		}
		catch ( std::bad_cast ){
			std::cerr << "bad cast"<<std::endl;
		}

		DVertex** tmp_dvlist= p_dvtria->get_DVertex();
		for( unsigned int j=0; j<3; j++ ) {
			DVertex* dvtmp= tmp_dvlist[j];

			for( unsigned int k=0; k<dvtmp->DVM()->nscalar();k++) {
				p_vec->push_back( dvtmp->get_scalar(k));

			}
		}
	}
	return PLSTAT_OK;
}

// protected ////////////////////////////////////////////////////////////////// //for DVertex

POLYLIB_STAT
	MPIPolylib::pack_tria_vector_data(
	std::vector<PL_REAL>* p_vec,
	const std::vector<PrivateTriangle*>* p_trias
	)
{
#ifdef DEBUG
	PL_DBGOSH << "MPIPolylib::pack_trias() in. " << std::endl;
#endif
	if( p_trias == NULL ) return PLSTAT_OK;

	// 出力配列に、三角形頂点座標を順に追加
	for( unsigned int i=0; i<p_trias->size(); i++ ) {
		PrivateTriangle* p_tria=p_trias->at(i);
		DVertexTriangle* p_dvtria=NULL;
		try{
			p_dvtria=dynamic_cast<DVertexTriangle*>(p_tria);
		}
		catch ( std::bad_cast ){
			std::cerr << "bad cast"<<std::endl;
		}

		DVertex** tmp_dvlist= p_dvtria->get_DVertex();
		for( unsigned int j=0; j<3; j++ ) {
			DVertex* dvtmp= tmp_dvlist[j];

			for(unsigned int k=0;
				k<dvtmp->DVM()->nvector();
				k++) {
					Vec3<PL_REAL> dvect;
					dvtmp->get_vector(k,&dvect);
					for(int l=0;l<3;l++) {
						// PL_DBGOSH<<__func__
						// 	       <<" "<<i
						// 	       <<" "<<j
						// 	       << " "<<l
						// 	       << " " <<dvect[l] <<std::endl;
						p_vec->push_back(dvect[l] );
					}

			}
		}
	}
	return PLSTAT_OK;
}



// protected //////////////////////////////////////////////////////////////////

POLYLIB_STAT
	MPIPolylib::pack_tria_ids(
	std::vector<int>* p_vec,
	const std::vector<PrivateTriangle*>* p_trias
	)
{
#ifdef DEBUG
	PL_DBGOSH << "MPIPolylib::pack_tria_ids() in. " << std::endl;
#endif
	if( p_trias == NULL ) return PLSTAT_OK;

	// 出力配列に、三角形IDを順に追加
	for( unsigned int i=0; i<p_trias->size(); i++ ) {
    //std::cout << p_trias->at(i)->get_id() << ",";
		p_vec->push_back( p_trias->at(i)->get_id() );
	}
	return PLSTAT_OK;
}


// protected //////////////////////////////////////////////////////////////////

POLYLIB_STAT
	MPIPolylib::pack_tria_exids(
	std::vector<int>* p_vec,
	const std::vector<PrivateTriangle*>* p_trias
	)
{
#ifdef DEBUG
	PL_DBGOSH << "MPIPolylib::pack_tria_exids() in. " << std::endl;
#endif
	if( p_trias == NULL ) return PLSTAT_OK;

	// 出力配列に、三角形のユーザ定義IDを順に追加
	for( unsigned int i=0; i<p_trias->size(); i++ ) {
		p_vec->push_back( p_trias->at(i)->get_exid() );
	}
	return PLSTAT_OK;
}


// protected //////////////////////////////////////////////////////////////////

POLYLIB_STAT
	MPIPolylib::erase_outbounded_polygons(
	)
{
	//#define DEBUG
#ifdef DEBUG
	PL_DBGOSH << "MPIPolylib::erase_outbounded_polygons() in. " << std::endl;
#endif
	POLYLIB_STAT ret;
	unsigned int i;
	std::vector<PolygonGroup*>::iterator group_itr;
	std::vector<PrivateTriangle*> const *p_trias;
	std::vector<PrivateTriangle*>  copy_trias;
	PolygonGroup *p_pg;


	// 各ポリゴングループのポリゴン情報を自領域分のみで再構築
	// 全グループに対して
#ifdef DEBUG
	int counter=0;
#endif
	for (group_itr = this->m_pg_list.begin(); group_itr != this->m_pg_list.end(); group_itr++) {
		p_pg = (*group_itr);

#ifdef DEBUG
		PL_DBGOSH << "MPIPolylib::erase_outbounded_polygons() poligon loop. "<< p_pg->get_triangles() << std::endl;
		if( p_pg->get_triangles() != NULL)
			PL_DBGOSH<<" " << p_pg->get_triangles()->size()  << std::endl;

#endif


		// ポリゴン情報を持つグループだけ
		if( p_pg->get_triangles() != NULL && p_pg->get_triangles()->size() != 0 ) {

			// 自領域内に一部でも含まれるポリゴンを検索
			//p_pg->build_polygon_tree();
			p_trias = p_pg->search( &(m_myproc.m_area.m_gcell_bbox), false );

			// 検索結果のディープコピーを作成
			copy_trias.clear();
			if( p_trias ) {
				for( i=0; i<p_trias->size(); i++ ) {
					copy_trias.push_back( new PrivateTriangle(*(p_trias->at(i))) );
				}
			}

			// 検索結果でポリゴン情報を再構築
			if( (ret = p_pg->init( &copy_trias, true )) != PLSTAT_OK ) {
				PL_ERROSH << "[ERROR]MPIPolylib::erase_outbounded_polygons():p_pg->init() failed. returns:"
					<< PolylibStat2::String(ret) << std::endl;
				return ret;
			}

#ifdef DEBUG
			PL_DBGOSH << "MPIPolylib::erase_outbounded_polygons() end of init. " << std::endl;
#endif

			// search結果の後始末
			if( p_trias ) delete p_trias;
			for( i=0; i<copy_trias.size(); i++ ) {
				delete copy_trias.at(i);
			}

#ifdef DEBUG
			PL_DBGOSH << "MPIPolylib::erase_outbounded_polygons() end of cleanup. " << std::endl;
#endif

		}
#ifdef DEBUG
		PL_DBGOSH << "MPIPolylib::erase_outbounded_polygons() poligon loop. end "<< counter << std::endl;
		counter++;
#endif

	}
	return PLSTAT_OK;
	//#undef DEBUG
}


// protected //////////////////////////////////////////////////////////////////

POLYLIB_STAT
	MPIPolylib::broadcast_config_from_rank0(
	)
{
#ifdef DEBUG
	PL_DBGOSH << "MPIPolylib::broadcast_config_from_rank0() in. " << std::endl;
#endif
	POLYLIB_STAT ret;
	int str_len;
	char *p_str;
	std::string config_str;

	// 文字数をb_castで受信(NULL文字も文字数に含まれる)
	if (MPI_Bcast( &str_len, 1, MPI_INT, 0, m_mycomm ) != MPI_SUCCESS) {
		PL_ERROSH << "[ERROR]MPIPolylib::broadcast_config_from_rank0()"
			<< ":MPI_Bcast,MPI_INT faild." << std::endl;
		return PLSTAT_MPI_ERROR;
	}

	// XML文字列をb_castで受信
	p_str = new char[str_len];
	if (MPI_Bcast( p_str, str_len, MPI_CHAR, 0, m_mycomm ) != MPI_SUCCESS) {
		PL_ERROSH << "[ERROR]MPIPolylib::broadcast_config_from_rank0()"
			<< ":MPI_Bcast,MPI_CHAR faild." << std::endl;
		return PLSTAT_MPI_ERROR;
	}
#ifdef DEBUG
	PL_DBGOSH << "received config-string:" << p_str << std::endl;
#endif

	// XML文字列からグループ階層構造を構築
	config_str = p_str;

	if( (ret = this->make_group_tree( this->tp )) != PLSTAT_OK ) {

		PL_ERROSH << "[ERROR]MPIPolylib::broadcast_config_from_rank0():make_group_tree() faild. returns:" << ret << std::endl;
		return ret;
	}

	return PLSTAT_OK;
}


// protected //////////////////////////////////////////////////////////////////

POLYLIB_STAT
	MPIPolylib::receive_polygons_from_rank0(
	)
{
	//#define DEBUG
#ifdef DEBUG
	PL_DBGOSH << "MPIPolylib::receive_polygons_from_rank0() in. " << std::endl;
#endif

	unsigned int i, j;
	unsigned int pos_id, pos_exid, pos_tria;
	MPI_Status mpi_stat;

	// グループIDとグループ毎三角形数の対をrank0から受信
	// グループ情報は配信済みなので、グループ数は予め分かっている
	int *p_intarray = new int[ this->m_pg_list.size() * 2 ];
	if (MPI_Recv( p_intarray, this->m_pg_list.size() * 2, MPI_INT, 0,
		MPITAG_NUM_TRIAS, m_mycomm, &mpi_stat ) != MPI_SUCCESS) {
			PL_ERROSH << "[ERROR]MPIPolylib::receive_polygons_from_rank0()"
				<< ":MPI_Recv,MPITAG_NUM_TRIAS faild." << std::endl;
			return PLSTAT_MPI_ERROR;
	}
#ifdef DEBUG
	PL_DBGOSH << "    pintarray:(";
	for( i=0; i< this->m_pg_list.size()*2; i++ ) {
		PL_DBGOS << p_intarray[i] << ",";
	}
	PL_DBGOS << ")" << std::endl;
#endif

	// 自領域の全三角形数を算出
	unsigned int total_tria_num = 0;
	for( i=1; i<this->m_pg_list.size() * 2; i+=2 ){
		total_tria_num += p_intarray[i];
	}
#ifdef DEBUG
	PL_DBGOSH << __func__ <<" total_tria_num "<<total_tria_num << std::endl;
#endif
	// 三角形IDリストをrank0から受信
	int *p_idarray = new int[ total_tria_num ];
	if (MPI_Recv( p_idarray,  total_tria_num, MPI_INT, 0,
		MPITAG_TRIA_IDS, m_mycomm, &mpi_stat ) != MPI_SUCCESS) {
			PL_ERROSH << "[ERROR]MPIPolylib::receive_polygons_from_rank0()"
				<< ":MPI_Recv,MPITAG_TRIA_IDS faild." << std::endl;
			return PLSTAT_MPI_ERROR;
	}
#ifdef DEBUG
	PL_DBGOSH << "    pidarray:(";
	for( i=0; i<total_tria_num; i++ ) {
		PL_DBGOS << p_idarray[i] << ",";
	}
	PL_DBGOS << ")" << std::endl;
#endif

	// 三角形のユーザ定義IDリストをrank0から受信
	int *p_exidarray = new int[ total_tria_num ];
	if (MPI_Recv( p_exidarray,  total_tria_num, MPI_INT, 0,
		MPITAG_TRIA_EXIDS, m_mycomm, &mpi_stat ) != MPI_SUCCESS) {
			PL_ERROSH << "[ERROR]MPIPolylib::receive_polygons_from_rank0()"
				<< ":MPI_Recv,MPITAG_TRIA_EXIDS faild." << std::endl;
			return PLSTAT_MPI_ERROR;
	}
#ifdef DEBUG
	PL_DBGOSH << "    pexidarray:(";
	for( i=0; i<total_tria_num; i++ ) {
		PL_DBGOS << p_exidarray[i] << ",";
	}
	PL_DBGOS << ")" << std::endl;
#endif

	// 三角形リストをrank0から受信
	PL_REAL *p_triaarray = new PL_REAL[ total_tria_num * 3 * 3 ];

	if(sizeof(PL_REAL)==4){
		//PL_DBGOSH << "MPIPolylib::"<<__func__<<" float"<<std::endl;
		if (MPI_Recv( p_triaarray, total_tria_num * 3 * 3, MPI_FLOAT, 0,
			MPITAG_TRIAS, m_mycomm, &mpi_stat ) != MPI_SUCCESS) {
				PL_ERROSH << "[ERROR]MPIPolylib::receive_polygons_from_rank0()"
					<< ":MPI_Recv,MPITAG_TRIAS faild." << std::endl;
				return PLSTAT_MPI_ERROR;
		}
	} else  if(sizeof(PL_REAL)==8){
		//PL_DBGOSH << "MPIPolylib::"<<__func__<<" double 1"<<std::endl;
		if (MPI_Recv( p_triaarray, total_tria_num * 3 * 3, MPI_DOUBLE, 0,
			MPITAG_TRIAS, m_mycomm, &mpi_stat ) != MPI_SUCCESS) {

				PL_ERROSH << "[ERROR]MPIPolylib::receive_polygons_from_rank0()"
					<< ":MPI_Recv,MPITAG_TRIAS faild." << std::endl;
				return PLSTAT_MPI_ERROR;
		}
	} else {
		PL_ERROSH << "[ERROR]MPIPolylib::receive_polygons_from_rank0()"
			<< ":MPI_Recv,MPITAG_TRIAS faild." << std::endl;
		return PLSTAT_MPI_ERROR;
	}

#ifdef DEBUG
	PL_DBGOSH << "    ptriaarray:(";
	for( i=0; i<total_tria_num*3*3; i++ ) {
		PL_DBGOS << p_triaarray[i] << ",";
	}
	PL_DBGOS << ")" << std::endl;
#endif
	//#define DEBUG
	// 各ポリゴングループに対して三角形情報を設定＆KD木構築
	pos_id = 0;
	pos_exid = 0;
	pos_tria = 0;
	int n_start_tri=0;
	int n_start_id=0;
	int n_start_exid=0;
	for( i=0; i<this->m_pg_list.size()*2-1; i+=2 ){	// 偶数番目の値を処理

		// ポリゴングループID
		int pg_id = p_intarray[i];

		// 当該ポリゴングループの三角形数
		unsigned int num_trias = p_intarray[i+1];

#ifdef DEBUG
		PL_DBGOSH << " pg_id " <<pg_id << " num_trias "<<num_trias << std::endl;
#endif
		// グループIDのポリゴングループインスタンス取得
		PolygonGroup* p_pg = this->get_group( pg_id );
		if( p_pg == NULL ) {
			PL_ERROSH << "[ERROR]MPIPolylib::receive_polygons_from_rank0():invalid pg_id:"
				<< pg_id << std::endl;
			return PLSTAT_NG;
		}

#ifdef DEBUG
		PL_DBGOSH << " get group instance " <<num_trias<< std::endl;
#endif


		// ポリゴングループに三角形リストを設定、KD木構築
		if( p_pg->init( p_triaarray, p_idarray, p_exidarray, n_start_tri, n_start_id, n_start_exid, num_trias ) != PLSTAT_OK ) {
			PL_ERROSH << "[ERROR]MPIPolylib::receive_polygons_from_rank0():p_pg->init() failed:" << std::endl;
			return PLSTAT_NG;
		}
		n_start_tri+=num_trias*9;
		n_start_id+=num_trias;
		n_start_exid+=num_trias;

		// // PrivateTriangleのベクタ生成
		// std::vector<PrivateTriangle*> tria_vec;

		// // ベクタに受信データ内容を設定
		// for( j=0; j<num_trias; j++ ) {
		// 	tria_vec.push_back( new PrivateTriangle(&p_triaarray[pos_tria], p_idarray[pos_id]) );
		// 	pos_id++;
		// 	pos_tria+=9;
		// }

		// // ポリゴングループに三角形リストを設定、KD木構築
		// if( p_pg->init( &tria_vec, true ) != PLSTAT_OK ) {
		// 	PL_ERROSH << "[ERROR]MPIPolylib::receive_polygons_from_rank0():p_pg->init() failed:" << std::endl;
		// 	return PLSTAT_NG;
		// }

		// // ベクタの内容あとしまつ
		// for( j=0; j<num_trias; j++ ) {
		// 	delete tria_vec.at(j);
		// }

	}

	// 受信領域あとしまつ
	delete[] p_intarray;
	delete[] p_idarray;
	delete[] p_exidarray;
	delete[] p_triaarray;

#ifdef DEBUG
	PL_DBGOSH << "MPIPolylib::receive_polygons_from_rank0() out. " << std::endl;
#endif
	//#undef DEBUG
	return PLSTAT_OK;
}


// protected //////////////////////////////////////////////////////////////////

POLYLIB_STAT
	MPIPolylib::gather_polygons(){
		//#define DEBUG
#ifdef DEBUG
		PL_DBGOSH << "MPIPolylib::gather_polygons() in. " << std::endl;
#endif

		POLYLIB_STAT ret;
		unsigned int i, j;
		int rank;
		unsigned int pos_id, pos_exid, pos_tria;
		MPI_Status mpi_stat;

		// 全rankからポリゴン情報を受信
		for( rank=1; rank<m_numproc; rank++ ) {

			// グループIDとグループ毎三角形数の対を受信
			// グループ情報は全rank共通なので、グループ数は予め分かっている
			int *p_intarray = new int[ this->m_pg_list.size() * 2 ];
			if (MPI_Recv( p_intarray, this->m_pg_list.size() * 2, MPI_INT, rank,
				MPITAG_NUM_TRIAS, m_mycomm, &mpi_stat ) != MPI_SUCCESS) {
					PL_ERROSH << "[ERROR]MPIPolylib::gather_polygons()"
						<< ":MPI_Recv,MPITAG_NUM_TRIAS faild.:rank=" << rank
						<< std::endl;
					return PLSTAT_MPI_ERROR;
			}
#ifdef DEBUG
			PL_DBGOSH << "MPIPolylib::gather_polygons() get group id and # of triangles." << std::endl;
#endif
			// 受信する全三角形数を算出
			unsigned int total_tria_num = 0;
			for( i=1; i<this->m_pg_list.size() * 2; i+=2 ){
				total_tria_num += p_intarray[i];
			}
#ifdef DEBUG
			PL_DBGOSH << "MPIPolylib::gather_polygons() calc # of triangles." << std::endl;
#endif

			// 三角形IDリストを受信
			int *p_idarray = new int[ total_tria_num ];
			if (MPI_Recv( p_idarray,  total_tria_num, MPI_INT, rank,
				MPITAG_TRIA_IDS, m_mycomm, &mpi_stat ) != MPI_SUCCESS) {
					PL_ERROSH << "[ERROR]MPIPolylib::gather_polygons()"
						<< ":MPI_Recv,MPITAG_TRIA_IDS faild.:rank=" << rank
						<< std::endl;
					return PLSTAT_MPI_ERROR;
			}
#ifdef DEBUG
			PL_DBGOSH << "MPIPolylib::gather_polygons() get id list." << std::endl;
#endif

			// 三角形のユーザ定義IDリストを受信
			int *p_exidarray = new int[ total_tria_num ];
			if (MPI_Recv( p_exidarray,  total_tria_num, MPI_INT, rank,
				MPITAG_TRIA_EXIDS, m_mycomm, &mpi_stat ) != MPI_SUCCESS) {
					PL_ERROSH << "[ERROR]MPIPolylib::gather_polygons()"
						<< ":MPI_Recv,MPITAG_TRIA_EXIDS faild.:rank=" << rank
						<< std::endl;
					return PLSTAT_MPI_ERROR;
			}
#ifdef DEBUG
			PL_DBGOSH << "MPIPolylib::gather_polygons() get id list." << std::endl;
#endif

			// 三角形リストを受信
			PL_REAL *p_triaarray = new PL_REAL[ total_tria_num * 3 * 3 ];

			if(sizeof(PL_REAL)==4){
				if (MPI_Recv( p_triaarray, total_tria_num * 3 * 3, MPI_FLOAT, rank,
					MPITAG_TRIAS, m_mycomm, &mpi_stat ) != MPI_SUCCESS) {
						PL_ERROSH << "[ERROR]MPIPolylib::gather_polygons()"
							<< ":MPI_Recv,MPITAG_TRIA_IDS faild.:rank=" << rank
							<< std::endl;
						return PLSTAT_MPI_ERROR;
				}
			} else if(sizeof(PL_REAL)==8){
				if (MPI_Recv( p_triaarray, total_tria_num * 3 * 3, MPI_DOUBLE, rank,
					MPITAG_TRIAS, m_mycomm, &mpi_stat ) != MPI_SUCCESS)  {
						PL_ERROSH << "[ERROR]MPIPolylib::gather_polygons()"
							<< ":MPI_Recv,MPITAG_TRIA_IDS faild.:rank=" << rank
							<< std::endl;
						return PLSTAT_MPI_ERROR;
				}
			} else {
				PL_ERROSH << "[ERROR]MPIPolylib::gather_polygons()"
					<< ":MPI_Recv,MPITAG_TRIA_IDS faild.:rank=" << rank
					<< std::endl;
				return PLSTAT_MPI_ERROR;

			}
#ifdef DEBUG
			PL_DBGOSH << "MPIPolylib::gather_polygons() get triangle list." << std::endl;
#endif

			// 各ポリゴングループに対して受信した三角形情報を追加
			pos_id = 0;
			pos_exid = 0;
			pos_tria = 0;
			int n_start_tri=0;
			int n_start_id=0;
			int n_start_exid=0;

			for( i=0; i<this->m_pg_list.size() * 2; i++ ){

				// ポリゴングループID
				int pg_id = p_intarray[i];

				// 当該ポリゴングループの三角形数
				unsigned int num_trias = p_intarray[i+1];

				// グループIDのポリゴングループインスタンス取得
#ifdef DEBUG
				PL_DBGOSH << "MPIPolylib::gather_polygons() polygon id. "<< pg_id << std::endl;
#endif

				PolygonGroup* p_pg = this->get_group( pg_id );
				if( p_pg == NULL ) {
#ifdef DEBUG
					PL_DBGOSH << "MPIPolylib::gather_polygons() polygongropup pointer. "<< p_pg << std::endl;
#endif

					PL_ERROSH << "[ERROR]MPIPolylib::gather_polygons():invalid pg_id:"
						<< pg_id << std::endl;
					return PLSTAT_NG;
				}

#ifdef DEBUG
				PL_DBGOSH << "MPIPolylib::gather_polygons() get group pointer. "<< i << " rank "<< rank << std::endl;
				PL_DBGOSH << "MPIPolylib::gather_polygons() n_start_tri "<< n_start_tri << " n_start_id "<< n_start_id << num_trias << std::endl;
#endif



				//ポリゴングループに三角形リストを追加
				if( p_pg->add_triangles( p_triaarray, p_idarray, p_exidarray, n_start_tri,n_start_id,n_start_exid,num_trias ) != PLSTAT_OK ) {
					PL_ERROSH << "[ERROR]MPIPolylib::gather_polygons():p_pg->add() failed. returns:"
						<< PolylibStat2::String(ret) << std::endl;
					return ret;

				} else {

#ifdef DEBUG
					PL_DBGOSH << "MPIPolylib::gather_polygons() get group pointer. "<< i << std::endl;
#endif

				}
				n_start_tri+=num_trias*9;
				n_start_id+=num_trias;
				n_start_exid+=num_trias;
#ifdef DEBUG
				PL_DBGOSH << "MPIPolylib::gather_polygons() add triangles to group. "<< i << " " << p_pg << std::endl;
#endif
				// //PrivateTriangleのベクタ生成
				// std::vector<PrivateTriangle*> tria_vec;

				// //ベクタに受信データ内容を設定
				// for( j=0; j<num_trias; j++ ) {
				// 	tria_vec.push_back( new PrivateTriangle(&p_triaarray[pos_tria], p_idarray[pos_id]) );
				// 	pos_id++;
				// 	pos_tria+=9;
				// }

				// //ポリゴングループに三角形リストを追加
				// if( (ret = p_pg->add_triangles( &tria_vec )) != PLSTAT_OK ) {
				// 	PL_ERROSH << "[ERROR]MPIPolylib::gather_polygons():p_pg->init() failed. returns:" << PolylibStat2::String(ret) << std::endl;
				// 	return ret;
				// }

				// //ベクタの内容あとしまつ
				// for( j=0; j<num_trias; j++ ) {
				// 	delete tria_vec.at(j);
				// }

				//次のポリゴングループID位置へ
				i++;
			}

#ifdef DEBUG
			PL_DBGOSH << "MPIPolylib::gather_polygons() add triangles." << std::endl;
#endif
			// 受信領域あとしまつ
			if( p_intarray != NULL )  delete[] p_intarray;
			if( p_idarray != NULL )   delete[] p_idarray;
			if( p_exidarray != NULL )   delete[] p_exidarray;
			if( p_triaarray != NULL ) delete[] p_triaarray;
		}



		// for( i=0; i<this->m_pg_list.size() ; i++ ){
		//  this->m_pg_list[i]->finalize_DVertex();
		// }
		return PLSTAT_OK;


		//#undef DEBUG
}


// protected //////////////////////////////////////////////////////////////////

POLYLIB_STAT
	MPIPolylib::gather_polygons_vtk(){

		//#define DEBUG
#ifdef DEBUG
		PL_DBGOSH << "MPIPolylib::gather_polygons_vtk() in. " << std::endl;
#endif

		POLYLIB_STAT ret;
		unsigned int i, j;
		int rank;
		unsigned int pos_id, pos_exid, pos_tria;
		MPI_Status mpi_stat;

		// 全rankからポリゴン情報を受信
		for( rank=1; rank<m_numproc; rank++ ) {

			// グループIDとグループ毎三角形数の対を受信
			// グループ情報は全rank共通なので、グループ数は予め分かっている
			int *p_intarray = new int[ this->m_pg_list.size() * 2 ];
			if (MPI_Recv( p_intarray, this->m_pg_list.size() * 2, MPI_INT, rank,
				MPITAG_NUM_TRIAS, m_mycomm, &mpi_stat ) != MPI_SUCCESS) {
					PL_ERROSH << "[ERROR]MPIPolylib::gather_polygons_vtk()"
						<< ":MPI_Recv,MPITAG_NUM_TRIAS faild.:rank=" << rank
						<< std::endl;
					return PLSTAT_MPI_ERROR;
			}


#ifdef DEBUG
			PL_DBGOSH << "MPIPolylib::gather_polygons_vtk() group info. " << std::endl;
#endif

			// 受信する全三角形数を算出
			unsigned int total_tria_num = 0;
			for( i=1; i<this->m_pg_list.size() * 2; i+=2 ){
				total_tria_num += p_intarray[i];
			}

#ifdef DEBUG
			PL_DBGOSH << "MPIPolylib::gather_polygons_vtk() MPITAG_TRIA_IDS. before" << std::endl;
#endif

			// 三角形IDリストを受信
			int *p_idarray = new int[ total_tria_num ];
			if (MPI_Recv( p_idarray,  total_tria_num, MPI_INT, rank,
				MPITAG_TRIA_IDS, m_mycomm, &mpi_stat ) != MPI_SUCCESS) {
					PL_ERROSH << "[ERROR]MPIPolylib::gather_polygons_vtk()"
						<< ":MPI_Recv,MPITAG_TRIA_IDS faild.:rank=" << rank
						<< std::endl;
					return PLSTAT_MPI_ERROR;
			}


#ifdef DEBUG
			PL_DBGOSH << "MPIPolylib::gather_polygons_vtk() MPITAG_TRIA_IDS. " << std::endl;
#endif

			// 三角形のユーザ定義IDリストを受信
			int *p_exidarray = new int[ total_tria_num ];
			if (MPI_Recv( p_exidarray,  total_tria_num, MPI_INT, rank,
				MPITAG_TRIA_EXIDS, m_mycomm, &mpi_stat ) != MPI_SUCCESS) {
					PL_ERROSH << "[ERROR]MPIPolylib::gather_polygons_vtk()"
						<< ":MPI_Recv,MPITAG_TRIA_EXIDS faild.:rank=" << rank
						<< std::endl;
					return PLSTAT_MPI_ERROR;
			}


#ifdef DEBUG
			PL_DBGOSH << "MPIPolylib::gather_polygons_vtk() MPITAG_TRIA_IDS. " << std::endl;
#endif


			// 三角形リストを受信
			PL_REAL *p_triaarray = new PL_REAL[ total_tria_num * 3 * 3 ];


#ifdef DEBUG
			PL_DBGOSH << "MPIPolylib::gather_polygons_vtk() MPITAG_TRIAS. before" << std::endl;
#endif

			if(sizeof(PL_REAL)==4){
				if (MPI_Recv( p_triaarray, total_tria_num * 3 * 3, MPI_FLOAT, rank,
					MPITAG_TRIAS, m_mycomm, &mpi_stat ) != MPI_SUCCESS) {
						PL_ERROSH << "[ERROR]MPIPolylib::gather_polygons_vtk()"
							<< ":MPI_Recv,MPITAG_TRIAS faild.:rank=" << rank
							<< std::endl;
						return PLSTAT_MPI_ERROR;
				}
			} else if(sizeof(PL_REAL)==8){
				if (MPI_Recv( p_triaarray, total_tria_num * 3 * 3, MPI_DOUBLE, rank,
					MPITAG_TRIAS, m_mycomm, &mpi_stat ) != MPI_SUCCESS)  {
						PL_ERROSH << "[ERROR]MPIPolylib::gather_polygons_vtk()"
							<< ":MPI_Recv,MPITAG_TRIAS faild.:rank=" << rank
							<< std::endl;
						return PLSTAT_MPI_ERROR;
				}
			} else {
				PL_ERROSH << "[ERROR]MPIPolylib::gather_polygons_vtk()"
					<< ":MPI_Recv,MPITAG_TRIAS faild.:rank=" << rank
					<< std::endl;
				return PLSTAT_MPI_ERROR;

			}


#ifdef DEBUG
			PL_DBGOSH << "MPIPolylib::gather_polygons_vtk() MPITAG_TRIAS. " << std::endl;
#endif

			// DVertex
			//ndata,nvectorを受信
			int *p_ndata_array = new int[ this->m_pg_list.size() * 2 ];



#ifdef DEBUG
			PL_DBGOSH << "MPIPolylib::gather_polygons_vtk() MPITAG_TRIA_NDATA. before" << std::endl;
			PL_DBGOSH << "MPIPolylib::gather_polygons_vtk() MPITAG_TRIA_NDATA. size "
				<< this->m_pg_list.size() * 2 << std::endl;
#endif


			if (MPI_Recv( p_ndata_array, this->m_pg_list.size() * 2 , MPI_INT, rank,
				MPITAG_TRIA_NDATA, m_mycomm, &mpi_stat ) != MPI_SUCCESS) {
					PL_ERROSH << "[ERROR]MPIPolylib::gather_polygons_vtk()"
						<< ":MPI_Recv,MPITAG_TRIA_NDATA faild.:rank=" << rank
						<< std::endl;
					return PLSTAT_MPI_ERROR;
			}


#ifdef DEBUG
			PL_DBGOSH << "MPIPolylib::gather_polygons_vtk() MPITAG_TRIA_NDATA. " << std::endl;
			PL_DBGOSH << "MPIPolylib::gather_polygons_vtk() MPITAG_TRIA_NDATA. p_ndata_array[0] " << p_ndata_array[0] << std::endl;
			PL_DBGOSH << "MPIPolylib::gather_polygons_vtk() MPITAG_TRIA_NDATA. p_ndata_array[1] " << p_ndata_array[1] << std::endl;
			//	PL_DBGOSH << "MPIPolylib::gather_polygons_vtk() MPITAG_TRIA_NDATA. p_ndata_array[2] " << p_ndata_array[2] << std::endl;
			//	PL_DBGOSH << "MPIPolylib::gather_polygons_vtk() MPITAG_TRIA_NDATA. p_ndata_array[3] " << p_ndata_array[3] << std::endl;
#endif


			// 受信するデータ数を受信
			unsigned int total_tria_nscalar = 0;
			unsigned int total_tria_nvector = 0;

			for( i=1; i<this->m_pg_list.size() * 2; i+=2 ){ // loop over PG
				// total_tria_nscalar += p_intarray[i]*3*p_ndata_array[0];
				// total_tria_nvector += p_intarray[i]*3*p_ndata_array[1]*3;
				total_tria_nscalar += p_intarray[i]*3*p_ndata_array[i-1];
				total_tria_nvector += p_intarray[i]*3*p_ndata_array[i]*3;
#ifdef DEBUG
				PL_DBGOSH << "ntri="<<p_intarray[i] << std::endl;
				PL_DBGOSH << i-1<<" nscalar="<<p_ndata_array[i-1] << std::endl;
				PL_DBGOSH << i<<" nvector="<<p_ndata_array[i] << std::endl;
				PL_DBGOSH<<"total_tria_nscalar="<<total_tria_nscalar<< std::endl;
				PL_DBGOSH<<"total_tria_nvector="<<total_tria_nvector<< std::endl;
#endif
			}


#ifdef DEBUG
			PL_DBGOSH << "MPIPolylib::gather_polygons_vtk() MPITAG_TRIA_NDATA. calc" << std::endl;
#endif




			//  Dvertex scalar リストを受信
			PL_REAL *p_tria_scalar_array = new PL_REAL[ total_tria_nscalar ];

#ifdef DEBUG
			PL_DBGOSH << "MPIPolylib::gather_polygons_vtk() MPITAG_TRIA_SCALAR. before " << total_tria_nscalar<<std::endl;
#endif

			if(sizeof(PL_REAL)==4){
				if (MPI_Recv( p_tria_scalar_array, total_tria_nscalar, MPI_FLOAT, rank,
					MPITAG_TRIA_SCALAR, m_mycomm, &mpi_stat ) != MPI_SUCCESS) {
						PL_ERROSH << "[ERROR]MPIPolylib::gather_polygons_vtk()"
							<< ":MPI_Recv,MPITAG_TRIA_SCALAR faild.:rank=" << rank
							<< std::endl;
						return PLSTAT_MPI_ERROR;
				}
			} else if(sizeof(PL_REAL)==8){
				if (MPI_Recv( p_tria_scalar_array, total_tria_nscalar, MPI_DOUBLE, rank,
					MPITAG_TRIA_SCALAR, m_mycomm, &mpi_stat ) != MPI_SUCCESS) {
						PL_ERROSH << "[ERROR]MPIPolylib::gather_polygons_vtk()"
							<< ":MPI_Recv,MPITAG_TRIA_SCALAR faild.:rank=" << rank
							<< std::endl;
						return PLSTAT_MPI_ERROR;
				}
			} else {
				PL_ERROSH << "[ERROR]MPIPolylib::gather_polygons_vtk()"
					<< ":MPI_Recv,MPITAG_TRIA_SCALAR faild.:rank=" << rank
					<< std::endl;
				return PLSTAT_MPI_ERROR;

			}



#ifdef DEBUG
			PL_DBGOSH << "MPIPolylib::gather_polygons_vtk() MPITAG_TRIA_SCALAR. " << std::endl;
#endif

			//  Dvertex vector リストを受信
			PL_REAL *p_tria_vector_array = new PL_REAL[ total_tria_nvector ];


#ifdef DEBUG
			PL_DBGOSH << "MPIPolylib::gather_polygons_vtk() MPITAG_TRIA_VECTOR. before" << std::endl;
#endif


			if(sizeof(PL_REAL)==4){
				if (MPI_Recv( p_tria_vector_array, total_tria_nvector, MPI_FLOAT, rank,
					MPITAG_TRIA_VECTOR, m_mycomm, &mpi_stat ) != MPI_SUCCESS) {
						PL_ERROSH << "[ERROR]MPIPolylib::gather_polygons_vtk()"
							<< ":MPI_Recv,MPITAG_TRIA_VECTOR faild.:rank=" << rank
							<< std::endl;
						return PLSTAT_MPI_ERROR;
				}
			} else if(sizeof(PL_REAL)==8){
				if (MPI_Recv( p_tria_vector_array, total_tria_nvector, MPI_DOUBLE, rank,
					MPITAG_TRIA_VECTOR, m_mycomm, &mpi_stat ) != MPI_SUCCESS) {
						PL_ERROSH << "[ERROR]MPIPolylib::gather_polygons_vtk()"
							<< ":MPI_Recv,MPITAG_TRIA_VECTOR faild.:rank=" << rank
							<< std::endl;
						return PLSTAT_MPI_ERROR;
				}
			} else {
				PL_ERROSH << "[ERROR]MPIPolylib::gather_polygons_vtk()"
					<< ":MPI_Recv,MPITAG_TRIA_VECTOR faild.:rank=" << rank
					<< std::endl;
				return PLSTAT_MPI_ERROR;

			}


#ifdef DEBUG
			PL_DBGOSH << "MPIPolylib::gather_polygons_vtk() MPITAG_TRIA_VECTOR. " << std::endl;
#endif



			// 各ポリゴングループに対して受信した三角形情報を追加
			pos_id = 0;
			pos_exid = 0;
			pos_tria = 0;
			int n_start_tri=0;
			int n_start_id=0;
			int n_start_exid=0;
			int n_start_scalar=0;
			int n_start_vector=0;

			for( i=0; i<this->m_pg_list.size() * 2; i++ ){

				// ポリゴングループID
				int pg_id = p_intarray[i];

				// 当該ポリゴングループの三角形数
				unsigned int num_trias = p_intarray[i+1];

				// 当該ポリゴングループのスカラーデータ 数
				unsigned int num_scalar = p_ndata_array[i];
				// 当該ポリゴングループのスカラーデータ 数
				unsigned int num_vector = p_ndata_array[i+1];


#ifdef DEBUG
				PL_DBGOSH << i <<" pg_id "<<pg_id << std::endl;
				PL_DBGOSH << "num_trias "<<num_trias << std::endl;
				PL_DBGOSH << "num_scalar "<<num_scalar << std::endl;
				PL_DBGOSH << "num_vector "<<num_vector << std::endl;
#endif


				// グループIDのポリゴングループインスタンス取得
				PolygonGroup* p_pg = this->get_group( pg_id );
				if( p_pg == NULL ) {
					PL_ERROSH << "[ERROR]MPIPolylib::gather_polygons_vtk():invalid pg_id:"
						<< pg_id << std::endl;
					return PLSTAT_NG;
				}


				//ポリゴングループに三角形リストを追加
				// if( p_pg->init( p_triaarray, p_idarray, n_start_tri,n_start_id,num_trias ) != PLSTAT_OK ) {
				//   PL_ERROSH << "[ERROR]MPIPolylib::gather_polygons_vtk():p_pg->add() failed. returns:"
				// 	    << PolylibStat2::String(ret) << std::endl;
				// 	return ret;
				// }

#ifdef DEBUG
				PL_DBGOSH << "before add_dvertex "<< std::endl;
				PL_DBGOSH << "n_start_tri "<<n_start_tri<< std::endl;
				PL_DBGOSH << "n_start_id "<<n_start_id<< std::endl;
				PL_DBGOSH << "n_start_exid "<<n_start_exid<< std::endl;
				PL_DBGOSH << "n_start_scalar "<<n_start_scalar<< std::endl;
				PL_DBGOSH << "n_start_vector "<<n_start_vector<< std::endl;

#endif


				if( p_pg->add_dvertex( p_triaarray, p_idarray, p_exidarray, p_tria_scalar_array ,p_tria_vector_array,
					n_start_tri,n_start_id,n_start_exid,n_start_scalar,n_start_vector,
					num_trias,num_scalar,num_vector ) != PLSTAT_OK ) {
						PL_ERROSH << "[ERROR]MPIPolylib::gather_polygons_vtk():p_pg->add() failed. returns:"
							<< PolylibStat2::String(ret) << std::endl;
						return ret;
				}

#ifdef DEBUG
				PL_DBGOSH << "after add_dvertex "<< i << std::endl;
#endif
				n_start_tri+=num_trias*9;
				n_start_id+=num_trias;
				n_start_exid+=num_trias;
				n_start_scalar+=num_trias*3*num_scalar;
				n_start_vector+=num_trias*3*num_vector*3;

				i++;
			}

			// 受信領域あとしまつ
			if( p_intarray != NULL )  delete[] p_intarray;
			if( p_idarray != NULL )   delete[] p_idarray;
			if( p_exidarray != NULL )   delete[] p_exidarray;
			if( p_triaarray != NULL ) delete[] p_triaarray;
			if( p_ndata_array != NULL ) delete[] p_ndata_array;
			if( p_tria_scalar_array != NULL ) delete[] p_tria_scalar_array;
			if( p_tria_vector_array != NULL ) delete[] p_tria_vector_array;

		}

		for( i=0; i<this->m_pg_list.size() ; i++ ){
#ifdef DEBUG
			PL_DBGOSH << "before finalize_DVertex "<< i << " "<<this->m_pg_list[i]->get_triangles()<<std::endl;
#endif
			if(this->m_pg_list[i]->get_triangles()!=NULL)  this->m_pg_list[i]->finalize_DVertex();
#ifdef DEBUG
			PL_DBGOSH << "after finalize_DVertex "<< i << std::endl;
#endif

		}


		return PLSTAT_OK;
		//#undef DEBUG
}


// protected //////////////////////////////////////////////////////////////////

POLYLIB_STAT
	MPIPolylib::send_polygons_to_rank0(){

		//#define DEBUG
#ifdef DEBUG
		PL_DBGOSH << "MPIPolylib::send_polygons_to_rank0() in. " << std::endl;
#endif
		unsigned int i;
		std::vector<PolygonGroup*>::iterator group_itr;
		PolygonGroup *p_pg;
		std::vector<PrivateTriangle*> const *p_trias;

		std::vector<int>   send_num_trias;
		int          *p_send_num_trias_array;
		std::vector<int>   send_tria_ids;
		int          *p_send_tria_ids_array;
		std::vector<int>   send_tria_exids;
		int          *p_send_tria_exids_array;
		std::vector<PL_REAL> send_trias;
		PL_REAL        *p_send_trias_array;

		// 送信用一時データ初期化
		p_trias = NULL;
		send_num_trias.clear();
		send_trias.clear();
		send_tria_ids.clear();
		send_tria_exids.clear();

		// 全グループに対して
		for (group_itr = this->m_pg_list.begin(); group_itr != this->m_pg_list.end(); group_itr++) {
			p_pg = (*group_itr);

			// ポリゴン情報を持つグループだけ
			if( p_pg->get_triangles() != NULL && p_pg->get_triangles()->size() != 0 ) {

				// 当該PE領域内に一部でも含まれるポリゴンを検索
				p_trias = p_pg->search( &(m_myproc.m_area.m_gcell_bbox), false );
			}

			// グループIDと当該グループの三角形数の対を送信データに追加
			pack_num_trias( &send_num_trias, p_pg->get_internal_id(), p_trias );

			// 三角形情報を送信データに追加
			pack_trias( &send_trias, p_trias );

			// 三角形ID情報を送信データに追加
			pack_tria_ids( &send_tria_ids, p_trias );

			// 三角形のユーザ定義ID情報を送信データに追加
			pack_tria_exids( &send_tria_exids, p_trias );

			// search結果の後始末
			if( p_trias ) delete p_trias;
			p_trias = NULL;
		}

		// 送信データをシリアライズ
		p_send_num_trias_array = new int[ send_num_trias.size() ];
		for( i=0; i<send_num_trias.size(); i++ )
			p_send_num_trias_array[i] = send_num_trias[i];

		p_send_tria_ids_array  = new int[ send_tria_ids.size() ];
		for( i=0; i<send_tria_ids.size(); i++ )
			p_send_tria_ids_array[i] = send_tria_ids[i];

		p_send_tria_exids_array  = new int[ send_tria_exids.size() ];
		for( i=0; i<send_tria_exids.size(); i++ )
			p_send_tria_exids_array[i] = send_tria_exids[i];

		p_send_trias_array =     new PL_REAL[ send_trias.size() ];
		for( i=0; i<send_trias.size(); i++ )
			p_send_trias_array[i] = send_trias[i];

		// rank0へ送信
#ifdef DEBUG
		PL_DBGOSH << "sending polygons rank:" << m_myrank << " -> rank:0 ";
		for( i=0; i< send_num_trias.size(); i+=2 ) {
			PL_DBGOS << "(gid:" << send_num_trias[i]
			<< ",num_tria:" << send_num_trias[i+1] << ")";
		}
		PL_DBGOS << std::endl;
#endif
		if (MPI_Send( p_send_num_trias_array, send_num_trias.size(), MPI_INT, 0,
			MPITAG_NUM_TRIAS, m_mycomm ) != MPI_SUCCESS) {
				PL_ERROSH << "[ERROR]MPIPolylib::send_polygons_to_rank0()"
					<< ":MPI_Send,MPITAG_NUM_TRIAS faild." << std::endl;
				return PLSTAT_MPI_ERROR;
		}
		if (MPI_Send( p_send_tria_ids_array,  send_tria_ids.size(),  MPI_INT, 0,
			MPITAG_TRIA_IDS,  m_mycomm ) != MPI_SUCCESS) {
				PL_ERROSH << "[ERROR]MPIPolylib::send_polygons_to_rank0()"
					<< ":MPI_Send,MPITAG_TRIA_IDS faild." << std::endl;
				return PLSTAT_MPI_ERROR;
		}
		if (MPI_Send( p_send_tria_exids_array,  send_tria_exids.size(),  MPI_INT, 0,
			MPITAG_TRIA_EXIDS,  m_mycomm ) != MPI_SUCCESS) {
				PL_ERROSH << "[ERROR]MPIPolylib::send_polygons_to_rank0()"
					<< ":MPI_Send,MPITAG_TRIA_EXIDS faild." << std::endl;
				return PLSTAT_MPI_ERROR;
		}

		if(sizeof(PL_REAL)==4){
			if (MPI_Send( p_send_trias_array,     send_trias.size(),   MPI_FLOAT, 0,
				MPITAG_TRIAS,     m_mycomm ) != MPI_SUCCESS) {
					PL_ERROSH << "[ERROR]MPIPolylib::send_polygons_to_rank0()"
						<< ":MPI_Send,MPITAG_TRIAS faild." << std::endl;
					return PLSTAT_MPI_ERROR;
			}
		}else  if(sizeof(PL_REAL)==8){
			if (MPI_Send( p_send_trias_array,     send_trias.size(),   MPI_DOUBLE, 0,
				MPITAG_TRIAS,     m_mycomm ) != MPI_SUCCESS) {
					PL_ERROSH << "[ERROR]MPIPolylib::send_polygons_to_rank0()"
						<< ":MPI_Send,MPITAG_TRIAS faild." << std::endl;
					return PLSTAT_MPI_ERROR;
			}
		} else {
			PL_ERROSH << "[ERROR]MPIPolylib::send_polygons_to_rank0()"
				<< ":MPI_Send,MPITAG_TRIAS faild." << std::endl;
			return PLSTAT_MPI_ERROR;

		}

		// 送信データの後始末
		delete[] p_send_num_trias_array;
		delete[] p_send_tria_ids_array;
		delete[] p_send_tria_exids_array;
		delete[] p_send_trias_array;

		return PLSTAT_OK;
		//#undef DEBUG
}

// protected //////////////////////////////////////////////////////////////////

POLYLIB_STAT
	MPIPolylib::send_polygons_to_rank0_vtk(){
		//#define DEBUG
#ifdef DEBUG
		PL_DBGOSH << "MPIPolylib::send_polygons_to_rank0_vtk() in. " << std::endl;
#endif

		unsigned int i;
		std::vector<PolygonGroup*>::iterator group_itr;
		PolygonGroup *p_pg;
		std::vector<PrivateTriangle*> const *p_trias;
		//std::vector<DVertexTriangle*> const *p_dvtrias;

		std::vector<int>   send_num_trias;
		int          *p_send_num_trias_array;
		std::vector<int>   send_tria_ids;
		int          *p_send_tria_ids_array;
		std::vector<int>   send_tria_exids;
		int          *p_send_tria_exids_array;
		std::vector<PL_REAL> send_trias;
		PL_REAL        *p_send_trias_array;

		std::vector<int> send_tria_ndata;
		int       *p_send_tria_ndata_array;

		std::vector<PL_REAL> send_tria_scalar;
		PL_REAL        *p_send_tria_scalar_array;
		std::vector<PL_REAL> send_tria_vector;
		PL_REAL        *p_send_tria_vector_array;


		// 送信用一時データ初期化
		p_trias = NULL;
		//p_dvtrias = NULL;
		send_num_trias.clear();
		send_trias.clear();
		send_tria_ids.clear();
		send_tria_exids.clear();

		send_tria_scalar.clear();
		send_tria_vector.clear();

		// 全グループに対して
		for (group_itr = this->m_pg_list.begin();
			group_itr != this->m_pg_list.end();
			group_itr++) {
				p_pg = (*group_itr);

				// ポリゴン情報を持つグループだけ
				if( p_pg->get_triangles() != NULL && p_pg->get_triangles()->size() != 0 ) {

					// 当該PE領域内に一部でも含まれるポリゴンを検索
					p_trias = p_pg->search( &(m_myproc.m_area.m_gcell_bbox), false );

				}

				// グループIDと当該グループの三角形数の対を送信データに追加
				pack_num_trias( &send_num_trias, p_pg->get_internal_id(), p_trias );

				// 三角形情報を送信データに追加
				pack_trias( &send_trias, p_trias );

				// 三角形ID情報を送信データに追加
				pack_tria_ids(&send_tria_ids, p_trias );

				// 三角形のユーザ定義ID情報を送信データに追加
				pack_tria_exids(&send_tria_exids, p_trias );

#ifdef DEBUG
				PL_DBGOSH << "MPIPolylib::send_polygons_to_rank0_vtk() pack_tria_ndata. " << std::endl;
#endif

				// 附属データ数を送信
				pack_tria_ndata(&send_tria_ndata,p_trias);
#ifdef DEBUG
				for(int jjj=0;jjj<send_tria_ndata.size();jjj++){
					PL_DBGOSH << "MPIPolylib::send_polygons_to_rank0_vtk() after pack_tria_ndata. "<< send_tria_ndata[jjj] << std::endl;
				}

				PL_DBGOSH << "MPIPolylib::send_polygons_to_rank0_vtk() pack_tria_ndata. end. " << std::endl;
#endif


				// scalar_data
				pack_tria_scalar_data(&send_tria_scalar, p_trias );

				//vector data
				pack_tria_vector_data(&send_tria_vector, p_trias );
				// search結果の後始末
				if( p_trias ) delete p_trias;
				p_trias = NULL;
		}// end group

		// 送信データをシリアライズ
		p_send_num_trias_array = new int[ send_num_trias.size() ];
#ifdef DEBUG
		PL_DBGOSH << "MPIPolylib::send_polygons_to_rank0_vtk() send_num_trias.size "<<  send_num_trias.size()  << std::endl;
#endif

		for( i=0; i<send_num_trias.size(); i++ )
			p_send_num_trias_array[i] = send_num_trias[i];


		p_send_tria_ids_array  = new int[ send_tria_ids.size() ];
		for( i=0; i<send_tria_ids.size(); i++ )
			p_send_tria_ids_array[i] = send_tria_ids[i];

		p_send_tria_exids_array  = new int[ send_tria_exids.size() ];
		for( i=0; i<send_tria_exids.size(); i++ )
			p_send_tria_exids_array[i] = send_tria_exids[i];

		p_send_trias_array =     new PL_REAL[ send_trias.size() ];
		for( i=0; i<send_trias.size(); i++ )
			p_send_trias_array[i] = send_trias[i];

		// add for VTK DVertex

		p_send_tria_ndata_array = new int[send_tria_ndata.size()];
		for( i=0; i<send_tria_ndata.size(); i++ )
			p_send_tria_ndata_array[i] = send_tria_ndata[i];

		p_send_tria_scalar_array = new PL_REAL[send_tria_scalar.size()];
		for( i=0; i<send_tria_scalar.size(); i++ )
			p_send_tria_scalar_array[i] = send_tria_scalar[i];

		p_send_tria_vector_array = new PL_REAL[send_tria_vector.size()];
		for( i=0; i<send_tria_vector.size(); i++ )
			p_send_tria_vector_array[i] = send_tria_vector[i];

		// rank0へ送信
#ifdef DEBUG
		PL_DBGOSH << "sending polygons rank:" << m_myrank << " -> rank:0 ";
		for( i=0; i< send_num_trias.size(); i+=2 ) {
			PL_DBGOS << "(gid:" << send_num_trias[i]
			<< ",num_tria:" << send_num_trias[i+1] << ")";
		}
		PL_DBGOS << std::endl;
#endif
		if (MPI_Send( p_send_num_trias_array, send_num_trias.size(), MPI_INT, 0,
			MPITAG_NUM_TRIAS, m_mycomm ) != MPI_SUCCESS) {
				PL_ERROSH << "[ERROR]MPIPolylib::send_polygons_to_rank0_vtk()"
					<< ":MPI_Send,MPITAG_NUM_TRIAS faild." << std::endl;
				return PLSTAT_MPI_ERROR;
		}
		if (MPI_Send( p_send_tria_ids_array,  send_tria_ids.size(),  MPI_INT, 0,
			MPITAG_TRIA_IDS,  m_mycomm ) != MPI_SUCCESS) {
				PL_ERROSH << "[ERROR]MPIPolylib::send_polygons_to_rank0_vtk()"
					<< ":MPI_Send,MPITAG_TRIA_IDS faild." << std::endl;
				return PLSTAT_MPI_ERROR;
		}
		if (MPI_Send( p_send_tria_exids_array,  send_tria_exids.size(),  MPI_INT, 0,
			MPITAG_TRIA_EXIDS,  m_mycomm ) != MPI_SUCCESS) {
				PL_ERROSH << "[ERROR]MPIPolylib::send_polygons_to_rank0_vtk()"
					<< ":MPI_Send,MPITAG_TRIA_EXIDS faild." << std::endl;
				return PLSTAT_MPI_ERROR;
		}

		if(sizeof(PL_REAL)==4){
			if (MPI_Send( p_send_trias_array,     send_trias.size(),   MPI_FLOAT, 0,
				MPITAG_TRIAS,     m_mycomm ) != MPI_SUCCESS) {
					PL_ERROSH << "[ERROR]MPIPolylib::send_polygons_to_rank0_vtk()"
						<< ":MPI_Send,MPITAG_TRIAS faild." << std::endl;
					return PLSTAT_MPI_ERROR;
			}
		}else  if(sizeof(PL_REAL)==8){
			if (MPI_Send( p_send_trias_array,     send_trias.size(),   MPI_DOUBLE, 0,
				MPITAG_TRIAS,     m_mycomm ) != MPI_SUCCESS) {
					PL_ERROSH << "[ERROR]MPIPolylib::send_polygons_to_rank0_vtk()"
						<< ":MPI_Send,MPITAG_TRIAS faild." << std::endl;
					return PLSTAT_MPI_ERROR;
			}
		} else {
			PL_ERROSH << "[ERROR]MPIPolylib::send_polygons_to_rank0_vtk()"
				<< ":MPI_Send,MPITAG_TRIAS faild." << std::endl;
			return PLSTAT_MPI_ERROR;

		}

		//nscalar and nvector
		if (MPI_Send( p_send_tria_ndata_array,
			send_tria_ndata.size(),
			MPI_INT, 0,
			MPITAG_TRIA_NDATA, m_mycomm ) != MPI_SUCCESS) {
				PL_ERROSH << "[ERROR]MPIPolylib::send_polygons_to_rank0_vtk()"
					<< ":MPI_Send,MPITAG_TRIAS faild." << std::endl;
				return PLSTAT_MPI_ERROR;
		}

		//scalar
		if(sizeof(PL_REAL)==4){
			if (MPI_Send( p_send_tria_scalar_array,     send_tria_scalar.size(),   MPI_FLOAT, 0,
				MPITAG_TRIA_SCALAR,     m_mycomm ) != MPI_SUCCESS) {
					PL_ERROSH << "[ERROR]MPIPolylib::send_polygons_to_rank0_vtk()"
						<< ":MPI_Send,MPITAG_TRIAS faild." << std::endl;
					return PLSTAT_MPI_ERROR;
			}
		}else  if(sizeof(PL_REAL)==8){
			if (MPI_Send( p_send_tria_scalar_array,     send_tria_scalar.size(),   MPI_DOUBLE, 0,
				MPITAG_TRIA_SCALAR,     m_mycomm ) != MPI_SUCCESS) {
					PL_ERROSH << "[ERROR]MPIPolylib::send_polygons_to_rank0_vtk()"
						<< ":MPI_Send,MPITAG_TRIAS faild." << std::endl;
					return PLSTAT_MPI_ERROR;
			}
		} else {
			PL_ERROSH << "[ERROR]MPIPolylib::send_polygons_to_rank0_vtk()"
				<< ":MPI_Send,MPITAG_TRIAS faild." << std::endl;
			return PLSTAT_MPI_ERROR;

		}

		//vector
		if(sizeof(PL_REAL)==4){
			if (MPI_Send( p_send_tria_vector_array,     send_tria_vector.size(),   MPI_FLOAT, 0,
				MPITAG_TRIA_VECTOR,     m_mycomm ) != MPI_SUCCESS) {
					PL_ERROSH << "[ERROR]MPIPolylib::send_polygons_to_rank0_vtk()"
						<< ":MPI_Send,MPITAG_TRIAS faild." << std::endl;
					return PLSTAT_MPI_ERROR;
			}
		}else  if(sizeof(PL_REAL)==8){
			if (MPI_Send( p_send_tria_vector_array,     send_tria_vector.size(),   MPI_DOUBLE, 0,
				MPITAG_TRIA_VECTOR,     m_mycomm ) != MPI_SUCCESS) {
					PL_ERROSH << "[ERROR]MPIPolylib::send_polygons_to_rank0_vtk()"
						<< ":MPI_Send,MPITAG_TRIAS faild." << std::endl;
					return PLSTAT_MPI_ERROR;
			}
		} else {
			PL_ERROSH << "[ERROR]MPIPolylib::send_polygons_to_rank0_vtk()"
				<< ":MPI_Send,MPITAG_TRIAS faild." << std::endl;
			return PLSTAT_MPI_ERROR;

		}


		// 送信データの後始末
		delete[] p_send_num_trias_array;
		delete[] p_send_tria_ids_array;
		delete[] p_send_tria_exids_array;
		delete[] p_send_trias_array;
		delete[] p_send_tria_ndata_array;
		delete[] p_send_tria_scalar_array;
		delete[] p_send_tria_vector_array;

		//#undef DEBUG
		return PLSTAT_OK;
}


// protected //////////////////////////////////////////////////////////////////

POLYLIB_STAT
	MPIPolylib::select_excluded_trias(
	PolygonGroup *p_pg){
#ifdef DEBUG
		PL_DBGOSH << "MPIPolylib::select_excluded_trias() in. " << std::endl;
#endif

		unsigned int i, j;
		std::vector<PrivateTriangle*> const *p_trias;
		std::vector<int> ids;

		// 全隣接PEについて
		for( i=0; i<m_neibour_procs.size(); i++ ) {
			ids.clear();

			// 隣接PE領域(ガイドセル含)に懸かる三角形IDリストを作成
			p_trias = p_pg->search( &(m_neibour_procs.at(i)->m_area.m_gcell_bbox), false );
			for( j=0; j<p_trias->size(); j++ ) {
				ids.push_back( p_trias->at(j)->get_id() );
			}
#ifdef DEBUG
			PL_DBGOSH << "gid:" << p_pg->get_id() << " neibour_rank:" << m_neibour_procs.at(i)->m_rank
				<< " 除外三角形数:" << ids.size() << std::endl;
#endif

			// migrate除外三角形IDマップに追加
			m_neibour_procs.at(i)->m_exclusion_map[p_pg->get_internal_id()] = ids;

			// search結果あとしまつ
			delete p_trias;
		}
		return PLSTAT_OK;

}


///
/// Polylib::load()のオーバライドメソッド。
/// @attention 並列環境では利用できません。
///
/// @param[in] config_filename	初期化ファイル名。
/// @return 常に PLSTAT_NG が返ります。
///
POLYLIB_STAT
	MPIPolylib::load(
	std::string config_filename
	)
{
	return PLSTAT_NG;
}


///
/// Polylib::save()のオーバライドメソッド。
/// @attention 並列環境では利用できません。
///
/// @param[out] p_config_filename	初期化ファイル名。
/// @return 常に PLSTAT_NG が返ります。
///
POLYLIB_STAT
	MPIPolylib::save(
	std::string *p_config_filename
	)
{
	return PLSTAT_NG;
}

///
/// m_myprocの内容をget
/// @return 自PE領域情報
///
ParallelInfo MPIPolylib::get_myproc()
{
	return m_myproc;
}

} //namespace PolylibNS
