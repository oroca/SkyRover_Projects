//----------------------------------------------------------------------------
//    프로그램명 	: Radio 관련 함수 헤더
//
//    만든이     	: 
//
//    날  짜     	: 
//    
//    최종 수정  	: 
//
//    MPU_Type 		:
//
//    파일명     	: AP_Radio.h
//----------------------------------------------------------------------------




                                                                                                 
#ifndef  _AP_RADIO_H
#define  _AP_RADIO_H



#include "Ap.h"

#include "Hw_Si47xx.h"



#ifdef   AP_RADIO_LOCAL
#define  EXT_AP_RADIO_DEF
#else
#define  EXT_AP_RADIO_DEF     extern
#endif


#define ERR_RADIO_TIME_OUT				0x0001
#define ERR_RADIO_HW_ERR				0x0002
#define ERR_RADIO_BAND_ERR				0x0003


#define MODE_RADIO_BAND_NONE			0
#define MODE_RADIO_BAND_FM				1
#define MODE_RADIO_BAND_AM				2

		







EXT_AP_RADIO_DEF u16 Ap_Radio_Init(void);



EXT_AP_RADIO_DEF u16 Ap_Radio_BandSetFM( void );
EXT_AP_RADIO_DEF u16 Ap_Radio_BandSetAM( void );
EXT_AP_RADIO_DEF u16 Ap_Radio_BandGet( void );


EXT_AP_RADIO_DEF u16 Ap_Radio_VolumeSet    ( u16 Volume );
EXT_AP_RADIO_DEF u16 Ap_Radio_VolumeGet    ( u16 *pVolume );
EXT_AP_RADIO_DEF u16 Ap_Radio_VolumeSetUp  ( u16 Step );
EXT_AP_RADIO_DEF u16 Ap_Radio_VolumeSetDown( u16 Step );


EXT_AP_RADIO_DEF u16 Ap_Radio_MuteSetOn ( void );
EXT_AP_RADIO_DEF u16 Ap_Radio_MuteSetOff( void );


EXT_AP_RADIO_DEF u16 Ap_Radio_ChannelSet     ( u16 Channel );
EXT_AP_RADIO_DEF u16 Ap_Radio_ChannelGet     ( u16 *pChannel );
EXT_AP_RADIO_DEF u16 Ap_Radio_ChannelSetFirst( void );
EXT_AP_RADIO_DEF u16 Ap_Radio_ChannelSetLast ( void );
EXT_AP_RADIO_DEF u16 Ap_Radio_ChannelSetNext ( u16 Step );
EXT_AP_RADIO_DEF u16 Ap_Radio_ChannelSetPrev ( u16 Step);


EXT_AP_RADIO_DEF u16 Ap_Radio_SeekFromFirst( u8 *pFreqValid );
EXT_AP_RADIO_DEF u16 Ap_Radio_SeekFromLast ( u8 *pFreqValid );
EXT_AP_RADIO_DEF u16 Ap_Radio_SeekToFirst  ( u8 *pFreqValid );
EXT_AP_RADIO_DEF u16 Ap_Radio_SeekToLast   ( u8 *pFreqValid );


#endif
                                                                                                 
                                                                                                 

