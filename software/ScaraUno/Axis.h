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
    HOME_STATE_ERROR, // something went wrong
    HOME_STATE_REHOME // ready to be re-homed again
    
};

//single axis
class Axis
{
    public:

        Axis(uint16_t steps,uint8_t dirPin , uint8_t stepPin, uint8_t enablePin, float stepsPerDegree, uint8_t homePin, bool invertDir,
        uint8_t microSteps, uint8_t homeRPM, uint8_t homeRetractDistance, uint8_t RPM, uint16_t maxDistance, uint16_t acceleration, uint16_t deceleration ) 
        : _basicStepperDriver(steps,dirPin , stepPin, enablePin)
        {Init(homePin, invertDir,microSteps,stepsPerDegree,homeRPM, homeRetractDistance, RPM,maxDistance,acceleration,deceleration); }
 
        float GetAngle() {return _currentAngle;}
        float SetAngleAbs(float angle) { _currentAngle = angle; } //absolute angle
        float SetAngleInc(float angle) { _currentAngle += angle; } //incremental angle

        float GetStepsPerDegree() {return _stepsPerDegree;}

        void SetRPM(float RPM) { _basicStepperDriver.setRPM(RPM);}
        bool IsHomed(){return _homed;}
        bool SetHomed(bool homeStatus){_homed = homeStatus;}
        
        int Home();
        void LimitISR();
        void HomeISR();
        void LimitHit();
        void Move(long distance);

        HOME_STATE UpdateHoming();
        HOME_STATE GetHomingState() {return _homeState;}
         
        BasicStepperDriver _basicStepperDriver; //should be private but an issue using the getter in sync driver is fixed by making it public
    private:
        void Init(uint8_t homePin, bool invertDir,uint8_t microSteps,float stepsPerDegree, uint8_t homeRPM, 
        uint8_t homeRetractDistance,uint8_t RPM,uint16_t maxDistance, uint16_t acceleration, uint16_t deceleration);

        void TransitionHomingState();
        float _stepsPerDegree;
        short _homePin;
        long _rotationProgress;
        float _currentAngle;
        bool _homed;
        bool _invertDir;
        volatile HOME_STATE _homeState; //changed from imit switch ISR
        
        uint8_t _homeRPM;
        uint8_t _homeRetractDistance;
        uint8_t _RPM;
        uint16_t _maxDistance;
        uint16_t _acceleration;
        uint16_t _deceleration;
        
        
         
};

#endif