#include <QTimer>

#include "alcotesterDeviceTest.h"

using namespace std;

AlcotesterDeviceTest::AlcotesterDeviceTest(QObject *parent)
    : QObject(parent)
{
    _personValues = QList<double>();
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
    _personValues.clear();
}

void AlcotesterDeviceTest::test(AlcoTestCallback callback)
{
    QTimer::singleShot(1000, [=] {
        float value = static_cast <float> (rand()) /( static_cast <float> (RAND_MAX));
        value = std::trunc(value * 20) / 10;
        _personValues.append(value);
        callback(OK, value);
    });
}

void AlcotesterDeviceTest::abort(DeviceCallback callback)
{
    callback(OK);
}

QList<double> AlcotesterDeviceTest::getPersonsValues() const
{
    return _personValues;
}
