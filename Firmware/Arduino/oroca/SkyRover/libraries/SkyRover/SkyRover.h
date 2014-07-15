/*
 * TwoWire.h - TWI/I2C library for Arduino Due
 * Copyright (c) 2011 Cristian Maglie <c.maglie@bug.st>.
 * All rights reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifndef SkyRover_h
#define SkyRover_h


#include "Arduino.h"


class SkyRover
{
public:
	void begin();
	void delay( int32_t delay_data );
	void usb_printf( char *format, ... );
	void uart1_printf( char *format, ... );

	int16_t imu_GetAngleRoll( void );
	int16_t imu_GetAnglePitch( void );
	int16_t imu_GetAngleYaw( void );


private:
};


#endif

