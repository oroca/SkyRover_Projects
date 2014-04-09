//----------------------------------------------------------------------------
//    프로그램명 	: Uart 관련 함수
//
//    만든이     	: 
//
//    날  짜     	: 
//    
//    최종 수정  	: 
//
//    MPU_Type		:
//
//    파일명     	: Hw_Uart.h
//----------------------------------------------------------------------------




//----- 헤더파일 열기
//
#define  HW_UART_LOCAL

#include "Hw_Uart.h"
#include "Hw_Uart_Q.h"

//-- MAC 인경우 stdarg.h 가 없다고 나온다. 좀더 찾아봐야 겠다. 
//
//#include <stdarg.h >


//-- 내부 선언
//



//-- 내부 변수
//
HW_UART_OBJ Hw_Uart_Ch[HW_UART_MAX_CH];


//-- 내부 함수
//
void Hw_Uart_Open_COM1( u32 BaudData, void (*ISR_FuncPtr)(char Ch) );
void Hw_Uart_Open_COM2( u32 BaudData, void (*ISR_FuncPtr)(char Ch) );




//-- UART1 ISR
//
void Hw_ISR_UART1_Handler(void)
{
	u8 UartData;
				
	UartData = REG_USART1_DR;
	
	Hw_Uart_Q_Push( 0, &UartData );

	if( Hw_Uart_Ch[HW_UART_COM1].ISR_FuncPtr != NULL )
	{
		(*Hw_Uart_Ch[HW_UART_COM1].ISR_FuncPtr)(UartData);	
	} 
}


//-- UART2 ISR
//
void Hw_ISR_UART2_Handler(void)
{
	u8 UartData;
				
	UartData = REG_USART2_DR;
	

	Hw_Uart_Q_Push( 1, &UartData );

	if( Hw_Uart_Ch[HW_UART_COM2].ISR_FuncPtr != NULL )
	{
		(*Hw_Uart_Ch[HW_UART_COM2].ISR_FuncPtr)(UartData);	
	} 
}



