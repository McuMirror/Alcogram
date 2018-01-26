#include "status.h"

Status::Status(int errorCoder, DeviceName deviceName, RequestName requestName)
    : _errorCode(errorCoder)
    , _deviceName(deviceName)
    , _requestName(requestName)
{
}

int Status::getErrorCode() const
{
    return _errorCode;
}

DeviceName Status::getDeviceName() const
{
    return _deviceName;
}

RequestName Status::getRequestName() const
{
    return _requestName;
}


