//----------------------------------------------------------------------------
//    프로그램명 	:
//
//    만든이     	: 
//
//    날  짜     	:
//    
//    최종 수정  	:
//
//    MPU_Type		:
//
//    파일명     	: Ap_Define.h
//----------------------------------------------------------------------------
                                                                                                 
#ifndef __AP_DEFINE_H__
#define __AP_DEFINE_H__


#ifdef   AP_DEFINE_LOCAL
#define  EXT_AP_DEFINE_DEF
#else
#define  EXT_AP_DEFINE_DEF     extern
#endif



#include "Hw.h"




#ifndef    NULL_FUNC
#define    NULL_FUNC    ((void *)0)
#endif



#endif
