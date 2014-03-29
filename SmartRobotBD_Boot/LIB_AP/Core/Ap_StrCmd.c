//----------------------------------------------------------------------------
//    프로그램명 	: StrCmd 관련 함수
//
//    만든이     	: 
//
//    날  짜     	: 
//    
//    최종 수정  	: 
//
//    MPU_Type 		:
//
//    파일명     	: AP_StrCmd.c
//----------------------------------------------------------------------------





//----- 헤더파일 열기
//
#define  AP_STRCMD_LOCAL  

#include "Ap_StrCmd.h"
#include "Ap_StrCmd_Q.h"


//#include <stdarg.h >
#include <ctype.h>
#include <string.h>


//-- 내부 선언
//



//-- 내부 변수
//
AP_STRCMD_OBJ 	Ap_StrCmd_Dic_Ptr[AP_STRCMD_DIC_MAX];	// 명령어 정보를 가지고 있는 구조체.

volatile u16 	Ap_StrCmd_Size;							// 명령어 갯수


volatile u16 	Ap_StrCmd_ChSize;     					// 시리얼로 들어오는 문자 갯수
volatile char	Ap_StrCmd_CmdCheck;   					// 명령어가 완료 되었는지?
char			Ap_StrCmd_CmdStr[AP_STRCMD_STR_MAX];	// 명령어 문자열
char			Ap_StrCmd_CmdStrExe[AP_STRCMD_STR_MAX]; // 실행하기 위한 

void            (*Ap_StrCmd_PutchPtr)( char );  		// 시리얼로 데이터 1바이트 보내는것.
void            (*Ap_StrCmd_PrintPtr)( char *);  

volatile u8		Ap_StrCmd_EchoEnable = FALSE;

//-- 내부 함수
//
u16  Ap_StrCmd_StrLen( char *Dest );
u16  Ap_StrCmd_CmdStrLen( char *Dest );
void Ap_StrCmd_StrCopy( char *Dest, char *Origin );
                              

void Ap_StrCmd_Echo( u8 OpCode,  void *arg );


void Ap_StrCmd_PutFunc( char Ch )
{
	Hw_Uart_Putch( HW_USE_UART_CH_STRCMD, Ch );
}



/*---------------------------------------------------------------------------
     TITLE   : Ap_StrCmd_Init
     WORK    : 
     ARG     : void
     RET     : void   
---------------------------------------------------------------------------*/
void Ap_StrCmd_Init( void )
{
	int i;
		
	//----- 명령어 사전 포인터 초기화
	//
	for( i=0; i<AP_STRCMD_DIC_MAX; i++)
	{
		AP_STRCMD_DIC_FUNC(i)   = NULL_FUNC;  // 모두 어떠한 함수도 가르키지 않는다
	}    
		
	Ap_StrCmd_Size     	= 0;				
	Ap_StrCmd_ChSize   	= 0;
	Ap_StrCmd_CmdCheck 	= 0;	
	
	Ap_StrCmd_PutchPtr 	= NULL;
	Ap_StrCmd_PrintPtr 	= NULL;
	
	Ap_StrCmd_EchoEnable = FALSE;

	Ap_StrCmd_AddCmd( "HELP", "Menu Help",        Ap_StrCmd_ShowMenuHelp );	
	Ap_StrCmd_AddCmd( "MENU", "Display Menu",     Ap_StrCmd_ShowMenu );
	Ap_StrCmd_AddCmd( "INFO", "Info",     		  Ap_StrCmd_ShowInfo );
	Ap_StrCmd_AddCmd( "ECHO", "Echo",     		  Ap_StrCmd_Echo );


	Ap_StrCmd_Q_Init();


	//-- 통신 함수 설정 
	//
	Ap_StrCmd_SetFunc_Putch( Ap_StrCmd_PutFunc );

	Hw_Uart_SetReceiveFuncISR( HW_USE_UART_CH_STRCMD, Ap_StrCmd_CallBack_RxdISR );
}










