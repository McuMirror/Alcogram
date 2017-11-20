#include "alcotesterDeviceTest.h"

AlcotesterDeviceTest::AlcotesterDeviceTest(QObject *parent)
    : QObject(parent)
{

}

void AlcotesterDeviceTest::turnOn(DeviceCallback callback)
{
    callback(OK);
}

void AlcotesterDeviceTest::turnOff(DeviceCallback callback)
{
    callback(OK);
}

void AlcotesterDeviceTest::reset()
{

}

void AlcotesterDeviceTest::test(AlcoTestCallback callback)
{
    //callback(OK, 1.5);
}

void AlcotesterDeviceTest::abort(DeviceCallback callback)
{
    callback(OK);
}

QList<double> AlcotesterDeviceTest::getPersonsValues() const
{
    return QList<double>();
}
