#pragma once

#include <QObject>
#include <QTimer>

#include "deviceInterfaces/billacceptorinterface.h"

class BillAcceptorDeviceTest : public QObject
                             , public BillAcceptorInterface
{
    Q_OBJECT
public:
    explicit BillAcceptorDeviceTest(QObject *parent = 0);

    // BaseDeviceInterface interface
    void start(DeviceCallback onStart);
    void finish(DeviceCallback onFinish);
    void restart(DeviceCallback onRestart);
    void checkStatus(DeviceCallback onCheckStatus);
    void connectionStatus(DeviceCallback onConnection);
    void isConnected(OnIsConnectedCallback onIsConnected);

    // BillAcceptorInterface interface
    void activate(DeviceCallback onActivation);
    void setBillAcceptorCallback(BillAcceptorCallback onMoneyReceived, DeviceCallback onMoneyRejected);
    void deactivate(DeviceCallback onDeactivation);

private:
    BillAcceptorCallback _onMoneyReceived;
    DeviceCallback _onMoneyRejected;
    QTimer _timer;
};
