#ifndef AXIS_H
#define AXIS_H
#include <Arduino.h>
#include "BasicStepperDriver.h"



//single axis
class Axis
{
    public:

        Axis(short steps,uint8_t dirPin , uint8_t stepPin, uint8_t enablePin) : _basicStepperDriver(steps,dirPin , stepPin, enablePin){};

        void Init(short steps,uint8_t dirPin , uint8_t stepPin, uint8_t enablePin, uint8_t homePin, int stepsPerDegree);
        float GetAngle() {return _currentAngle;}
        bool IsHomed(){return _homed;}
        bool SetHomed(bool homeStatus){_homed = homeStatus;} 
         
        BasicStepperDriver _basicStepperDriver; //should be private but an issue using the getter in sync driver is fixed by making it public
    private:
        
        short _stepsPerDegree;
        short _homePin;
        long _rotationProgress;
        float _currentAngle;
        bool _homed;
         
};

#endif