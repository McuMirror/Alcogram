#include "billAcceptorDeviceTest.h"

BillAcceptorDeviceTest::BillAcceptorDeviceTest(QObject *parent) : QObject(parent)
{

}

// BaseDeviceInterface interface
void BillAcceptorDeviceTest::start(DeviceCallback onStart)
{
    onStart(QSharedPointer<Status>(new Status(0, BILL_ACCEPTOR, START_DEVICE)));
}

void BillAcceptorDeviceTest::finish(DeviceCallback onFinish)
{
    onFinish(QSharedPointer<Status>(new Status(0, BILL_ACCEPTOR, FINISH_DEVICE)));
}

void BillAcceptorDeviceTest::restart(DeviceCallback onRestart)
{
    onRestart(QSharedPointer<Status>(new Status(0, BILL_ACCEPTOR, RESTART_DEVICE)));
}

void BillAcceptorDeviceTest::checkStatus(DeviceCallback onCheckStatus)
{
    onCheckStatus(QSharedPointer<Status>(new Status(STATUS_OK, BILL_ACCEPTOR, CHECK_STATUS)));
}

void BillAcceptorDeviceTest::connectionStatus(DeviceCallback onConnection)
{
    onConnection(QSharedPointer<Status>(new Status(DEVICE_CONNECTED, BILL_ACCEPTOR, CHECK_CONNECTION)));
}

void BillAcceptorDeviceTest::isConnected(OnIsConnectedCallback onIsConnected)
{

}

void BillAcceptorDeviceTest::activate(DeviceCallback onActivation)
{
    onActivation(QSharedPointer<Status>(new Status(0, BILL_ACCEPTOR, ACTIVATE_BILL_ACCEPTOR)));

    _timer.setInterval(1000);

    QObject::connect(&_timer, &QTimer::timeout, [=] {
        _onMoneyReceived(50, QSharedPointer<Status>(new Status(0, BILL_ACCEPTOR, MONEY_REQUEST)));
    });

    _timer.start();
}

void BillAcceptorDeviceTest::setBillAcceptorCallback(BillAcceptorCallback onMoneyReceived, DeviceCallback onMoneyRejected)
{
    _onMoneyReceived = onMoneyReceived;
    _onMoneyRejected = onMoneyRejected;
}

void BillAcceptorDeviceTest::deactivate(DeviceCallback onDeactivation)
{
    _timer.stop();
    _onMoneyReceived = nullptr;
    _onMoneyRejected = nullptr;

    onDeactivation(QSharedPointer<Status>(new Status(0, BILL_ACCEPTOR, DEACTIVATE_BILL_ACCEPTOR)));
}
