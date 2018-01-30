#pragma once

#include <QList>

#include "baseDeviceInterface.h"

const int NOT_RECOGNIZED = 4; // TODO: get rid of it

typedef std::function<void(QSharedPointer<Status>, double)> AlcotesterCallback; // _1 - status code, _2 - permille

class AlcotesterInterface : public BaseDeviceInterface
{
public:
    // start warming up the alcotester
    virtual void warmUp(DeviceCallback onWarmedUp) = 0;

    // get data from the alcotester
    virtual void activate(AlcotesterCallback onDataRead, DeviceCallback onDataRejected) = 0;

    // start cooling down the alcotester
    virtual void coolDown(DeviceCallback onCooledDown) = 0;
};
