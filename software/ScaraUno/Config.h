#ifndef CONFIG_H
#define CONFIG_H

#define STEPPER_ENABLE LOW //GRBL use active low
#define STEPPER_DISABLE HIGH  





#ifdef ARDUINO_AVR_UNO || ARDUINO_AVR_MEGA2560 
#pragma message ("Using Board Arduino Uno or Mega" )
 
#define NUM_AXIS 4



#define HOME_FIRST_SEEK_RPM 10 //firct pass to home
#define HOME_SEEK_DEG -360*2 
#define RETRACT_DISTANCE_DEG 50 //distance to retract after first home limit hit, setup for second seek
#define HOME_SECOND_SEEK_RPM 5 //second slower pass to find final home


#define STEPS 800

#define MICROSTEPS 4
#define RPM 100

#define ACCL 2000
#define DECL 2000

//pins for uno, taken from GRBL shield
#define ENABLE_PIN 8

//map grbl pins to joints
#define Z_AXIS 0
#define A_AXIS 1
#define B_AXIS 2
#define C_AXIS 3

//limit switched
#define X_LIMIT 9 //grbl limit switch pins
#define Y_LIMIT 10
#define Z_LIMIT 11


#define X_STEP_BIT      2  // Uno Digital Pin 2
#define Y_STEP_BIT      3  // Uno Digital Pin 3
#define Z_STEP_BIT      4  // Uno Digital Pin 4

#define X_DIRECTION_BIT   5  // Uno Digital Pin 5
#define Y_DIRECTION_BIT   6  // Uno Digital Pin 6
#define Z_DIRECTION_BIT   7  // Uno Digital Pin 7



//maps robot axis to grbl shield pins
#define Z_STEP X_STEP_BIT   
#define Z_DIR  X_DIRECTION_BIT
#define Z_AXIS_LIMIT  Z_LIMIT //this is where the Z limit pin goes on the shield

#define A_STEP Y_STEP_BIT   //maps to grbl y-axis
#define A_DIR  Y_DIRECTION_BIT
#define A_AXIS_LIMIT  Y_LIMIT //this is where the Y limit pin goes on the shield

#define B_STEP Z_STEP_BIT  //maps to grbl z-axis
#define B_DIR  Z_DIRECTION_BIT
#define B_AXIS_LIMIT  X_LIMIT

#define C_STEP  12  //maps to grbl shield 4th axis
#define C_DIR 13  //Digital Pin 12 for Stepping signal and Digital Pin 13 as direction signal
#define C_AXIS_LIMIT  0 //need to work this out


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