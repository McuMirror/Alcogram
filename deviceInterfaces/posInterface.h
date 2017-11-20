#pragma once

#include "baseDeviceInterface.h"

typedef std::function<void(int, int)> PayCallback; // _1 - status code, _2 - total entered amount

class POSInterface : public BaseDeviceInterface {
public:
    virtual void sendPrice(int price, DeviceCallback callback) = 0;
    virtual void getPaymentResponce(PayCallback payCallback) = 0;
    virtual void abortPayment(DeviceCallback callback) = 0;
};
