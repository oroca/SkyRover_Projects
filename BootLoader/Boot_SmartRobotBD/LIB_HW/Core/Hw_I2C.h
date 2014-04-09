//----------------------------------------------------------------------------
//    프로그램명 	: I2C 관련 헤더
//
//    만든이     	: 
//
//    날  짜     	:
//    
//    최종 수정  	:
//
//    MPU_Type		:
//
//    파일명     	: Hw_I2C.h
//----------------------------------------------------------------------------
                                                                                                 
#ifndef __HW_I2C_H__
#define __HW_I2C_H__


#ifdef   HW_I2C_LOCAL
#define  EXT_HW_I2C_DEF
#else
#define  EXT_HW_I2C_DEF     extern
#endif



#include "Hw.h"
#include "Hw_I2C_SW.h"

#define HW_I2C_USE_SW_I2C	1



EXT_HW_I2C_DEF void Hw_I2C_Init( void );

EXT_HW_I2C_DEF u16  Hw_I2C_Write( u8 Ch, u16 i2c_addr, u8 *pdata, u16 len );
EXT_HW_I2C_DEF u16  Hw_I2C_Read ( u8 Ch, u16 i2c_addr, u8 *pdata, u16 len );


#endif
