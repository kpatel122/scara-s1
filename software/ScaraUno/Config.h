#ifndef CONFIG_H
#define CONFIG_H

#define STEPPER_ENABLE LOW //GRBL use active low
#define STEPPER_DISABLE HIGH  





#ifdef ARDUINO_AVR_UNO || ARDUINO_AVR_MEGA2560 
#pragma message ("Using Board Arduino Uno or Mega" )
 
#define NUM_AXIS 4



#define HOME_FIRST_SEEK_RPM 20 //first pass to home
#define HOME_SEEK_DEG -400 
#define RETRACT_DISTANCE_DEG 10 //distance to retract after first home limit hit, setup for second seek
#define HOME_SECOND_SEEK_RPM 5 //second slower seek to find final home


#define STEPS 800

#define MICROSTEPS 4
#define RPM 20

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
#define X_LIMIT 9 //grbl shield limit switch pins
#define Y_LIMIT 10
#define Z_LIMIT 11

#define INVERT_Z 0 //0 for no inverse, 1 for invert
#define INVERT_A 0
#define INVERT_B 0
#define INVERT_C 0


#define X_STEP_BIT      2  // Uno Digital Pin 2 as defined by grbl cnc shield
#define Y_STEP_BIT      3  // Uno Digital Pin 3
#define Z_STEP_BIT      4  // Uno Digital Pin 4

#define X_DIRECTION_BIT   5  // Uno Digital Pin 5 as defined by grbl cnc shield
#define Y_DIRECTION_BIT   6  // Uno Digital Pin 6
#define Z_DIRECTION_BIT   7  // Uno Digital Pin 7

//maps stepper pin entry to grbl cnc shield pins
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

//motor steps calculations
#define A_MOTOR_GEAR_RATIO ((float)20/ (float)130)  //motor pulley / idle pulley
#define A_MOTOR_MICROSTEPS 16
#define A_MOTOR_STEP_DEGREE 1.8

#define B_MOTOR_GEAR_RATIO ((float)16/ (float)60)  //C has same ratio
#define B_MOTOR_MICROSTEPS 16
#define B_MOTOR_STEP_DEGREE 1.8

//calculate how many steps needed to move 1 degree based on gear ratios and microsteps
#define A_STEPS_PER_DEGREE (float)(1 / ((float)A_MOTOR_GEAR_RATIO * ((float)A_MOTOR_STEP_DEGREE / (float)A_MOTOR_MICROSTEPS)))
#define B_STEPS_PER_DEGREE (float)(1 / ((float)B_MOTOR_GEAR_RATIO * ((float)B_MOTOR_STEP_DEGREE / (float)B_MOTOR_MICROSTEPS)))
#define C_STEPS_PER_DEGREE B_STEPS_PER_DEGREE
#define Z_STEPS_PER_MM 400

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