/*---------------------------------------------------------------------------
     TITLE   : Ap_StrCmd_Menu
     WORK    : 
     ARG     : void
     RET     : void   
---------------------------------------------------------------------------*/
void Ap_StrCmd_ShowMenu( u8 OpCode, void *arg )
{
	u16 i;
	char StrBuf[200];
	
	OpCode = OpCode;
	arg = arg;
	
	for( i=0; i<Ap_StrCmd_GetSize(); i++ )
	{
		Lb_sprintf( StrBuf, "%c%s\n", AP_STRCMD_FRAME_PRINT, Ap_StrCmd_Dic_Ptr[i].NameStr );
		Ap_StrCmd_Print( StrBuf );
	}			

	Ap_StrCmd_CmdClear();
}





/*---------------------------------------------------------------------------
     TITLE   : Ap_StrCmd_ShowMenuHelp
     WORK    : 
     ARG     : void
     RET     : void   
---------------------------------------------------------------------------*/
void Ap_StrCmd_ShowMenuHelp( u8 OpCode,  void *arg )
{
	u16 i;
	char StrBuf[100];
	
	arg = arg;

	
	for( i=0; i<Ap_StrCmd_GetSize(); i++ )
	{
		Lb_sprintf( StrBuf, "%c<%s> - %s\n", AP_STRCMD_FRAME_PRINT, Ap_StrCmd_Dic_Ptr[i].NameStr, Ap_StrCmd_Dic_Ptr[i].MenuStr );			
		Ap_StrCmd_Print( StrBuf );
	}			
	
	Ap_StrCmd_CmdClear();

}





/*---------------------------------------------------------------------------
     TITLE   : Ap_StrCmd_ShowInfo
     WORK    : 
     ARG     : void
     RET     : void   
---------------------------------------------------------------------------*/
void Ap_StrCmd_ShowInfo( u8 OpCode,  void *arg )
{
	OpCode = OpCode;
	arg = arg;

	Ap_StrCmd_Printf( "%c----------------------------------------- \r\n", AP_STRCMD_FRAME_PRINT );
	Ap_StrCmd_Printf( "%c             Ap_StrCmd v0.1               \r\n", AP_STRCMD_FRAME_PRINT );
	Ap_StrCmd_Printf( "%c                                          \r\n", AP_STRCMD_FRAME_PRINT );
	Ap_StrCmd_Printf( "%c        made by : Cho Han Cheol           \r\n", AP_STRCMD_FRAME_PRINT );	
	Ap_StrCmd_Printf( "%c----------------------------------------- \r\n", AP_STRCMD_FRAME_PRINT );
	

	Ap_StrCmd_CmdClear();
}






/*---------------------------------------------------------------------------
     TITLE   : Ap_StrCmd_Echo
     WORK    : 
     ARG     : void
     RET     : void   
---------------------------------------------------------------------------*/
void Ap_StrCmd_Echo( u8 OpCode,  void *arg )
{
	char CmdMain[10];
	char Arg1[10];

	OpCode = OpCode;
	arg = arg;

	sscanf(arg, "%s %s", CmdMain, Arg1);

	if( strncmp( Arg1, "ON", 2 ) == 0 )
	{
		Ap_StrCmd_EchoEnable = TRUE;
		Ap_StrCmd_Printf("%cEcho on\n", AP_STRCMD_FRAME_PRINT);
	}
	else
	{
		Ap_StrCmd_EchoEnable = FALSE;
		Ap_StrCmd_Printf("%cEcho off\n", AP_STRCMD_FRAME_PRINT);	
	}

	Ap_StrCmd_CmdClear();
}




/*---------------------------------------------------------------------------
     TITLE   : Ap_StrCmd_SetFunc_Putch
     WORK    : 
     ARG     : void
     RET     : void   
---------------------------------------------------------------------------*/
void Ap_StrCmd_SetFunc_Putch( void (*FuncPtr)( char ) )
{
	Ap_StrCmd_PutchPtr = FuncPtr;
}





