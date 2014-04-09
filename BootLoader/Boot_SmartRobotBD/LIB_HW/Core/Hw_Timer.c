//----------------------------------------------------------------------------
//    프로그램명 	: Timer 관련 함수
//
//    만든이     	: Cho Han Cheol
//
//    날  짜     	: 
//    
//    최종 수정  	: 
//
//    MPU_Type		:
//
//    파일명     	: Hw_Timer.c
//----------------------------------------------------------------------------



//----- 헤더파일 열기
//
#define  HW_TIMER_LOCAL  

#include "Hw_TImer.h"






//-- 내부 선언
//







//-- 내부 변수
//

static volatile u32 Hw_Timer_Counter  = 0;	

static volatile u16 Tmr_Ctn  = 0;	// Delay용 타이머 변수.
static volatile u16 Tmr_HandleIndex = 0;

static TMR  Timer_Tbl[ TIMER_MAX ];    // 타이머 배열 선언




           


//-- 내부 함수
//

void Hw_Timer_SetupISR( void );
void Hw_Timer_Setup( void );



void Hw_Timer_ISR(void)
{

	Hw_Timer_Tick();  

}





/*---------------------------------------------------------------------------
     TITLE   : Hw_Timer_Init
     WORK    :
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
void Hw_Timer_Init( void )
{
	u8 i;
	static u8 Excute = 0;

	
	if( Excute == 1 ) return;  // 이미 한번 실행했다면 정지.
	
	
	// 구조체 초기화
	for(i=0; i<TIMER_MAX; i++)
	{
		Timer_Tbl[i].Timer_En   = OFF;
		Timer_Tbl[i].Timer_Ctn  = 0;
		Timer_Tbl[i].Timer_Init = 0;
		Timer_Tbl[i].TmrFnct    = NULL;
	}	                   
	
	
	Hw_Timer_SetupISR();
	Hw_Timer_Setup();	

	Excute = 1;

}





/*---------------------------------------------------------------------------
     TITLE   : Hw_Timer_DeInit
     WORK    :
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
void Hw_Timer_DeInit( void )
{
	CLR_BIT( REG_STK_CTRL, 1 );	// 인터럽트 비활성화		
	CLR_BIT( REG_STK_CTRL, 0 );	// Counter Enable
}





/*---------------------------------------------------------------------------
     TITLE   : Hw_Timer_SetupISR
     WORK    :
     	 	 	 타이머 H/W 기능을 초기화 한다.
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
void Hw_Timer_SetupISR( void )
{	
	Hw_ISR_SetIRQFuncByAddr( 0x003C, (u32)Hw_Timer_ISR, 0 );	
}




/*---------------------------------------------------------------------------
     TITLE   : Hw_Timer_Setup
     WORK    :
     	 	 	 타이머 H/W 기능을 초기화 한다.
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
void Hw_Timer_Setup( void )
{

	REG_STK_LOAD = 72000 - 1;	// 72000000 / 72000 = 1000us = 1ms
	
	REG_STK_VAL  = 0;	
	
	SET_BIT( REG_STK_CTRL, 2 );	// CLKSOURCE = Processor Clock 72Mhz		
	SET_BIT( REG_STK_CTRL, 1 );	// 인터럽트 활성화		
	SET_BIT( REG_STK_CTRL, 0 );	// Counter Enable
	
}





/*---------------------------------------------------------------------------
     TITLE	: Timer_Tick
     WORK	:
			타이머인터럽트 발생시에 실행되는 함수(1ms 혹은 10ms 단위로 실행)
     ARG	: void
     RET	: void
---------------------------------------------------------------------------*/
void Hw_Timer_Tick(void)
{
	u8 i;

	
	Hw_Timer_Counter++;


	if( Tmr_Ctn ) Tmr_Ctn--;
	
	
			   
	for(i=0; i<TIMER_MAX; i++)								// 타이머 갯수만큼
	{
		if( Timer_Tbl[i].Timer_En == ON)   					// 타이머가 활성화 됬니?
		{
			Timer_Tbl[i].Timer_Ctn--;  						// 타이머값 감소

			if(Timer_Tbl[i].Timer_Ctn == 0) 				// 타이머 오버플로어
			{
				if(Timer_Tbl[i].Timer_Mode == ONE_TIME)  	// 한번만 실행하는거면

				Timer_Tbl[i].Timer_En = OFF;     			// 타이머 OFF 한다.

				Timer_Tbl[i].Timer_Ctn = Timer_Tbl[i].Timer_Init; // 타이머 초기화

				(*Timer_Tbl[i].TmrFnct)();  				// 전달변수 없이 함수 실행
			}
		}
	}
	
}





