#pragma once

#include <QObject>
#include <QMap>

#include "deviceInterfaces/baseDeviceInterface.h"
#include "deviceInterfaces/posinterface.h"
#include "deviceInterfaces/cameraInterface.h"
#include "deviceInterfaces/alcotesterinterface.h"

enum DeviceName {
    POS
    , CAMERA
    , ALCOTESTER
};

class DeviceManager : public QObject
{
    Q_OBJECT
public:
    explicit DeviceManager(QObject *parent = 0);

    POSInterface* getPOSDevice() const;
    CameraInterface* getCameraDevice() const;
    AlcotesterInterface* getAlcotester() const;

private:
    QMap<DeviceName, BaseDeviceInterface*> _devices;
};
