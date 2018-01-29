#pragma once

#include <QObject>

#include "devices/machinery.h"


class MainWindowInterface;

class DevicesChecker : public QObject
{
    Q_OBJECT
public:
    explicit DevicesChecker(QObject* parent, MainWindowInterface* mainWindow);

    void startDevices();
    void finishDevices();
    void checkDevicesStatus();
    void checkDevicesConnection();

    void checkDisabledDevicesStatus();
    void checkDisabledDevicesConnection();

signals:
    void devicesStarted();
    void devicesFinished();
    void devicesStatusOk();
    void devicesConnected();

    void someDevicesNotStarted();
    void someDevicesNotFinshed();
    void someDevicesStatusNotOk();
    void someDevicesNotConnected();

private:
    enum CheckMode {
        START_DEVICES, FINISH_DEVICES, CHECK_STATUS, CHECK_CONNECTION, IDLE
    };

    bool isAllDevicesHandled();
    void initCheck(CheckMode checkMode,bool checkAll);
    void deviceCheckFinished(QSharedPointer<Status> status, bool error = false);

    void onDeviceStarted(QSharedPointer<Status> status);
    void onDeviceFinished(QSharedPointer<Status> status);
    void onError(QSharedPointer<Status> status);
    void onReceivedDeviceStatus(QSharedPointer<Status> status);
    void onReceivedDeviceConnectionStatus(QSharedPointer<Status> status);

    MainWindowInterface* _mainWindow;
    Machinery* _machinery;
    bool _checkAll;
    int _handledDevicesCount;
    int _disabledDevicesCount;
    CheckMode _checkMode = IDLE;
    QMap<DeviceName, QSharedPointer<Status>> _disabledDevices;
};
