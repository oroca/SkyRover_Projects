//----------------------------------------------------------------------------
//    프로그램명 : SPI 관련 함수
//
//    만든이     : Cho Han Cheol 
//
//    날  짜     : 
//    
//    최종 수정  : 
//
//    MPU_Type   : 
//
//    파일명     : Hw_SPI.c
//----------------------------------------------------------------------------



//----- 헤더파일 열기
//
#define  HW_SPI_LOCAL  

#include "Hw_SPI.h"





//----- 내부 함수
void Hw_SPI_Ch1_Init(void);
void Hw_SPI_Ch2_Init(void);



//----- 내부 변수



/*---------------------------------------------------------------------------
     TITLE   : Hw_SPI_Init
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
void Hw_SPI_Init(void)
{
	static u8 Called = 0;


	if( Called == 1 ) return;


	// 1. Clock Enable
	//
	REG_RCC_APB1ENR |= (1<<14);		// SPI2 Clock enable	APB1 : 36Mhz
	
	Hw_SPI_Ch1_Init();
	Hw_SPI_Ch2_Init();

	Called = 1;	
}





/*---------------------------------------------------------------------------
     TITLE   : Hw_SPI_Ch1_Init
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
void Hw_SPI_Ch1_Init(void)
{
}





/*---------------------------------------------------------------------------
     TITLE   : Hw_SPI_Ch2_Init
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
void Hw_SPI_Ch2_Init(void)
{
	u16 i;

	// 1. GPIO Setup
	//
	
	// PB12 - SPI2_NSS
	REG_GPIOB_CRH &= ~(0x0F << (16));	// Clear
	REG_GPIOB_CRH |=  (0x03 << (16));	// MODE   - Output mode, max speed 50Mhz
	REG_GPIOB_CRH |=  (0x02 << (18));	// CNF    - Alternate function output Push-pull
										     	    	
	// PB13 - SPI2_SCK
	REG_GPIOB_CRH &= ~(0x0F << (20));	// Clear
	REG_GPIOB_CRH |=  (0x03 << (20));	// MODE   - Output mode, max speed 50Mhz
	REG_GPIOB_CRH |=  (0x02 << (22));	// CNF    - Alternate function output Push-pull

	// PB15 - SPI2_MOSI
	REG_GPIOB_CRH &= ~(0x0F << (28));	// Clear
	REG_GPIOB_CRH |=  (0x03 << (28));	// MODE   - Output mode, max speed 50Mhz
	REG_GPIOB_CRH |=  (0x02 << (30));	// CNF    - Alternate function output Push-pull



	// 2. SPI 레지스터 설정 - 8bit, transmit-only mode, 2Mhz
	//
	REG_SPI2_CR1 = 0
        | ( 0 << 15 )   // BIDMODE   - Bidirectional data mode enable
        				//             0 : 2-line unidirectional data mode selected
		                //             1 : 1-line bidirectional data mode selected
        | ( 1 << 14 )   // BIDIOE    - Output enable in bidirectional mode
        				//             0 : Output disabled ( receive-only mode )
        				//             1 : Output enabled  (transmit-only mode )
        | ( 0 << 13 )   // CRCEN     - Hardware CRC calculation enable
        				//             0 : Disabled
        				//             1 : Enabled
        | ( 0 << 12 )   // CRCNEXT
        | ( 0 << 11 )   // DFF       - Data frame format
        				//             0 :  8 bit
        				//             1 : 16 bit
        | ( 0 << 10 )   // RXONLY    - Receive only
        				//             0 : Full duplex
        				//             1 : Output disabled
        | ( 0 <<  9 )   // SSM		 - Software slave managemnet
        				//             0 : Disabled
        				//             1 : Enabled
        | ( 0 <<  8 )   // SSI       - Internal slave select
        | ( 0 <<  7 )   // LSBFIRST  - Frame format
        				//             0 : MSB transmitted first
        				//             1 : LSB transmitted first
		| ( 0 <<  6 )   // SPE       - SPI Enable
		                //             0 : Peripheral disabled
						//             1 : Peripheral enabled
        | ( 3 <<  3 )   // BR        - Baud rate control  36Mhz/16 = 2Mhz 
		                //             0 : fPCLK/2
						//             1 : fPCLK/4
						//             2 : fPCLK/8
						//             3 : fPCLK/16   
		| ( 1 <<  2 )   // MSTR      - Master selection
		                //             0 : Slave
						//             1 : Master
        | ( 0 <<  1 )   // CPOL      - Clock polarity
        				//             0 : CK to 0 when idle
        				//             1 : CK to 1 when idle  
        | ( 0 <<  0 );  // CPHA      - Clock phase
        				//             0 : the first clock transition is the first data capture edge
        				//             1 : the second clock transition is the first data capture edge


	REG_SPI2_CR2 = 0
        | ( 1 <<  2 );  // SSOE      - SS output enable
        				//             0 : Disabled
		                //             1 : Enabled

	Hw_SPI_Enable(HW_SPI_CH2);	
}





/*---------------------------------------------------------------------------
     TITLE   : Hw_SPI_Enable

     WORK    : 

     ARG     : void
              
     RET     : void
---------------------------------------------------------------------------*/
void Hw_SPI_Enable( u8 Ch )
{
	switch( Ch )
	{
		case HW_SPI_CH1:
			break;
			
		case HW_SPI_CH2:
			SET_BIT( REG_SPI2_CR1, 6 );
			break;
			
		case HW_SPI_CH3:
			break;
	}
}





