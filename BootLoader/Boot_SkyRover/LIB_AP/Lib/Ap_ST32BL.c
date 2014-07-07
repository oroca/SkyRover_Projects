//----------------------------------------------------------------------------
//    프로그램명 	: ST32BL 관련 함수
//
//    만든이     	: 
//
//    날  짜     	: 
//    
//    최종 수정  	: 
//
//    MPU_Type 		:
//
//    파일명     	: AP_ST32BL.c
//----------------------------------------------------------------------------





//----- 헤더파일 열기
//
#define  AP_ST32BL_LOCAL  

#include "Ap_ST32BL.h"
#include "stm32f10x_flash.h"

#include <ctype.h>
#include <string.h>


//-- 내부 선언
//

#define HW_ST32BL_USE_DEBUG				0



#define HW_ST32BL_FLASH_START_ADDRESS 	((u32)0x08003000)
#define HW_ST32BL_FLASH_PAGE_SIZE    	((u16)0x00000400)
#define HW_ST32BL_FLASH_END_ADDRESS		((u32)0x08020000)
//#define HW_ST32BL_FLASH_END_ADDRESS	((u32)0x0800FFFF)

#define HW_ST32BL_FLASH_ADDRESS_OFFSET 	((u32)0x0003000)


#define HW_STM32_WRITE_BUFSIZE			256


//-- 내부 변수
//

u8	Ap_ST32BL_EraseSectorFlag[128];


typedef  void (*pFunction)(void);



void Ap_ST32BL_SendByte( u8 ByteData );
u8 	 Ap_ST32BL_WaitCmd( void );
u8   Ap_ST32BL_CheckFlashSector( u32 Address );





/*---------------------------------------------------------------------------
     TITLE   : Ap_ST32BL_Init
     WORK    : 
     ARG     : void
     RET     : void   
---------------------------------------------------------------------------*/
void Ap_ST32BL_Init( void )
{
	u32 i;


	REG_GPIOB_CRL &= ~(0x0F << (0));	// Clear
	REG_GPIOB_CRL |=  (0x00 << (0));	// MODE, PB.0 Input Mode
	REG_GPIOB_CRL |=  (0x02 << (2));	// CNF,  PB.0 input	pull-up/pull-down
		
	CLR_BIT( REG_GPIOB_ODR, 0 );


	if( IS_SET_BIT(REG_GPIOB_IDR, 0) )
	{
		Ap_ST32BL_RunBoot = TRUE;
	}
	else
	{
		Ap_ST32BL_RunBoot = FALSE;
	}

	for( i=0; i<128; i++ )
	{
		Ap_ST32BL_EraseSectorFlag[i] = FALSE;
	}

	Ap_ST32BL_UartPort = HW_UART_COM1;	
}










