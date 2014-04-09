//----------------------------------------------------------------------------
//    프로그램명 : Timer 함수 헤더
//
//    만든이     : Cho Han Cheol
//
//    날  짜     : 
//    
//    최종 수정  : 
//
//    MPU_Type   : 
//
//    파일명     : Timer_Lib.h
//----------------------------------------------------------------------------



#ifndef __HW_TIMER_H__
#define __HW_TIMER_H__ 


#ifdef   HW_TIMER_LOCAL
#define  EXT_HW_TIMER_DEF 
#else
#define  EXT_HW_TIMER_DEF     extern
#endif



#include "Hw.h"






#ifndef ON
#define ON 1
#endif

#ifndef OFF
#define OFF 0
#endif

#ifndef NULL
#define NULL 0
#endif


#define TIMER_MAX    10

#define ONE_TIME     1
#define LOOP_TIME    2

#define TIMER_HANDLE  s16

typedef struct tmr 
{

	u8   Timer_En;                 // 타이머 인에이블 신호
	u8   Timer_Mode;               // 타이머 모드
	u16  Timer_Ctn;                // 현제의 타이머 값
	u16  Timer_Init;               // 타이머 초기화될때의 카운트 값
	void (*TmrFnct)(void);         // 만료될때 실행될 함수
	void *TmrFnctArg;              // 함수로 전달할 인수들
} TMR;


typedef struct 
{
	u32 Start;
} HW_TIMER_TIME_OBJ;



EXT_HW_TIMER_DEF void Hw_Timer_Init( void );
EXT_HW_TIMER_DEF void Hw_Timer_DeInit( void );
EXT_HW_TIMER_DEF void Hw_Timer_Tick(void);
EXT_HW_TIMER_DEF void Hw_Timer_Set(u8 TmrNum, u16 TmrData, u8 TmrMode, void (*Fnct)(void),void *arg);
EXT_HW_TIMER_DEF void Hw_Timer_Start(u8 TmrNum);
EXT_HW_TIMER_DEF void Hw_Timer_Stop(u8 TmrNum);
EXT_HW_TIMER_DEF void Hw_Timer_Reset(u8 TmrNum);
EXT_HW_TIMER_DEF void Hw_Timer_Delay_ms( u16 DelayData );
EXT_HW_TIMER_DEF s16  Hw_Timer_Get_Handle( void );
EXT_HW_TIMER_DEF u32  Hw_Timer_Get_CountValue( void );

EXT_HW_TIMER_DEF void Hw_Timer_TimeStart( HW_TIMER_TIME_OBJ *TimerTimePtr );
EXT_HW_TIMER_DEF u32  Hw_Timer_TimeGetElapse( HW_TIMER_TIME_OBJ *TimerTimePtr );


#endif





