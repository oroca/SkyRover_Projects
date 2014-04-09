//----------------------------------------------------------------------------
//    프로그램명 : DMA 관련 함수 헤더
//
//    만든이     : Cho Han Cheol 
//
//    날  짜     : 
//    
//    최종 수정  : 
//
//    MPU_Type   : 
//
//    파일명     : Hw_DMA.h
//----------------------------------------------------------------------------
                       
#ifndef __HW_DMA_H__
#define __HW_DMA_H__ 




#ifdef   HW_DMA_LOCAL
#define  EXT_HW_DMA_DEF 
#else
#define  EXT_HW_DMA_DEF     extern
#endif


#include "Hw.h"
 


EXT_HW_DMA_DEF  void Hw_DMA_Init(void);

EXT_HW_DMA_DEF  void Hw_DMA_SPI2_Setup( u32 WriteAddress, u32 ReadAddress, u32 BufSize );
EXT_HW_DMA_DEF  u8   Hw_DMA_SPI2_IsTransFinished( void );

#endif