/*---------------------------------------------------------------------------
     TITLE   : Ap_ST32BL_Main
     WORK    : 
     ARG     : void
     RET     : void   
---------------------------------------------------------------------------*/
void Ap_ST32BL_Main( void )
{
	u8 	Cmd;
	u8 	Resp[300];
	u32 Length;
	u32 Address;

	u32 i;
	u8  CheckSum;
	u8  CheckSumCalc;
	u8  Ret;

	while(1)
	{
		//Hw_Uart_Printf(HW_UART_VCOM, "ST32BL\n");
		//Hw_Timer_Delay_ms(500);

		Cmd = Ap_ST32BL_WaitCmd();

		if( Cmd == STM32_CMD_INIT )
		{

		}


		if( Cmd == STM32_CMD_GET_COMMAND )
		{
			Ap_ST32BL_SendByte(0x00);
			Ap_ST32BL_SendByte(0x22);
			Ap_ST32BL_SendByte(STM32_COMM_ACK);
		}


		if( Cmd == STM32_CMD_GET_ID )
		{
			Ap_ST32BL_SendByte(0x01);
			Ap_ST32BL_SendByte(0x04);
			Ap_ST32BL_SendByte(0x10);
			Ap_ST32BL_SendByte(STM32_COMM_ACK);			
		}


		if( Cmd == STM32_CMD_ERASE_FLASH )
		{
			Resp[0] = Ap_ST32BL_ReceiveByte();
			Resp[1] = Ap_ST32BL_ReceiveByte();


			if( Ap_ST32BL_EraseFlash() == TRUE )	Ap_ST32BL_SendByte(STM32_COMM_ACK);
			else									Ap_ST32BL_SendByte(STM32_COMM_NACK);

			#if HW_ST32BL_USE_DEBUG == 1
			Hw_Uart_Printf(HW_UART_COM1, "Erase %x %x \r\n", Resp[0], Resp[1] );
			#endif			
		}




		if( Cmd == STM32_CMD_WRITE_FLASH )
		{

			//-- Receive Address 
			//
			CheckSumCalc = 0;
			for( i=0; i<4; i++ )
			{
				Resp[i] = Ap_ST32BL_ReceiveByte();
				CheckSumCalc ^= Resp[i];	
			}
			CheckSum = Ap_ST32BL_ReceiveByte();

			Address  = Resp[0] << 24;
			Address |= Resp[1] << 16;
			Address |= Resp[2] <<  8;
			Address |= Resp[3] <<  0;



			#if HW_ST32BL_USE_DEBUG == 1
			Hw_Uart_Printf(HW_UART_COM1, "Addrr %x %x %x %x %x %x\r\n", Resp[0], Resp[1], Resp[2], Resp[3], CheckSum, CheckSumCalc );
			#endif		

			if( CheckSum == CheckSumCalc )
			{
				Ap_ST32BL_SendByte(STM32_COMM_ACK);
				#if HW_ST32BL_USE_DEBUG == 1
				Hw_Uart_Printf(HW_UART_COM1, "ACK\r\n");
				#endif	
			}
			else
			{
				Ap_ST32BL_SendByte(STM32_COMM_NACK);	
				#if HW_ST32BL_USE_DEBUG == 1
				Hw_Uart_Printf(HW_UART_COM1, "NACK\r\n");
				#endif	
			}	


			//-- Receive Data
			//
			CheckSumCalc = 0;
			Length  = Ap_ST32BL_ReceiveByte();
			CheckSumCalc ^= Length;
			Length++;

			#if HW_ST32BL_USE_DEBUG == 1
			Hw_Uart_Printf(HW_UART_COM1, "Length %d \r\n", Length );
			#endif	

			for( i=0; i<Length; i++ )
			{
				Resp[i] = Ap_ST32BL_ReceiveByte();
				CheckSumCalc ^= Resp[i];					
			}
			CheckSum = Ap_ST32BL_ReceiveByte();


			#if HW_ST32BL_USE_DEBUG == 1
			Hw_Uart_Printf(HW_UART_COM1, "Receive Data %d %x %x\r\n", Length, CheckSum, CheckSumCalc );
			#endif		


			//-- Flash Write
			//

			Ret = Ap_ST32BL_WriteFlash( Address + HW_ST32BL_FLASH_ADDRESS_OFFSET, Resp, Length );

		
			if( CheckSum == CheckSumCalc && Ret == TRUE ) 	
			{
				Ap_ST32BL_SendByte(STM32_COMM_ACK);
			}
			else							
			{
				Ap_ST32BL_SendByte(STM32_COMM_NACK);
			}

		}
		

		//-- Go Command
		//
		if( Cmd == STM32_CMD_GO )
		{
			//-- Receive Address 
			//
			CheckSumCalc = 0;
			for( i=0; i<4; i++ )
			{
				Resp[i] = Ap_ST32BL_ReceiveByte();
				CheckSumCalc ^= Resp[i];	
			}
			CheckSum = Ap_ST32BL_ReceiveByte();

			Address  = Resp[0] << 24;
			Address |= Resp[1] << 16;
			Address |= Resp[2] <<  8;
			Address |= Resp[3] <<  0;	
			

			Ap_ST32BL_SendByte(STM32_COMM_ACK);	

			Hw_Timer_Delay_ms(100);

			//-- Go
			//
			Ap_ST32BL_JumpToApp( Address );

					
		}
	}

}





/*---------------------------------------------------------------------------
     TITLE   : Ap_ST32BL_WaitCmd
     WORK    : 
     ARG     : void
     RET     : void   
---------------------------------------------------------------------------*/
u8 Ap_ST32BL_WaitCmd( void )
{
	u8 Ch;
	u8 Cmd = 0xFF;
	u8 CmdC;

	Cmd = Ap_ST32BL_ReceiveByte();

	switch( Cmd )
	{
		case STM32_CMD_INIT:
			Ap_ST32BL_SendByte(STM32_COMM_ACK);
			break;

		case STM32_CMD_GET_COMMAND:
		case STM32_CMD_ERASE_FLASH:
		case STM32_CMD_EXTENDED_ERASE_FLASH:
		case STM32_CMD_GET_ID:
		case STM32_CMD_WRITE_FLASH:
		case STM32_CMD_WRITE_UNPROTECT:
		case STM32_CMD_READ_FLASH:
		case STM32_CMD_GO:
		case 0xFF:
			CmdC = Ap_ST32BL_ReceiveByte();

			Ap_ST32BL_SendByte(STM32_COMM_ACK);				
			break;

		default:
			Ap_ST32BL_SendByte(STM32_COMM_NACK);
			break;
	}


	return Cmd;
}





