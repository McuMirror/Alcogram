#include "baseDeviceInterface.h"

void BaseDeviceInterface::setOnErrorCallback(DeviceCallback onError)
{
    _onError = onError;
}
