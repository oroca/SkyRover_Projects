//----------------------------------------------------------------------------
//    프로그램명 	: I2C SW 관련 함수
//
//    만든이     	: 
//
//    날  짜     	: 
//    
//    최종 수정  	:
//
//    MPU_Type		:
//
//    파일명     	: Hw_I2C_SW.c
//----------------------------------------------------------------------------


//----- 헤더파일 열기
//
#define  HW_I2C_SW_LOCAL


#include "Hw_I2C_SW.h"



#define HW_I2C_SW_CH_MAX  			2



#define HW_I2C_SW_PULSE_DELAY   	10



void Hw_I2C_SW_IO_Setup( void );

void Hw_I2C_SW_SCL_HIGH( u8 Ch );
void Hw_I2C_SW_SCL_LOW ( u8 Ch );
void Hw_I2C_SW_SDA_HIGH( u8 Ch );
void Hw_I2C_SW_SDA_LOW ( u8 Ch );
u8   Hw_I2C_SW_SDA     ( u8 Ch );
void Hw_I2C_SW_SetDir_SDA_Input ( u8 Ch );
void Hw_I2C_SW_SetDir_SDA_Output( u8 Ch );

void Hw_I2C_SW_StartTransmit( u8 Ch );
void Hw_I2C_SW_EndTransmit  ( u8 Ch );


u8 Hw_I2C_SW_WriteByte_NoAck( u8 Ch, u8   Data );
u8 Hw_I2C_SW_WriteByte_Ack  ( u8 Ch, u8   Data );
u8 Hw_I2C_SW_ReadByte_NoAck ( u8 Ch, u8 *pData );
u8 Hw_I2C_SW_ReadByte_Ack   ( u8 Ch, u8 *pData );






/*---------------------------------------------------------------------------
     TITLE   : Hw_I2C_SW_Init
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
void Hw_I2C_SW_Init( void )
{
	//-- I/O 초기화
	//
	Hw_I2C_SW_IO_Setup();	
}





/*---------------------------------------------------------------------------
     TITLE   : Hw_I2C_SW_IO_Setup
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
void Hw_I2C_SW_IO_Setup( void )
{
	u8 i;

	//-- SCL PB.6
	//
	REG_GPIOB_CRL &= ~(0x0F << (24));	// Clear
	REG_GPIOB_CRL |=  (0x03 << (24));	// MODE, PB.6 Output mode, max speed 50Mhz
	REG_GPIOB_CRL |=  (0x00 << (26));	// CNF,  PB.6 General purpose output push-pul	


	//-- SDA PB.7
	//
	REG_GPIOB_CRL &= ~(0x0F << (28));	// Clear
	REG_GPIOB_CRL |=  (0x03 << (28));	// MODE, PB.7 Output mode, max speed 50Mhz
	REG_GPIOB_CRL |=  (0x00 << (30));	// CNF,  PB.7 General purpose output push-pul	


	// 핀 기본 상태 
	for( i=0; i<HW_I2C_SW_CH_MAX; i++ )
	{
		Hw_I2C_SW_SCL_HIGH( i );
		Hw_I2C_SW_SDA_HIGH( i );
	}

}





/*---------------------------------------------------------------------------
     TITLE   : Hw_I2C_SW_Write
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
u16 Hw_I2C_SW_Write( u8 Ch, u16 i2c_addr, u8* pdata, u16 len )
{
    u16 Ret = 0;;
	u8  i;

	Hw_I2C_SW_StartTransmit(Ch);

	Ret |= Hw_I2C_SW_WriteByte_Ack(Ch, (i2c_addr<<1) | (0<<0)); // Address


	for( i=0; i<len; i++ )
	{
		Ret |= Hw_I2C_SW_WriteByte_Ack(Ch, pdata[i]);  			// Data	
	}
	
	Hw_I2C_SW_EndTransmit(Ch);

    return Ret;
}





/*---------------------------------------------------------------------------
     TITLE   : Hw_I2C_SW_Read
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
u16 Hw_I2C_SW_Read( u8 Ch, u16 i2c_addr, u8 *pdata, u16 len )
{
	u16 Ret = 0;
	u8  i;
	u8  LastIndex = 0;

	if( len > 0 ) LastIndex = len - 1;

	Hw_I2C_SW_StartTransmit(Ch);

	Ret |= Hw_I2C_SW_WriteByte_Ack(Ch, (i2c_addr<<1) | (1<<0));  		// Address

	for( i=0; i<len; i++ )
	{
		if( i < LastIndex )
		{
			Ret |= Hw_I2C_SW_ReadByte_Ack(Ch, &pdata[i] );				// Read Data
		}
		else
		{
			Ret |= Hw_I2C_SW_ReadByte_NoAck(Ch, &pdata[i] );			// Read Data 마지막 데이터는 NACK 처리
		}
	}
	
	Hw_I2C_SW_EndTransmit(Ch);
	

    return Ret;
}





/*---------------------------------------------------------------------------
     TITLE   : Hw_I2C_SW_StartTransmit
     WORK    : 
     ARG     : 
     			u8 Ch : I2C 채널
     RET     : void
---------------------------------------------------------------------------*/
void Hw_I2C_SW_StartTransmit( u8 Ch )
{
	//-- SDA Rising Edge 생성
	//
								// SCL	SDA
	Hw_I2C_SW_SCL_HIGH( Ch );	//  1    1
	Hw_I2C_SW_SDA_HIGH( Ch );	//  1    1
	Hw_I2C_SW_SCL_HIGH( Ch );	//	1    1
	Hw_I2C_SW_SDA_LOW ( Ch );	//  1    0
	Hw_I2C_SW_SCL_LOW ( Ch );	//  0    0
}





