#include "Axis.h"

void Axis::Move(long distance)
{
    _basicStepperDriver.move(distance * _stepsPerDegree); //steps per degree means stepsPerMM for lead screws
}

void Axis::TransitionHomingState() 
{
    
    
    switch(_homeState)
    {
         
        case HOME_STATE_FIRST_SEEK: 
        {
            _basicStepperDriver.stop();
            _homeState = HOME_STATE_RETRACT;
             
            
        }break;
        
        case HOME_STATE_SECOND_SEEK:
        {
            _basicStepperDriver.stop();
            _homeState = HOME_STATE_HOMED;

            Serial.println("Setting RPM");
            _basicStepperDriver.setRPM(50); //reset regular speed;
            _currentAngle = 0;
            _homed = true;

        }break; 
    }
}

void Axis::LimitHit()
{
    TransitionHomingState();
}

void Axis::InitHoming(uint8_t homePin, bool invertDir)
{
    _homePin = homePin;
    _homeState = HOME_STATE_NOT_HOMED;
    _invertDir = invertDir;
    
}

HOME_STATE Axis::UpdateHoming()
{
    switch(_homeState)
    {
        case HOME_STATE_NOT_HOMED: 
        {
            _homeState = HOME_STATE_FIRST_SEEK;
            _basicStepperDriver.setRPM(HOME_FIRST_SEEK_RPM*2);
             _invertDir ? Move(-HOME_SEEK_DEG) : Move(HOME_SEEK_DEG);//_basicStepperDriver.rotate(-HOME_SEEK_DEG) : _basicStepperDriver.rotate(HOME_SEEK_DEG);
        }break;
        case HOME_STATE_FIRST_SEEK: 
        {
            //end of first seek, setup retract
           
           

        }break;
        case HOME_STATE_RETRACT:
        {

           
               _invertDir ? Move(-RETRACT_DISTANCE_DEG) : Move(RETRACT_DISTANCE_DEG);//_basicStepperDriver.rotate(-RETRACT_DISTANCE_DEG) : _basicStepperDriver.rotate(RETRACT_DISTANCE_DEG); 
                
               
               
               if(_basicStepperDriver.getStepsRemaining() == 0) //check if we have reached the end
               {
                   
                  _homeState = HOME_STATE_SECOND_SEEK;
                  Serial.println("Seek Finished!");
                  _basicStepperDriver.setRPM(HOME_SECOND_SEEK_RPM);
                  
                  
                  //_invertDir ? _basicStepperDriver.rotate(-HOME_SEEK_DEG) : _basicStepperDriver.rotate(HOME_SEEK_DEG);//move back toward the limit switch slowly
                    _invertDir ? Move(-HOME_SEEK_DEG) : Move(HOME_SEEK_DEG);


                //_basicStepperDriver.rotate(HOME_SEEK_DEG);   
               
               
               }
               
               
            

        }break;
        case HOME_STATE_SECOND_SEEK:
        { 
               //limit hit here is handled in the interupt function TransitionHomingState
        }break; 
        case HOME_STATE_HOMED:
        {
            
             
        }break;
        case HOME_STATE_ERROR:
        {}break;
         
    }

    return _homeState;
}

int Axis::Home()
{
    return 1;
}

      