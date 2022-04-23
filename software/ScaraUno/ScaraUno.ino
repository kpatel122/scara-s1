#include <Arduino.h>
#include "Config.h"
#include "MultiDriver.h"
#include "SyncDriver.h"
#include "Axis.h"
#include "AxisController.h"
#include "PinChangeInterrupt.h"
#include <ServoSpeed.h>
#include <gcode.h>

/*GCODE functions*/

void G28_Home(); // home axis
void G1_Move();  // joint move

void E0_EffectorClose(); // end effector close
void E1_EffectorOpen();  // end effector open
void E2_EffectorSpeed(); // end effector speed

void D0_Demo();          // demo
void M114_GetPosition(); // print psotions of all joints

/*gcode callbacks*/
#define NUM_GCODE_COMMANDS 7
/*
G-Code reference

G28- Home all axis
G1 [a, b,c,z] [value]- Move axis

E0- Close gripper
E1- Open Gripper
E2 [s] [value]- gripper speed, default 14

D0- demo mode
*/

// callback array
commandscallback commands[NUM_GCODE_COMMANDS] = {{"G28", G28_Home}, //map the gcode command to their callback function
                                                 {"G1", G1_Move}, 
                                                 {"E0", E0_EffectorClose}, 
                                                 {"E1", E1_EffectorOpen}, 
                                                 {"E2", E2_EffectorSpeed}, 
                                                 {"D0", D0_Demo}, 
                                                 {"M114", M114_GetPosition}};

// gcode commands
gcode Commands(NUM_GCODE_COMMANDS, commands);

// configuration values for configuring from serial moitor
#define CONFIG_MAX_SERVO_ANGLE 'h'
#define CONFIG_MIN_SERVO_ANGLE 'g'
#define CONFIG_GRIPPER_SPEED 'j'
#define GRIPPER_OPEN 's'
#define GRIPPER_CLOSE 'a'
#define SERVO_PULSE_WIDTH_MIN 544 
#define SERVO_PULSE_WIDTH_MAX 2400

// axis acontroller
AxisController axisController;

// servo controller
CServoSpeed gripperServo;

//configuration input
int incomingByte = 0;

//gripper servo values
int servoMin = 90;
int servoMax = 180;
int servoSpeed = 14;
 
//non blocking delay
unsigned long period = 2000;
unsigned long time_now = 0;

/**
 * @brief used for demo movement only
 *
 * Wrapper for void SetAnimationSpeed(uint assetID, float speed) as unity WebGl SendMessage only accepts a single string or int parameter
 * this method is called from the website with parameters split using the STRPARAMS_DELIMINATOR
 */

void PickAndPlace()
{

  // move 1
  axisController.Move(0, -45, 0, 0);
  axisController.Move(-30, 0, 0, 0);
  E0_EffectorClose();
  time_now = millis();
  while (1)
  {
    if (millis() >= time_now + period)
    {
      time_now += period;
      Serial.println("Hello");
      break;
    }
    else
    {
      gripperServo.update();
    }
  }

  axisController.Move(30, 0, 0, 0);
  axisController.Move(0, 70, 0, 0);

  E1_EffectorOpen();
  time_now = millis();
  while (1)
  {
    if (millis() >= time_now + period)
    {
      time_now += period;
      Serial.println("Hello");
      break;
    }
    else
    {
      gripperServo.update();
    }
  }

  axisController.Move(0, -30, 0, 0);
}

/**
 * @brief D0 command, demo movement for testing only
 */
void D0_Demo()
{
  // home axis
  HomeAxis();

  //move z to initial position 
  axisController.Move(30, 0, 0, 0);
  
  //run pick and place demo
  PickAndPlace();
}


/**
 * @brief M114 command, prints the current axis positions
 */
void M114_GetPosition()
{
  //print all axis positions and units
  Serial.print("Z:");
  Serial.print(axisController.pGetAxis(Z_AXIS)->GetAngle());
  Serial.print("mm A:");
  Serial.print(axisController.pGetAxis(A_AXIS)->GetAngle());
  Serial.print("deg B:");
  Serial.print(axisController.pGetAxis(B_AXIS)->GetAngle());
  Serial.print("deg C:");
  Serial.print(axisController.pGetAxis(C_AXIS)->GetAngle());
}


