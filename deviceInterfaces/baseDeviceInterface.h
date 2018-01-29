#pragma once

#include <QSharedPointer>

#include <functional>
#include "status.h"

typedef std::function<void(QSharedPointer<Status>)> DeviceCallback; // _1 - status
typedef std::function<void(bool)> OnIsConnectedCallback;

class BaseDeviceInterface
{
public:
    // device start
    virtual void start(DeviceCallback onStart) = 0;

    // device shutdown
    virtual void finish(DeviceCallback onFinish) = 0;

    // device restart
    virtual void restart(DeviceCallback onRestart) = 0;

    // check device status
    virtual void checkStatus(DeviceCallback onCheckStatus) = 0;

    // check device connection
    virtual void connectionStatus(DeviceCallback onConnection) = 0;
    virtual void isConnected(OnIsConnectedCallback onIsConnected) = 0;

    void setOnErrorCallback(DeviceCallback onError);

protected:
    DeviceCallback _onError;
};
