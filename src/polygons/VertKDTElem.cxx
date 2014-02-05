#include "polygons/VertKDTElem.h"

#include <vector>
#include "common/Vec3.h"
#include "common/BBox.h"
#include "common/PolylibCommon.h"
#include "common/PolylibStat.h"
#include "polygons/Vertex.h"
#include "polygons/VertexList.h"


namespace PolylibNS{



	////////////////////////////////////////////////////////////////////////////
	///
	///  クラス:VertKDTElem
	///  VertexKD木の要素クラスです。
	///
	////////////////////////////////////////////////////////////////////////////


	/// 
	/// コンストラクタ。
	///
	/// @param[in] 
	/// @attention ポインタを格納するが、参照のみ。deleteは行わない。
	/// 
	VertKDTElem::VertKDTElem(Vertex* vertex)
	{
		m_vertex=vertex;
	}

	//setter/getter
	///
	/// return vertex.
	///
	Vertex* VertKDTElem::get_vertex()
	{
		return m_vertex;
	}

	//setter/getter

	///
	/// return vertex.
	///
	Vec3<REAL_TYPE>* VertKDTElem::get_pos() const
	{
		// PL_DBGOSH << __func__ << " " <<(*m_vertex)[0]
		// 		<< " " <<(*m_vertex)[1]
		// 		<< " " <<(*m_vertex)[2]
		// 		<<std::endl;
		return (Vec3<REAL_TYPE>*) m_vertex;
	}

}// end of namespace PolylibNS