/*---------------------------------------------------------------------------
     TITLE   : Hw_ADT75_EndTransmit
     WORK    : 
     ARG     : 
     			u8 Ch : I2C 채널
     RET     : void
---------------------------------------------------------------------------*/
void Hw_I2C_SW_EndTransmit( u8 Ch )
{
	//-- SDA Falling Edge 생성
	//
								// SCL	SDA   
	Hw_I2C_SW_SDA_LOW ( Ch );	//  0    0
	Hw_I2C_SW_SCL_LOW ( Ch );	//  0    0
	Hw_I2C_SW_SCL_HIGH( Ch );	//  1    0
	Hw_I2C_SW_SDA_HIGH( Ch );	//  1    1
	Hw_I2C_SW_SCL_HIGH( Ch );	//  1    1
}





/*---------------------------------------------------------------------------
     TITLE   : Hw_I2C_SW_WriteByte_NoAck
     WORK    : 
     ARG     : 
     			u8 Ch   : I2C 채널
     			u8 Data : 전송할 바이트
     RET     : 
     			u8 : 에러코드
---------------------------------------------------------------------------*/
u8 Hw_I2C_SW_WriteByte_NoAck( u8 Ch, u8 Data )
{
	u8 i;

	for( i=0; i<8; i++ )
	{
		if( Data & (1<<7) )  Hw_I2C_SW_SDA_HIGH( Ch );
		else                 Hw_I2C_SW_SDA_LOW ( Ch );

		Data <<= 1;

		Hw_I2C_SW_SCL_HIGH( Ch );
		Hw_I2C_SW_SCL_LOW ( Ch );
	}

	Hw_I2C_SW_SCL_HIGH( Ch );
	Hw_I2C_SW_SCL_LOW( Ch );

	return 0;
}





/*---------------------------------------------------------------------------
     TITLE   : Hw_ADT75_WriteByte_Ack
     WORK    : 
     ARG     : 
     			u8 Ch   : I2C 채널
     			u8 Data : 전송할 바이트
     RET     : 
     			u8 : 0 -> Ack
---------------------------------------------------------------------------*/
u8 Hw_I2C_SW_WriteByte_Ack( u8 Ch, u8 Data )
{
	u8 i;
	u8 Ack = 1;

	for( i=0; i<8; i++ )
	{
		if( Data & (1<<7) )  Hw_I2C_SW_SDA_HIGH( Ch );
		else                 Hw_I2C_SW_SDA_LOW ( Ch );

		Data <<= 1;

		Hw_I2C_SW_SCL_HIGH( Ch );
		Hw_I2C_SW_SCL_LOW( Ch );
	}


	Hw_I2C_SW_SetDir_SDA_Input( Ch );



	// Ack 검사 
	//
	Hw_I2C_SW_SCL_HIGH( Ch );

	if( Hw_I2C_SW_SDA( Ch ) == 0 ) Ack = 0;

	Hw_I2C_SW_SCL_LOW( Ch );


	Hw_I2C_SW_SetDir_SDA_Output( Ch );

	return Ack;
}





