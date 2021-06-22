#include "Axis.h"

 

void Axis::LimitHit()
{
    switch(_homeState)
    {
         
        case HOME_STATE_FIRST_SEEK: 
        {
            _homeState = HOME_STATE_RETRACT;
            //_basicStepperDriver.startBrake();
            _basicStepperDriver.stop();
        }break;
        
        case HOME_STATE_SECOND_SEEK:
        {
            _homeState = HOME_STATE_HOMED;

        }break; 
    }
}

void Axis::InitHoming(uint8_t homePin)
{
    _homePin = homePin;
    _homeState = HOME_STATE_NOT_HOMED;
    
    
    //Serial.print("Homing init pin ");
    //Serial.println(_homePin);

    //attachPCINT(digitalPinToPCINT(_homePin), pinISR, CHANGE);
}

HOME_STATE Axis::UpdateHoming()
{
    switch(_homeState)
    {
        case HOME_STATE_NOT_HOMED: 
        {
            _homeState = HOME_STATE_FIRST_SEEK;
            _basicStepperDriver.setRPM(10);
             _basicStepperDriver.rotate(-360*2);
        }break;
        case HOME_STATE_FIRST_SEEK: 
        {
           
            //delay(10);

        }break;
        case HOME_STATE_RETRACT:
        {}break;
        case HOME_STATE_SECOND_SEEK:
        {}break; 
        case HOME_STATE_HOMED:
        {}break;
        case HOME_STATE_ERROR:
        {}break;
         
    }

    return _homeState;
}

int Axis::Home()
{
    return 1;
}

      