/*---------------------------------------------------------------------------
     TITLE   : Hw_Uart_Init
     WORK    :
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
void Hw_Uart_Init( void )
{
	u32 i;
	
	Hw_Uart_Q_Init();
	
	
	for( i=0; i<HW_UART_MAX_CH; i++ )
	{
		Hw_Uart_Ch[i].Baud 		  = 0;
		Hw_Uart_Ch[i].ISR_FuncPtr = NULL;
	}	
	
	
	Hw_Uart_Open( HW_UART_COM1, 115200, NULL );
	Hw_Uart_Open( HW_UART_COM2, 115200, NULL );	
}





/*---------------------------------------------------------------------------
     TITLE   : Hw_Uart_DeInit
     WORK    :
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
void Hw_Uart_DeInit( void )
{
	//-- COM1
	CLR_BIT( REG_RCC_APB2ENR, 14 );				// USART1 Clock Disable
	CLR_BIT( REG_NVIC_ISER( 37/32 ), 37%32 );	// NVIC 37번 USART1 인터럽트 비활성화
	CLR_BIT( REG_USART1_CR1, 5 );				// RX 인터럽트 비활성화
	CLR_BIT( REG_USART1_CR1, 13 );	    		// USART Disable

	//-- COM2
	CLR_BIT( REG_RCC_APB1ENR, 17 );				// USART2 Clock Disable
	CLR_BIT( REG_NVIC_ISER( 38/32 ), 38%32 );	// NVIC 37번 USART1 인터럽트 비활성화
	CLR_BIT( REG_USART2_CR1, 5 );				// RX 인터럽트 비활성화	
	CLR_BIT( REG_USART2_CR1, 13 );	    		// USART Disable
}





/*---------------------------------------------------------------------------
     TITLE   : Hw_Uart_Open
     WORK    :
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
void Hw_Uart_Open( u8 Ch, u32 BaudData, void (*ISR_FuncPtr)(char Ch) )
{	
	switch( Ch )
	{
		case HW_UART_COM1:
			Hw_Uart_Open_COM1( BaudData, ISR_FuncPtr );
			break;
			
		case HW_UART_COM2:
			Hw_Uart_Open_COM2( BaudData, ISR_FuncPtr );
			break;
			
		case HW_UART_COM3:
			break;

		case HW_UART_COM4:
			break;

		case HW_UART_COM5:
			break;			
	}
}





/*---------------------------------------------------------------------------
     TITLE   : Hw_Uart_Open_COM1
     WORK    :
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
void Hw_Uart_Open_COM1( u32 BaudData, void (*ISR_FuncPtr)(char Ch) )
{	

	Hw_Uart_Ch[HW_UART_COM1].Baud 		 = BaudData;
	Hw_Uart_Ch[HW_UART_COM1].ISR_FuncPtr = ISR_FuncPtr;
	
	
	//-- Clock Enable
	//
	SET_BIT( REG_RCC_APB2ENR, 14 );		// USART1 Clock Enable
	
	
	//-- I/O Port 설정
	//
	
	
	//-- USART1 설정
	//	
	REG_USART1_CR1  = 0 
					| ( 0 << 12 )		// 1 Start bit, 8 Data bits, n Stop bit
					| ( 0 << 10 )		// Parity control disabled
					| ( 1 <<  3 )		// Transmitter Enable
					| ( 1 <<  2 );		// Receiver Enable					
	REG_USART1_CR2  = ( 0 << 12 );		// 1 stop bit	
	REG_USART1_CR3  = 0;
	
	
	//-- 115200bps로 통신 속도 설정
	//
	REG_USART1_BRR  = 0
					| ( 39 << 4 )		// DIV_Mantissa
					| (  1 << 0 );		// DIV Fraction
	

	//-- 인터럽트 설정
	//
	SET_BIT( REG_NVIC_ISER( 37/32 ), 37%32 );	// NVIC 37번 USART1 인터럽트 활성화
	SET_BIT( REG_USART1_CR1, 5 );				// RX 인터럽트 활성화
	
	Hw_ISR_SetIRQFunc( 37, (u32)Hw_ISR_UART1_Handler, 0 );
	
	REG_USART1_CR1 |= ( 1 << 13 );	    // USART Enable
}





/*---------------------------------------------------------------------------
     TITLE   : Hw_Uart_Open_COM2
     WORK    :
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
void Hw_Uart_Open_COM2( u32 BaudData, void (*ISR_FuncPtr)(char Ch) )
{	

	Hw_Uart_Ch[HW_UART_COM2].Baud 		 = BaudData;
	Hw_Uart_Ch[HW_UART_COM2].ISR_FuncPtr = ISR_FuncPtr;
	
	
	//-- Clock Enable
	//
	SET_BIT( REG_RCC_APB1ENR, 17 );		// USART2 Clock Enable	PCLK2 = 72Mhz
	
	
	//-- I/O Port 설정
	//
	
	// PA2 - TX
	REG_GPIOA_CRL &= ~(0x0F << ( 8));	// Clear
	REG_GPIOA_CRL |=  (0x03 << ( 8));	// MODE   - Output mode, max speed 50Mhz
	REG_GPIOA_CRL |=  (0x02 << (10));	// CNF    - Alternate function output Push-pull
										     	    	
	// PA3 - RX
	REG_GPIOA_CRL &= ~(0x0F << (12));	// Clear
	REG_GPIOA_CRL |=  (0x00 << (12));	// MODE   - Input mode
	REG_GPIOA_CRL |=  (0x01 << (14));	// CNF    - Floating input	
	

	//-- USART2 설정
	//	
	REG_USART2_CR1  = 0 
					| ( 0 << 12 )		// 1 Start bit, 8 Data bits, n Stop bit
					| ( 0 << 10 )		// Parity control disabled
					| ( 1 <<  3 )		// Transmitter Enable
					| ( 1 <<  2 );		// Receiver Enable					
	REG_USART2_CR2  = ( 0 << 12 );		// 1 stop bit	
	REG_USART2_CR3  = 0;
	
	
	//-- 115200bps로 통신 속도 설정
	//
	REG_USART2_BRR  = 0
					| ( 19 << 4 )		// DIV_Mantissa
					| (  8 << 0 );		// DIV Fraction
	

	//-- 인터럽트 설정
	//
	SET_BIT( REG_NVIC_ISER( 38/32 ), 38%32 );	// NVIC 37번 USART1 인터럽트 활성화
	SET_BIT( REG_USART2_CR1, 5 );				// RX 인터럽트 활성화
	
	Hw_ISR_SetIRQFunc( 38, (u32)Hw_ISR_UART2_Handler, 0 );
	
	REG_USART2_CR1 |= ( 1 << 13 );	    // USART Enable
}





/*---------------------------------------------------------------------------
     TITLE	: Hw_Uart_SetReceiveFuncISR
     WORK
			: 시리얼 포트에서 데이터 수신시 실행할 함수 설정 .
     ARG
     RET
---------------------------------------------------------------------------*/
void Hw_Uart_SetReceiveFuncISR( u8 Ch, void (*ISR_FuncPtr)(char Ch) )
{
	if( Ch == HW_UART_VCOM )
	{
		Hw_VCom_SetReceiveFuncISR( ISR_FuncPtr );
	}
	else
	{
		Hw_Uart_Ch[Ch].ISR_FuncPtr = ISR_FuncPtr;			
	}
}