/*---------------------------------------------------------------------------
     TITLE   : Hw_I2C_SW_ReadByte_NoAck
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
u8 Hw_I2C_SW_ReadByte_NoAck( u8 Ch, u8 *pData )
{
	u8 i;
	u8 Data = 0;


	Hw_I2C_SW_SetDir_SDA_Input( Ch );

	for( i=0; i<8; i++ )
	{
		Data <<= 1;

		Hw_I2C_SW_SCL_HIGH( Ch );
		
		if( Hw_I2C_SW_SDA( Ch ) == 1 ) Data |= 1;

		Hw_I2C_SW_SCL_LOW( Ch );
	}


	Hw_I2C_SW_SDA_HIGH( Ch );

	Hw_I2C_SW_SetDir_SDA_Output( Ch );

	Hw_I2C_SW_SCL_HIGH( Ch );
	Hw_I2C_SW_SCL_LOW( Ch );

	*pData = Data;

	return 0;
}





/*---------------------------------------------------------------------------
     TITLE   : Hw_I2C_SW_ReadByte_Ack
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
u8 Hw_I2C_SW_ReadByte_Ack( u8 Ch, u8 *pData )
{
	u8 i;
	u8 Data = 0;
	u8 Ack = 0;


	Hw_I2C_SW_SetDir_SDA_Input( Ch );

	for( i=0; i<8; i++ )
	{
		Data <<= 1;

		Hw_I2C_SW_SCL_HIGH( Ch );
		
		if( Hw_I2C_SW_SDA( Ch ) == 1 ) Data |= 1;

		Hw_I2C_SW_SCL_LOW( Ch );
	}

	*pData = Data;


	Hw_I2C_SW_SetDir_SDA_Output( Ch );

	// Ack 발생 
	//
	Hw_I2C_SW_SDA_LOW( Ch );

	Hw_I2C_SW_SCL_HIGH( Ch );
	Hw_I2C_SW_SCL_LOW( Ch );


	return Ack;
}





/*---------------------------------------------------------------------------
     TITLE   : Hw_I2C_SW_SCL_HIGH
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
void Hw_I2C_SW_SCL_HIGH( u8 Ch )
{
	Hw_Wait_Usec(HW_I2C_SW_PULSE_DELAY);

	if( Ch == 0 )  
	{
		SET_BIT( REG_GPIOB_ODR, 6 );
	}
	else           
	{
	}

	Hw_Wait_Usec(HW_I2C_SW_PULSE_DELAY);
}





/*---------------------------------------------------------------------------
     TITLE   : Hw_I2C_SW_SCL_LOW
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
void Hw_I2C_SW_SCL_LOW( u8 Ch )
{
	Hw_Wait_Usec(HW_I2C_SW_PULSE_DELAY);

	if( Ch == 0 )  
	{
		CLR_BIT( REG_GPIOB_ODR, 6 );	
	}
	else           
	{
	}

	Hw_Wait_Usec(HW_I2C_SW_PULSE_DELAY);
}





/*---------------------------------------------------------------------------
     TITLE   : Hw_I2C_SW_SDA_HIGH
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
void Hw_I2C_SW_SDA_HIGH( u8 Ch )
{
	if( Ch == 0 )
	{
		SET_BIT( REG_GPIOB_ODR, 7 );
	}
	else           
	{
	}
}	





/*---------------------------------------------------------------------------
     TITLE   : Hw_I2C_SW_SDA_LOW
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
void Hw_I2C_SW_SDA_LOW( u8 Ch )
{
	if( Ch == 0 )  
	{
		CLR_BIT( REG_GPIOB_ODR, 7 );
	}
	else           
	{
	}
}	





/*---------------------------------------------------------------------------
     TITLE   : Hw_I2C_SW_SDA
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
u8 Hw_I2C_SW_SDA( u8 Ch )
{
	u8 Data = 0;

	if( Ch == 0 )
	{
		if( REG_GPIOB_IDR & (1<<7) ) Data = 1;
		else						 Data = 0;
	}
	else
	{
	}

	return Data;	
}	





/*---------------------------------------------------------------------------
     TITLE   : Hw_I2C_SW_SetDir_SDA_Input
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
void Hw_I2C_SW_SetDir_SDA_Input( u8 Ch )
{
	if( Ch == 0 )
	{
		REG_GPIOB_CRL &= ~(0x0F << (28));	// Clear
		REG_GPIOB_CRL |=  (0x00 << (28));	// MODE, PB.7 Input Mode
		//REG_GPIOB_CRL |=  (0x01 << (30));	// CNF,  PB.7 Floating input	
		REG_GPIOB_CRL |=  (0x02 << (30));	// CNF,  PB.7 input	pull-up/pull-down
		
		SET_BIT( REG_GPIOB_ODR, 7 );
	}
	else
	{
	}
}





/*---------------------------------------------------------------------------
     TITLE   : Hw_I2C_SW_SetDir_SDA_Output
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
void Hw_I2C_SW_SetDir_SDA_Output( u8 Ch )
{
	if( Ch == 0 )
	{
		REG_GPIOB_CRL &= ~(0x0F << (28));	// Clear
		REG_GPIOB_CRL |=  (0x03 << (28));	// MODE, PB.7 Output mode, max speed 50Mhz
		REG_GPIOB_CRL |=  (0x00 << (30));	// CNF,  PB.7 General purpose output push-pul	
	}
	else
	{
	}
}

