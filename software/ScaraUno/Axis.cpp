#include "Axis.h"



/**
 * @brief moves the stepper 
 * @param distance long, how far to move
 * @return None
 */

void Axis::Move(long distance)
{
    //call low level library move 
    _basicStepperDriver.move(distance * _stepsPerDegree); //steps per degree means stepsPerMM for lead screws
}

/**
 * @brief changes the homing state 
 * @return None
 */
void Axis::TransitionHomingState() 
{
    
    //check the cyrrent homing state
    switch(_homeState)
    {
        //initial limit hit, transition state to retract
        case HOME_STATE_FIRST_SEEK: 
        {

            _basicStepperDriver.stop();
            _homeState = HOME_STATE_RETRACT;
             
            
        }break;
        
        //second seek is finished, homing is done
        case HOME_STATE_SECOND_SEEK:
        {
            //homing finished, set default values
            _basicStepperDriver.stop();
            _homeState = HOME_STATE_HOMED;
            _basicStepperDriver.setRPM(_RPM); //reset regular speed;
            _currentAngle = 0;
            _homed = true;

        }break; 
    }
}

/**
 * @brief axis limit switch has been hit
 * @todo needs to handle limit hits, after homing is done 
 * @return None
 */
void Axis::LimitHit()
{
    //TODO: might already be homed
    TransitionHomingState();
}


/**
 * @brief Initialise the axis
 * @param homePin-uint8_t the limit switch for the axis
 * @param invertDir-bool invert axis movement
 * @param microSteps-uint8_t microsteps for the steper
 * @param stepsPerDegree-float number of steps needed for 1 degree of movement(don't think it needs to be a float)
 * @param homeRPM-uint8_t speed of first homing seek
 * @param homeRetractDistance uint8_t speed of retract after hitting homing switch during first seek cycle
 * @param RPM-uint8_t speed of the axis
 * @param maxDistance-the maximum distance this axis can travel
 * @param acceleration uint16_t stepper accelration value
 * @param deceleration uint16_t stepper deccelration value
 * @todo 1. don't think stepsPerDegree needs to be a float. 2. need to have a second retract speed, hard coded to 1/3 the first seek speed at the moment
 * @return None
 */
void Axis::Init(uint8_t homePin, bool invertDir,uint8_t microSteps,float stepsPerDegree, uint8_t homeRPM, 
               uint8_t homeRetractDistance,uint8_t RPM,uint16_t maxDistance, uint16_t acceleration, uint16_t deceleration)
{
    
    //set axis values
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

/**
 * @brief updates the homing status
 * @todo  1. make the second seek speed a parameter 2. get rid of un needed states
 * @return current HOME_STATE
 */
HOME_STATE Axis::UpdateHoming()
{
    //check the current homing state
    switch(_homeState)
    {
        case HOME_STATE_REHOME:
        case HOME_STATE_NOT_HOMED: 
        {
            //set to first seek
            _homeState = HOME_STATE_FIRST_SEEK;
            _basicStepperDriver.setRPM(_homeRPM);
            //reverse drection if needed
             _invertDir ? Move(-HOME_SEEK_DEG) : Move(HOME_SEEK_DEG); 
        }break;
        case HOME_STATE_FIRST_SEEK: 
        {
            //end of first seek, setup retract
        }break;
        case HOME_STATE_RETRACT:
        {

            //retract reverse direction if needed
            _invertDir ? Move(-_homeRetractDistance) : Move(_homeRetractDistance);//_basicStepperDriver.rotate(-RETRACT_DISTANCE_DEG) : _basicStepperDriver.rotate(RETRACT_DISTANCE_DEG); 
                
               
            //check if we have reached the end of the retract movement   
            if(_basicStepperDriver.getStepsRemaining() == 0) 
            {
                //start the second seek
                 _homeState = HOME_STATE_SECOND_SEEK;
                Serial.println("Seek Finished!");
                
                //set the second seek speed, TODO, make the speed a parameter
                _basicStepperDriver.setRPM(_homeRPM /3);
                  
                //_invertDir ? _basicStepperDriver.rotate(-HOME_SEEK_DEG) : _basicStepperDriver.rotate(HOME_SEEK_DEG);//move back toward the limit switch slowly
                 _invertDir ? Move(-HOME_SEEK_DEG) : Move(HOME_SEEK_DEG);
            }
               
        }break;
        case HOME_STATE_SECOND_SEEK:
        { 
               //limit hit here is handled in the interupt function TransitionHomingState
        }break; 
        case HOME_STATE_HOMED:
        {
            //handled in interrupt for the limit switch
            //TODO: remove
        }break;
        case HOME_STATE_ERROR:
        {
            //TODO: remove
        }break;
         
    }

    //send the back the current homing state
    return _homeState;
}

/**
 * @brief homes the axis
 * @todo  1. doesn't need to return 1
 */
int Axis::Home()
{
    //check if the axis has already been homed once
   if(_homeState == HOME_STATE_HOMED && _currentAngle !=0)// user has homed once already and wants to rehome
   {
       _homeState = HOME_STATE_REHOME;
   }  

   //loop until homing cycle complete for axis
   while(_homeState != HOME_STATE_HOMED)
   {
      //update homing state
      UpdateHoming();
       
   }

   //TODO: remove this 
   return 1;
}