/**
 * @brief G28 command, homes all axis
 * @return None
 */
void G28_Home()
{
  HomeAxis();
}


/**
 * @brief G1 command, axis movement
 * 
 * G1 direct axis movement
 * e.g  G1 Z10 #moves the Z axis up 30mm
 *      G1 A30 B20 #moves the A axis 30 deg and B axis 20 deg
 * <b>Motor sync is reliable up to 3 axis at a time, should not attempt all 4 axis movment in a single G1 command, split the movements instead</b>
 * @param A,B,C,Z axis values stored in serial string
 * @return None
 */
void G1_Move()
{
  Commands.comment("G1 called");

  double a, b, c, z; // axis values

  //set all initial axis values
  a = b = c = z = 0; 

  //check for axis values present in the commands data structure
  if (Commands.availableValue('Z'))
    z = Commands.GetValue('Z');
  if (Commands.availableValue('A'))
    a = Commands.GetValue('A');
  if (Commands.availableValue('B'))
    b = Commands.GetValue('B');
  if (Commands.availableValue('C'))
    c = Commands.GetValue('C');

  //print axis values, debugging only
  Serial.println("axis values are");
  Serial.print(" a ");
  Serial.println(a);
  Serial.print(" b ");
  Serial.println(b);
  Serial.print(" c ");
  Serial.println(c);
  Serial.print(" z ");
  Serial.println(z);

  //b axis will offset the c axis so we must keep b and c in sync
  c = c + b;

  //send the movement command to the axis controller
  axisController.Move(z, a, b, c);
}

/**
 * @brief E0 command, closes gripper
 * @return None
 */
void E0_EffectorClose()
{
  // close gripper
  gripperServo.write(servoMax); 
}

/**
 * @brief E1 command, opens gripper
 * @return None
 */
void E1_EffectorOpen()
{

   
  gripperServo.write(servoMin); // open gripper
}

/**
 * @brief E2 command, sets the speed of the gripper servo
 * 
 * e.g  E2 S100 #sets the servo speed to 100
 * @param 'S'[speed value]
 * @return None
 */
void E2_EffectorSpeed()
{
  int s = servoSpeed; // get the current servo speed

  //check if the 'S' param has been given in the gcode command
  if (Commands.availableValue('S'))
  {
    //get the speed value
    s = Commands.GetValue('S');
    
    //set the global servo speed variable
    servoSpeed = s;

    //assign the new speed value to the servo
    gripperServo.setSpeed(servoSpeed);
  }
}


/**
 * @brief limit pins interrupt functions
 * @return None
 */
void PinISRZ() { axisController.pGetAxis(Z_AXIS)->LimitHit(); }
void PinISRA() { axisController.pGetAxis(A_AXIS)->LimitHit(); }
void PinISRB() { axisController.pGetAxis(B_AXIS)->LimitHit(); }
void PinISRC() { axisController.pGetAxis(C_AXIS)->LimitHit(); }


/**
 * @brief initialise interrupts, these are triggered when limit switches are hit  
 * @return None
 */
void InitISR()
{
  //set pin modes
  pinMode(B_AXIS_LIMIT, INPUT_PULLUP);
  pinMode(A_AXIS_LIMIT, INPUT_PULLUP);
  pinMode(Z_AXIS_LIMIT, INPUT_PULLUP);
  pinMode(C_AXIS_LIMIT, INPUT_PULLUP);

  //attach the interrupt functions
  attachPCINT(digitalPinToPCINT(B_AXIS_LIMIT), PinISRB, CHANGE);
  attachPCINT(digitalPinToPCINT(A_AXIS_LIMIT), PinISRA, CHANGE);
  attachPCINT(digitalPinToPCINT(Z_AXIS_LIMIT), PinISRZ, CHANGE);
  attachPCINT(digitalPinToPCINT(C_AXIS_LIMIT), PinISRC, CHANGE);
}

/**
 * @brief initialise axis- creates axis objects and sets the interrupts for their limit switches  
 * @return None
 */
