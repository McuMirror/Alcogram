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

public:
    void turnOn(DeviceCallback callback);
    void turnOff(DeviceCallback callback);
    void reset();
    void print(const QImage &image, int amount, PrinterCallback callback);

private:
    QTimer _timer;
    int _amountLeft;
};
