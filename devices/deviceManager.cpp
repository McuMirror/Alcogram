#include "deviceManager.h"
#include "posDeviceTest.h"
#include "cameraDeviceTest.h"
#include "alcotesterDeviceTest.h"
#include "printerDeviceTest.h"

DeviceManager::DeviceManager(QObject *parent)
    : QObject(parent)
{
    _devices.insert(POS, new POSDeviceTest(this));
    _devices.insert(CAMERA, new CameraDeviceTest(this));
    _devices.insert(ALCOTESTER, new AlcotesterDeviceTest(this));
    _devices.insert(PRINTER, new PrinterDeviceTest(this));
}

POSInterface* DeviceManager::getPOSDevice() const
{
    return static_cast<POSInterface*> (_devices[POS]);
}

CameraInterface* DeviceManager::getCameraDevice() const
{
    return static_cast<CameraInterface*> (_devices[CAMERA]);
}

AlcotesterInterface* DeviceManager::getAlcotesterDevice() const
{
    return static_cast<AlcotesterInterface*> (_devices[ALCOTESTER]);
}

PrinterInterface* DeviceManager::getPrinterDevice() const
{
    return static_cast<PrinterInterface*> (_devices[PRINTER]);
}

