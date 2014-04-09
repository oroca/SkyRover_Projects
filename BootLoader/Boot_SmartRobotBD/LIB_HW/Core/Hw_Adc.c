//----------------------------------------------------------------------------
//    프로그램명 	: ADC  관련 함수
//
//    만든이     	: Cho Han Cheol
//
//    날  짜     	: 2013. 1. 23.
//    
//    최종 수정  	:
//
//    MPU_Type	:
//
//    파일명     	: Hw_Adc.c
//----------------------------------------------------------------------------


//----- 헤더파일 열기
//
#define  HW_ADC_LOCAL

#include "Hw_Adc.h"




void Hw_Adc_PortInit( void );





/*---------------------------------------------------------------------------
     TITLE   : Hw_Adc_Init
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
void Hw_Adc_Init( void )
{

	//-- ADC GPIO 설정
	//
	Hw_Adc_PortInit();


	// PCLK2 is the APB2 clock
	// ADCCLK = PCLK2/6 = 72/6 = 12MHz
	//
	SET_BIT( REG_RCC_CFGR, 15 );
	CLR_BIT( REG_RCC_CFGR, 14 );


	// Enable ADC1 clock so that we can talk to it
	//
	SET_BIT( REG_RCC_APB2ENR, 9 );


	// ADC1 Configuration ------------------------------------------------------
	//
	REG_ADC1_CR1 = 0
        | ( 0 << 23 )   // AWDEN     - Analogwatchdogenableonregularchannels
        				//             0 : Disabled
        				//             1 : Enabled
        | ( 0 << 22 )   // JAWDEN    - Analogwatchdogenableoninjectedchannels
        				//             0 : Disabled
        				//             1 : Enabled
        | ( 0 << 16 )   // DUALMOD[3:0] - Dualmodeselection
        | ( 0 << 13 )   // DISCNUM[2:0] - Discontinuous mode channel count
        				//             0~7
        | ( 0 << 12 )   // JDISCEN   - Discontinuousmodeoninjectedchannels
        				//             0 : 8bits
        				//             1 : 32
        | ( 0 << 11 )   // DISCEN    - Discontinuousmodeonregularchannels
        				//             0 : Disabled
        				//             1 : Enabled
        | ( 0 << 10 )   // JAUTO	 - AutomaticInjectedGroupconversion
        				//             0 : Disabled
        				//             1 : Enabled
        | ( 0 <<  9 )   // AWDSGL    - Enablethewatchdogonasinglechannelinscanmode
        				//             0 : Analog watchdog enabled on all channels
        				//             1 : Analog watchdog enabled on a single channel
        | ( 0 <<  8 )   // SCAN      - Scan mode
        				//             0 : Disabled
        				//             1 : Enabled
		| ( 0 <<  7 )   // JEOCIE    - Interrupt enable for injected channels
		                //             0 : Disabled
						//             1 : Enabled
        | ( 0 <<  6 )   // AWDIE     - Analog watchdog interrupt enable 
		                //             0 : Disabled
						//             1 : Enabled
		| ( 0 <<  5 )   // EOCIE     - Interrupt enable for EOC
		                //             0 : EOC interrupt disabled
						//             1 : EOC interrupt enabled. An interrupt is generated when the EOC bit is set
        | ( 0 <<  0 );  // AWDCH[4:0]- 
        				//             0~17
        				//             



	REG_ADC1_CR2 = 0
        | ( 1 << 23 )   // TSVREFE  - Temperature sensor and VREFINT enable
        				//             0 : Disabled
        				//             1 : Enabled
        | ( 0 << 22 )   // SWSTART  - Startconversionofregularchannels
        				//             0 : Reset state
        				//             1 : Starts conversion of regular channels
        | ( 0 << 21 )   // JSWSTART - Startconversionofinjectedchannels
        				//             0 : Disabled
        				//             1 : Enabled
        | ( 0 << 20 )   // EXTTRIG   - Externaltriggerconversionmodeforregularchannels
        | ( 0 << 17 )   // EXTSEL[2:0] - Externaleventselectforregulargroup
        | ( 0 << 15 )   // JEXTTRIG  - Externaltriggerconversionmodeforinjectedchannels
        				//             0 : Conversion on external event disabled
        				//             1 : Conversion on external event enabled
        | ( 0 << 12 )   // JEXTSEL[2:0] - Externaleventselectforinjectedgroup
        | ( 0 << 10 )   // JAUTO	 - AutomaticInjectedGroupconversion
        				//             0 : Disabled
        				//             1 : Enabled
        | ( 0 << 11 )   // ALIGN     - Dataalignment
        				//             0 : Right Alignment
        				//             1 : Left Alignment
        | ( 0 <<  8 )   // DMA       - Direct memory access mode
        				//             0 : Disabled
        				//             1 : Enabled
		| ( 0 <<  3 )   // RSTCAL    - Resetcalibration
		                //             0 : Calibration register initialized.
						//             1 : Initialize calibration register.
        | ( 0 <<  2 )   // CAL       - A/D Calibration
		                //             0 : Calibration completed
						//             1 : Enable calibration
		| ( 0 <<  1 )   // CONT      - Continuousconversion
		                //             0 : Single conversion mode
						//             1 : Continuous conversion mode
        | ( 0 <<  0 );  // ADON      - A/D converter ON / OFF
		                //             0 : Disabled
						//             1 : Enabled


	// Enable ADC1
	//
	SET_BIT( REG_ADC1_CR2, 0 );

	// Enable ADC1 reset calibaration register
	//
	SET_BIT( REG_ADC1_CR2, 3 );	

	// Check the end of ADC1 reset calibration register
	//
	while(IS_SET_BIT(REG_ADC1_CR2, 3));

	// Start ADC1 calibaration
	//
	//ADC_StartCalibration(ADC1);
	SET_BIT( REG_ADC1_CR2, 2 );

	// Check the end of ADC1 calibration
	//
	while(IS_SET_BIT(REG_ADC1_CR2, 2));


	REG_ADC1_SQR1  = 0;
	REG_ADC1_SQR1 |= (1<<20);	// 1개 채널만 컨버젼
}





/*---------------------------------------------------------------------------
     TITLE   : Hw_Adc_PortInit
     WORK    :
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
void Hw_Adc_PortInit( void )
{
	//GPIO_InitTypeDef GPIO_InitStructure;

	//-- ADC GPIO 설정  PA7 : AIN7
	//
	REG_GPIOA_CRL &= ~(0x0F << (28));	// Clear
	REG_GPIOA_CRL |=  (0x00 << (28));	// MODE, PA.7 Input Mode
	REG_GPIOA_CRL |=  (0x00 << (30));	// CNF,  PA.7 Analog Mode	
}




/*---------------------------------------------------------------------------
     TITLE   : Hw_Adc_GetData
     WORK    :
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
u16 Hw_Adc_GetData( u8 Ch )
{
	u32 TimeOut;

	//-- 채널 설정
	//
	if( Ch >= 10 )
	{
		REG_ADC1_SMPR1 &= ~(0x07<<(Ch-10)*3);
		REG_ADC1_SMPR1 |=  (0x05<<(Ch-10)*3);	// 55.5 cycles
	}
	else
	{
		REG_ADC1_SMPR2 &= ~(0x07<<(Ch*3));
		REG_ADC1_SMPR2 |=  (0x05<<(Ch*3));	// 55.5 cycles
	}

	REG_ADC1_SQR3 = Ch;


	//-- Start the conversion
	//
	SET_BIT( REG_ADC1_CR2, 0 );

	//-- Wait until conversion completion
	//
	TimeOut = 10000;
	while( IS_CLR_BIT( REG_ADC1_SR, 1) )
	{
		TimeOut--;
		if( TimeOut == 0 ) break;
	} 

	// Get the conversion value

	return REG_ADC1_DR;
}





/*---------------------------------------------------------------------------
     TITLE   : Hw_Adc_StartConv
     WORK    :
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
void Hw_Adc_StartConv( u8 Ch )
{
	u32 TimeOut;

	//-- 채널 설정
	//
	if( Ch >= 10 )
	{
		REG_ADC1_SMPR1 &= ~(0x07<<(Ch-10)*3);
		REG_ADC1_SMPR1 |=  (0x05<<(Ch-10)*3);	// 55.5 cycles
	}
	else
	{
		REG_ADC1_SMPR2 &= ~(0x07<<(Ch*3));
		REG_ADC1_SMPR2 |=  (0x05<<(Ch*3));	// 55.5 cycles
	}

	REG_ADC1_SQR3 = Ch;


	//-- Start the conversion
	//
	SET_BIT( REG_ADC1_CR2, 0 );
}





/*---------------------------------------------------------------------------
     TITLE   : Hw_Adc_GetConv
     WORK    :
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
u8 Hw_Adc_GetConv( u16 *pAdcData )
{
	
	if( IS_SET_BIT( REG_ADC1_SR, 1) )
	{
		*pAdcData = REG_ADC1_DR;
	}
	else
	{
		return FALSE;
	}
	

	return TRUE;
}


