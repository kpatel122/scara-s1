#ifndef AXIS_H
#define AXIS_H
#include <Arduino.h>
#include "Config.h"
#include "BasicStepperDriver.h"
#include "PinChangeInterrupt.h"

enum HOME_STATE
{
    HOME_STATE_NOT_HOMED, //initial state
    HOME_STATE_FIRST_SEEK, //first pass to hit the limit switch
    HOME_STATE_RETRACT, //retract after first hit
    HOME_STATE_SECOND_SEEK,  //second pass 
    HOME_STATE_HOMED,   //finished homing
    HOME_STATE_ERROR // something went wrong
};

//single axis
class Axis
{
    public:

        Axis(uint16_t steps,uint8_t dirPin , uint8_t stepPin, uint8_t enablePin, uint16_t stepsPerDegree, uint8_t homePin, bool invertHomingDir ) 
        : _basicStepperDriver(steps,dirPin , stepPin, enablePin){_homed=false; _stepsPerDegree=stepsPerDegree;  InitHoming(homePin, invertHomingDir);}
 
        float GetAngle() {return _currentAngle;}
        bool IsHomed(){return _homed;}
        bool SetHomed(bool homeStatus){_homed = homeStatus;}
        
        int Home();
        void LimitISR();
        void HomeISR();
        void LimitHit();
        HOME_STATE UpdateHoming();
        HOME_STATE GetHomingState() {return _homeState;}
         
        BasicStepperDriver _basicStepperDriver; //should be private but an issue using the getter in sync driver is fixed by making it public
    private:
        void InitHoming(uint8_t homePin, bool invertHomingDir);
        void TransitionHomingState();
        short _stepsPerDegree;
        short _homePin;
        long _rotationProgress;
        float _currentAngle;
        bool _homed;
        bool _invertHomingDir;
        volatile HOME_STATE _homeState;
        
         
};

#endif