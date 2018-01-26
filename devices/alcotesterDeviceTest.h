#pragma once

#include <QObject>

#include "deviceInterfaces/alcotesterinterface.h"

class AlcotesterDeviceTest : public QObject
                            , public AlcotesterInterface
{
    Q_OBJECT
public:
    explicit AlcotesterDeviceTest(QObject *parent = 0);

    // BaseDeviceInterface interface
    void start(DeviceCallback onStart);
    void finish(DeviceCallback onFinish);
    void restart(DeviceCallback onRestart);
    void checkStatus(DeviceCallback onCheckStatus);
    void connectionStatus(DeviceCallback onConnection);
    void isConnected(OnIsConnectedCallback onIsConnected);

    // AlcotesterInterface interface
    void warmUp(DeviceCallback onWarmedUp);
    void activate(AlcotesterCallback onDataRead, DeviceCallback onDataRejected);
    void coolDown(DeviceCallback onCooledDown);
};
