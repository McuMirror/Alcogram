#pragma once

#include <QObject>
#include <QMap>

#include "posDeviceTest.h"

enum DeviceName {
    POS
};

class DeviceManager : public QObject
{
    Q_OBJECT
public:
    explicit DeviceManager(QObject *parent = 0);

    POSInterface* getPOSDevice() const;

private:
    QMap<DeviceName, BaseDeviceInterface*> _devices;
};
