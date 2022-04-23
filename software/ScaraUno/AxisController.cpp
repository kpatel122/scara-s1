#include "AxisController.h"
/**
 * @brief homes the sepecific axis
 * @param axisIndex-uint8_t the index of the axis to home
 * @return None
 */
void AxisController::Home(uint8_t axisIndex)
{
    //call axis move
    _pAxis[axisIndex]->Home();
}
void AxisController::Home()
{
    //TODO: remove
    /*Home all axis*/
}

#define CHECK_FLAG_INVERT_DIR(v,f) if(v){ if(f){ v=-v;} } //inverts value based on flag
#define SET_ANGLE(a,i) if(a!=0) { _pAxis[i]->SetAngleInc(a); }


/**
 * @brief moves the axis
 * @param mmZ-long z axis distance in mm
 * @param degreesA-long a axis distance in deg
 * @param degreesB-long b axis distance in deg
 * @param degreesC-long c axis distance in deg
 * @return None
 */
void AxisController::Move(long mmZ,long degreesA,long degreesB,long degreesC)
{
    
    //inverse the movement value if needed
    CHECK_FLAG_INVERT_DIR(mmZ,INVERT_Z)
    CHECK_FLAG_INVERT_DIR(degreesA,INVERT_A)
    CHECK_FLAG_INVERT_DIR(degreesB,INVERT_B)
    CHECK_FLAG_INVERT_DIR(degreesC,INVERT_C)


    //perform a sync movement, should only move 3 axis at a time per the sync driver documentation
    _pSyncDriveController->move(mmZ * _pAxis[Z_AXIS]->GetStepsPerDegree(),
                             degreesA * _pAxis[A_AXIS]->GetStepsPerDegree(),
                             degreesB * _pAxis[B_AXIS]->GetStepsPerDegree(),
                             (degreesC * _pAxis[C_AXIS]->GetStepsPerDegree()));

    
    //update angles
    SET_ANGLE(mmZ,Z_AXIS)
    SET_ANGLE(degreesA,A_AXIS)
    SET_ANGLE(degreesB,B_AXIS)
    SET_ANGLE(degreesC,C_AXIS)
    

   
}

/**
 * @brief creates the stepper sync driver controller
 * @todo 1. remove return value, not doing anything useful 
 * @return None
 */

int AxisController::CreateSyncDriveController()
{


    //create the sync driver
    _pSyncDriveController = new SyncDriver(_pAxis[Z_AXIS]->_basicStepperDriver,
                                           _pAxis[A_AXIS]->_basicStepperDriver,
                                           _pAxis[B_AXIS]->_basicStepperDriver,
                                           _pAxis[C_AXIS]->_basicStepperDriver);
    
    //TODO: remove or return something useful
    return 1;
}

AxisController::AxisController()
{
     
}

AxisController::~AxisController()
{
    //release sync driver memory if pointer is valid
    if(_pSyncDriveController) {delete _pSyncDriveController;}
}