#include "AxisController.h"

/*
int AxisController::AddAxis(uint8_t axisIndex, short steps,uint8_t dirPin , uint8_t stepPin, uint8_t enablePin, uint8_t homePin, int stepsPerDegree)
{
    //axis index should be 0 indexed
    //create a single axis
    if(axisIndex >= NUM_AXIS)
        return 0;
    _axis[axisIndex].Init(steps,dirPin , stepPin,enablePin, homePin, stepsPerDegree);

    return 1;
}
*/

/*

    BasicStepperDriver stepperA(STEPS, A_DIR, A_STEP,ENABLE_PIN);
    BasicStepperDriver stepperB(STEPS, B_DIR, B_STEP,ENABLE_PIN);
    BasicStepperDriver stepperC(STEPS, C_DIR, C_STEP,ENABLE_PIN);
    BasicStepperDriver stepperD(STEPS, D_DIR, D_STEP,ENABLE_PIN);
*/


int AxisController::CreateSyncDriveController()
{


    
    //_pSyncDriveController = new SyncDriver(*_axis[0].pGetBasicStepperDriver(),*_axis[1].pGetBasicStepperDriver(), *_axis[2].pGetBasicStepperDriver(), *_axis[3].pGetBasicStepperDriver());
   
/*
      stepperA.begin(RPM, MICROSTEPS);
  stepperB.begin(RPM, MICROSTEPS);
  stepperC.begin(RPM, MICROSTEPS);
  stepperD.begin(RPM, MICROSTEPS);

    stepperA.setEnableActiveState(LOW);
  stepperB.setEnableActiveState(LOW);
  stepperC.setEnableActiveState(LOW);
  stepperD.setEnableActiveState(LOW);

  stepperA.enable();
  stepperB.enable();
  stepperC.enable();
  stepperD.enable();



  stepperA.setSpeedProfile(stepperA.LINEAR_SPEED,ACCL,DECL);
  stepperB.setSpeedProfile(stepperB.LINEAR_SPEED,ACCL,DECL);
  stepperC.setSpeedProfile(stepperC.LINEAR_SPEED,ACCL,DECL);
  stepperD.setSpeedProfile(stepperD.LINEAR_SPEED,ACCL,DECL);
    
*/
    // _pSyncDriveController = new SyncDriver(stepperA,stepperB, stepperC, stepperD);
     

   // _pSyncDriveController = new SyncDriver(stepperA,stepperB, stepperC, stepperD);
    _pSyncDriveController = new SyncDriver(_pAxis[0]->_basicStepperDriver,_pAxis[1]->_basicStepperDriver,_pAxis[2]->_basicStepperDriver,_pAxis[3]->_basicStepperDriver);
    
    
    /*
    switch(NUM_AXIS)
    {
        case 2:
            _pSyncDriveController = new SyncDriver(*_axis[0].pGetBasicStepperDriver(),*_axis[1].pGetBasicStepperDriver() );
        case 3:
            _pSyncDriveController = new SyncDriver(*_axis[0].pGetBasicStepperDriver(),*_axis[1].pGetBasicStepperDriver(), *_axis[2].pGetBasicStepperDriver());
        case 4:
            _pSyncDriveController = new SyncDriver(*_axis[0].pGetBasicStepperDriver(),*_axis[1].pGetBasicStepperDriver(), *_axis[2].pGetBasicStepperDriver(), *_axis[3].pGetBasicStepperDriver());
        
        default: return 0;
    }
    */
    return 1;
}

AxisController::AxisController()
{
     
}

AxisController::~AxisController()
{
    if(_pSyncDriveController) {delete _pSyncDriveController;}
}