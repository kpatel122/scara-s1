#ifndef CONFIG_H
#define CONFIG_H

#define STEPPER_ENABLE LOW //GRBL use active low
#define STEPPER_DISABLE HIGH  





#ifdef ARDUINO_AVR_UNO || ARDUINO_AVR_MEGA2560 
#pragma message ("Using Board Arduino Uno or Mega" )
 

#define LIMIT_X 9 //grbl limit switch pins
#define LIMIT_Y 10
#define LIMIT_Z 11

#define NUM_AXIS 4

#define STEPS 800

#define MICROSTEPS 4
#define RPM 100

#define ACCL 2000
#define DECL 2000

//pins for uno, taken from GRBL shield
#define ENABLE_PIN 8

#define X_STEP_BIT      2  // Uno Digital Pin 2
#define Y_STEP_BIT      3  // Uno Digital Pin 3
#define Z_STEP_BIT      4  // Uno Digital Pin 4

#define X_DIRECTION_BIT   5  // Uno Digital Pin 5
#define Y_DIRECTION_BIT   6  // Uno Digital Pin 6
#define Z_DIRECTION_BIT   7  // Uno Digital Pin 7

#define A_STEP X_STEP_BIT   //maps to grbl x-axis
#define A_DIR  X_DIRECTION_BIT

#define B_STEP Y_STEP_BIT   //maps to grbl y-axis
#define B_DIR  Y_DIRECTION_BIT

#define C_STEP Z_STEP_BIT  //maps to grbl z-axis
#define C_DIR  Z_DIRECTION_BIT

#define D_STEP  12  //maps to grbl shield 4th axis
#define D_DIR 13  //Digital Pin 12 for Stepping signal and Digital Pin 13 as direction signal

#endif


#ifdef ESP32

#pragma message ("Using Board ESP32" )

/*
Arduino to D1 R32 pin mapping
STEPX 2=26
STEPY 3=25
STEPZ 4=17

DIRX  5=16
DIRY  6=27
DIRZ  7=14

8=12

12=19
13=18
*/

#define ARD2 D26 
#define ARD3 25  
#define ARD4 17  
#define ARD5 16  
#define ARD6 27  
#define ARD7 14  
#define ARD8 12
#define ARD12 19  
#define ARD13 18

//pins for uno, taken from GRBL shield
#define ENABLE_PIN ARD8

#define X_STEP_BIT      ARD2  // Uno Digital Pin 2
#define Y_STEP_BIT      ARD3  // Uno Digital Pin 3
#define Z_STEP_BIT      ARD4  // Uno Digital Pin 4

#define X_DIRECTION_BIT   ARD5  // Uno Digital Pin 5
#define Y_DIRECTION_BIT   ARD6  // Uno Digital Pin 6
#define Z_DIRECTION_BIT   ARD7  // Uno Digital Pin 7

#define A_STEP X_STEP_BIT   //maps to grbl x-axis
#define A_DIR  X_DIRECTION_BIT

#define B_STEP Y_STEP_BIT   //maps to grbl y-axis
#define B_DIR  Y_DIRECTION_BIT

#define C_STEP Z_STEP_BIT  //maps to grbl z-axis
#define C_DIR  Z_DIRECTION_BIT

#define D_STEP ARD12  //maps to grbl shield 4th axis
#define D_DIR ARD13   //Digital Pin 12 for Stepping signal and Digital Pin 13 as direction signal

#endif



#endif