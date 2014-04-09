//----------------------------------------------------------------------------
//    프로그램명 	: Radio 관련 함수
//
//    만든이     	: HanCheol Cho
//
//    날  짜     	: 
//    
//    최종 수정  	: 
//
//    MPU_Type		:
//
//    파일명     	: Ap_Radio.h
//----------------------------------------------------------------------------



//----- 헤더파일 열기
//
#define  AP_RADIO_LOCAL

#include "Ap_Radio.h"



//-- 내부 선언
//

typedef struct
{
	u16 Band_Mode;
	u16 Mute_Mode;
	
	u16 Volume_Min;
	u16 Volume_Max;

	u16 FM_Channel_Min;
	u16 FM_Channel_Max;

	u16 AM_Channel_Min;
	u16 AM_Channel_Max;
		
	u16	Volume_Current;
	u16	Channel_Current;
	
	u16 Err_Hw;
} AP_RADIO_OBJ;



//-- 내부 변수
//
AP_RADIO_OBJ	Ap_Radio_Log;


//-- 내부 함수
//
void Ap_Radio_ApiStart( void );


u16 Ap_Radio_FM_ChannelSet( u16 Channel );
u16 Ap_Radio_FM_ChannelGet( u16 *pChannel );
u16 Ap_Radio_AM_ChannelSet( u16 Channel );
u16 Ap_Radio_AM_ChannelGet( u16 *pChannel );





/*---------------------------------------------------------------------------
     TITLE   : Ap_Radio_Init
     WORK    : 
     ARG     : void
     RET     : 
     			u16 - Error Code     
---------------------------------------------------------------------------*/
u16 Ap_Radio_Init(void)
{
	u16 Ret = ERR_NONE;
	
		
	Hw_Si47xx_Init();
	
	
	//-- Set default values
	//
	Ap_Radio_Log.FM_Channel_Min = 7600;
	Ap_Radio_Log.FM_Channel_Max = 10790;
	Ap_Radio_Log.AM_Channel_Min = 520;
	Ap_Radio_Log.AM_Channel_Max = 1710;
	
	Ap_Radio_Log.Volume_Min = 0;
	Ap_Radio_Log.Volume_Max = 63;	
	
	Ap_Radio_Log.Band_Mode = MODE_RADIO_BAND_NONE;
	Ap_Radio_Log.Mute_Mode = 0;
	
	Ap_Radio_Log.Err_Hw = 0;
	
	
	Ret = Ap_Radio_BandSetFM();
	
	if( Ret == ERR_NONE )
	{
		Ret = Ap_Radio_ChannelSetFirst();
	}
	
	return Ret;
}





/*---------------------------------------------------------------------------
     TITLE   : Ap_Radio_ApiStart
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
void Ap_Radio_ApiStart( void )
{
	Ap_Radio_Log.Err_Hw = 0;
}





/*---------------------------------------------------------------------------
     TITLE   : Ap_Radio_BandSetFM
     WORK    : 
     ARG     : void
     RET     : 
     			u16 - Error Code
---------------------------------------------------------------------------*/
u16 Ap_Radio_BandSetFM( void )
{
	u16 Ret = ERR_NONE;
	
	Ap_Radio_ApiStart();
	
	Ap_Radio_Log.Err_Hw = Hw_Si47xx_FM_Config();	
	
	if( Ap_Radio_Log.Err_Hw == ERR_NONE )
	{
		Ap_Radio_Log.Band_Mode = MODE_RADIO_BAND_FM;
	}
	else
	{
		Ret = ERR_RADIO_HW_ERR;
	}
	
	return Ret;
}





