#ifndef AXISCONTROLLER_H
#define AXISCONTROLLER_H

#include "Config.h"
#include "SyncDriver.h"
#include "Axis.h"

/*all axis container class*/
class AxisController
{
    public:
        AxisController();
        ~AxisController();
        int AddAxis(uint8_t axisIndex, Axis *axis) {_pAxis[axisIndex] = axis;} ;//uint8_t axisIndex, short steps,uint8_t dirPin , uint8_t stepPin, uint8_t enablePin, uint8_t homePin, int stepsPerDegree);
        int CreateSyncDriveController(); //must be called after all add axis'
        void Move(long mmZ,long degreesA,long degreesB,long degreesC);
        void Home(uint8_t axisIndex);
        void Home(); //home all
        SyncDriver* pGetSyncDriver() {return _pSyncDriveController;}
        Axis* pGetAxis(uint8_t axisIndex){ return _pAxis[axisIndex]; }
        

    private:
        Axis* _pAxis[4]; //max 4 axks is supported
        SyncDriver *_pSyncDriveController;
};


#endif