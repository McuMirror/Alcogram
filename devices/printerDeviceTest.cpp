#include "printerDeviceTest.h"

PrinterDeviceTest::PrinterDeviceTest(QObject *parent) : QObject(parent)
{

}

// BaseDeviceInterface interface
void PrinterDeviceTest::start(DeviceCallback onStart)
{
    onStart(QSharedPointer<Status>(new Status(0, PRINTER, START_DEVICE)));
}

void PrinterDeviceTest::finish(DeviceCallback onFinish)
{
    onFinish(QSharedPointer<Status>(new Status(0, PRINTER, FINISH_DEVICE)));
}

void PrinterDeviceTest::restart(DeviceCallback onRestart)
{
    onRestart(QSharedPointer<Status>(new Status(0, PRINTER, RESTART_DEVICE)));
}

void PrinterDeviceTest::checkStatus(DeviceCallback onCheckStatus)
{
    onCheckStatus(QSharedPointer<Status>(new Status(STATUS_OK, PRINTER, CHECK_STATUS)));
}

void PrinterDeviceTest::connectionStatus(DeviceCallback onConnection)
{
    onConnection(QSharedPointer<Status>(new Status(DEVICE_CONNECTED, PRINTER, CHECK_CONNECTION)));
}

void PrinterDeviceTest::isConnected(OnIsConnectedCallback onIsConnected)
{

}

// PrinterInterface interface
void PrinterDeviceTest::warmUp(DeviceCallback onWarmUp)
{
    onWarmUp(QSharedPointer<Status>(new Status(0, PRINTER, WARMING_UP_PRINTER)));
}

void PrinterDeviceTest::coolDown(DeviceCallback onCoolDown)
{
    onCoolDown(QSharedPointer<Status>(new Status(0, PRINTER, COOLING_DOWN_PRINTER)));
}

void PrinterDeviceTest::printImage(const QImage &image, DeviceCallback onImagePrinted)
{
    QTimer::singleShot(1000, [=] {
            onImagePrinted(QSharedPointer<Status>(new Status(0, PRINTER, PRINT_IMAGE)));
        });
}