/*---------------------------------------------------------------------------
     TITLE   : Ap_Radio_BandSetAM
     WORK    : 
     ARG     : void
     RET     : 
     			u16 - Error Code     
---------------------------------------------------------------------------*/
u16 Ap_Radio_BandSetAM( void )
{
	u16 Ret = ERR_NONE;
	
	Ap_Radio_ApiStart();
	
	
	Ap_Radio_Log.Err_Hw = Hw_Si47xx_AM_Config();	
	
	if( Ap_Radio_Log.Err_Hw == ERR_NONE )
	{
		Ap_Radio_Log.Band_Mode = MODE_RADIO_BAND_AM;
	}
	else
	{
		Ret = ERR_RADIO_HW_ERR;
	}

	
	return Ret;
}





	
/*---------------------------------------------------------------------------
     TITLE   : Ap_Radio_BandGet
     WORK    : 
     ARG     : void
     RET     : void
---------------------------------------------------------------------------*/
u16 Ap_Radio_BandGet( void )
{
	Ap_Radio_ApiStart();
	
	return Ap_Radio_Log.Band_Mode;
}





/*---------------------------------------------------------------------------
     TITLE   : Ap_Radio_VolumeSet
     WORK    : 
     ARG     : 
	   			u16 Volume - Volume value(0~100)
     RET     : 
     			u16 - Error Code     
---------------------------------------------------------------------------*/
u16 Ap_Radio_VolumeSet( u16 Volume )
{
	u16 Ret;
	u16 Radio_Volume;
	
	Ap_Radio_ApiStart();
	
	
	if( Volume > Ap_Radio_Log.Volume_Max ) Volume = Ap_Radio_Log.Volume_Max;
	
	Radio_Volume = Volume * 63 / Ap_Radio_Log.Volume_Max;
	
	Ret = Hw_Si47xx_SET_PROPERTY( 0x4000, Radio_Volume );	
		
	if( Ret != ERR_NONE )
	{
		Ap_Radio_Log.Err_Hw = Ret;
		
		Ret = ERR_RADIO_HW_ERR;
	}
	
	return Ret;
}





/*---------------------------------------------------------------------------
     TITLE   : Ap_Radio_VolumeSetUp
     WORK    : 
     ARG     : 
	   			u16 Step - Volume incresement step (0~100)
     RET     : 
     			u16 - Error Code     
---------------------------------------------------------------------------*/
u16 Ap_Radio_VolumeSetUp( u16 Step )
{
	u16 Ret;
	u16 Volume;
	u16 VolumeTarget;
	
	Ap_Radio_ApiStart();
	
	
	Ret = Ap_Radio_VolumeGet( &Volume );
	
	if( Ret == ERR_NONE )
	{
		VolumeTarget = Volume + Step;
		
		if( VolumeTarget > Ap_Radio_Log.Volume_Max ) VolumeTarget = Ap_Radio_Log.Volume_Max;
		
		Ret = Ap_Radio_VolumeSet( VolumeTarget );	
	}
	
	return Ret;
}





/*---------------------------------------------------------------------------
     TITLE   : Ap_Radio_VolumeSetDown
     WORK    : 
     ARG     : 
	   			u16 Step - Volume decresement step (0~100)     
     RET     : 
     			u16 - Error Code     
---------------------------------------------------------------------------*/
u16 Ap_Radio_VolumeSetDown( u16 Step )
{
	u16 Ret;
	u16 Volume;
	u16 VolumeTarget;
	
	Ap_Radio_ApiStart();
	
	
	Ret = Ap_Radio_VolumeGet( &Volume );
	
	if( Ret == ERR_NONE )
	{
		if( Volume > Step )
		{
			VolumeTarget = Volume - Step;
		}
		else
		{
			VolumeTarget = 0;
		}
				
		Ret = Ap_Radio_VolumeSet( VolumeTarget );	
	}	
	
	return Ret;
}





/*---------------------------------------------------------------------------
     TITLE   : Ap_Radio_VolumeGet
     WORK    : 
     ARG     : 
     			u16 *pVolume - Pointer of volume value
     RET     : 
     			u16 - Error Code     
---------------------------------------------------------------------------*/
u16 Ap_Radio_VolumeGet( u16 *pVolume )
{
	u16 Ret;
	u16 Volume = 0;
	
	Ap_Radio_ApiStart();
	
	
	Ret = Hw_Si47xx_GET_PROPERTY( 0x4000, &Volume );
	
	if( Ret == ERR_NONE )
	{
		
		*pVolume = Volume * Ap_Radio_Log.Volume_Max / 63;
	}
	else
	{
		Ap_Radio_Log.Err_Hw = Ret;
		
		Ret = ERR_RADIO_HW_ERR;
	}
	
	return Ret;
}





