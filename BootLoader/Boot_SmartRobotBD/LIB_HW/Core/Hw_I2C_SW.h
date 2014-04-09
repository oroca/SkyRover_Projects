//----------------------------------------------------------------------------
//    프로그램명 	: I2C SW 관련 헤더
//
//    만든이     	: 
//
//    날  짜     	:
//    
//    최종 수정  	:
//
//    MPU_Type		:
//
//    파일명     	: Hw_I2C_SW.h
//----------------------------------------------------------------------------
                                                                                                 
#ifndef __HW_I2C_SW_H__
#define __HW_I2C_SW_H__


#ifdef   HW_I2C_SW_LOCAL
#define  EXT_HW_I2C_SW_DEF
#else
#define  EXT_HW_I2C_SW_DEF     extern
#endif



#include "Hw.h"



EXT_HW_I2C_SW_DEF void  Hw_I2C_SW_Init( void );

EXT_HW_I2C_SW_DEF u16   Hw_I2C_SW_Write( u8 Ch, u16 i2c_addr, u8* pdata, u16 len );
EXT_HW_I2C_SW_DEF u16   Hw_I2C_SW_Read ( u8 Ch, u16 i2c_addr, u8 *pdata, u16 len );


#endif
