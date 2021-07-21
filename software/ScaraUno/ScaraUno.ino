#include <Arduino.h>
#include "Config.h"
#include "MultiDriver.h"
#include "SyncDriver.h"
#include "Axis.h"
#include "AxisController.h"
#include "PinChangeInterrupt.h"
#include <ServoSpeed.h>
#include <gcode.h>
 
void Gcode_g28();
void Gcode_g1();


/*gcode callbacks*/
#define NUM_GCODE_COMMANDS 2
commandscallback commands[NUM_GCODE_COMMANDS] = {{"G28",Gcode_g28}, {"G1",Gcode_g1}};
gcode Commands(NUM_GCODE_COMMANDS,commands);

/*config values*/

#define CONFIG_MAX_SERVO_ANGLE 'h' 
#define CONFIG_MIN_SERVO_ANGLE 'g'
#define CONFIG_GRIPPER_SPEED 'j'
#define GRIPPER_OPEN 's'
#define GRIPPER_CLOSE 'a'

AxisController axisController;
CServoSpeed gripperServo;



void Gcode_g28()
{
  HomeAxis();
}

void Gcode_g1()
{
  Commands.comment("G1 called");

  double a,b,c,z; //axis values

  a = b = c = z = 0;

  if(Commands.availableValue('Z'))
      z = Commands.GetValue('Z');
  if(Commands.availableValue('A'))
      a = Commands.GetValue('A');
  if(Commands.availableValue('B'))
      b = Commands.GetValue('B');
  if(Commands.availableValue('C'))
      c = Commands.GetValue('C');

  Serial.println("axis values are");
  Serial.print(" a ");
  Serial.println(a);
  Serial.print(" b ");
  Serial.println(b);
  Serial.print(" c ");
  Serial.println(c);
  Serial.print(" z ");
  Serial.println(z);

  axisController.Move(z,a,b,c);
   
}

void pin_ISRZ() {  axisController.pGetAxis(Z_AXIS)->LimitHit(); }

void pin_ISRA() { axisController.pGetAxis(A_AXIS)->LimitHit();}

void pin_ISRB() {  axisController.pGetAxis(B_AXIS)->LimitHit(); }

int incomingByte = 0;

int servoMin = 90;
int servoMax = 180;
int servoSpeed = 14;
int dir = 0;

void InitISR()
{
  pinMode(B_AXIS_LIMIT, INPUT_PULLUP);
  pinMode(A_AXIS_LIMIT, INPUT_PULLUP);
  pinMode(Z_AXIS_LIMIT, INPUT_PULLUP);
  
  //todo add c axis
  //pinMode(C_AXIS_LIMIT, INPUT_PULLUP);
  //attachPCINT(digitalPinToPCINT(C_AXIS_LIMIT), pin_ISRC, CHANGE);
 


  attachPCINT(digitalPinToPCINT(B_AXIS_LIMIT), pin_ISRB, CHANGE);
  attachPCINT(digitalPinToPCINT(A_AXIS_LIMIT), pin_ISRA, CHANGE);
  attachPCINT(digitalPinToPCINT(Z_AXIS_LIMIT), pin_ISRZ, CHANGE);
}

void InitAxis()
{

  //create axis
  axisController.AddAxis(Z_AXIS, new Axis(Z_STEPS, Z_DIR, Z_STEP,ENABLE_PIN,
                                          Z_STEPS_PER_MM,Z_AXIS_LIMIT,INVERT_Z,Z_MICROSTEPS,Z_HOME_RPM,Z_RETRACT_DIST,Z_RPM,
                                          Z_MAX_DIST,Z_ACCEL,Z_DECEL) );
  
  axisController.AddAxis(A_AXIS, new Axis(A_STEPS, A_DIR, A_STEP,ENABLE_PIN,
                                          A_STEPS_PER_DEGREE,A_AXIS_LIMIT,INVERT_A,A_MICROSTEPS,A_HOME_RPM/2,A_RETRACT_DIST,A_RPM,
                                          A_MAX_DIST,A_ACCEL,A_DECEL) );
  
  axisController.AddAxis(B_AXIS, new Axis(B_STEPS, B_DIR, B_STEP,ENABLE_PIN,
                                          B_STEPS_PER_DEGREE,B_AXIS_LIMIT,INVERT_B,B_MICROSTEPS,B_HOME_RPM,B_RETRACT_DIST,B_RPM,
                                          B_MAX_DIST,B_ACCEL,B_DECEL) );
  
  
  axisController.AddAxis(C_AXIS, new Axis(C_STEPS, C_DIR, C_STEP,ENABLE_PIN,
                                          C_STEPS_PER_DEGREE,C_AXIS_LIMIT,INVERT_C,C_MICROSTEPS,C_HOME_RPM,C_RETRACT_DIST,C_RPM,
                                          C_MAX_DIST,C_ACCEL,C_DECEL) );
  
  //add axis to sync controller, must create axis' before this call
  axisController.CreateSyncDriveController();

  //attch interrupts
  InitISR();
}

void InitGripper()
{

  gripperServo.attach(GRIPPER_PIN,544,2400);
  gripperServo.setSpeed(servoSpeed);

  dir = 1;
}

void HomeAxis()
{
  axisController.Home(Z_AXIS);
  axisController.Home(B_AXIS);
  axisController.Home(A_AXIS);
}

void setup() {

  //Serial.begin(115200);
  Commands.begin(115200,"ok");
   


  InitAxis();

  InitGripper();
  
  
  int degrees = 90;

 // axisController.Move(0,degrees,degrees,0);

  

}

bool IsConfigValue(int value)
{
  return (value == CONFIG_MAX_SERVO_ANGLE || value == CONFIG_MIN_SERVO_ANGLE || value == CONFIG_GRIPPER_SPEED);

}

bool IsServoControl(int value)
{
  return (value == GRIPPER_OPEN || value == GRIPPER_CLOSE);
}

void GripperAction(int value)
{
  if(incomingByte == GRIPPER_OPEN)
  {
    Serial.print("servoMax ");
    Serial.println(servoMax);
    gripperServo.write(servoMax);
  }
  else if(incomingByte == GRIPPER_CLOSE)
  {
    Serial.print("servoMin ");
    Serial.println(servoMin);
    gripperServo.write(servoMin);
  }
}

void ReadConfig()
{
  
      Serial.print("waiting for input value");
      while(!Serial.available())
      {
      }

      int value = Serial.parseInt();
      
      if(incomingByte == CONFIG_MAX_SERVO_ANGLE)
      {
        Serial.print("setting max angle ");
        Serial.println(value);
        //gripperServo.setMaximumPulse(pulse);
        servoMax = value;
      }
      
      else if(incomingByte == CONFIG_MIN_SERVO_ANGLE)
      {
        Serial.print("setting min angle ");
        Serial.println(value);
        //gripperServo.setMinimumPulse(pulse);
        servoMin = value;
      }

      else if(incomingByte == CONFIG_GRIPPER_SPEED)
      {
        Serial.print("setting speed ");
        Serial.println(value);
        //gripperServo.setMinimumPulse(pulse);
        servoSpeed = value;
        gripperServo.setSpeed(servoSpeed);
      }

}



// the loop function runs over and over again forever
void loop() {

/*
  if (Serial.available() > 0) {
    // read the incoming byte:
    incomingByte = Serial.read();
    if(IsConfigValue(incomingByte))
    {
        ReadConfig();
    }
    if(IsServoControl(incomingByte))
    {
        GripperAction(incomingByte);
    }
  }
  */
  Commands.available();
  //gripperServo.update();

}