/*---------------------------------------------------------------------------
     TITLE   : Ap_Radio_ChannelSet
     WORK    : 
     ARG     : 
     			u16 Channel - Frequency
     RET     : 
     			u16 - Error Code     
---------------------------------------------------------------------------*/
u16 Ap_Radio_ChannelSet( u16 Channel )
{
	u16 Ret;
	
	if( Ap_Radio_Log.Band_Mode == MODE_RADIO_BAND_FM ) 
	{
		Ret = Ap_Radio_FM_ChannelSet( Channel );
	}
	else
	if( Ap_Radio_Log.Band_Mode == MODE_RADIO_BAND_AM )
	{
		Ret = Ap_Radio_AM_ChannelSet( Channel );
	}
	else
	{
		Ret = ERR_RADIO_BAND_ERR;
	}
	
	return Ret;
}





/*---------------------------------------------------------------------------
     TITLE   : Ap_Radio_ChannelGet
     WORK    : 
     ARG     : 
     			u16 *pChannel - Pointer of Frequency
     RET     : 
     			u16 - Error Code     
---------------------------------------------------------------------------*/
u16 Ap_Radio_ChannelGet( u16 *pChannel )
{
	u16 Ret;
	u16 Channel;
	
	if( Ap_Radio_Log.Band_Mode == MODE_RADIO_BAND_FM ) 
	{
		Ret = Ap_Radio_FM_ChannelGet( &Channel );

		if( Ret == ERR_NONE ) *pChannel = Channel;
	}
	else
	if( Ap_Radio_Log.Band_Mode == MODE_RADIO_BAND_AM )
	{
		Ret = Ap_Radio_AM_ChannelGet( &Channel );

		if( Ret == ERR_NONE ) *pChannel = Channel;
	}
	else
	{
		Ret = ERR_RADIO_BAND_ERR;
	}
	
	return Ret;	
}





/*---------------------------------------------------------------------------
     TITLE   : Ap_Radio_FM_ChannelSet
     WORK    : 
     ARG     : 
     			u16 Channel - Frequency
     RET     : 
     			u16 - Error Code     
---------------------------------------------------------------------------*/
u16 Ap_Radio_FM_ChannelSet( u16 Channel )
{
	u16 Ret;
	
	Ap_Radio_ApiStart();


	if( Channel < Ap_Radio_Log.FM_Channel_Min ) Channel = Ap_Radio_Log.FM_Channel_Min;
	if( Channel > Ap_Radio_Log.FM_Channel_Max ) Channel = Ap_Radio_Log.FM_Channel_Max;

	Ret = Hw_Si47xx_FM_TUNE_FREQ( Channel );
		
	if( Ret == ERR_NONE )
	{
		Ret = Hw_Si47xx_WaitForSTC( 500 );		if( Ret != ERR_NONE ) return Ret;
		
		if( Ret == ERR_NONE )
		{
			Ret = Hw_Si47xx_FM_TUNE_STATUS(1);
			
			if( Ret == ERR_NONE )
			{
				
			}
		}
	}
	
	
	if( Ret != ERR_NONE )
	{
		Ap_Radio_Log.Err_Hw = Ret;
		
		Ret = ERR_RADIO_HW_ERR;		
	}
			
	return Ret;
}





/*---------------------------------------------------------------------------
     TITLE   : Ap_Radio_AM_ChannelSet
     WORK    : 
     ARG     : 
     			u16 Channel - Frequency
     RET     : 
     			u16 - Error Code     
---------------------------------------------------------------------------*/
u16 Ap_Radio_AM_ChannelSet( u16 Channel )
{
	u16 Ret;
	
	Ap_Radio_ApiStart();


	if( Channel < Ap_Radio_Log.AM_Channel_Min ) Channel = Ap_Radio_Log.AM_Channel_Min;
	if( Channel > Ap_Radio_Log.AM_Channel_Max ) Channel = Ap_Radio_Log.AM_Channel_Max;

	Ret = Hw_Si47xx_AM_TUNE_FREQ( Channel );
		
	if( Ret == ERR_NONE )
	{
		Ret = Hw_Si47xx_WaitForSTC( 500 );		if( Ret != ERR_NONE ) return Ret;
		
		if( Ret == ERR_NONE )
		{
			Ret = Hw_Si47xx_AM_TUNE_STATUS(1);
			
			if( Ret == ERR_NONE )
			{
				
			}
		}
	}
	
	
	if( Ret != ERR_NONE )
	{
		Ap_Radio_Log.Err_Hw = Ret;
		
		Ret = ERR_RADIO_HW_ERR;		
	}
			
	return Ret;
}