/*---------------------------------------------------------------------------
     TITLE   : Ap_ST32BL_SendByte
     WORK    : 
     ARG     : void
     RET     : void   
---------------------------------------------------------------------------*/
void Ap_ST32BL_SendByte( u8 ByteData )
{
	Hw_Uart_Putch( Ap_ST32BL_UartPort, ByteData );
}





/*---------------------------------------------------------------------------
     TITLE   : Ap_ST32BL_ReceiveByte
     WORK    : 
     ARG     : void
     RET     : void   
---------------------------------------------------------------------------*/
u8 Ap_ST32BL_ReceiveByte( void )
{
	Hw_Uart_Getch( Ap_ST32BL_UartPort );
}




/*---------------------------------------------------------------------------
     TITLE   : Ap_ST32BL_EraseFlash
     WORK    : 
     ARG     : void
     RET     : void   
---------------------------------------------------------------------------*/
u8 Ap_ST32BL_EraseFlash( void )
{
	u8  Ret = TRUE;
	u32 PageCount;
	u32 i;

	
	volatile FLASH_Status FLASHStatus;

	FLASHStatus = FLASH_COMPLETE;

	FLASH_Unlock();
	FLASH_ClearFlag(FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);


	for( i=0; i<128; i++ )
	{
		Ap_ST32BL_EraseSectorFlag[i] = FALSE;
	}

	PageCount = 0;


	FLASHStatus = FLASH_ErasePage(HW_ST32BL_FLASH_START_ADDRESS);

	Ap_ST32BL_EraseSectorFlag[PageCount] = TRUE;

	if( FLASHStatus != FLASH_COMPLETE )
	{
		FLASH_Lock();
		Ret = FALSE;
	}

	return Ret;
}

#if 0
u8 Ap_ST32BL_EraseFlash( void )
{
	u8   Ret = TRUE;

	vu32 PageCount    = 0;
	u32  i;

	volatile FLASH_Status FLASHStatus;


	FLASHStatus = FLASH_COMPLETE;



	FLASH_Unlock();
	FLASH_ClearFlag(FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);


	PageCount = ((HW_ST32BL_FLASH_END_ADDRESS - HW_ST32BL_FLASH_START_ADDRESS) / HW_ST32BL_FLASH_PAGE_SIZE );


	for( i=0; (i < PageCount) && (FLASHStatus == FLASH_COMPLETE); i++ )
	{
		FLASHStatus = FLASH_ErasePage(HW_ST32BL_FLASH_START_ADDRESS + (HW_ST32BL_FLASH_PAGE_SIZE * i));

		#if HW_ST32BL_USE_DEBUG == 1
		//Hw_Uart_Printf(HW_UART_COM1, "Erase %d %x\r\n", i, FLASHStatus );
		#endif			
	}

	#if HW_ST32BL_USE_DEBUG == 1
	Hw_Uart_Printf(HW_UART_COM1, "Erase End %x\r\n", FLASHStatus );
	#endif	

	if( FLASHStatus != FLASH_COMPLETE )
	{
		FLASH_Lock();
		Ret = FALSE;
	}

	return Ret;
}
#endif





/*---------------------------------------------------------------------------
     TITLE   : Ap_ST32BL_CheckFlashSector
     WORK    : 
     ARG     : void
     RET     : void   
---------------------------------------------------------------------------*/
u8 Ap_ST32BL_CheckFlashSector( u32 Address )
{
	u8  Ret = TRUE;
	u32 PageCount;

	volatile FLASH_Status FLASHStatus;

	FLASHStatus = FLASH_COMPLETE;


	if( Address < HW_ST32BL_FLASH_START_ADDRESS	) return FALSE;


	PageCount = (Address - HW_ST32BL_FLASH_START_ADDRESS) / HW_ST32BL_FLASH_PAGE_SIZE;


	if( Ap_ST32BL_EraseSectorFlag[PageCount] == FALSE )
	{
		FLASHStatus = FLASH_ErasePage(HW_ST32BL_FLASH_START_ADDRESS + (HW_ST32BL_FLASH_PAGE_SIZE * PageCount));

		Ap_ST32BL_EraseSectorFlag[PageCount] = TRUE;

		if( FLASHStatus != FLASH_COMPLETE )
		{
			FLASH_Lock();
			Ret = FALSE;
		}
	}


	return Ret;
}



