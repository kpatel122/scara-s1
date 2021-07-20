#include <Arduino.h>
#include "Config.h"
#include "MultiDriver.h"
#include "SyncDriver.h"
#include "Axis.h"
#include "AxisController.h"
#include "PinChangeInterrupt.h"
 #include <ServoSpeed.h>
 


AxisController axisController;
CServoSpeed gripperServo;



enum ROBOT_STATE {
  STATE_NOT_HOMED,
  STATE_HOMING,
  STATE_HOMED
};


void pin_ISRZ() { Serial.println("Z"); axisController.pGetAxis(Z_AXIS)->LimitHit(); }

void pin_ISRA() { Serial.println("A"); axisController.pGetAxis(A_AXIS)->LimitHit();}

void pin_ISRB() { Serial.println("B"); axisController.pGetAxis(B_AXIS)->LimitHit(); }

int incomingByte = 0;

int servoMin = 90;
int servoMax = 180;
int servoSpeed = 14;
int dir = 0;

void InitISR()
{
  pinMode(B_AXIS_LIMIT, INPUT_PULLUP);
  pinMode(A_AXIS_LIMIT, INPUT_PULLUP);
  //pinMode(Z_AXIS_LIMIT, INPUT_PULLUP);
  
  //todo add c axis
  //pinMode(C_AXIS_LIMIT, INPUT_PULLUP);
  //attachPCINT(digitalPinToPCINT(C_AXIS_LIMIT), pin_ISRC, CHANGE);
 


  attachPCINT(digitalPinToPCINT(B_AXIS_LIMIT), pin_ISRB, CHANGE);
  attachPCINT(digitalPinToPCINT(A_AXIS_LIMIT), pin_ISRA, CHANGE);
  //attachPCINT(digitalPinToPCINT(Z_AXIS_LIMIT), pin_ISRZ, CHANGE);
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
}

void InitGripper()
{

  gripperServo.attach(GRIPPER_PIN,544,2400);
  gripperServo.setSpeed(servoSpeed);

  dir = 1;
}

void setup() {

  Serial.begin(115200);

  //InitAxis();
  //InitISR();
  InitGripper();
  

//axisController.Home(Z_AXIS);
//axisController.Home(B_AXIS);
//axisController.Home(A_AXIS);

//int degrees = 90;
 // int mm = 10; 

//  axisController.Move(mm,degrees,degrees,0);

}




// the loop function runs over and over again forever
void loop() {


  if (Serial.available() > 0) {
    // read the incoming byte:
    incomingByte = Serial.read();

    if(incomingByte == 'h' ||  incomingByte == 'g' ||  incomingByte == 'j' )
    {
      Serial.print("witing for input ");
      while(!Serial.available())
      {
      }

      int pulse = Serial.parseInt();
      


      if(incomingByte == 'h')
      {
        Serial.print("setting max angle ");
        Serial.println(pulse);
        //gripperServo.setMaximumPulse(pulse);
        servoMax = pulse;
      }
      
      if(incomingByte == 'g')
      {
        Serial.print("setting min angle ");
        Serial.println(pulse);
        //gripperServo.setMinimumPulse(pulse);
        servoMin = pulse;
      }

       if(incomingByte == 'j')
      {
        Serial.print("setting speed ");
        Serial.println(pulse);
        //gripperServo.setMinimumPulse(pulse);
        servoSpeed = pulse;
        gripperServo.setSpeed(servoSpeed);
      }

    }


    if(incomingByte == 's')
    {
       int degrees = 10;
        int mm = 10; 

      //axisController.Move(mm,degrees,degrees,0);
      
      if(dir == 0)
      {
        Serial.print("servoMin ");
        Serial.println(servoMin);
        
        
        gripperServo.write(servoMin);
        
        dir = 1;
      }
      else if (dir == 1)
      {
          Serial.print("servoMax ");
          Serial.println(servoMax);
          
           
          gripperServo.write(servoMax);
          
          dir = 0;
      }
      
    }

  }
  gripperServo.update();

}