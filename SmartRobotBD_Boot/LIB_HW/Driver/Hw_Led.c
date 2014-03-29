//----------------------------------------------------------------------------
//    프로그램명 	: LED 관련 함수
//
//    만든이     	: 
//
//    날  짜     	: 
//    
//    최종 수정  	:
//
//    MPU_Type		:
//
//    파일명     	: Hw_Led.c
//----------------------------------------------------------------------------


//----- 헤더파일 열기
//
#define  HW_LED_LOCAL


#include "Hw_Led.h"





/*---------------------------------------------------------------------------
     TITLE   : Hw_Led_Init
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
void Hw_Led_Init( void )
{
	u32 PinNum;

	//-- JTAG 핀을 Disable하고 PB3,4를 GPIO로 사용한다.(핀구성에 따라 사용하지 않을수 있음)
	//
	REG_AFIO_MAPR &= ~(0x07 << 24);
	REG_AFIO_MAPR |=  (0x04 << 24);
	
	
	
	PinNum = 4;
	REG_GPIOB_CRL &= ~(0x0F << (PinNum*4+0));	// Clear
	REG_GPIOB_CRL |=  (0x03 << (PinNum*4+0));	// MODE, PB.3 Output mode, max speed 50Mhz
	REG_GPIOB_CRL |=  (0x00 << (PinNum*4+2));	// CNF,  PB.3 General purpose output push-pul
	
	PinNum = 5;
	REG_GPIOB_CRL &= ~(0x0F << (PinNum*5+0));	// Clear
	REG_GPIOB_CRL |=  (0x03 << (PinNum*5+0));	// MODE, PB.3 Output mode, max speed 50Mhz
	REG_GPIOB_CRL |=  (0x00 << (PinNum*5+2));	// CNF,  PB.3 General purpose output push-pul

	Hw_Led_Off(0);
	Hw_Led_Off(1);	
	
}





/*---------------------------------------------------------------------------
     TITLE   : Hw_Led_On
     WORK    :
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
void Hw_Led_On( u8 Ch )
{	
	switch( Ch )
	{
		case 0:
			CLR_BIT( REG_GPIOB_ODR, 4 );
			break;

		case 1:
			CLR_BIT( REG_GPIOB_ODR, 5 );
			break;
	}
}





/*---------------------------------------------------------------------------
     TITLE   : Hw_Led_On
     WORK    :
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
void Hw_Led_Off( u8 Ch )
{
	switch( Ch )
	{
		case 0:
			SET_BIT( REG_GPIOB_ODR, 4 );
			break;

		case 1:
			SET_BIT( REG_GPIOB_ODR, 5 );
			break;
	}
}





/*---------------------------------------------------------------------------
     TITLE   : Hw_Led_On
     WORK    :
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
void Hw_Led_Toggle( u8 Ch )
{
	switch( Ch )
	{
		case 0:
			TGL_BIT( REG_GPIOB_ODR, 4 );
			break;

		case 1:
			TGL_BIT( REG_GPIOB_ODR, 5 );
			break;
	}
}





/*---------------------------------------------------------------------------
     TITLE   : Hw_Led_Wait
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
void Hw_Led_Wait( u32 delay )
{
    volatile u32 i;
    for ( i = 0 ; i < delay ; i++ ){ };
}



















 
