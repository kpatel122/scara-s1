#ifndef AXISCONTROLLER_H
#define AXISCONTROLLER_H

#include "Config.h"
#include "SyncDriver.h"
#include "Axis.h"

class AxisController
{
    public:
        AxisController();
        ~AxisController();
        int AddAxis(uint8_t axisIndex, Axis *axis) {_pAxis[axisIndex] = axis;} ;//uint8_t axisIndex, short steps,uint8_t dirPin , uint8_t stepPin, uint8_t enablePin, uint8_t homePin, int stepsPerDegree);
        int CreateSyncDriveController(); //must be called after all add axis'
        SyncDriver* pGetSyncDriver() {return _pSyncDriveController;}
        Axis* pGetAxis(uint8_t axisIndex){ return _pAxis[axisIndex]; }

    private:
        Axis* _pAxis[4];
        SyncDriver *_pSyncDriveController;
};


#endif