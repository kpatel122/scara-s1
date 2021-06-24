#include "AxisController.h"


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