#include "Axis.h"

void Axis::Init(short steps,uint8_t dirPin , uint8_t stepPin, uint8_t enablePin, uint8_t homePin, int stepsPerDegree)
{
    //_pBasicStepperDriver = new BasicStepperDriver (steps, dirPin, stepPin,enablePin);
    _homePin = homePin;
    _rotationProgress = 0;
    _currentAngle = 0;
    _homed = false;
    _stepsPerDegree = stepsPerDegree;
}

      