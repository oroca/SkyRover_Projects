//----------------------------------------------------------------------------
//    프로그램명 	: Printf 관련 함수 헤더
//
//    만든이     	: 
//
//    날  짜     	: 
//    
//    최종 수정  	: 
//
//    MPU_Type 		:
//
//    파일명     	: Lb_Printf.h
//----------------------------------------------------------------------------




                                                                                                 
#ifndef  _LB_PRINTF_H
#define  _LB_PRINTF_H



#include "Hw.h"




#ifdef   LB_PRINTF_LOCAL
#define  EXT_LB_PRINTF_DEF
#else
#define  EXT_LB_PRINTF_DEF     extern
#endif


typedef char *Lb_va_list;


#define Lb_va_start(ap, p)			(ap = (char *) (&(p)+1))
#define Lb_va_arg(ap, type)		((type *) (ap += sizeof(type)))[-1]
#define Lb_va_end(ap)


EXT_LB_PRINTF_DEF void Lb_printf( char *fmt, ... );                                            
EXT_LB_PRINTF_DEF void Lb_sprintf( char *pStr, char *fmt, ... );
EXT_LB_PRINTF_DEF void Lb_vsprintf( char *pStr, char *fmt, Lb_va_list args );

#endif
                                                                                                 
                                                                                                 

