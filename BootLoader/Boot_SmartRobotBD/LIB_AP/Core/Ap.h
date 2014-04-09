//----------------------------------------------------------------------------
//    프로그램명 	:
//
//    만든이     	: Cho Han Cheol
//
//    날  짜     	:
//    
//    최종 수정  	:
//
//    MPU_Type	:
//
//    파일명     	: Ap.h
//----------------------------------------------------------------------------
                                                                                                 
#ifndef __AP_H__
#define __AP_H__


#ifdef   AP_LOCAL
#define  EXT_AP_DEF
#else
#define  EXT_AP_DEF     extern
#endif





#include "Hw.h"
#include "Ap_Define.h"
#include "Lb_Util.h"

#include "Ap_ST32BL.h"



EXT_AP_DEF void Ap_Init( void );



#endif
