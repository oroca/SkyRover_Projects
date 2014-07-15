
#include "SkyRover.h"


//-- 외부 변수
//
extern int16_t angle[2];
extern int16_t heading;


static void uart1_putc(void *p, char c)
{
    serialWrite(core.mainport, c);
}

static void usb_putc(void *p, char c)
{
    Hw_VCom_Putch(c);
}


/*---------------------------------------------------------------------------
     TITLE	: begin
     WORK	:
     ARG	:
     RET	:
---------------------------------------------------------------------------*/
void SkyRover::begin(void) 
{
	DEBUG_PRINT("SkyRover Begin\r\n");
}


/*---------------------------------------------------------------------------
     TITLE	: begin
     WORK	:
     ARG	:
     RET	:
---------------------------------------------------------------------------*/
void SkyRover::delay( int32_t delay_data ) 
{
	osDelay( delay_data );
}


/*---------------------------------------------------------------------------
     TITLE	: usb_printf
     WORK	:
     ARG	:
     RET	:
---------------------------------------------------------------------------*/
void SkyRover::usb_printf( char *format, ... )
{
	//Hw_VCom_Printf( format );
    va_list va;
    va_start(va, format);
    tfp_format(NULL, usb_putc, format, va);
    va_end(va);		
}


/*---------------------------------------------------------------------------
     TITLE	: uart1_printf
     WORK	:
     ARG	:
     RET	:
---------------------------------------------------------------------------*/
void SkyRover::uart1_printf( char *format, ... )
{
	//printf( format );
    va_list va;
    va_start(va, format);
    tfp_format(NULL, uart1_putc, format, va);
    va_end(va);	
}


/*---------------------------------------------------------------------------
     TITLE	: imu_GetRollAngle
     WORK	:
     ARG	:
     RET	:
---------------------------------------------------------------------------*/
int16_t SkyRover::imu_GetAngleRoll( void )
{
	return angle[ROLL];
}


/*---------------------------------------------------------------------------
     TITLE	: imu_GetPitchAngle
     WORK	:
     ARG	:
     RET	:
---------------------------------------------------------------------------*/
int16_t SkyRover::imu_GetAnglePitch( void )
{
	return angle[PITCH];
}


/*---------------------------------------------------------------------------
     TITLE	: imu_GetYawAngle
     WORK	:
     ARG	:
     RET	:
---------------------------------------------------------------------------*/
int16_t SkyRover::imu_GetAngleYaw( void )
{
	return heading;
}

