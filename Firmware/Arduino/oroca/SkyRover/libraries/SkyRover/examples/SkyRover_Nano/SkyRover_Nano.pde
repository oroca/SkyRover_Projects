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

  DEBUG_PRINT("Thread Loop Tick Uart1\r\n");
  osDelay(500);
}
