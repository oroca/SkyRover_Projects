#include <SkyRover.h>

SkyRover Sky;


void setup() 
{
  // put your setup code here, to run once:
                                     
  Sky.begin();
}

void loop() 
{
  // put your main code here, to run repeatedly:

  Sky.usb_printf("R:%d\t P:%d\t Y:%d\n", Sky.imu_GetAngleRoll()/10
                                       , Sky.imu_GetAnglePitch()/10
                                       , Sky.imu_GetAngleYaw() );
  Sky.delay(100);
}