#pragma once

#include <QSharedPointer>

#include <functional>
#include "status.h"

typedef std::function<void(QSharedPointer<Status>)> DeviceCallback; // common devices callback, _1 - status
typedef std::function<void(bool)> OnIsConnectedCallback;

class BaseDeviceInterface
{
public:
    // start a device
    virtual void start(DeviceCallback onStart) = 0;

    // finish a device
    virtual void finish(DeviceCallback onFinish) = 0;

    // restart a device
    virtual void restart(DeviceCallback onRestart) = 0;

    // check device status
    virtual void checkStatus(DeviceCallback onCheckStatus) = 0;

    // check device connection
    virtual void connectionStatus(DeviceCallback onConnection) = 0;
    virtual void isConnected(OnIsConnectedCallback onIsConnected) = 0;

    // sets error callback
    void setOnErrorCallback(DeviceCallback onError);

protected:
    DeviceCallback _onError;
};
