//----------------------------------------------------------------------------
//    프로그램명 	: StrCmd Q 관련 함수
//
//    만든이     	: 
//
//    날  짜     	: 
//    
//    최종 수정  	: 
//
//    MPU_Type 		:
//
//    파일명     	: AP_StrCmd_Q.c
//----------------------------------------------------------------------------




#define   AP_STRCMD_Q_LOCAL


#include "Ap_StrCmd_Q.h"





/*---------------------------------------------------------------------------
     TITLE   : Ap_StrCmd_Q_Init
     WORK    : 
     ARG     : void
     RET     : void   
---------------------------------------------------------------------------*/
void Ap_StrCmd_Q_Init(void)
{
	u8 i;
	
	for( i=0; i<AP_STRCMD_Q_CH_MAX; i++ )
	{
		Ap_StrCmd_Q_Start[i] = Ap_StrCmd_Q_End[i] = 0;
	}
}        





/*---------------------------------------------------------------------------
     TITLE   : Ap_StrCmd_Q_SIZE
     WORK    : 
     ARG     : void
     RET     : void   
---------------------------------------------------------------------------*/
u32 Ap_StrCmd_Q_SIZE( u8 Ch )
{
	return (Ap_StrCmd_Q_Start[Ch] - Ap_StrCmd_Q_End[Ch] + AP_STRCMD_BUFFER_MAX) % AP_STRCMD_BUFFER_MAX;
}





/*---------------------------------------------------------------------------
     TITLE   : Ap_StrCmd_Q_VAILD
     WORK    : 
     ARG     : void
     RET     : void   
---------------------------------------------------------------------------*/
u32 Ap_StrCmd_Q_VAILD( u8 Ch )
{
	return Ap_StrCmd_Q_SIZE(Ch);
}





/*---------------------------------------------------------------------------
     TITLE   : Ap_StrCmd_Q_PushReady
     WORK    : 
     ARG     : void
     RET     : void   
---------------------------------------------------------------------------*/
u16 Ap_StrCmd_Q_PushReady( u8 Ch )
{
	if( Ap_StrCmd_Q_Size[Ch] < AP_STRCMD_BUFFER_MAX ) 	return TRUE;
	else                                      			return FALSE;
}





/*---------------------------------------------------------------------------
     TITLE   : Ap_StrCmd_Q_PushReady
     WORK    : 
     ARG     : void
     RET     : void   
---------------------------------------------------------------------------*/
s8 Ap_StrCmd_Q_Push( u8 Ch, AP_STRCMD_DATA_PTR_TYPE  *Push_Ptr )
{
        
	if (Ap_StrCmd_Q_SIZE(Ch) == (AP_STRCMD_BUFFER_MAX-1)) return FALSE;
	
	Ap_StrCmd_Q_Buffer[Ch][Ap_StrCmd_Q_Start[Ch]++] = *Push_Ptr;
	Ap_StrCmd_Q_Start[Ch] %= AP_STRCMD_BUFFER_MAX;
	
	return TRUE;
}                                                  





/*---------------------------------------------------------------------------
     TITLE   : Ap_StrCmd_Q_Pop
     WORK    : 
     ARG     : void
     RET     : void   
---------------------------------------------------------------------------*/
s8 Ap_StrCmd_Q_Pop( u8 Ch, AP_STRCMD_DATA_PTR_TYPE *Pop_Ptr )
{
	if (Ap_StrCmd_Q_SIZE(Ch) == 0) return FALSE;
	
	*Pop_Ptr = Ap_StrCmd_Q_Buffer[Ch][Ap_StrCmd_Q_End[Ch]++];
	
	Ap_StrCmd_Q_End[Ch] %= AP_STRCMD_BUFFER_MAX;

    return AP_STRCMD_POP_SUCCESS;
}