/*---------------------------------------------------------------------------
     TITLE   : Hw_SPI_StartDMA

     WORK    : 

     ARG     : void
              
     RET     : void
---------------------------------------------------------------------------*/
void Hw_SPI_StartDMA( u8 Ch )
{
	switch( Ch )
	{
		case HW_SPI_CH1:
			break;
			
		case HW_SPI_CH2:
			SET_BIT( REG_SPI2_CR2, 1 );	// TX DMA Enable
			SET_BIT( REG_SPI2_CR2, 0 );	// RX DMA Enable
			break;
			
		case HW_SPI_CH3:
			break;
	}
}





/*---------------------------------------------------------------------------
     TITLE   : Hw_SPI_StopDMA
     WORK    : 
     ARG     : void             
     RET     : void
---------------------------------------------------------------------------*/
void Hw_SPI_StopDMA( u8 Ch )
{
	switch( Ch )
	{
		case HW_SPI_CH1:
			break;
			
		case HW_SPI_CH2:
			CLR_BIT( REG_SPI2_CR2, 1 );	// TX DMA Disable
			CLR_BIT( REG_SPI2_CR2, 0 );	// RX DMA Disable
			break;
			
		case HW_SPI_CH3:
			break;
	}
}





/*---------------------------------------------------------------------------
     TITLE   : Hw_SPI_Disable
     WORK    : 
     ARG     : void             
     RET     : void
---------------------------------------------------------------------------*/
void Hw_SPI_Disable( u8 Ch )
{
}




/*---------------------------------------------------------------------------
     TITLE   : Hw_SPI_Write8Bit
     WORK    : 
     ARG     : void           
     RET     : void
---------------------------------------------------------------------------*/
u16 Hw_SPI_Write8Bit( u8 Ch, u8 WriteData )
{
	u32 TimeOut;
	u32 ReadData;

	switch( Ch )
	{
		case HW_SPI_CH1:
			break;
			
		case HW_SPI_CH2:
			TimeOut = 10000;	
			while( TimeOut-- )
			{
				if( (REG_SPI2_SR & (1<<1)) ) break;
			}
			if( TimeOut == 0 ) return 1;
			REG_SPI2_DR = WriteData;

			
			TimeOut = 10000;	
			while( TimeOut-- )
			{
				if( (REG_SPI2_SR & (1<<0)) ) break;
			}
			if( TimeOut == 0 ) return 1;
		
			ReadData = REG_SPI2_DR;
			
			break;
			
		case HW_SPI_CH3:
			break;
	}
	

    return 0;
}





/*---------------------------------------------------------------------------
     TITLE   : Hw_SPI_Write16Bit
     WORK    : 
     ARG     : void             
     RET     : void
---------------------------------------------------------------------------*/
u16 Hw_SPI_Write16Bit( u8 Ch, u16 WriteData )
{
	u32 TimeOut;
	u32 ReadData;

	switch( Ch )
	{
		case HW_SPI_CH1:
			break;
			
		case HW_SPI_CH2:
			TimeOut = 10000;	
			while( TimeOut-- )
			{
				if( (REG_SPI2_SR & (1<<1)) ) break;
			}
			if( TimeOut == 0 ) return 1;
		
			REG_SPI2_DR = WriteData;

			TimeOut = 10000;	
			while( TimeOut-- )
			{
				if( (REG_SPI2_SR & (1<<0)) ) break;
			}
			if( TimeOut == 0 ) return 1;
		
			ReadData = REG_SPI2_DR;			
			break;
			
		case HW_SPI_CH3:
			break;
	}
	

    return 0;
}