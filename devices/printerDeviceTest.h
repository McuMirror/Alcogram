#pragma once

#include <QObject>
#include <QTimer>

#include "deviceInterfaces/printerinterface.h"

class PrinterDeviceTest : public QObject
                        , public PrinterInterface
{
    Q_OBJECT
public:
    explicit PrinterDeviceTest(QObject *parent = 0);

    // BaseDeviceInterface interface
    void start(DeviceCallback onStart);
    void finish(DeviceCallback onFinish);
    void restart(DeviceCallback onRestart);
    void checkStatus(DeviceCallback onCheckStatus);
    void connectionStatus(DeviceCallback onConnection);
    void isConnected(OnIsConnectedCallback onIsConnected);

    // PrinterInterface interface
    void warmUp(DeviceCallback onWarmUp);
    void coolDown(DeviceCallback onCoolDown);
    void printImage(const QImage &image, DeviceCallback onImagePrinted);

private:
    QTimer _timer;
};
