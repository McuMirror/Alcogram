#include <stdlib.h>
#include <time.h>

#include "posDeviceTest.h"

POSDeviceTest::POSDeviceTest(QObject *parent)
    : QObject(parent)
{
}

// BaseDeviceInterface interface
void POSDeviceTest::start(DeviceCallback onStart)
{
    onStart(QSharedPointer<Status>(new Status(0, POS, START_DEVICE)));
}

void POSDeviceTest::finish(DeviceCallback onFinish)
{
    onFinish(QSharedPointer<Status>(new Status(0, POS, FINISH_DEVICE)));
}

void POSDeviceTest::restart(DeviceCallback onRestart)
{
    onRestart(QSharedPointer<Status>(new Status(0, POS, RESTART_DEVICE)));
}

void POSDeviceTest::checkStatus(DeviceCallback onCheckStatus)
{
    onCheckStatus(QSharedPointer<Status>(new Status(STATUS_OK, POS, CHECK_STATUS)));
}

void POSDeviceTest::connectionStatus(DeviceCallback onConnection)
{
    onConnection(QSharedPointer<Status>(new Status(DEVICE_CONNECTED, POS, CHECK_CONNECTION)));
}

void POSDeviceTest::isConnected(OnIsConnectedCallback onIsConnected)
{

}

// POSInterface interface
void POSDeviceTest::activate(DeviceCallback onActivation)
{
    onActivation(QSharedPointer<Status>(new Status(0, POS, ACTIVATE_POS)));
}

void POSDeviceTest::deactivate(DeviceCallback onDeactivation)
{
    onDeactivation(QSharedPointer<Status>(new Status(0, POS, ACTIVATE_POS)));
}

void POSDeviceTest::takeMoney(double money, POSCallback onTransactionSucceded, DeviceCallback onTransactionFailed)
{
    //onTransactionSucceded(money, QSharedPointer<Status>(new Status(0, POS, TAKE_MONEY)));
}
