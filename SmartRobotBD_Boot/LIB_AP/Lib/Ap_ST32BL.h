//----------------------------------------------------------------------------
//    프로그램명 	: ST32BL 관련 함수 헤더
//
//    만든이     	: 
//
//    날  짜     	: 
//    
//    최종 수정  	: 
//
//    MPU_Type 		:
//
//    파일명     	: AP_ST32BL.h
//----------------------------------------------------------------------------
                                                                                                 
                                                                                                 
#ifndef  _AP_ST32BL_H
#define  _AP_ST32BL_H


#ifdef   AP_ST32BL_LOCAL
#define  EXT_AP_ST32BL_DEF 
#else
#define  EXT_AP_ST32BL_DEF     extern
#endif


#include "Ap.h"


enum
{
  STM32_OK = 0,
  STM32_PORT_OPEN_ERROR,
  STM32_COMM_ERROR,
  STM32_INIT_ERROR,
  STM32_TIMEOUT_ERROR,
  STM32_NOT_INITIALIZED_ERROR
};

// Communication data
#define STM32_COMM_ACK      0x79
#define STM32_COMM_NACK     0x1F
#define STM32_COMM_TIMEOUT  2000000
#define STM32_WRITE_BUFSIZE 256
#define STM32_FLASH_START_ADDRESS 0x08000000
enum
{
  STM32_CMD_INIT = 0x7F,
  STM32_CMD_GET_COMMAND = 0x00,
  STM32_CMD_ERASE_FLASH = 0x43,
  STM32_CMD_EXTENDED_ERASE_FLASH = 0x44,
  STM32_CMD_GET_ID = 0x02,
  STM32_CMD_WRITE_FLASH = 0x31,
  STM32_CMD_WRITE_UNPROTECT = 0x73,
  STM32_CMD_READ_FLASH = 0x11,
  STM32_CMD_GO = 0x21
};


EXT_AP_ST32BL_DEF u8 Ap_ST32BL_RunBoot;
EXT_AP_ST32BL_DEF u8 Ap_ST32BL_UartPort;


EXT_AP_ST32BL_DEF void Ap_ST32BL_Init( void );
EXT_AP_ST32BL_DEF void Ap_ST32BL_Main( void );

EXT_AP_ST32BL_DEF u8   Ap_ST32BL_ReceiveByte( void );
EXT_AP_ST32BL_DEF u8   Ap_ST32BL_EraseFlash( void );
EXT_AP_ST32BL_DEF u8   Ap_ST32BL_WriteFlash( u32 Address, u8 *pData, u32 DataSize );
EXT_AP_ST32BL_DEF void Ap_ST32BL_JumpToApp( u32 Address );

#endif
