#ifndef polylib_VertKDTElem_h
#define polylib_VertKDTElem_h
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


class VertKDTElem{
public:
	/// 
	/// コンストラクタ。
	///
	/// @param[in] 
	/// @attention ポインタを格納するが、参照のみ。deleteは行わない。
	/// 
	VertKDTElem(Vertex* vertex);

	//setter/getter
	///
	/// return vertex.
	///
	Vertex* get_vertex();
	//setter/getter

	///
	/// return vertex.
	///
	Vec3<REAL_TYPE>* get_pos() const;

private:
	///
	/// vertex
	///
	Vertex* m_vertex; 


};// end of class 

}// end of namespace PolylibNS
#endif //polylib_VertKDTElem_h
