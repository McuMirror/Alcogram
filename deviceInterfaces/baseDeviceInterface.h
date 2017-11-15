#pragma once

#include <functional>

typedef std::function<void(int)> DeviceCallback; // _1 - status code

class BaseDeviceInterface
{
public:
    virtual void turnOn(DeviceCallback callback) = 0;
    virtual void turnOff(DeviceCallback callback) = 0;
};
