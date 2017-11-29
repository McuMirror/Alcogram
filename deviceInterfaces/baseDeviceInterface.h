#pragma once

#include <functional>

const int OK = 0;    // TODO: get rid of it
const int DEVICE_ERROR = 1; //

typedef std::function<void(int)> DeviceCallback; // _1 - status code

class BaseDeviceInterface
{
public:
    virtual void turnOn(DeviceCallback callback) = 0; // TODO: logging
    virtual void turnOff(DeviceCallback callback) = 0; // TODO: logging
    virtual void reset() = 0;
};
