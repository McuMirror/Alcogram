#pragma once

#include <QObject>
#include <QMap>

#include "deviceInterfaces/baseDeviceInterface.h"
#include "deviceInterfaces/posinterface.h"
#include "deviceInterfaces/cameraInterface.h"
#include "deviceInterfaces/alcotesterinterface.h"
#include "deviceInterfaces/printerinterface.h"

enum DeviceName {
    POS
    , CAMERA
    , ALCOTESTER
    , PRINTER
};

class DeviceManager : public QObject
{
    Q_OBJECT
public:
    explicit DeviceManager(QObject *parent = 0);

    POSInterface* getPOSDevice() const;
    CameraInterface* getCameraDevice() const;
    AlcotesterInterface* getAlcotesterDevice() const;
    PrinterInterface* getPrinterDevice() const;

private:
    QMap<DeviceName, BaseDeviceInterface*> _devices;
};
