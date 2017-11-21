#include "printerDeviceTest.h"

PrinterDeviceTest::PrinterDeviceTest(QObject *parent) : QObject(parent)
{

}

void PrinterDeviceTest::turnOn(DeviceCallback callback)
{
    callback(OK);
}

void PrinterDeviceTest::turnOff(DeviceCallback callback)
{
    callback(OK);
}

void PrinterDeviceTest::reset()
{

}

void PrinterDeviceTest::print(const QImage& image, int amount, PrinterCallback callback)
{
    _amountLeft = amount;
    _timer.setInterval(1000);

    QObject::connect(&_timer, &QTimer::timeout, [=]{
        _amountLeft--;

        if (_amountLeft == 0) {
            _timer.stop();
        }

        callback(_amountLeft == 0 ? PRINT_COMPLETE : OK, amount - _amountLeft);
    });

    _timer.start();
}
