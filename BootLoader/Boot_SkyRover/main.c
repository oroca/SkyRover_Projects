//----------------------------------------------------------------------------
//    프로그램명 	: Radio
//
//    만든이     	: Cho Han Cheol (Baram)
//
//    날  짜     	: 2013. 8.20.
//
//    최종 수정  	:
//
//    MPU_Type		:
//
//    파일명     	: Main.c
//----------------------------------------------------------------------------
/*
	130820 
		- 초기 프로젝트 작업 시작
	130823 
		- Uart 인터럽트 구현
		
*/




//----- 헤더파일 열기
//
#define  MAIN_LOCAL

#include "Main.h"
#include "hw_config.h"
#include "usb_lib.h"
#include "usb_desc.h"
#include "usb_pwr.h"



//-- 내부 선언
//



//-- 내부 변수
//



//-- 내부 함수
//
static void Main_Init( void );



void LED_Tick( void )
{
	Hw_Led_Toggle(0);
	Hw_Led_Toggle(1);	
}




/*---------------------------------------------------------------------------
     TITLE   :	main          
     WORK    : 	
     			메인 함수
     ARG     : 	void
     RET     : 	
     			int
---------------------------------------------------------------------------*/
int main(void)
{
	u8 Ch;
	u8 Ret;
	u8 *pMem = (u8 *)0x08003000;
	u32 i;
	u8 Data[200];
	u8 Uart_GetData;


	Main_Init();


	
	if( Ap_ST32BL_RunBoot == TRUE )
	{
		//Lb_printf("a");
		//Hw_VCom_Init();

		//-- 먼저 명령을 보낸 포트로 다운로드 시작 
		//
		#if 1
		while(1)
		{
			if( HW_VCOM_Q_VAILD(0) > 0 )
			{
				Hw_VCom_Q_Pop( 0, &Uart_GetData );

				if( Uart_GetData == STM32_CMD_INIT )
				{
					Ap_ST32BL_UartPort = HW_UART_VCOM;
					Ap_ST32BL_SendByte(STM32_COMM_ACK);
					break;
				}
			}

			if( HW_UART_Q_VAILD(HW_UART_COM1) > 0 )
			{
				Hw_Uart_Q_Pop( HW_UART_COM1, &Uart_GetData );

				if( Uart_GetData == STM32_CMD_INIT )
				{
					Ap_ST32BL_UartPort = HW_UART_COM1;
					Ap_ST32BL_SendByte(STM32_COMM_ACK);
					break;
				}
			}	
		}
		#endif
		Ap_ST32BL_Main();
	}
	else
	{		
		Ap_ST32BL_JumpToApp(0);
	}
    
    return 0;
}





/*---------------------------------------------------------------------------
     TITLE   : Main_Init
     WORK    :
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
void Main_Init( void )
{
	Hw_Init();
	Ap_Init();
	
	Hw_Timer_Set  ( HW_TIMER_CH_LED, 1000, LOOP_TIME, LED_Tick, NULL );
	Hw_Timer_Start( HW_TIMER_CH_LED );	
}



