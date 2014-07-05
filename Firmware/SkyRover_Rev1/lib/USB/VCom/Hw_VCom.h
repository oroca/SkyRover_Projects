//----------------------------------------------------------------------------
//    프로그램명 	: VCom 관련 함수 헤더
//
//    만든이     	: 
//
//    날  짜     	: 
//    
//    최종 수정  	: 
//
//    MPU_Type 		:
//
//    파일명     	: Hw_VCom.h
//----------------------------------------------------------------------------




                                                                                                 
#ifndef  _HW_VCOM_H
#define  _HW_VCOM_H



#include "Hw.h"


#ifdef   HW_VCOM_LOCAL
#define  EXT_HW_VCOM_DEF
#else
#define  EXT_HW_VCOM_DEF     extern
#endif


EXT_HW_VCOM_DEF void (*Hw_VCom_RxdFuncPtr)(char Ch);



EXT_HW_VCOM_DEF void Hw_VCom_Init( void );

EXT_HW_VCOM_DEF u8   Hw_VCom_Getch( void );
EXT_HW_VCOM_DEF void Hw_VCom_Putch( char Uart_PutData );

#endif
                                                                                                 
                                                                                                 

