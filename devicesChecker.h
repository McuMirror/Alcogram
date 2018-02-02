#pragma once

#include <QObject>

#include "devices/machinery.h"


class MainWindowInterface;

// uses for applying the same operation for all devices
class DevicesChecker : public QObject
{
    Q_OBJECT
public:
    explicit DevicesChecker(QObject* parent, MainWindowInterface* mainWindow);

    // start all devices
    void startDevices();

    // finish all devices
    void finishDevices();

    // check all devices status
    void checkDevicesStatus();

    // check all devices connection status
    void checkDevicesConnection();

    // сheck status for devices that have errors on the previous operation
    void checkDisabledDevicesStatus();

    // сheck the connection status for devices that have errors on the previous operation
    void checkDisabledDevicesConnection();

    // add error status
    void addDisabledDevice(QSharedPointer<Status> status);

    // get devices with errors
    QMap<DeviceName, QSharedPointer<Status>>& getDisabledDevices();

signals:
    // emits after all devices have started
    void devicesStarted();

    // emits after all devices have finished
    void devicesFinished();

    // emits after receiving the status of all devices
    void devicesStatusOk();

    // emits after receiving the connection status of all devices
    void devicesConnected();

    // emits after response to the start request came from all devices
    // but some devices are not started
    void someDevicesNotStarted();

    // emits after response to the finish request came from all devices
    // but some devices are not finished
    void someDevicesNotFinshed();

    // emits after receiving the status of all devices and status of some devices are not ok
    void someDevicesStatusNotOk();

    // emits after receiving the connection status of all devices and some devices are not connected
    void someDevicesNotConnected();

private:
    enum CheckMode {
        START_DEVICES, FINISH_DEVICES, CHECK_STATUS, CHECK_CONNECTION, IDLE
    };

    // check if all devices have responded to the request
    bool isAllDevicesHandled();

    void initCheck(CheckMode checkMode,bool checkAll);

    // calls on responce from device
    void deviceCheckFinished(QSharedPointer<Status> status, bool error = false);

    // set callback to Machinery signals
    void connectToMachinery();

    // reset all callback to Machinery signals
    void disconnectFromMachinery();

    // callback for Machinery signals
    void onDeviceStarted(QSharedPointer<Status> status);
    void onDeviceFinished(QSharedPointer<Status> status);
    void onError(QSharedPointer<Status> status);
    void onReceivedDeviceStatus(QSharedPointer<Status> status);
    void onReceivedDeviceConnectionStatus(QSharedPointer<Status> status);

    MainWindowInterface* _mainWindow;
    Machinery* _machinery;
    bool _checkAll; // true - check all devices, false - check only devices with errors
    int _handledDevicesCount; // number of devices which have responded to the request
    int _disabledDevicesCount; // number of devices with errors
                               // backup of _disabledDevices size before new operation on devices with errors
    CheckMode _checkMode = IDLE; // operaction type
    QMap<DeviceName, QSharedPointer<Status>> _disabledDevices; // devices with errors
};
