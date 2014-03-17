// -*- Mode:c++ -*-
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

#ifndef polylib_dvertex_h
#define polylib_dvertex_h

#include <vector>
#include <string>
#include "common/PolylibCommon.h"
#include "common/Vec3.h"
#include "polygons/Vertex.h"
#include "polygons/DVertexManager.h"

using namespace Vec3class;

class DVertexManager;

namespace PolylibNS{

////////////////////////////////////////////////////////////////////////////
///
/// クラス:DVertex
///   polygon の頂点クラス。 
///   Vertex   クラスを継承　任意の実数型のスカラーデータを保持できる。
/// 
///
////////////////////////////////////////////////////////////////////////////

class DVertex :public Vertex{

private:
	PL_REAL* m_scalar;
	std::vector<Vec3<PL_REAL>*>* m_vector;
	DVertexManager* DVM_ptr;

public:
	// DVertex(int nscaler,int nvector):m_scalar(NULL),m_vector(NULL){
	//   if(m_scalar==NULL) m_scalar=new DT[nscaler];

	//   if(m_vector==NULL) m_vector=new std::vector<DT>[nvector];
	//   for(int i=0;i<nvector;++i){
	// 	m_vector[i]=new std::vector<DT>;
	//   }

	DVertex(DVertexManager* DVM):m_scalar(NULL),m_vector(NULL),DVM_ptr(NULL){
		//#define DEBUG
#ifdef DEBUG
		PL_DBGOSH << "DVertex::"<< __func__<< " in "<< std::endl;
#endif //
		if(DVM_ptr==NULL) DVM_ptr=DVM;

#ifdef DEBUG
		PL_DBGOSH << "DVertex::"<< __func__<< " 1 "<< std::endl;
#endif //

		if(DVM_ptr!=NULL) {
#ifdef DEBUG
			PL_DBGOSH << "DVertex::"<< __func__<< " 2 "<< std::endl;
#endif //

			if(m_scalar==NULL) m_scalar=new PL_REAL[DVM_ptr->nscalar()];
#ifdef DEBUG
			PL_DBGOSH << "DVertex::"<< __func__<< " 3 "<< std::endl;
#endif //

			if(m_vector==NULL) {
#ifdef DEBUG
				PL_DBGOSH << "DVertex::"<< __func__<< " 4 "<< std::endl;
#endif //

				m_vector=new std::vector<Vec3<PL_REAL>*>;
#ifdef DEBUG
				PL_DBGOSH << "DVertex::"<< __func__<< " 5 "<< std::endl;
#endif //

				for(int i=0;i<DVM_ptr->nvector();++i){
					Vec3<PL_REAL>* tmp=new Vec3<PL_REAL>;
					m_vector->push_back(tmp);
				}
#ifdef DEBUG
				PL_DBGOSH << "DVertex::"<< __func__<< " 6 "<< std::endl;
#endif //
				//#undef DEBUG
			}
		}
	}


	DVertex():m_scalar(NULL),m_vector(NULL),DVM_ptr(NULL){}

	void set_DVM(DVertexManager* DVM){
		if(DVM_ptr==NULL) DVM_ptr=DVM;
		if(DVM_ptr!=NULL) {
			if(m_scalar==NULL) m_scalar=new PL_REAL[DVM_ptr->nscalar()];
			if(m_vector==NULL) {
				m_vector=new std::vector<Vec3<PL_REAL>*>;
				for(int i=0;i<DVM_ptr->nvector();++i){
					Vec3<PL_REAL>* tmp=new Vec3<PL_REAL>;
					m_vector->push_back(tmp);
				}
			}
		}
	}


	~DVertex(){
		if(m_scalar!=NULL){ 
			delete[] m_scalar;
			m_scalar=NULL;
		}
		if(m_vector!=NULL){ 
			for(int i=0;i<DVM_ptr->nvector();++i){
				delete (*m_vector)[i];
			}
			delete m_vector;
			m_vector=NULL;
		}
	}

	/// スカラー値の登録
	/// スカラー値を登録する。
	///
	/// @param[in] i スカラーのインデックス。0で開始。
	/// @param[in] d 登録するスカラー値。

	void set_scalar(const int i,const PL_REAL d){
		//      std::cout << __func__ << " start "<<this <<std::endl;
		if(i>=DVM_ptr->nscalar()){
			PL_ERROSH<< "error in DVertex " << __func__
				<<" wrong index for DVertex scalar data. index "
				<< i << "  nscalar = "<<DVM_ptr->nscalar()<<std::endl;
		} else {
			m_scalar[i] = d;
		}
	}

	/// スカラー値の参照
	///
	/// @param[in] i スカラーのインデックス。0で開始。
	/// @return スカラー値

	PL_REAL get_scalar(int i) const {
		if(i>=DVM_ptr->nscalar()){
			PL_ERROSH<< "error in DVertex " << __func__
				<<" wrong index for DVertex scalar data. index "
				<< i << "  nscalar = "<<DVM_ptr->nscalar()<<std::endl;
			return 0.;
		} else {

			return m_scalar[i];
		}
	}

	/// ベクター値の登録
	/// ベクター値を登録する。
	///
	/// @param[in] i i番目ベクトルのインデックス。0で開始。
	/// @param[in] vec 登録する Vec3<PL_REAL> ベクター


	void set_vector(const int i, const Vec3<PL_REAL> vec){
		// PL_DBGOSH << " "<< i<< " " << vec << " " 
		// 		<< DVM_ptr->nvector() <<std::endl;

		if(i>=DVM_ptr->nvector()){
			PL_ERROSH<< "error in DVertex " << __func__
				<<" wrong index for DVertex scalar data. index "
				<< i << "  nscalar = "<<DVM_ptr->nvector()<<std::endl;
		} else {

			Vec3<PL_REAL>* vector=new Vec3<PL_REAL>;
			(*vector) = vec;
			(*m_vector)[i]=vector;
		}
	}
	/// ベクター値の参照
	///
	/// @param[in] i i番目ベクトルのインデックス。0で開始。
	/// @param[in] vec Vec3 ベクトル
	void get_vector(const int i, Vec3<PL_REAL>* vec){
		if(i>=DVM_ptr->nvector()){
			PL_ERROSH<< "error in DVertex " << __func__
				<<" wrong index for DVertex vector data. index "
				<< i << "  nvector = "<<DVM_ptr->nvector()<<std::endl;
		} else {
			(*vec)=*((*m_vector)[i]);
		}
	}

	DVertexManager* DVM(){return DVM_ptr;}  

};

}

#endif // polylib_dvertex_h
