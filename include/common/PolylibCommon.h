/*
 * Polylib - Polygon Management Library.
 *
 * Copyright (c) RIKEN, Japan. All right reserved. 2010-
 *
 */
#ifndef polylib_common_h
#define polylib_common_h

#include <iostream>

namespace PolylibNS {

////////////////////////////////////////////////////////////////////////////
///
/// デバッグ出力先、エラー時出力先
///
////////////////////////////////////////////////////////////////////////////
#define PL_DBGOS    std::cout
#define PL_DBGOSH   std::cout<<gs_rankno
#define PL_ERROS    std::cerr
#define PL_ERROSH   std::cerr<<gs_rankno

////////////////////////////////////////////////////////////////////////////
///
/// デバッグ出力用ランク番号グローバル文字列
///
////////////////////////////////////////////////////////////////////////////
extern std::string gs_rankno;

} // end of namespace PolylibNS
#endif // polylib_common_h
