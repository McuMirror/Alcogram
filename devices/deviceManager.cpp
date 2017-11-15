#include "deviceManager.h"

DeviceManager::DeviceManager(QObject *parent) : QObject(parent)
{
    _devices.insert(POS, new POSDeviceTest(this));
}

POSInterface* DeviceManager::getPOSDevice() const
{
    return static_cast<POSInterface*> (_devices[POS]);
}
