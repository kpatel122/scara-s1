#include "AxisController.h"

void AxisController::Move(long mmZ,long degreesA,long degreesB,long degreesC)
{
    
    
    _pSyncDriveController->move(mmZ * _pAxis[Z_AXIS]->GetStepsPerDegree(),
                             degreesA * _pAxis[A_AXIS]->GetStepsPerDegree(),
                             degreesB * _pAxis[B_AXIS]->GetStepsPerDegree(),
                             degreesC * _pAxis[C_AXIS]->GetStepsPerDegree());
    

   /*
   _pSyncDriveController->rotate(mmZ,
                             degreesA,
                             degreesB,
                             degreesC);
    */
}

int AxisController::CreateSyncDriveController()
{



    _pSyncDriveController = new SyncDriver(_pAxis[Z_AXIS]->_basicStepperDriver,
                                           _pAxis[A_AXIS]->_basicStepperDriver,
                                           _pAxis[B_AXIS]->_basicStepperDriver,
                                           _pAxis[C_AXIS]->_basicStepperDriver);
    
    return 1;
}

AxisController::AxisController()
{
     
}

AxisController::~AxisController()
{
    if(_pSyncDriveController) {delete _pSyncDriveController;}
}