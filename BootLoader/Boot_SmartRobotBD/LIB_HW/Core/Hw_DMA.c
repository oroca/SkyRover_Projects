//----------------------------------------------------------------------------
//    프로그램명 : DMA 관련 함수
//
//    만든이     : Cho Han Cheol 
//
//    날  짜     : 
//    
//    최종 수정  : 
//
//    MPU_Type   : 
//
//    파일명     : Hw_DMA.c
//----------------------------------------------------------------------------



//----- 헤더파일 열기
//
#define  HW_DMA_LOCAL  

#include "Hw_DMA.h"





//----- 내부 함수




//----- 내부 변수



/*---------------------------------------------------------------------------
     TITLE   : Hw_DMA_Init
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
void Hw_DMA_Init(void)
{
	static u8 Called = 0;


	if( Called == 1 ) return;


	// 1. Clock Enable
	//
	REG_RCC_AHBENR |= (1<<0);		// DMA1 Clock enable
	

	Called = 1;	
}





/*---------------------------------------------------------------------------
     TITLE   : Hw_DMA_SPI2_Setup
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
void Hw_DMA_SPI2_Setup( u32 WriteAddress, u32 ReadAddress, u32 BufSize )
{
	
	//-- DMA1 Ch4 - SPI2_RX
	//   
	REG_DMA1_CCR(4)   = 0;
	REG_DMA1_CPAR(4)  = (u32)&REG_SPI2_DR;	 
	REG_DMA1_CMAR(4)  = ReadAddress;
	REG_DMA1_CNDTR(4) = BufSize;
	
	
	
	REG_DMA1_CCR(4) = 0
        | ( 0 << 14 )   // MEM2MEM   - Memory to memory mode
        				//             0 : Memory to memory mode disabled
        				//             1 : Memory to memory mode enabled
        | ( 1 << 12 )   // PL        - Channel priority level
        				//             00 : Low
        				//             01 : Medium
        				//             10 : High
        				//             11 : Very High
        | ( 0 << 10 )   // MSIZE     - Memory Size
        				//             00 : 8bits
        				//             01 : 16
        				//             10 : 32
        				//             11 : Reserved
        | ( 0 <<  8 )   // PSIZE     - Peripheral size
        				//             00 : 8bits
        				//             01 : 16
        				//             10 : 32
        				//             11 : Reserved
        | ( 1 <<  7 )   // MINC      - Memory increment mode
        				//             0 : Disabled
        				//             1 : Enabled
        | ( 0 <<  6 )   // PINC		 - Peripheral increment mode
        				//             0 : Disabled
        				//             1 : Enabled
        | ( 0 <<  5 )   // CIRC      - Circular mode
        				//             0 : Disabled
        				//             1 : Enabled
        | ( 0 <<  4 )   // DIR       - Data transfer direction
        				//             0 : Read from peripheral
        				//             1 : Read from memory
		| ( 0 <<  3 )   // TEIE      - Transfer error interrupt enable
		                //             0 : Disabled
						//             1 : Enabled
        | ( 0 <<  2 )   // HTIE      - Half transfer interrupt enable 
		                //             0 : Disabled
						//             1 : Enabled
		| ( 0 <<  1 )   // TCIE      - Transfer complete interrupt enable
		                //             0 : Disabled
						//             1 : Enabled
        | ( 0 <<  0 );  // EN        - Channel enable
        				//             0 : Disabled
        				//             1 : Enabled
        				
    REG_DMA1_CCR(4) |= (1<<0);	// Channel enable
        
        
	//-- DMA1 Ch5 - SPI2_TX
	//   
	REG_DMA1_CCR(5)   = 0;
	REG_DMA1_CPAR(5)  = (u32)&REG_SPI2_DR;	 
	REG_DMA1_CMAR(5)  = WriteAddress;
	REG_DMA1_CNDTR(5) = BufSize;
	
	
	
	REG_DMA1_CCR(5) = 0
        | ( 0 << 14 )   // MEM2MEM   - Memory to memory mode
        				//             0 : Memory to memory mode disabled
        				//             1 : Memory to memory mode enabled
        | ( 0 << 12 )   // PL        - Channel priority level
        				//             00 : Low
        				//             01 : Medium
        				//             10 : High
        				//             11 : Very High
        | ( 0 << 10 )   // MSIZE     - Memory Size
        				//             00 : 8bits
        				//             01 : 16
        				//             10 : 32
        				//             11 : Reserved
        | ( 0 <<  8 )   // PSIZE     - Peripheral size
        				//             00 : 8bits
        				//             01 : 16
        				//             10 : 32
        				//             11 : Reserved
        | ( 1 <<  7 )   // MINC      - Memory increment mode
        				//             0 : Disabled
        				//             1 : Enabled
        | ( 0 <<  6 )   // PINC		 - Peripheral increment mode
        				//             0 : Disabled
        				//             1 : Enabled
        | ( 0 <<  5 )   // CIRC      - Circular mode
        				//             0 : Disabled
        				//             1 : Enabled
        | ( 1 <<  4 )   // DIR       - Data transfer direction
        				//             0 : Read from peripheral
        				//             1 : Read from memory
		| ( 0 <<  3 )   // TEIE      - Transfer error interrupt enable
		                //             0 : Disabled
						//             1 : Enabled
        | ( 0 <<  2 )   // HTIE      - Half transfer interrupt enable 
		                //             0 : Disabled
						//             1 : Enabled
		| ( 0 <<  1 )   // TCIE      - Transfer complete interrupt enable
		                //             0 : Disabled
						//             1 : Enabled
        | ( 0 <<  0 );  // EN        - Channel enable
        				//             0 : Disabled
        				//             1 : Enabled
        				
    REG_DMA1_CCR(5) |= (1<<0);	// Channel enable
}





/*---------------------------------------------------------------------------
     TITLE   : Hw_DMA_SPI2_IsTransFinished
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
u8 Hw_DMA_SPI2_IsTransFinished( void )
{
	u8 Ret = 0;
	
	if( REG_DMA1_ISR & (1<<13) )
	{
		if( REG_DMA1_ISR & (1<<17) )
		{
			REG_DMA1_IFCR = (1<<17) | (1<<13);
			Ret = 1;
		}
	}
	return Ret;
}