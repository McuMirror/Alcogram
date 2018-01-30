#pragma once

#include "baseDeviceInterface.h"

typedef std::function<void(double, QSharedPointer<Status>)> POSCallback; // _1 - status code, _2 - total entered amount

class POSInterface : public BaseDeviceInterface {
public:
    // activate POS
    virtual void activate(DeviceCallback onActivation) = 0;

    // deactivate POS
    virtual void deactivate(DeviceCallback onDeactivation) = 0;

    // send amount of money to the POS device for payment
    virtual void takeMoney(double money, POSCallback onTransactionSucceded
                           , DeviceCallback onTransactionFailed) = 0;
};
