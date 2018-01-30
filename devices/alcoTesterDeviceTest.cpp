#include <QTimer>

#include "alcotesterDeviceTest.h"

using namespace std;

AlcotesterDeviceTest::AlcotesterDeviceTest(QObject *parent)
    : QObject(parent)
{

}

// BaseDeviceInterface interface
void AlcotesterDeviceTest::start(DeviceCallback onStart)
{
    onStart(QSharedPointer<Status>(new Status(0, ALCOTESTER, START_DEVICE)));
}

void AlcotesterDeviceTest::finish(DeviceCallback onFinish)
{
    onFinish(QSharedPointer<Status>(new Status(0, ALCOTESTER, FINISH_DEVICE)));
}

void AlcotesterDeviceTest::restart(DeviceCallback onRestart)
{
    onRestart(QSharedPointer<Status>(new Status(0, ALCOTESTER, RESTART_DEVICE)));
}

void AlcotesterDeviceTest::checkStatus(DeviceCallback onCheckStatus)
{
    onCheckStatus(QSharedPointer<Status>(new Status(0, ALCOTESTER, CHECK_STATUS)));
}

void AlcotesterDeviceTest::connectionStatus(DeviceCallback onConnection)
{
    onConnection(QSharedPointer<Status>(new Status(DEVICE_CONNECTED, ALCOTESTER, CHECK_CONNECTION)));
    //_onError(QSharedPointer<Status>(new Status(CONNECTED, ALCOTESTER, CHECK_CONNECTION)));
}

void AlcotesterDeviceTest::isConnected(OnIsConnectedCallback onIsConnected)
{

}

// AlcotesterInterface interface
void AlcotesterDeviceTest::warmUp(DeviceCallback onWarmedUp)
{
    onWarmedUp(QSharedPointer<Status>(new Status(0, ALCOTESTER, WARMING_UP_ALCOTESTER)));
}

void AlcotesterDeviceTest::coolDown(DeviceCallback onCooledDown)
{
    onCooledDown(QSharedPointer<Status>(new Status(0, ALCOTESTER, COOLING_DOWN_ALCOTESTER)));
}

void AlcotesterDeviceTest::activate(AlcotesterCallback onDataRead, DeviceCallback onDataRejected)
{
    float value = static_cast <float> (rand()) /( static_cast <float> (RAND_MAX));
    value = std::trunc(value * 20) / 10;

    QTimer::singleShot(1000, [=, value] {
        onDataRead(QSharedPointer<Status>(new Status(0, ALCOTESTER, ACTIVATE_ALCOTESTER)), value);
    });
}
