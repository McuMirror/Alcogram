#include "deviceManager.h"
#include "posDeviceTest.h"
#include "cameraDeviceTest.h"
#include "alcotesterDeviceTest.h"

DeviceManager::DeviceManager(QObject *parent)
    : QObject(parent)
{
    _devices.insert(POS, new POSDeviceTest(this));
    _devices.insert(CAMERA, new CameraDeviceTest(this));
    _devices.insert(ALCOTESTER, new AlcotesterDeviceTest(this));
}

POSInterface* DeviceManager::getPOSDevice() const
{
    return static_cast<POSInterface*> (_devices[POS]);
}

CameraInterface* DeviceManager::getCameraDevice() const
{
    return static_cast<CameraInterface*> (_devices[CAMERA]);
}

AlcotesterInterface* DeviceManager::getAlcotester() const
{
    return static_cast<AlcotesterInterface*> (_devices[ALCOTESTER]);
}
