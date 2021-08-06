#include "AxisController.h"

void AxisController::Home(uint8_t axisIndex)
{
    _pAxis[axisIndex]->Home();
}
void AxisController::Home()
{
    /*Home all axis*/
}

#define CHECK_FLAG_INVERT_DIR(v,f) if(v){ if(f){ v=-v;} } //inverts value based on flag

void AxisController::Move(long mmZ,long degreesA,long degreesB,long degreesC)
{
    
    Serial.print("before C ");
    Serial.println(degreesC);

    Serial.print("before Z ");
    Serial.println(mmZ);

    CHECK_FLAG_INVERT_DIR(mmZ,INVERT_Z)
    CHECK_FLAG_INVERT_DIR(degreesA,INVERT_A)
    CHECK_FLAG_INVERT_DIR(degreesB,INVERT_B)
    CHECK_FLAG_INVERT_DIR(degreesC,INVERT_C)

    Serial.print(" After C ");
    Serial.println(degreesC);

    Serial.print(" After Z ");
    Serial.println(mmZ);
    
    _pSyncDriveController->move(mmZ * _pAxis[Z_AXIS]->GetStepsPerDegree(),
                             degreesA * _pAxis[A_AXIS]->GetStepsPerDegree(),
                             degreesB * _pAxis[B_AXIS]->GetStepsPerDegree(),
                             (degreesC * _pAxis[C_AXIS]->GetStepsPerDegree()));
    

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