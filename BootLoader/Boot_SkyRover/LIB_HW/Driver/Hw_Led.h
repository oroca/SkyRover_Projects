//----------------------------------------------------------------------------
//    프로그램명 	: LED 관련 헤더
//
//    만든이     	: 
//
//    날  짜     	: 
//    
//    최종 수정  	:
//
//    MPU_Type		:
//
//    파일명     	: Hw_Led.h
//----------------------------------------------------------------------------
                                                                                                 
#ifndef __HW_LED_H__
#define __HW_LED_H__


#ifdef   HW_LED_LOCAL
#define  EXT_HW_LED_DEF
#else
#define  EXT_HW_LED_DEF     extern
#endif



#include "Hw.h"



EXT_HW_LED_DEF void  Hw_Led_Init( void );

EXT_HW_LED_DEF void Hw_Led_On( u8 Ch );
EXT_HW_LED_DEF void Hw_Led_Off( u8 Ch );
EXT_HW_LED_DEF void Hw_Led_Toggle( u8 Ch );


#endif