/*---------------------------------------------------------------------------
     TITLE   : Ap_StrCmd_CallBack_RxdISR
     WORK    : 
     ARG     : void
     RET     : void   
---------------------------------------------------------------------------*/
void Ap_StrCmd_CallBack_RxdISR( char Ch )
{	

	if( Ap_StrCmd_ChSize >= AP_STRCMD_STR_MAX )
	{
		Ap_StrCmd_ChSize = 0;
	}
	
	//Ch = toupper( Ch );
	
	Ap_StrCmd_CmdStr[ Ap_StrCmd_ChSize ] = Ch;
	
	Ap_StrCmd_ChSize++;
	

	if( Ch == 0x0D )  // '\n'
	{
		Ap_StrCmd_CmdStr[ Ap_StrCmd_ChSize ] = 0;
		
		Ap_StrCmd_ChSize    = 0;
		Ap_StrCmd_DlyCheck  = 1;
		
		Ap_StrCmd_CmdCheck  = 1;
		Ap_StrCmd_LoopCheck = 1;
		
		Ap_StrCmd_Q_Push( AP_STRCMD_Q_RX_CH, (AP_STRCMD_CMD_OBJ *)Ap_StrCmd_CmdStr );
	}

	if( Ap_StrCmd_EchoEnable == TRUE )
	{
		if( Ap_StrCmd_PutchPtr != NULL_FUNC )
		{

			(*Ap_StrCmd_PutchPtr)( Ch );
			
			if( Ch == '\r' )
			{
				(*Ap_StrCmd_PutchPtr)( '\n' );	
			}
		}		
	}    
}





/*---------------------------------------------------------------------------
     TITLE   : Ap_StrCmd_Putch
     WORK    : 
     ARG     : void
     RET     : void   
---------------------------------------------------------------------------*/
void Ap_StrCmd_Putch( char Ch )
{
	if( Ap_StrCmd_PutchPtr != NULL_FUNC )
	{
		(*Ap_StrCmd_PutchPtr)( Ch );
	}
}





/*---------------------------------------------------------------------------
     TITLE   : Ap_StrCmd_AddCmd
     WORK    : 
     ARG     : void
     RET     : void   
---------------------------------------------------------------------------*/
u8 Ap_StrCmd_AddCmd( char *NameStr, char *MenuStr,  Ap_StrCmd_FuncType Func_Ptr )
{

	if( Ap_StrCmd_Size >= AP_STRCMD_DIC_MAX ) return FALSE;  // 사이즈 오버플로어
	
	Ap_StrCmd_StrCopy( Ap_StrCmd_Dic_Ptr[Ap_StrCmd_Size].NameStr, NameStr );
	Ap_StrCmd_StrCopy( Ap_StrCmd_Dic_Ptr[Ap_StrCmd_Size].MenuStr, MenuStr );
	AP_STRCMD_DIC_FUNC_SET( Ap_StrCmd_Size, Func_Ptr );

	Ap_StrCmd_Size++;
	
	return TRUE;
}





/*---------------------------------------------------------------------------
     TITLE   : Ap_StrCmd_CmdClear
     WORK    : 
     ARG     : void
     RET     : void   
---------------------------------------------------------------------------*/
void Ap_StrCmd_CmdClear( void )
{
	Ap_StrCmd_CmdCheck  = 0;
	Ap_StrCmd_DlyCheck  = 0;
	Ap_StrCmd_LoopCheck = 0;	
}





/*---------------------------------------------------------------------------
     TITLE   : Ap_StrCmd_GetSize
     WORK    : 
     ARG     : void
     RET     : void   
---------------------------------------------------------------------------*/
u16 Ap_StrCmd_GetSize( void )
{
	return Ap_StrCmd_Size;
}





/*---------------------------------------------------------------------------
     TITLE   : Ap_StrCmd_Print
     WORK    : 
     ARG     : void
     RET     : void   
---------------------------------------------------------------------------*/
void Ap_StrCmd_Print( char *Buffer )
{
	while( *Buffer != '\0') 
	{
		if( *Buffer == '\n' )
		{
			Ap_StrCmd_Putch( '\r' );
		}

		Ap_StrCmd_Putch( *Buffer ) ;                              		
		Buffer++;
	}
}





/*---------------------------------------------------------------------------
     TITLE   : Ap_StrCmd_Printf
     WORK    : 
     ARG     : void
     RET     : void   
---------------------------------------------------------------------------*/
void Ap_StrCmd_Printf( char *format, ... )
{
	char Str[200];
	Lb_va_list ap;
	
	Lb_va_start( ap, format );

	Lb_vsprintf( Str, format, ap );	

	Lb_va_end(ap);	
	
	Ap_StrCmd_Print(Str);
}