void InitAxis()
{

  // create axis
  axisController.AddAxis(Z_AXIS, new Axis(Z_STEPS, Z_DIR, Z_STEP, ENABLE_PIN,
                                          Z_STEPS_PER_MM, Z_AXIS_LIMIT, INVERT_Z, Z_MICROSTEPS, Z_HOME_RPM, Z_RETRACT_DIST, Z_RPM,
                                          Z_MAX_DIST, Z_ACCEL, Z_DECEL));

  axisController.AddAxis(A_AXIS, new Axis(A_STEPS, A_DIR, A_STEP, ENABLE_PIN,
                                          A_STEPS_PER_DEGREE, A_AXIS_LIMIT, INVERT_A, A_MICROSTEPS, A_HOME_RPM / 2, A_RETRACT_DIST, A_RPM,
                                          A_MAX_DIST, A_ACCEL, A_DECEL));

  axisController.AddAxis(B_AXIS, new Axis(B_STEPS, B_DIR, B_STEP, ENABLE_PIN,
                                          B_STEPS_PER_DEGREE, B_AXIS_LIMIT, INVERT_B, B_MICROSTEPS, B_HOME_RPM, B_RETRACT_DIST, B_RPM,
                                          B_MAX_DIST, B_ACCEL, B_DECEL));

  axisController.AddAxis(C_AXIS, new Axis(C_STEPS, C_DIR, C_STEP, ENABLE_PIN,
                                          C_STEPS_PER_DEGREE, C_AXIS_LIMIT, INVERT_C, C_MICROSTEPS, C_HOME_RPM, C_RETRACT_DIST, C_RPM,
                                          C_MAX_DIST, C_ACCEL, C_DECEL));

  // add axis to sync controller, must create axis' before this call
  axisController.CreateSyncDriveController();

  // attch interrupts
  InitISR();
}

/**
 * @brief initialise the gripper servo  
 * @return None
 */
void InitGripper()
{
  //attach the gripper servo tot he gripper pin
  gripperServo.attach(GRIPPER_PIN, SERVO_PULSE_WIDTH_MIN, SERVO_PULSE_WIDTH_MAX);
  
  //set the servo speed
  gripperServo.setSpeed(servoSpeed);  
}

/**
 * @brief Homes all axis  
 * @return None
 */
void HomeAxis()
{
  //Home each individual axis then move to default position
  axisController.Home(A_AXIS);
  axisController.Move(0, 90, 0, 0); //1 axis default position
  axisController.Home(C_AXIS);
  axisController.Move(0, 0, 0, 90); //c axis default position
  axisController.Home(B_AXIS);
  axisController.Move(0, 0, 90, 0); //b axis default position
  axisController.Home(Z_AXIS);
  axisController.Move(20, 0, 0, 0); //z axis default position
}

void setup()
{
  //start commands library
  Commands.begin(115200, "ok");

  //initialse axis
  InitAxis();

  //initialse gripper
  InitGripper();
}

/**
 * @brief allows the min and max servo angles to be set at runtime 
 * @todo make this a gcode command
 * @return None
 */
void ReadConfig()
{

  //wait for user input
  Serial.print("waiting for input value");
  while (!Serial.available())
  {
  }

  //read the user input
  int value = Serial.parseInt();

  //check which config value is being set
  if (incomingByte == CONFIG_MAX_SERVO_ANGLE)
  {
    //set the max angle value of the servo
    Serial.print("setting max angle ");
    Serial.println(value);
    servoMax = value;
  }

  else if (incomingByte == CONFIG_MIN_SERVO_ANGLE)
  {
    //set the min angle value of the servo
    Serial.print("setting min angle ");
    Serial.println(value);
    servoMin = value;
  }

  else if (incomingByte == CONFIG_GRIPPER_SPEED)
  {
    //set the speed of the servo
    Serial.print("setting speed ");
    Serial.println(value);
    servoSpeed = value;
    gripperServo.setSpeed(servoSpeed);
  }
}

// the loop function runs over and over again forever
void loop()
{
  //read available gcode commands
  Commands.available();

  //update the servo
  gripperServo.update();
}
