#include "Axis.h"

void Axis::Move(long distance)
{
    _basicStepperDriver.move(distance * _stepsPerDegree); //steps per degree means stepsPerMM for lead screws
    //_basicStepperDriver.rotate(distance);
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
            _basicStepperDriver.setRPM(_RPM); //reset regular speed;
            _currentAngle = 0;
            _homed = true;

        }break; 
    }
}

void Axis::LimitHit()
{
    TransitionHomingState();
}

void Axis::Init(uint8_t homePin, bool invertDir,uint8_t microSteps,float stepsPerDegree, uint8_t homeRPM, 
               uint8_t homeRetractDistance,uint8_t RPM,uint16_t maxDistance, uint16_t acceleration, uint16_t deceleration)
{
    _homePin = homePin;
    _homeRPM= homeRPM;
    _homeRetractDistance = homeRetractDistance;
    _homeState = HOME_STATE_NOT_HOMED;
    _invertDir = invertDir;
    _RPM = RPM;
    _maxDistance = maxDistance;
    _acceleration = acceleration;
    _deceleration = deceleration;
    _homed=false; 
    _stepsPerDegree=stepsPerDegree;  

    /* start stepper */
    _basicStepperDriver.begin(homeRPM, microSteps);
    _basicStepperDriver.setEnableActiveState(LOW);
    _basicStepperDriver.enable();
    _basicStepperDriver.setSpeedProfile(_basicStepperDriver.LINEAR_SPEED,acceleration,deceleration);
    
}

HOME_STATE Axis::UpdateHoming()
{
    switch(_homeState)
    {
        case HOME_STATE_NOT_HOMED: 
        {
            _homeState = HOME_STATE_FIRST_SEEK;
            _basicStepperDriver.setRPM(_homeRPM);
             _invertDir ? Move(-HOME_SEEK_DEG) : Move(HOME_SEEK_DEG);//_basicStepperDriver.rotate(-HOME_SEEK_DEG) : _basicStepperDriver.rotate(HOME_SEEK_DEG);
        }break;
        case HOME_STATE_FIRST_SEEK: 
        {
            //end of first seek, setup retract
           
           

        }break;
        case HOME_STATE_RETRACT:
        {

           
               _invertDir ? Move(-_homeRetractDistance) : Move(_homeRetractDistance);//_basicStepperDriver.rotate(-RETRACT_DISTANCE_DEG) : _basicStepperDriver.rotate(RETRACT_DISTANCE_DEG); 
                
               
               
               if(_basicStepperDriver.getStepsRemaining() == 0) //check if we have reached the end
               {
                   
                  _homeState = HOME_STATE_SECOND_SEEK;
                  Serial.println("Seek Finished!");
                  _basicStepperDriver.setRPM(_homeRPM /2);
                  
                  
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
   while(_homeState != HOME_STATE_HOMED)
   {
      UpdateHoming();
   }

   return 1;
}

      