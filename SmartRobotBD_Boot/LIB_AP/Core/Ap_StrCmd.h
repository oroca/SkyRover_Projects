//----------------------------------------------------------------------------
//    프로그램명 	: StrCmd 관련 함수 헤더
//
//    만든이     	: 
//
//    날  짜     	: 
//    
//    최종 수정  	: 
//
//    MPU_Type 		:
//
//    파일명     	: AP_StrCmd.h
//----------------------------------------------------------------------------
                                                                                                 
                                                                                                 
#ifndef  _AP_STRCMD_H
#define  _AP_STRCMD_H


#ifdef   AP_STRCMD_LOCAL
#define  EXT_AP_STRCMD_DEF 
#else
#define  EXT_AP_STRCMD_DEF     extern
#endif


#include "Ap.h"



//----- 상수 선언
//

#define  AP_STRCMD_DIC_MAX         	20    // 총 명령어 갯수
#define  AP_STRCMD_STR_MAX      	60    // 한개의 명령어 문자열 길이
#define  AP_STRCMD_COMMENT_STR_MAX 	20    // 명령어 설명 문자열 길이

#define  AP_STRCMD_FRAME_OPCODE		'#'
#define  AP_STRCMD_FRAME_PRINT		'*'
#define  AP_STRCMD_FRAME_RET		'>'



typedef void (*Ap_StrCmd_FuncType)( u8,void*);  // 명령어가 실행할 함수 타입.


typedef struct
{
	u8  Str[AP_STRCMD_STR_MAX];
} AP_STRCMD_CMD_OBJ;


//----- 프로그램 사전에 연결된 함수 실행
//
#define AP_STRCMD_DIC_FUNC_EXEC(Opcode,arg)                 ( *Ap_StrCmd_Dic_Ptr[ Opcode ].Func_Ptr )( Opcode, arg )       
#define AP_STRCMD_DIC_FUNC_SET(Opcode,FuncData)             Ap_StrCmd_Dic_Ptr[Opcode].Func_Ptr = FuncData;          
#define AP_STRCMD_DIC_FUNC(Opcode)                          Ap_StrCmd_Dic_Ptr[Opcode].Func_Ptr



typedef struct
{
	char NameStr[20];
	char MenuStr[ AP_STRCMD_COMMENT_STR_MAX ];
	
	Ap_StrCmd_FuncType  Func_Ptr;
} AP_STRCMD_OBJ;


EXT_AP_STRCMD_DEF  u8      Ap_StrCmd_DlyCheck;   // 딜레이 함수 내애서 사용할 변수
EXT_AP_STRCMD_DEF  u8      Ap_StrCmd_LoopCheck;	 // 루프 함수 내애서 사용할 변수.


EXT_AP_STRCMD_DEF void Ap_StrCmd_Init( void );
EXT_AP_STRCMD_DEF void Ap_StrCmd_SetFunc_Putch( void (*FuncPtr)( char ) );
EXT_AP_STRCMD_DEF void Ap_StrCmd_CallBack_RxdISR( char Ch );

EXT_AP_STRCMD_DEF void Ap_StrCmd_ShowInfo( u8 OpCode,  void *arg );
EXT_AP_STRCMD_DEF void Ap_StrCmd_ShowMenuHelp( u8 OpCode,  void *arg );
EXT_AP_STRCMD_DEF void Ap_StrCmd_ShowMenu( u8 OpCode,  void *arg );

EXT_AP_STRCMD_DEF u16  Ap_StrCmd_GetSize( void );
EXT_AP_STRCMD_DEF u8   Ap_StrCmd_AddCmd( char *NameStr, char *MenuStr,  Ap_StrCmd_FuncType Func_Ptr );

EXT_AP_STRCMD_DEF void Ap_StrCmd_CmdClear( void );

EXT_AP_STRCMD_DEF u8   Ap_StrCmd_ExeCmd( void );
EXT_AP_STRCMD_DEF s16  Ap_StrCmd_ExeFunc( void );

EXT_AP_STRCMD_DEF s16  Ap_StrCmd_Loop(void);
EXT_AP_STRCMD_DEF s16  Ap_StrCmd_Delay( long DlyData  );

EXT_AP_STRCMD_DEF void Ap_StrCmd_Putch( char Ch );
EXT_AP_STRCMD_DEF void Ap_StrCmd_Print( char *Buffer );
EXT_AP_STRCMD_DEF void Ap_StrCmd_Printf( char *Buffer, ... );


#endif
