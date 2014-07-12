#pragma once


#define DEBUG_OUT        1



// SkyRover
#define LED0_GPIO   GPIOB
#define LED0_PIN    Pin_4 // PB4 (LED)
#define LED1_GPIO   GPIOB
#define LED1_PIN    Pin_5 // PB5 (LED)
#define BEEP_GPIO   GPIOB
#define BEEP_PIN    Pin_12 // PA12 (Buzzer)
#define BARO_GPIO   GPIOC
#define BARO_PIN    Pin_13

#define GYRO
#define ACC
#define MAG
#define BARO
//#define LEDRING
//#define SONAR
//#define BUZZER

#define SENSORS_SET (SENSOR_ACC | SENSOR_BARO | SENSOR_MAG )


// Helpful macros
#define LED0_TOGGLE              digitalToggle(LED0_GPIO, LED0_PIN);
#define LED0_OFF                 digitalHi(LED0_GPIO, LED0_PIN);
#define LED0_ON                  digitalLo(LED0_GPIO, LED0_PIN);

#define LED1_TOGGLE              digitalToggle(LED1_GPIO, LED1_PIN);
#define LED1_OFF                 digitalHi(LED1_GPIO, LED1_PIN);
#define LED1_ON                  digitalLo(LED1_GPIO, LED1_PIN);

#ifdef BEEP_GPIO
#define BEEP_TOGGLE              digitalToggle(BEEP_GPIO, BEEP_PIN);
#define BEEP_OFF                 systemBeep(false);
#define BEEP_ON                  systemBeep(true);
#else
#define BEEP_TOGGLE              ;
#define BEEP_OFF                 ;
#define BEEP_ON                  ;
#endif