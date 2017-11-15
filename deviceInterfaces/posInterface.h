#pragma once

#include "deviceInterfaces/baseDeviceInterface.h"

typedef std::function<void(int, int)> PayCallback; // _1 - status code, _2 - total entered amount

class POSInterface : public BaseDeviceInterface {
public:
    virtual void sendPrice(int price, DeviceCallback callback) {}
    virtual void getPaymentResponce(PayCallback payCallback) {}
    virtual void abortPayment(DeviceCallback callback) {}
};
