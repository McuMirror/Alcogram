#pragma once

#include <QObject>
#include <QTimer>

#include "deviceInterfaces/posinterface.h"

class POSDeviceTest : public QObject
                    , public POSInterface
{
    Q_OBJECT
public:
    explicit POSDeviceTest(QObject *parent = 0);

    // BaseDeviceInterface interface
    void start(DeviceCallback onStart);
    void finish(DeviceCallback onFinish);
    void restart(DeviceCallback onRestart);
    void checkStatus(DeviceCallback onCheckStatus);
    void connectionStatus(DeviceCallback onConnection);
    void isConnected(OnIsConnectedCallback onIsConnected);

    // POSInterface interface
    void activate(DeviceCallback onActivation);
    void deactivate(DeviceCallback onDeactivation);
    void takeMoney(double money, POSCallback onTransactionSucceded, DeviceCallback onTransactionFailed);
};
