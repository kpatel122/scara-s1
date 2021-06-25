#include <Arduino.h>
#include "Config.h"
#include "MultiDriver.h"
#include "SyncDriver.h"
#include "Axis.h"
#include "AxisController.h"
#include "PinChangeInterrupt.h"

//steps needs to be configured







AxisController axisController;

enum ROBOT_STATE {
  STATE_NOT_HOMED,
  STATE_HOMING,
  STATE_HOMED
};


void pin_ISRZ() { axisController.pGetAxis(Z_AXIS)->LimitHit(); }

void pin_ISRA() { Serial.println("A_AXIS"); axisController.pGetAxis(A_AXIS)->LimitHit();}

void pin_ISRB() { axisController.pGetAxis(B_AXIS)->LimitHit(); }

int incomingByte = 0;

void setAxisParameters()
{
  for(int i=0; i<4;i++)
  {
    
    axisController.pGetAxis(i)->_basicStepperDriver.begin(RPM, MICROSTEPS);
    axisController.pGetAxis(i)->_basicStepperDriver.setEnableActiveState(LOW);
    axisController.pGetAxis(i)->_basicStepperDriver.enable();
    axisController.pGetAxis(i)->_basicStepperDriver.setSpeedProfile(axisController.pGetAxis(i)->_basicStepperDriver.LINEAR_SPEED,ACCL,DECL);
  
  }
}

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
  axisController.AddAxis(Z_AXIS, new Axis(STEPS, Z_DIR, Z_STEP,ENABLE_PIN,0,Z_AXIS_LIMIT,INVERT_Z_HOME) );
  axisController.AddAxis(A_AXIS, new Axis(STEPS, A_DIR, A_STEP,ENABLE_PIN,A_STEPS_PER_DEGREE,A_AXIS_LIMIT,INVERT_A_HOME) );
  axisController.AddAxis(B_AXIS, new Axis(STEPS, B_DIR, B_STEP,ENABLE_PIN,B_STEPS_PER_DEGREE,B_AXIS_LIMIT,INVERT_B_HOME) );
  axisController.AddAxis(C_AXIS, new Axis(STEPS, C_DIR, C_STEP,ENABLE_PIN,C_STEPS_PER_DEGREE,C_AXIS_LIMIT,INVERT_C_HOME) );
  
  //add axis to sync controller
  axisController.CreateSyncDriveController();

  //set axis parameters
  setAxisParameters();

}

void HomeAxis(uint8_t axis)
{
   while(axisController.pGetAxis(axis)->GetHomingState() != HOME_STATE_HOMED)
   {
     axisController.pGetAxis(axis)->UpdateHoming();
   }
   Serial.println("Homing Finished");
}

void setup() {

  Serial.begin(115200);

  InitAxis();
  InitISR();
  HomeAxis(B_AXIS);
  HomeAxis(A_AXIS);
  
  

  int degrees = 90; 

  axisController.Move(0,degrees,degrees,0);

}

// the loop function runs over and over again forever
void loop() {


  if (Serial.available() > 0) {
    // read the incoming byte:
    incomingByte = Serial.read();

    if(incomingByte == 's')
    {
      Serial.print("limit status ");
    }

    
  }

}