/*---------------------------------------------------------------------------
     TITLE   : Ap_Radio_VolumeGet
     WORK    : 
     ARG     : 
     			u16 *pChannel - Pointer of Frequency
     RET     : 
     			u16 - Error Code     
---------------------------------------------------------------------------*/
u16 Ap_Radio_FM_ChannelGet( u16 *pChannel )
{
	u16 Ret;
	
	Ap_Radio_ApiStart();
	
	Ret = Hw_Si47xx_FM_TUNE_STATUS(1);
			
	if( Ret == ERR_NONE )
	{
		*pChannel = (Hw_Si47xx_ReadResp(2)<<8) | (Hw_Si47xx_ReadResp(3)<<0);	
	}
	
	return Ret;				
}





/*---------------------------------------------------------------------------
     TITLE   : Ap_Radio_AM_ChannelGet
     WORK    : 
     ARG     : 
     			u16 *pChannel - Pointer of Frequency
     RET     : 
     			u16 - Error Code     
---------------------------------------------------------------------------*/
u16 Ap_Radio_AM_ChannelGet( u16 *pChannel )
{
	u16 Ret;
	
	Ap_Radio_ApiStart();
	
	Ret = Hw_Si47xx_AM_TUNE_STATUS(1);
			
	if( Ret == ERR_NONE )
	{
		*pChannel = (Hw_Si47xx_ReadResp(2)<<8) | (Hw_Si47xx_ReadResp(3)<<0);	
	}
	
	return Ret;				
}





/*---------------------------------------------------------------------------
     TITLE   : Ap_Radio_ChannelSetFirst
     WORK    : 
     ARG     : void
     RET     : 
     			u16 - Error Code     
---------------------------------------------------------------------------*/
u16 Ap_Radio_ChannelSetFirst( void )
{
	u16 Ret;
	
	if( Ap_Radio_Log.Band_Mode == MODE_RADIO_BAND_FM ) 
	{
		Ret = Ap_Radio_FM_ChannelSet( Ap_Radio_Log.FM_Channel_Min );
	}
	else
	if( Ap_Radio_Log.Band_Mode == MODE_RADIO_BAND_AM )
	{
		Ret = Ap_Radio_AM_ChannelSet( Ap_Radio_Log.AM_Channel_Min );
	}
	else
	{
		Ret = ERR_RADIO_BAND_ERR;
	}	
	
	return Ret;
}





/*---------------------------------------------------------------------------
     TITLE   : Ap_Radio_ChannelSetLast
     WORK    : 
     ARG     : void
     RET     : 
     			u16 - Error Code     
---------------------------------------------------------------------------*/
u16 Ap_Radio_ChannelSetLast( void )
{
	u16 Ret;
	
	if( Ap_Radio_Log.Band_Mode == MODE_RADIO_BAND_FM ) 
	{
		Ret = Ap_Radio_FM_ChannelSet( Ap_Radio_Log.FM_Channel_Max );
	}
	else
	if( Ap_Radio_Log.Band_Mode == MODE_RADIO_BAND_AM )
	{
		Ret = Ap_Radio_AM_ChannelSet( Ap_Radio_Log.AM_Channel_Max );
	}
	else
	{
		Ret = ERR_RADIO_BAND_ERR;
	}	
	
	return Ret;	
}





