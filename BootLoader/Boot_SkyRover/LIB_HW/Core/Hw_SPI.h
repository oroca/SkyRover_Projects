//----------------------------------------------------------------------------
//    프로그램명 : SPI 관련 함수 헤더
//
//    만든이     : Cho Han Cheol 
//
//    날  짜     : 
//    
//    최종 수정  : 
//
//    MPU_Type   : 
//
//    파일명     : Hw_SPI.h
//----------------------------------------------------------------------------
                       
#ifndef __HW_SPI_H__
#define __HW_SPI_H__ 




#ifdef   HW_SPI_LOCAL
#define  EXT_HW_SPI_DEF 
#else
#define  EXT_HW_SPI_DEF     extern
#endif


#include "Hw.h"


#define HW_SPI_CH1		0
#define HW_SPI_CH2		1
#define HW_SPI_CH3		2
 


EXT_HW_SPI_DEF  void Hw_SPI_Init(void);

EXT_HW_SPI_DEF  void Hw_SPI_Enable( u8 Ch );
EXT_HW_SPI_DEF  void Hw_SPI_Disable( u8 Ch );
EXT_HW_SPI_DEF  u16  Hw_SPI_Write8Bit( u8 Ch, u8 WriteData );
EXT_HW_SPI_DEF  u16  Hw_SPI_Write16Bit( u8 Ch, u16 WriteData );

EXT_HW_SPI_DEF  void Hw_SPI_StartDMA( u8 Ch );
EXT_HW_SPI_DEF  void Hw_SPI_StopDMA ( u8 Ch );

#endif
