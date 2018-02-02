#pragma once

#include "baseDeviceInterface.h"

typedef std::function<void(double, QSharedPointer<Status>)> BillAcceptorCallback;

class BillAcceptorInterface : public BaseDeviceInterface {
public:
    // activate bill acceptor
    virtual void activate(DeviceCallback onActivation) = 0;

    virtual void setBillAcceptorCallback(BillAcceptorCallback onMoneyReceived, DeviceCallback onMoneyRejected) = 0;

    // deactivate bill acceptor
    virtual void deactivate(DeviceCallback onDeactivation) = 0;
};

