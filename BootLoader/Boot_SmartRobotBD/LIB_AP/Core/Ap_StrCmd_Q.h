//----------------------------------------------------------------------------
//    프로그램명 	: StrCmd Q 관련 함수 헤더
//
//    만든이     	: 
//
//    날  짜     	: 
//    
//    최종 수정  	: 
//
//    MPU_Type 		:
//
//    파일명     	: AP_StrCmd_Q.h
//----------------------------------------------------------------------------


#ifndef  _AP_STRCMD_Q_H
#define  _AP_STRCMD_Q_H

#ifdef   AP_STRCMD_Q_LOCAL
#define  EXT_AP_STRCMD_Q_DEF 
#else
#define  EXT_AP_STRCMD_Q_DEF     extern
#endif


#include "Ap_StrCmd.h"



#define AP_STRCMD_Q_CH_MAX       	1
#define AP_STRCMD_BUFFER_MAX     	5                //  버퍼 최대 갯수

#define AP_STRCMD_Q_RX_CH     		0


typedef AP_STRCMD_CMD_OBJ AP_STRCMD_DATA_PTR_TYPE;  


#define AP_STRCMD_Q_EMPTY        	1
#define AP_STRCMD_Q_FULL         	0
#define AP_STRCMD_POP_SUCCESS    	2

#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif


EXT_AP_STRCMD_Q_DEF u32 Ap_StrCmd_Q_SIZE( u8 Ch );
EXT_AP_STRCMD_Q_DEF u32 Ap_StrCmd_Q_VAILD( u8 Ch );
             
EXT_AP_STRCMD_Q_DEF AP_STRCMD_DATA_PTR_TYPE    Ap_StrCmd_Q_Buffer[AP_STRCMD_Q_CH_MAX][ AP_STRCMD_BUFFER_MAX+1 ];  


EXT_AP_STRCMD_Q_DEF s16  Ap_StrCmd_Q_Size [AP_STRCMD_Q_CH_MAX];	// 버퍼의 데이터 숫자  
EXT_AP_STRCMD_Q_DEF s16  Ap_StrCmd_Q_Start[AP_STRCMD_Q_CH_MAX];	// 버퍼의 시작 포인터
EXT_AP_STRCMD_Q_DEF s16  Ap_StrCmd_Q_End  [AP_STRCMD_Q_CH_MAX];	// 버퍼의 끝 포인터


EXT_AP_STRCMD_Q_DEF void Ap_StrCmd_Q_Init(void);
EXT_AP_STRCMD_Q_DEF u16  Ap_StrCmd_Q_PushReady( u8 Ch );
EXT_AP_STRCMD_Q_DEF s8   Ap_StrCmd_Q_Push( u8 Ch, AP_STRCMD_DATA_PTR_TYPE  *Push_Ptr );
EXT_AP_STRCMD_Q_DEF s8   Ap_StrCmd_Q_Pop( u8 Ch, AP_STRCMD_DATA_PTR_TYPE *Pop_Ptr );

#endif
