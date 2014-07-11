/******************************************************************************
 * @file      start_stm32f10x.c
 * @author    SmartRobot : e2g1234@naver.com
 * @version   V0.1
 * @date      04/15/2013
 *******************************************************************************/

//----- 헤더파일 열기
//
#define  HW_STARTUP_LOCAL

#include "Hw_StartUp.h"





//-- Private typedef
//
typedef void( *const intfunc )( void );


//-- init value for the stack pointer. defined in linker script 
//
extern unsigned long _estack;
extern unsigned long _sidata;    /*!< Start address for the initialization 
                                      values of the .data section.            */
extern unsigned long _sdata;     /*!< Start address for the .data section     */    
extern unsigned long _edata;     /*!< End address for the .data section       */    
extern unsigned long _sbss;      /*!< Start address for the .bss section      */
extern unsigned long _ebss;      /*!< End address for the .bss section        */      
extern void 	     _eram;      /*!< End address for ram                     */


//-- Private function prototypes
//
void Hw_StartUp_Init(void) __attribute__((__interrupt__));





__attribute__ ((section(".isr_vectorsflash")))    //linker scriptor에서 사용됨
//-- vector table
//
void (* g_pfnVectors[])(void) =
{
    (intfunc)((unsigned long)&_estack),		// The stack pointer after relocation
    Hw_StartUp_Init,                      	// Reset Handler
};



extern int main();


/*---------------------------------------------------------------------------
     TITLE   : 	Hw_StartUp_Init
     WORK    :	 
     			보드 리셋시에 처음 실행되는 함수
     ARG     : 	void
     RET     : 	void
---------------------------------------------------------------------------*/
void Hw_StartUp_Init(void)
{
	
	u32 TimeOutCnt  = 0;
	u32 HSEStatus 	= 0;
	unsigned long *pulSrc, *pulDest;	
	

	//-- Initialize data and bss
	//  
	
	// Copy the data segment initializers from flash to SRAM
	pulSrc = &_sidata;

	for(pulDest = &_sdata; pulDest < &_edata; )
	{
		*(pulDest++) = *(pulSrc++);
	}
  
	/* Zero fill the bss segment.  This is done with inline assembly since this
	   will clear the value of pulDest if it is not kept in a register. */
	__asm("  ldr     r0, =_sbss\n"
          "  ldr     r1, =_ebss\n"
          "  mov     r2, #0\n"
          "  .thumb_func\n"
          "zero_loop:\n"
          "    cmp     r0, r1\n"
          "    it      lt\n"
          "    strlt   r2, [r0], #4\n"
          "    blt     zero_loop");
    

#if 1
    /* Reset the RCC clock configuration to the default reset state(for debug purpose) */
    /* Set HSION bit */
    REG_RCC_CR |= (uint32_t) 0x00000001;

    /* Reset SW, HPRE, PPRE1, PPRE2, ADCPRE and MCO bits */
    REG_RCC_CFGR &= (uint32_t) 0xF8FF0000;

    /* Reset HSEON, CSSON and PLLON bits */
    REG_RCC_CR &= (uint32_t) 0xFEF6FFFF;

    /* Reset HSEBYP bit */
    REG_RCC_CR &= (uint32_t) 0xFFFBFFFF;

    /* Reset PLLSRC, PLLXTPRE, PLLMUL and USBPRE/OTGFSPRE bits */
    REG_RCC_CFGR &= (uint32_t) 0xFF80FFFF;

    /* Disable all interrupts and clear pending bits  */
    REG_RCC_CIR = 0x009F0000;

    REG_SCB_VTOR = (uint32_t)0x08000000;     /* Vector Table Relocation in Internal FLASH. */
#endif


  REG_RCC_APB1RSTR = 0xFFFFFFFF;
  REG_RCC_APB2RSTR = 0xFFFFFFFF;
  REG_RCC_APB1RSTR = 0x00000000;
  REG_RCC_APB2RSTR = 0x00000000;

	
    // HSI or HSE 선택
            
    //-- CR_HSEON_Set
    //
	REG_RCC_CR  |= (1<<16);	
		
	//-- 설정된 clock 이 설정 되었는지 확인
	//
	do
	{	
		HSEStatus = REG_RCC_CR & (1<<17);	// HSE 인경우
		TimeOutCnt++;		
	} while( (HSEStatus == 0) && (TimeOutCnt < 11280) );
  

	//-- FLASH Access 타이밍 설정
	//
	REG_FLASH_ACR |=  (1<<4);	// Prefetch buffer enable
	REG_FLASH_ACR &= ~(3<<0);	// LATENCY Clear
	REG_FLASH_ACR |=  (2<<0);	// LATENCY = two wait


    main();

}