/*---------------------------------------------------------------------------
     TITLE   : Ap_ST32BL_WriteFlash
     WORK    : 
     ARG     : void
     RET     : void   
---------------------------------------------------------------------------*/
u8 Ap_ST32BL_WriteFlash( u32 Address, u8 *pData, u32 DataSize )
{
	u32 StartAddress = Address;
	u32 WriteSize;
	u32 WriteData;
	u32 i;
	u32 DataIndex;
	u8	Ret = TRUE;
	volatile FLASH_Status FLASHStatus;


	WriteSize = DataSize / 4;	// 32Bit로 변경 

	if( (WriteSize%4) > 0 ) WriteSize++;


	//-- 섹터가 지워졌는지 검사
	//
	if( Ap_ST32BL_CheckFlashSector(Address) == FALSE )
	{
		return FALSE;
	}


	DataIndex = 0;

	for( i=0; i<WriteSize; i++ )
	{
		WriteData  = pData[ DataIndex++ ] << 0;
		WriteData |= pData[ DataIndex++ ] << 8;
		WriteData |= pData[ DataIndex++ ] << 16;
		WriteData |= pData[ DataIndex++ ] << 24;

		FLASHStatus = FLASH_ProgramWord(StartAddress+i*4, WriteData);

		if( FLASHStatus != FLASH_COMPLETE )
		{
			Ret = FALSE;
			break;
		}
	}

	return Ret;
}




extern void PowerOff();

/*---------------------------------------------------------------------------
     TITLE   : Ap_ST32BL_JumpToApp
     WORK    : 
     ARG     : void
     RET     : void   
---------------------------------------------------------------------------*/
void Ap_ST32BL_JumpToApp( u32 Address )
{
	pFunction JumpToApp;
	u32 	  JumpAddress;
	u32 	  RegData;

	//JumpToAddress = (pFunction)(HW_ST32BL_FLASH_START_ADDRESS + 4);


	JumpAddress =  *(u32 *)(HW_ST32BL_FLASH_START_ADDRESS + 4);
	

	JumpToApp = (pFunction) JumpAddress;


	//Lb_printf("jump..\n");
	Hw_Timer_Delay_ms(100);

	//REG_RCC_AHBENR  = 0x00000014;
	//REG_RCC_APB1ENR = 0x00000000;
	//REG_RCC_APB2ENR = 0x00000000;	

	PowerOff();


#if 0
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


	/*
	RegData = 0xFFFFFFFF;

	CLR_BIT(RegData, 17);			// UART2
	CLR_BIT(RegData, 18);			// UART3
	CLR_BIT(RegData, 19);			// UART4
	CLR_BIT(RegData, 20);			// UART5

	REG_RCC_APB1RSTR = RegData;


	RegData = 0xFFFFFFFF;

	CLR_BIT(RegData, 14);			// UART1

	
	REG_RCC_APB2RSTR = RegData;
	*/

	//-- Remap된 IO가 있으면 초기화
	//
	//REG_AFIO_MAPR    = 0x00000000;
	//REG_AFIO_MAPR2   = 0x00000000;



    //REG_RCC_CR |= 0x00000001;

    /* Reset SW, HPRE, PPRE1, PPRE2, ADCPRE and MCO bits */
    //REG_RCC_CFGR &= 0xF8FF0000;

    /* Reset HSEON, CSSON and PLLON bits */
    //REG_RCC_CR &= 0xFEF6FFFF;

    /* Reset HSEBYP bit */
    //REG_RCC_CR &= 0xFFFBFFFF;

    /* Reset PLLSRC, PLLXTPRE, PLLMUL and USBPRE/OTGFSPRE bits */
    //REG_RCC_CFGR &= 0xFF80FFFF;

    /* Disable all interrupts and clear pending bits  */
    //REG_RCC_CIR = 0x009F0000;



	JumpToApp();
}	