/*---------------------------------------------------------------------------
     TITLE   : Ap_Radio_ChannelSetNext
     WORK    : 
     ARG     : void
     RET     : 
     			u16 - Error Code     
---------------------------------------------------------------------------*/
u16 Ap_Radio_ChannelSetNext ( u16 Step )
{
	u16 Ret;
	u16 Channel;
	
	Ret = Ap_Radio_ChannelGet( &Channel );
	
	if( Ret == ERR_NONE )
	{
		Ret = Ap_Radio_ChannelSet( Channel + Step );
	}
	
	return Ret;		
}





/*---------------------------------------------------------------------------
     TITLE   : Ap_Radio_ChannelSetPrev
     WORK    : 
     ARG     : void
     RET     : 
     			u16 - Error Code     
---------------------------------------------------------------------------*/
u16 Ap_Radio_ChannelSetPrev( u16 Step )
{
	u16 Ret;
	u16 Channel;
	
	Ret = Ap_Radio_ChannelGet( &Channel );
	
	if( Ret == ERR_NONE )
	{
		Ret = Ap_Radio_ChannelSet( Channel - Step );
	}
	
	return Ret;			
}





/*---------------------------------------------------------------------------
     TITLE   : Ap_Radio_MuteSetOn
     WORK    : 
     ARG     : void
     RET     : 
     			u16 - Error Code     
---------------------------------------------------------------------------*/
u16 Ap_Radio_MuteSetOn ( void )
{
	u16 Ret;
	
	Ap_Radio_ApiStart();
	
	Ret = Hw_Si47xx_SET_PROPERTY( 0x4001, 3  );
	
	if( Ret != ERR_NONE )
	{
		Ap_Radio_Log.Err_Hw = Ret;
		
		Ret = ERR_RADIO_HW_ERR;
	}
	
	return Ret;
}





/*---------------------------------------------------------------------------
     TITLE   : Ap_Radio_MuteSetOff
     WORK    : 
     ARG     : void
     RET     : 
     			u16 - Error Code     
---------------------------------------------------------------------------*/
u16 Ap_Radio_MuteSetOff ( void )
{
	u16 Ret;
	
	Ap_Radio_ApiStart();
	
	Ret = Hw_Si47xx_SET_PROPERTY( 0x4001, 0  );
	
	if( Ret != ERR_NONE )
	{
		Ap_Radio_Log.Err_Hw = Ret;
		
		Ret = ERR_RADIO_HW_ERR;
	}
	
	return Ret;
}





/*---------------------------------------------------------------------------
     TITLE   : Ap_Radio_SeekToFirst
     WORK    : 
     ARG     : void
     RET     : 
     			u16 - Error Code     
---------------------------------------------------------------------------*/
u16 Ap_Radio_SeekToFirst(  u8 *pFreqValid )
{
	u16 Ret;
	
	Ap_Radio_ApiStart();

	if( Ap_Radio_Log.Band_Mode == MODE_RADIO_BAND_NONE ) return ERR_RADIO_BAND_ERR; 


	if( Ap_Radio_Log.Band_Mode == MODE_RADIO_BAND_FM )
	{
    	Ret = Hw_Si47xx_FM_SEEK_START(0, 0);
    }
    else
    {
    	Ret = Hw_Si47xx_AM_SEEK_START(0, 0);
    }
    
    if( Ret == ERR_NONE )
    {
    	Ret = Hw_Si47xx_WaitForSTC( 20000 );		
    	
    	if( Ret == ERR_NONE ) 
    	{
    		if( Ap_Radio_Log.Band_Mode == MODE_RADIO_BAND_FM )
    		{
    			Ret = Hw_Si47xx_FM_TUNE_STATUS(1);   
    		}
    		else
    		{
    			Ret = Hw_Si47xx_AM_TUNE_STATUS(1);   
    		}
    		
    		if( Ret == ERR_NONE && Hw_Si47xx_ReadResp(1) & (1<<0) )
    		{
    			*pFreqValid = 1;
    		}
    		else
    		{
    			*pFreqValid = 0;
    		}
		}	
	}
	
	
	if( Ret != ERR_NONE )
	{
		Ap_Radio_Log.Err_Hw = Ret;
		
		Ret = ERR_RADIO_HW_ERR;
	}
	
	return Ret;	
}





