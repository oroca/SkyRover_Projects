//----------------------------------------------------------------------------
//    프로그램명 	: PLL 관련 헤더
//
//    만든이     	: 
//
//    날  짜     	:
//    
//    최종 수정  	:
//
//    MPU_Type		:
//
//    파일명     	: Hw_PLL.h
//----------------------------------------------------------------------------
                                                                                                 
#ifndef __HW_PLL_H__
#define __HW_PLL_H__


#ifdef   HW_PLL_LOCAL
#define  EXT_HW_PLL_DEF
#else
#define  EXT_HW_PLL_DEF     extern
#endif



#include "Hw.h"



EXT_HW_PLL_DEF void  Hw_PLL_Init( void );



#endif