/*---------------------------------------------------------------------------
     TITLE	: Uart_Getch
     WORK
			: 시리얼 포트에서 문자 1바이트 읽는다.
     ARG
     RET
---------------------------------------------------------------------------*/
u8 Hw_Uart_Getch( u8 Ch )
{
	u8 Uart_GetData = 0;

	if( Ch == HW_UART_VCOM )
	{
		return Hw_VCom_Getch();
	}

	while( 1 )
	{
		// 버퍼에 데이터 들어올때까지 기다린다.
		if( HW_UART_Q_VAILD(Ch) > 0 ) break;
	}

	Hw_Uart_Q_Pop( Ch, &Uart_GetData );

	return Uart_GetData;
}





/*---------------------------------------------------------------------------
     TITLE	: Uart_Putch
     WORK
			: 시리얼 포트로 문자 1바이트 전송
     ARG
     RET
---------------------------------------------------------------------------*/
void Hw_Uart_Putch( u8 Ch,  char Uart_PutData )
{
	switch( Ch )
	{
		case HW_UART_COM1:
		    while( !(REG_USART1_SR & 0x80) );
    		REG_USART1_DR = Uart_PutData;
			break;
			
		case HW_UART_COM2:
		    while( !(REG_USART2_SR & 0x80) );
    		REG_USART2_DR = Uart_PutData;
			break;
			
		case HW_UART_COM3:
			break;

		case HW_UART_COM4:
			break;

		case HW_UART_COM5:
			break;		

		case HW_UART_VCOM:
			Hw_VCom_Putch( Uart_PutData );
			break;	
	}	
}





/*---------------------------------------------------------------------------
     TITLE	: Uart_PrintEx
     WORK
     ARG
     RET
---------------------------------------------------------------------------*/
void Hw_Uart_PrintEx( u8 Ch, char *UartPrintData )
{
	while( *UartPrintData != '\0' )
	{
		Hw_Uart_Putch( Ch, *UartPrintData );
		UartPrintData++;
	}        
}




/*---------------------------------------------------------------------------
     TITLE	: Uart_Printf
     WORK
     ARG
     RET
---------------------------------------------------------------------------*/
void Hw_Uart_Printf( u8 Ch,  char *format, ... )
{
	char Str[200];
	
	Lb_va_list ap;
	
	Lb_va_start( ap, format );

	//vsprintf( Str, format, ap );	

	Lb_vsprintf(Str, format, ap );
	
	Lb_va_end(ap);	
	
	
	
	
	Hw_Uart_PrintEx( Ch, Str );
}




/*---------------------------------------------------------------------------
     TITLE	: Uart_GetchNoWait
     WORK
     ARG
     RET
---------------------------------------------------------------------------*/
u8 Hw_Uart_GetchNoWait( u8 Ch, u8 *cReturn )
{
	// 버퍼에 데이터 없으면 실패로 종료.
	if( HW_UART_Q_VAILD(Ch) == 0 )
	{
		return FALSE;
	}
		   
	Hw_Uart_Q_Pop( Ch, cReturn );
	
	return TRUE;
}







//------------------------------- printf 포팅용 --------------------------------
//
void print_byte(unsigned int c)
{
#if HW_USE_USB_VCOM == 1
	if (c == '\n') Hw_VCom_Putch('\r');
    
    Hw_VCom_Putch(c);
#else	
	if (c == '\n') Hw_Uart_Putch(HW_USE_UART_CH_MENU, '\r');
    
    Hw_Uart_Putch(HW_USE_UART_CH_MENU, c);
#endif    
}


char get_byte(void)
{
	char Ch;
	
#if HW_USE_USB_VCOM == 1	
	Ch = Hw_VCom_Getch();
#else	
	Ch = Hw_Uart_Getch(HW_USE_UART_CH_MENU);
#endif
	
	return Ch;
}