/*---------------------------------------------------------------------------
     TITLE   : Ap_Radio_SeekToFirst
     WORK    : 
     ARG     : void
     RET     : 
     			u16 - Error Code     
---------------------------------------------------------------------------*/
u16 Ap_Radio_SeekToLast(  u8 *pFreqValid )
{
	u16 Ret;
	
	Ap_Radio_ApiStart();

	if( Ap_Radio_Log.Band_Mode == MODE_RADIO_BAND_NONE ) return ERR_RADIO_BAND_ERR; 

	
	if( Ap_Radio_Log.Band_Mode == MODE_RADIO_BAND_FM )
	{
    	Ret = Hw_Si47xx_FM_SEEK_START(1, 0);
    }
    else
    {
    	Ret = Hw_Si47xx_AM_SEEK_START(1, 0);
    }
    
    if( Ret == ERR_NONE )
    {
    	Ret = Hw_Si47xx_WaitForSTC( 20000 );		
    	
    	if( Ret == ERR_NONE ) 
    	{
    		if( Ap_Radio_Log.Band_Mode == MODE_RADIO_BAND_FM )
    		{
    			Ret = Hw_Si47xx_FM_TUNE_STATUS(1);   
    		}
    		else
    		{
    			Ret = Hw_Si47xx_AM_TUNE_STATUS(1);   
    		}
    		
    		if( Ret == ERR_NONE && Hw_Si47xx_ReadResp(1) & (1<<0) )
    		{
    			*pFreqValid = 1;
    		}
    		else
    		{
    			*pFreqValid = 0;
    		}    		
		}	
	}
	
	
	if( Ret != ERR_NONE )
	{
		Ap_Radio_Log.Err_Hw = Ret;
		
		Ret = ERR_RADIO_HW_ERR;
	}
	
	return Ret;		
}






/*---------------------------------------------------------------------------
     TITLE   : Ap_Radio_SeekFromFirst
     WORK    : 
     ARG     : void
     RET     : 
     			u16 - Error Code     
---------------------------------------------------------------------------*/
u16 Ap_Radio_SeekFromFirst(  u8 *pFreqValid )
{
	u16 Ret;
	u8  FreqValid = 0;
	
	if( Ap_Radio_Log.Band_Mode == MODE_RADIO_BAND_FM ) 
	{
		Ret = Ap_Radio_FM_ChannelSet( Ap_Radio_Log.FM_Channel_Min );
		
		if( Ret == ERR_NONE )
		{
			Ret = Ap_Radio_SeekToLast( &FreqValid );
		}
	}
	else
	if( Ap_Radio_Log.Band_Mode == MODE_RADIO_BAND_AM )
	{
		Ret = Ap_Radio_AM_ChannelSet( Ap_Radio_Log.AM_Channel_Min );
	}
	else
	{
		Ret = ERR_RADIO_BAND_ERR;
	}	
	
	*pFreqValid = FreqValid;
	
	return Ret;	
}





/*---------------------------------------------------------------------------
     TITLE   : Ap_Radio_SeekFromLast
     WORK    : 
     ARG     : void
     RET     : 
     			u16 - Error Code     
---------------------------------------------------------------------------*/
u16 Ap_Radio_SeekFromLast (  u8 *pFreqValid )
{
	u16 Ret;
	u8 FreqValid = 0;
	
	if( Ap_Radio_Log.Band_Mode == MODE_RADIO_BAND_FM ) 
	{
		Ret = Ap_Radio_FM_ChannelSet( Ap_Radio_Log.FM_Channel_Max );
		
		if( Ret == ERR_NONE )
		{
			Ret = Ap_Radio_SeekToFirst( &FreqValid );
		}
	}
	else
	if( Ap_Radio_Log.Band_Mode == MODE_RADIO_BAND_AM )
	{
		Ret = Ap_Radio_AM_ChannelSet( Ap_Radio_Log.AM_Channel_Max );
	}
	else
	{
		Ret = ERR_RADIO_BAND_ERR;
	}	
	
	*pFreqValid = FreqValid;
	
	return Ret;		
}