/*---------------------------------------------------------------------------
     TITLE   : Ap_StrCmd_ExeCmd
     WORK    : 
     ARG     : void
     RET     : void   
---------------------------------------------------------------------------*/
u8 Ap_StrCmd_ExeCmd( void )
{
	u8 Ret = FALSE;

	if( Ap_StrCmd_Q_SIZE(AP_STRCMD_Q_RX_CH) )
	{
		Ap_StrCmd_ExeFunc();
		Ret = TRUE;
	}

	return Ret; 
}





/*---------------------------------------------------------------------------
     TITLE   : Ap_StrCmd_ExeFunc
     WORK    : 
     ARG     : void
     RET     : void   
---------------------------------------------------------------------------*/
s16 Ap_StrCmd_ExeFunc( void )
{
	u16 CmdSize;
	u16 CmdStrSize;	
	u16 CmdBufStrSize;
	s16 CmdCheck;
	u16 i;
	u16 j;

	AP_STRCMD_CMD_OBJ CmdMsg;
	
	
	Disable_ISR();
	Ap_StrCmd_Q_Pop( AP_STRCMD_Q_RX_CH, &CmdMsg );
	Enable_ISR();
	
	CmdSize = Ap_StrCmd_GetSize();
	
	for( i=0; i<CmdSize; i++ )
	{
		CmdStrSize = Ap_StrCmd_StrLen( Ap_StrCmd_Dic_Ptr[i].NameStr );
		CmdBufStrSize = strlen( (char *)CmdMsg.Str );
		
		for( j=0; j<CmdBufStrSize; j++ )
		{
			CmdMsg.Str[j] = toupper( CmdMsg.Str[j] );
		}
		
		CmdCheck = strncmp( Ap_StrCmd_Dic_Ptr[i].NameStr, (char *)CmdMsg.Str, CmdStrSize );
		
		
		if( CmdCheck == 0 && Ap_StrCmd_Dic_Ptr[i].Func_Ptr != NULL )
		{		
			AP_STRCMD_DIC_FUNC_EXEC(i, CmdMsg.Str); 			
			break;
		}
	}
	
	
	return 0;
}





/*---------------------------------------------------------------------------
     TITLE   : Ap_StrCmd_Delay
     WORK    : 
     ARG     : void
     RET     : void   
---------------------------------------------------------------------------*/
s16 Ap_StrCmd_Delay( long DlyData  )
{
	u32 DlyDataIn = 1000;
	u32 i;
	
	Ap_StrCmd_DlyCheck = 0;
	
	while( DlyData-- )
	{
		for( i=0; i<DlyDataIn; i++ )
		{
			if( Ap_StrCmd_DlyCheck == 1 ) break;
		}
	}	
	
	Ap_StrCmd_DlyCheck = 0;
	
	return 0;
}






/*---------------------------------------------------------------------------
     TITLE   : Ap_StrCmd_Loop
     WORK    : 
     ARG     : void
     RET     : void   
---------------------------------------------------------------------------*/
s16 Ap_StrCmd_Loop(void)
{	
	s16 Ret;

	if( Ap_StrCmd_LoopCheck == 1 )
	{
		Ap_StrCmd_LoopCheck = 0;
		Ret = 0;
	}
	else
	{
		Ret = 1;
	}

	return Ret;
}






/*---------------------------------------------------------------------------
     TITLE   : Ap_StrCmd_StrLen
     WORK    : 
     ARG     : void
     RET     : void   
---------------------------------------------------------------------------*/
u16 Ap_StrCmd_StrLen( char *Dest )
{
	u16 Size = 0;

	while(1)
	{
		if( Dest[Size] == 0 ) break;

		Size++;
	}

	return Size;
}





/*---------------------------------------------------------------------------
     TITLE   : Ap_StrCmd_StrCopy
     WORK    : 
     ARG     : void
     RET     : void   
---------------------------------------------------------------------------*/
void Ap_StrCmd_StrCopy( char *Dest, char *Origin )
{
	u16 Size;
	u16 i;

	Size = Ap_StrCmd_StrLen( Origin );

	for( i=0; i<Size; i++ )
	{
		Dest[i] = Origin[i];
	}
	Dest[i] = 0;
}


