#include "devicesChecker.h"
#include "interfaces/mainwindowinterface.h"

DevicesChecker::DevicesChecker(QObject* parent, MainWindowInterface* mainWindow)
    : QObject(parent)
{
    _mainWindow = mainWindow;
    _machinery = _mainWindow->getMachinery();
}

void DevicesChecker::startDevices()
{
    initCheck(START_DEVICES, true);

    for (DeviceName deviceName : _machinery->getDeviceNames()) {
        _machinery->start(deviceName);
    }
}

void DevicesChecker::finishDevices()
{
    initCheck(FINISH_DEVICES, true);

    for (DeviceName deviceName : _machinery->getDeviceNames()) {
        _machinery->finish(deviceName);
    }
}

void DevicesChecker::checkDevicesStatus()
{
    initCheck(CHECK_STATUS, true);

    for (DeviceName deviceName : _machinery->getDeviceNames()) {
        _machinery->checkStatus(deviceName);
    }
}

void DevicesChecker::checkDevicesConnection()
{
    initCheck(CHECK_CONNECTION, true);

    for (DeviceName deviceName : _machinery->getDeviceNames()) {
        _machinery->checkConnectionStatus(deviceName);
    }
}

void DevicesChecker::checkDisabledDevicesStatus()
{
    initCheck(CHECK_STATUS, false);
}

void DevicesChecker::checkDisabledDevicesConnection()
{
    initCheck(CHECK_CONNECTION, false);
}

void DevicesChecker::addDisabledDevice(QSharedPointer<Status> status)
{
    _disabledDevices.insert(status->getDeviceName(), status);
}

QMap<DeviceName, QSharedPointer<Status>>& DevicesChecker::getDisabledDevices()
{
    return _disabledDevices;
}

bool DevicesChecker::isAllDevicesHandled()
{
    return _checkAll && _handledDevicesCount == _machinery->devicesCount()
            || !_checkAll && _handledDevicesCount == _disabledDevicesCount;
}

void DevicesChecker::initCheck(CheckMode checkMode, bool checkAll)
{
    if (!checkAll) {
        _disabledDevicesCount = _disabledDevices.size();
    }

    _disabledDevices.clear();
    _checkAll = checkAll;
    _handledDevicesCount = 0;
    _checkMode = checkMode;

    connectToMachinery();
}

void DevicesChecker::deviceCheckFinished(QSharedPointer<Status> status, bool error)
{
    if (_checkMode == IDLE) {
        return;
    }

    _handledDevicesCount++;

    if (error) {
        // error status
        _disabledDevices.insert(status->getDeviceName(), status);
    }

    if (isAllDevicesHandled()) {
        // response came from all devices

        if (_disabledDevices.isEmpty()) {
            // ok

            switch (_checkMode) {
                case START_DEVICES:
                    emit devicesStarted();
                    break;
                case FINISH_DEVICES:
                    emit devicesFinished();
                    break;
                case CHECK_STATUS:
                    emit devicesStatusOk();
                    break;
                case CHECK_CONNECTION:
                    emit devicesConnected();
                    break;
            }
        } else {
            // response from some devices came with an error

            switch (_checkMode) {
                case START_DEVICES:
                    emit someDevicesNotStarted();
                    break;
                case FINISH_DEVICES:
                    emit someDevicesNotFinshed();
                    break;
                case CHECK_STATUS:
                    emit someDevicesStatusNotOk();
                    break;
                case CHECK_CONNECTION:
                    emit someDevicesNotConnected();
                    break;
            }
        }

        disconnectFromMachinery();

        _checkMode = IDLE;
    }
}

void DevicesChecker::connectToMachinery()
{
    switch (_checkMode) {
        case START_DEVICES:
            QObject::connect(_machinery, &Machinery::deviceHasStarted, this, &DevicesChecker::onDeviceStarted);
            break;
        case FINISH_DEVICES:
            QObject::connect(_machinery, &Machinery::deviceHasFinished, this, &DevicesChecker::onDeviceFinished);
            break;
        case CHECK_STATUS:
            QObject::connect(_machinery, &Machinery::receivedDeviceStatus, this, &DevicesChecker::onReceivedDeviceStatus);
            break;
        case CHECK_CONNECTION:
            QObject::connect(_machinery, &Machinery::receivedDeviceConnectionStatus
                         , this, &DevicesChecker::onReceivedDeviceConnectionStatus);
            break;
    }

    QObject::connect(_machinery, &Machinery::error, this, &DevicesChecker::onError);
}

void DevicesChecker::disconnectFromMachinery()
{
    switch (_checkMode) {
        case START_DEVICES:
            QObject::disconnect(_machinery, &Machinery::deviceHasStarted, 0, 0);
            break;
        case FINISH_DEVICES:
            QObject::disconnect(_machinery, &Machinery::deviceHasFinished, 0, 0);
            break;
        case CHECK_STATUS:
            QObject::disconnect(_machinery, &Machinery::receivedDeviceStatus, 0, 0);
            break;
        case CHECK_CONNECTION:
            QObject::disconnect(_machinery, &Machinery::receivedDeviceConnectionStatus, 0, 0);
            break;
    }

    QObject::disconnect(_machinery, &Machinery::error, 0, 0);
}

void DevicesChecker::onDeviceStarted(QSharedPointer<Status> status)
{
    deviceCheckFinished(status);
}

void DevicesChecker::onDeviceFinished(QSharedPointer<Status> status)
{
    deviceCheckFinished(status);
}

void DevicesChecker::onError(QSharedPointer<Status> status)
{
    deviceCheckFinished(status, true);
}

void DevicesChecker::onReceivedDeviceStatus(QSharedPointer<Status> status)
{
    deviceCheckFinished(status, status->getErrorCode() == STATUS_NOT_OK);
}

void DevicesChecker::onReceivedDeviceConnectionStatus(QSharedPointer<Status> status)
{
    deviceCheckFinished(status, status->getErrorCode() == DEVICE_NOT_CONNECTED);
}
