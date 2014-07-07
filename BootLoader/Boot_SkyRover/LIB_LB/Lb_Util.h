//----------------------------------------------------------------------------
//    프로그램명 	: Util 관련 함수 헤더
//
//    만든이     	: 
//
//    날  짜     	: 
//    
//    최종 수정  	: 
//
//    MPU_Type 		:
//
//    파일명     	: Lb_Util.h
//----------------------------------------------------------------------------




                                                                                                 
#ifndef  _LB_UTIL_H
#define  _LB_UTIL_H



#include "Hw.h"



#ifdef   LB_UTIL_LOCAL
#define  EXT_LB_UTIL_DEF
#else
#define  EXT_LB_UTIL_DEF     extern
#endif



EXT_LB_UTIL_DEF void delay(volatile unsigned int timeCount);
EXT_LB_UTIL_DEF void delay_second(void);
                                            

#endif
                                                                                                 
                                                                                                 

