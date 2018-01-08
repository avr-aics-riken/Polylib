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

#include "CarGroup.h"
#include "polygons/Vertex.h"
#include "polygons/VertexList.h"

CarGroup::CarGroup(){
	this->m_parent	= 0;
	this->m_polygons	= new TriMesh();
	this->m_movable	= false;
	this->m_need_rebuild = false;
	this->m_trias_before_move = NULL;
}

CarGroup::CarGroup(PL_REAL tolerance){
	this->m_tolerance=tolerance;
	this->m_parent	= 0;
	this->m_polygons	= new TriMesh(tolerance);
	this->m_movable	= false;
	this->m_need_rebuild = false;
	this->m_trias_before_move = NULL;
	this->m_tolerance=tolerance;
}

string CarGroup::get_class_name()
{
	return "CarGroup";
}

string CarGroup::whoami()
{
	return get_class_name();
}

POLYLIB_STAT CarGroup::build_group_tree(Polylib* polylib,
										PolygonGroup* parent,
										TextParser* tp)
{
	TextParserError status = TP_NO_ERROR;
	m_velocity = 0;
	vector<string> leaves;
	tp->getLabels(leaves);
	vector<string>::iterator leaf_iter=find(leaves.begin(),leaves.end(),"velocity");

	if(leaf_iter!=leaves.end()){
		string value;
		status=tp->getValue(*leaf_iter,value);
		if(status!=TP_NO_ERROR){
			tp->TextParserErrorHandler(status," can not read velocity.");
			return PLSTAT_CONFIG_ERROR;
		}else{
			int error;
			m_velocity = tp->convertDouble(value,&error);
		}
	}

	//cout << whoami() <<" "<<__FUNCTION__<< " "<<  m_velocity << endl;
	PL_DBGOSH << whoami() <<" "<<__FUNCTION__<< " "<<  m_velocity << endl;

	POLYLIB_STAT stat = PolygonGroup::build_group_tree(polylib,parent,tp);

	return stat;
}


POLYLIB_STAT CarGroup::move(PolylibMoveParams& params){

	//   write code here


	//init_check_leaped();

	//VertexList<T> * m_polygons->m_get_vtx_list
	//#define DEBUG
#ifdef DEBUG
	PL_DBGOSH << "CarGroup::move() started."<<std::endl;
#endif // DEBUG


	// �����`�F�b�N
	if (params.m_current_step == params.m_next_step) return PLSTAT_OK;
	if (params.m_current_step > params.m_next_step) return PLSTAT_NG;
	if (params.m_delta_t <= 0.0) return PLSTAT_NG;




	// �ړ���
	// X �������� 1step ������ params.m_delta_t �~ m_velocity �����ړ��������̂Ƃ���.
	//    PL_REAL move_pos = (params.m_next_step - params.m_current_step) *
	PL_REAL move_pos = (params.m_next_step - params.m_current_step) *
		this->m_velocity * params.m_delta_t;

#ifdef DEBUG
	PL_DBGOSH << "CarGroup::move() next step."<< params.m_next_step <<std::endl;
	PL_DBGOSH << "CarGroup::move() current step."<< params.m_current_step <<std::endl;
	PL_DBGOSH << "CarGroup::move() deltat."<< params.m_delta_t <<std::endl;
	PL_DBGOSH << "CarGroup::move() velocity."<< this->m_velocity <<std::endl;
	PL_DBGOSH << "CarGroup::move() velocity."<< m_velocity <<std::endl;
	PL_DBGOSH << "CarGroup::move() move_pos."<< move_pos <<std::endl;
#endif // DEBUG

	std::vector<Vertex*>* vertexlist;
	vertexlist=this->m_polygons->get_vtx_list()->get_vertex_lists_mod();

#ifdef DEBUG
	PL_DBGOSH << "CarGroup::move() Vertexpointer. size="<< vertexlist->size()<<std::endl;
#endif // DEBUG

	for (int i =0;i<vertexlist->size();++i) {
		Vertex* v = (*vertexlist)[i];
		PL_DBGOSH << "CarGroup::move() "<< i <<" "<< vertexlist->size()
			<< " point "<< *v <<std::endl;
		/* PL_DBGOSH << "CarGroup::move() x "<< (*v)[0] <<std::endl; */
		/* PL_DBGOSH << "CarGroup::move() y "<< (*v)[1] <<std::endl; */
		/* PL_DBGOSH << "CarGroup::move() z "<< (*v)[2] <<std::endl; */

		(*v)[1]-=move_pos;

		/* PL_DBGOSH << "CarGroup::move() x "<< (*v)[0] <<std::endl; */
		/* PL_DBGOSH << "CarGroup::move() y "<< (*v)[1] <<std::endl; */
		/* PL_DBGOSH << "CarGroup::move() z "<< (*v)[2] <<std::endl; */

		PL_DBGOSH << "CarGroup::move() after "<< i <<" "<< vertexlist->size()
			<< " point "<< *v <<std::endl;

		//#undef DEBUG
	}





	// �O�p�`���X�g���擾
	/*    std::vector<PrivateTriangle<T>*>* tria_list = this->m_polygons->get_tri_list(); */
	/*     typename std::vector<PrivateTriangle<T>*>::iterator it; */
	/*     // �O�p�`���X�g���̑S�Ă̎O�p�`�ɂ��Ē��_���W���X�V */
	/*     for (it=tria_list->begin(); it!=tria_list->end(); it++) { */
	/*       PrivateTriangle<T> *tria = (*it); */
	/* f      //Vec3<T> **last_vtx = tria->get_vertex(); */
	/*       Vertex<T> **last_vtx = tria->get_vertex(); */
	/*       Vec3<T> moved_vtx[3]; */
	/*       // X ���W (x) �̂ݍX�V */
	/*       moved_vtx[0].x = last_vtx[0].x + move_pos; */
	/*       moved_vtx[1].x = last_vtx[1].x + move_pos; */
	/*       moved_vtx[2].x = last_vtx[2].x + move_pos; */
	/*       moved_vtx[0].y = last_vtx[0].y; */
	/*       moved_vtx[1].y = last_vtx[1].y; */
	/*       moved_vtx[2].y = last_vtx[2].y; */
	/*       moved_vtx[0].z = last_vtx[0].z; */
	/*       moved_vtx[1].z = last_vtx[1].z; */
	/*       moved_vtx[2].z = last_vtx[2].z; */

	/*       // �ړ����̒��_���W���ݒ�.�@���x�N�g�����Čv�Z */
	/*       tria->set_vertexes( moved_vtx, true, false ); */

	/*     } */
	/*     // ���_���W���ړ��������Ƃɂ���,KD �؂̍č\�z���K�v. */
	/*     // �v�č\�z�t���O�𗧂Ă�. */
	this->m_need_rebuild = true;

#ifdef DEBUG
	PL_DBGOSH << "CarGroup::move() end."<<std::endl;
#endif // DEBUG
#undef DEBUG

	return PLSTAT_OK;
}

POLYLIB_STAT CarGroup::mk_param_tag(
									TextParser* tp,
									string rank_no,
									string extend,
									string format
									) {

#ifdef DEBUG
										cout << "CarGroup::"<<__FUNCTION__ << endl;
#endif

										stringstream ss;
										POLYLIB_STAT	stat;
										//goto root
										tp->changeNode("/Polylib");
										tp->changeNode(this->acq_fullpath());
										//this is test. change m_velocity forcely
										//  m_velocity = -500.;
										ss<<m_velocity;
										string value;
										ss>>value;
										tp->updateValue("velocity",value);

										return PolygonGroup::mk_param_tag(tp,rank_no,extend,format);
}
