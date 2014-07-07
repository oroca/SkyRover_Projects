//----------------------------------------------------------------------------
//    프로그램명 	: ADC 헤더
//
//    만든이     	: Cho Han Cheol
//
//    날  짜     	:
//    
//    최종 수정  	:
//
//    MPU_Type	:
//
//    파일명     	: Hw_Adc.h
//----------------------------------------------------------------------------
                                                                                                 
#ifndef __HW_ADC_H__
#define __HW_ADC_H__


#ifdef   HW_ADC_LOCAL
#define  EXT_HW_ADC_DEF
#else
#define  EXT_HW_ADC_DEF     extern
#endif



#include "Hw.h"


#ifndef ON
#define ON  1
#endif

#ifndef OFF
#define OFF 0
#endif

#ifndef LOW
#define LOW 0
#endif

#ifndef HIGH
#define HIGH 1
#endif


#define HW_ADC_MAX_LIMIT	4095



EXT_HW_ADC_DEF void Hw_Adc_Init( void );
EXT_HW_ADC_DEF u16  Hw_Adc_GetData( u8 Ch );
EXT_HW_ADC_DEF void Hw_Adc_StartConv( u8 Ch );
EXT_HW_ADC_DEF u8   Hw_Adc_GetConv( u16 *pAdcData );

#endif
