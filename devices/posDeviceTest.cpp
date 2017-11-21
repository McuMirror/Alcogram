#include <stdlib.h>
#include <time.h>

#include "posDeviceTest.h"

POSDeviceTest::POSDeviceTest(QObject *parent)
    : QObject(parent)
{
}

void POSDeviceTest::turnOn(DeviceCallback callback)
{
    callback(OK);
}

void POSDeviceTest::turnOff(DeviceCallback callback)
{
    callback(OK);
}

void POSDeviceTest::sendPrice(int price, DeviceCallback callback)
{
    _price = price;
    callback(OK);
}

void POSDeviceTest::getPaymentResponce(PayCallback payCallback)
{
    _enteredAmount = 0;

    srand(time(NULL));

    _timer.setInterval(1000);
    QObject::disconnect(&_timer, &QTimer::timeout, 0, 0);
    QObject::connect(&_timer, &QTimer::timeout, [=] {
        int i = rand() % 2;

        _enteredAmount += _denominations[i];

        payCallback(OK, 100);//_enteredAmount);

        _timer.stop();
        /*if (_enteredAmount >= _price) {
            _timer.stop();
        }*/
    });
    _timer.start();
}

void POSDeviceTest::abortPayment(DeviceCallback callback)
{
    _timer.stop();
}

void POSDeviceTest::reset()
{

}
