//----------------------------------------------------------------------------
//    프로그램명 	: PLL 관련 함수
//
//    만든이     	: 
//
//    날  짜     	: 2013. 8.20.
//    
//    최종 수정  	:
//
//    MPU_Type		:
//
//    파일명     	: Hw_PLL.c
//----------------------------------------------------------------------------


//----- 헤더파일 열기
//
#define  HW_PLL_LOCAL


#include "Hw_PLL.h"



#define CLOCK_EXT		1		// 클럭을 외부로 사용하면 1



/*---------------------------------------------------------------------------
     TITLE   : Hw_PLL_Init
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
void Hw_PLL_Init( void )
{
	
#if	CLOCK_EXT == 1
	//-- PLL 설정 72Mhz로 설정함
	//
	REG_RCC_CFGR &= ~(0xF<<18 | 0x1<<17 | 0x1<<16 | 0xF<<4);
	REG_RCC_CFGR |=  (0x7<<18 | 0x1<<16);			// 8MHz
    REG_RCC_CR   |=  0x01 << 24;                    // PLLON

	while( (REG_RCC_CR & (1<<25)) == 0 );			// PLLRDY

#else	
	//-- PLL 설정 내부 36MHz 설정함
	//
	REG_RCC_CFGR |= (0x7<<18);              		// 내부 4MHz
	REG_RCC_CFGR |= (0x4<<18 | 0x1<<16);			// 12MHz
	REG_RCC_CR   |=  0x01<<24;						// PLLON
	while( (REG_RCC_CR & (1<<25)) == 0 );			// PLLRDY
#endif

    
    /////PLL을 선택한다.
    /////Bits 1:0 SW : System clock switch
    //00: HSI selected as system clock
    //01: HSE selected as system clock
    //10: PLL selected as system clock   <----------------- 선택됨
    //11: not allowed
	REG_RCC_CFGR &= ~0x3;
	REG_RCC_CFGR |=  0x2;

	// USB 72M * 2 / 3 = 48Mhz
	//
	CLR_BIT( REG_RCC_CFGR, 22 );

	// APB1 - PCLK1 36Mhz
	//
	REG_RCC_CFGR &= ~(0x7<<8);
	REG_RCC_CFGR |=  (0x4<<8);
	
	// APB2 - PCLK2 72Mhz
	//
	REG_RCC_CFGR &= ~(0x7<<11);
	REG_RCC_CFGR |=  (0x0<<11);

    
    while( (REG_RCC_CFGR & 0xC) != 0x08 );
    
    //APB2에 있는 UART 등 주변장치에 Clock 소스 동작제어
    //UART가 동작되기 위해서는 GPIO와 Alternation Function IO가 동작상태에 있어야 함
	REG_RCC_APB2ENR |= 0  |
				0x1 << 14 |	// USART1 
				0x1 <<  4 | // IOPC
				0x1 <<  3 | // IOPB
				0x1 <<  2 | // IOPA
				0x1 <<  0;  // AFIO	

					
}