#include "devicesChecker.h"
#include "interfaces/mainwindowinterface.h"

DevicesChecker::DevicesChecker(QObject* parent, MainWindowInterface* mainWindow)
    : QObject(parent)
{
    _mainWindow = mainWindow;
    _machinery = _mainWindow->getMachinery();

    QObject::connect(_machinery, &Machinery::deviceStarted, this, &DevicesChecker::onDeviceStarted);
    QObject::connect(_machinery, &Machinery::deviceFinished, this, &DevicesChecker::onDeviceFinished);
    QObject::connect(_machinery, &Machinery::error, this, &DevicesChecker::onError);
    QObject::connect(_machinery, &Machinery::receivedDeviceStatus, this, &DevicesChecker::onReceivedDeviceStatus);
    QObject::connect(_machinery, &Machinery::receivedDeviceConnectionStatus
                     , this, &DevicesChecker::onReceivedDeviceConnectionStatus);
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
}

void DevicesChecker::deviceCheckFinished(QSharedPointer<Status> status, bool error)
{
    if (_checkMode == IDLE) {
        return;
    }

    _handledDevicesCount++;

    if (error) {
        _disabledDevices.insert(status->getDeviceName(), status);
    }

    if (isAllDevicesHandled()) {
        if (_disabledDevices.isEmpty()) {
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

        _checkMode = IDLE;
    }
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
    deviceCheckFinished(status);
}

void DevicesChecker::onReceivedDeviceConnectionStatus(QSharedPointer<Status> status)
{
    deviceCheckFinished(status);
}
