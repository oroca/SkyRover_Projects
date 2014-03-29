//----------------------------------------------------------------------------
//    프로그램명 	: ISR 관련 함수
//
//    만든이     	: 
//
//    날  짜     	: 
//    
//    최종 수정  	:
//
//    MPU_Type		:
//
//    파일명     	: Hw_ISR.c
//----------------------------------------------------------------------------


//----- 헤더파일 열기
//
#define  HW_ISR_LOCAL


#include "Hw_ISR.h"


//-- 인터럽트 벡터는 Offset은 128 word 단위로 정렬되어야함( 512 bytes 단위 )
//
volatile u32 Hw_ISR_VectorTable[100] __attribute__((aligned(512)));




/*---------------------------------------------------------------------------
     TITLE   : Hw_ISR_Init
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
void Hw_ISR_Init( void )
{
	REG_SCB_VTOR = (u32)Hw_ISR_VectorTable;
}





/*---------------------------------------------------------------------------
     TITLE   : Hw_ISR_SetIRQFunc
     WORK    :
     ARG     : 
     			u8  ISR_Num      : 인터럽트 번호
     			u32 FuncAddress  : 인터럽트 발생시 실행하고자 하는 함수 주소
     			u8  ISR_Priority : 인터럽트 우선순위(작은값이 우선순위 높음)
     RET     : void
---------------------------------------------------------------------------*/
void Hw_ISR_SetIRQFunc( u8 ISR_Num, u32 FuncAddress, u8 ISR_Priority )
{	
	u32 ISR_StartOffset = 16;
	
	Hw_ISR_VectorTable[ ISR_StartOffset + ISR_Num ] = FuncAddress;	
}





/*---------------------------------------------------------------------------
     TITLE   : Hw_ISR_SetIRQFuncByAddr
     WORK    :
     ARG     : 
     			u8  ISR_Addr     : 인터럽트 주소
     			u32 FuncAddress  : 인터럽트 발생시 실행하고자 하는 함수 주소
     			u8  ISR_Priority : 인터럽트 우선순위(작은값이 우선순위 높음)
     RET     : void
---------------------------------------------------------------------------*/
void Hw_ISR_SetIRQFuncByAddr( u32 ISR_Addr, u32 FuncAddress, u8 ISR_Priority )
{	
	u32 ISR_StartOffset = ISR_Addr/4;
	
	Hw_ISR_VectorTable[ ISR_StartOffset ] = FuncAddress;	
}
 