/*---------------------------------------------------------------------------
     TITLE	: Timer_Set
     WORK
			소프트 타이머에서 이벤트 발생시 실행할 함수 연결
     ARG
     	 	U8 TmrNum 	: 타이머 번호
			U16 TmrData : 타이머 시간값( ms 단위 )
			U8 TmrMode 	: ONE_TIME : 타이머 시작후 한번만 실행
			LOOP_TIME 	: 타이머가 시작후 주기적으로 무한 실행.
			void (*Fnct)(void),void *arg : 실행할 타이머 함수.
     RET
     	 	void
---------------------------------------------------------------------------*/
void Hw_Timer_Set(u8 TmrNum, u16 TmrData, u8 TmrMode, void (*Fnct)(void),void *arg)
{
	Timer_Tbl[TmrNum].Timer_Mode = TmrMode;    // 모트설정
	Timer_Tbl[TmrNum].TmrFnct    = Fnct;       // 실행할 함수
	Timer_Tbl[TmrNum].TmrFnctArg = arg;        // 매개변수
	Timer_Tbl[TmrNum].Timer_Ctn  = TmrData;
	Timer_Tbl[TmrNum].Timer_Init = TmrData;
}





/*---------------------------------------------------------------------------
     TITLE	: Timer_Start
     WORK
     ARG
     RET
---------------------------------------------------------------------------*/
void Hw_Timer_Start(u8 TmrNum)
{
	if(TmrNum < TIMER_MAX)
		Timer_Tbl[TmrNum].Timer_En = ON;
}





/*---------------------------------------------------------------------------
     TITLE	: Hw_Timer_Stop
     WORK
     ARG
     RET
---------------------------------------------------------------------------*/
void Hw_Timer_Stop(u8 TmrNum)
{
	if(TmrNum < TIMER_MAX)
		Timer_Tbl[TmrNum].Timer_En = OFF;
}





/*---------------------------------------------------------------------------
     TITLE	: Timer_Reset
     WORK
     	 	 - 타이머값 초기화
     ARG
     RET
---------------------------------------------------------------------------*/
void Hw_Timer_Reset(u8 TmrNum)
{
	Timer_Tbl[TmrNum].Timer_En   = OFF;
	Timer_Tbl[TmrNum].Timer_Ctn  = Timer_Tbl[TmrNum].Timer_Init;
}





/*---------------------------------------------------------------------------
     TITLE	: Hw_Timer_Delay_ms
     WORK
     	 	 ms 단위로 딜레이를 준다.
     ARG
     RET
---------------------------------------------------------------------------*/
void Hw_Timer_Delay_ms( u16 DelayData )
{
	Tmr_Ctn = DelayData;
	
	while( Tmr_Ctn );
}





/*---------------------------------------------------------------------------
     TITLE	: Hw_Timer_Get_Handle
     WORK
     	 	 ms 단위로 딜레이를 준다.
     ARG
     RET
---------------------------------------------------------------------------*/
s16 Hw_Timer_Get_Handle( void )
{
	s16 TmrIndex = Tmr_HandleIndex;
	
	Tmr_HandleIndex++;
	
	return TmrIndex;
}





/*---------------------------------------------------------------------------
     TITLE	: Hw_Timer_Get_CountValue
     WORK
     ARG
     RET
---------------------------------------------------------------------------*/
u32 Hw_Timer_Get_CountValue( void )
{
	return Hw_Timer_Counter;
}




/*---------------------------------------------------------------------------
     TITLE	: Hw_Timer_TimeStart
     WORK
     ARG
     RET
---------------------------------------------------------------------------*/
void Hw_Timer_TimeStart( HW_TIMER_TIME_OBJ *TimerTimePtr )
{
	TimerTimePtr->Start = Hw_Timer_Counter;
}





/*---------------------------------------------------------------------------
     TITLE	: Hw_Timer_TimeStart
     WORK
     ARG
     RET
---------------------------------------------------------------------------*/
u32 Hw_Timer_TimeGetElapse( HW_TIMER_TIME_OBJ *TimerTimePtr )
{
	return (Hw_Timer_Counter - TimerTimePtr->Start);
}
