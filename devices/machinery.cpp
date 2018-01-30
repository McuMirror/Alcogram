#include <functional>

#include "machinery.h"
#include "devices/alcoTesterDeviceTest.h"
#include "devices/cameraDeviceTest.h"
#include "devices/posDeviceTest.h"
#include "devices/printerDeviceTest.h"

using namespace std::placeholders;

Machinery::Machinery(QObject *parent)
    : QObject(parent)
    , _camera(new CameraDeviceTest(this))
    , _alcotester(new AlcotesterDeviceTest(this))
    , _pos(new POSDeviceTest(this))
    , _printer(new PrinterDeviceTest(this))
{   
    _devices.insert(CAMERA, _camera);
    _devices.insert(ALCOTESTER, _alcotester);
    _devices.insert(POS, _pos);
    _devices.insert(PRINTER, _printer);

    for (BaseDeviceInterface* device : _devices.values()) {
        device->setOnErrorCallback(std::bind(&Machinery::onError, this, _1));
    }

    for (int deviceName = CAMERA; deviceName <= PRINTER; deviceName++) {
        _requests.insert(static_cast<DeviceName>(deviceName), QSet<RequestName>());
        _requestTimers.insert(static_cast<DeviceName>(deviceName), QMap<RequestName, QSharedPointer<QTimer>>());
    }
}

QList<DeviceName> Machinery::getDeviceNames() const
{
    return _devices.keys();
}

int Machinery::devicesCount() const
{
    return _devices.size();
}

// base device operations
void Machinery::start(DeviceName deviceName)
{
    QSharedPointer<QTimer> requestTimer = registerRequest(deviceName, START_DEVICE);

    _devices[deviceName]->start(std::bind(&Machinery::onDeviceStart, this, _1));

    requestTimer->start();

}

void Machinery::finish(DeviceName deviceName)
{
    QSharedPointer<QTimer> requestTimer = registerRequest(deviceName, FINISH_DEVICE);

    _devices[deviceName]->finish(std::bind(&Machinery::onDeviceFinish, this, _1));

    requestTimer->start();
}

void Machinery::restart(DeviceName deviceName)
{
    QSharedPointer<QTimer> requestTimer = registerRequest(deviceName, RESTART_DEVICE);

    _devices[deviceName]->restart(std::bind(&Machinery::onDeviceRestart, this, _1));

    requestTimer->start();
}

void Machinery::checkStatus(DeviceName deviceName)
{
    QSharedPointer<QTimer> requestTimer = registerRequest(deviceName, CHECK_STATUS);

    _devices[deviceName]->checkStatus(std::bind(&Machinery::onDeviceCheckStatus, this, _1));

    requestTimer->start();
}

void Machinery::checkConnectionStatus(DeviceName deviceName)
{
    QSharedPointer<QTimer> requestTimer = registerRequest(deviceName, CHECK_CONNECTION);

    _devices[deviceName]->connectionStatus(std::bind(&Machinery::onDeviceCheckConnection, this, _1));

    requestTimer->start();
}

// camera device operations
void Machinery::getImage()
{
    QSharedPointer<QTimer> requestTimer = registerRequest(CAMERA, GET_IMAGE);

    _camera->getImage(std::bind(&Machinery::onGetImage, this, _1, _2));

    requestTimer->start();
}

void Machinery::takeImage()
{
    QSharedPointer<QTimer> requestTimer = registerRequest(CAMERA, TAKE_IMAGE);

    _camera->takeImage(std::bind(&Machinery::onTakeImage, this, _1, _2));

    requestTimer->start();
}

void Machinery::stopGetImage()
{
    QSharedPointer<QTimer> requestTimer = registerRequest(CAMERA, STOP_GET_IMAGE);

    _camera->stopGetImage(std::bind(&Machinery::onStopGetImage, this, _1));

    requestTimer->start();
}

// alcotester device operations
void Machinery::warmingUpAlcotester()
{
    QSharedPointer<QTimer> requestTimer = registerRequest(ALCOTESTER, WARMING_UP_ALCOTESTER);

    _alcotester->warmUp(std::bind(&Machinery::onWarmingUpAlcotester, this, _1));

    requestTimer->start();
}

void Machinery::coolingDownAlcotester()
{
    QSharedPointer<QTimer> requestTimer = registerRequest(ALCOTESTER, COOLING_DOWN_ALCOTESTER);

    _alcotester->coolDown(std::bind(&Machinery::onCoolingDownAlcotester, this, _1));

    requestTimer->start();
}

void Machinery::activateAlcotester()
{
    QSharedPointer<QTimer> requestTimer = registerRequest(ALCOTESTER, ACTIVATE_ALCOTESTER);

    _alcotester->activate(std::bind(&Machinery::onAlcotesterDataReadSuccess, this, _1, _2)
                          , std::bind(&Machinery::onAlcotesterDataReadFail, this, _1));

    requestTimer->start();
}

// POS device operations
void Machinery::activatePOS()
{
    QSharedPointer<QTimer> requestTimer = registerRequest(POS, ACTIVATE_POS);

    _pos->activate(std::bind(&Machinery::onActivatePOS, this, _1));

    requestTimer->start();
}

void Machinery::deactivatePOS()
{
    QSharedPointer<QTimer> requestTimer = registerRequest(POS, DEACTIVATE_POS);

    _pos->deactivate(std::bind(&Machinery::onDeactivatePOS, this, _1));

    requestTimer->start();
}

void Machinery::takeMoney(double money)
{
    QSharedPointer<QTimer> requestTimer = registerRequest(POS, TAKE_MONEY);

    _pos->takeMoney(money, std::bind(&Machinery::onTransactionSucceded, this, _1, _2)
                          , std::bind(&Machinery::onTransactionFailed, this, _1));

    requestTimer->start();
}

// printer device operations

void Machinery::warmingUpPrinter()
{
    QSharedPointer<QTimer> requestTimer = registerRequest(PRINTER, WARMING_UP_PRINTER);

    _printer->warmUp(std::bind(&Machinery::onWarmingUpPrinter, this, _1));

    requestTimer->start();
}

void Machinery::coolingDownPrinter()
{
    QSharedPointer<QTimer> requestTimer = registerRequest(PRINTER, COOLING_DOWN_PRINTER);

    _printer->coolDown(std::bind(&Machinery::onCoolingDownPrinter, this, _1));

    requestTimer->start();
}

void Machinery::printImage(const QImage &image)
{
    QSharedPointer<QTimer> requestTimer = registerRequest(PRINTER, PRINT_IMAGE);

    _printer->printImage(image, std::bind(&Machinery::onPrintImage, this, _1));

    requestTimer->start();
}

QSharedPointer<QTimer> Machinery::registerRequest(DeviceName deviceName, RequestName requestName)
{
    _requests[deviceName].insert(requestName);

    // sets request timeout timer
    QSharedPointer<QTimer> requestTimer = QSharedPointer<QTimer>(new QTimer());
    requestTimer->setSingleShot(true);
    requestTimer->setInterval(2000); // TODO: get interval from config

    _requestTimers[deviceName].insert(requestName, requestTimer);

    QObject::connect(requestTimer.data(), &QTimer::timeout, [this, deviceName, requestName]{
        // request timeout
        _requestTimers[deviceName].remove(requestName);
        _requests[deviceName].remove(requestName);
        emit error(QSharedPointer<Status>(new Status(REQUEST_TIMEOUT, deviceName, requestName)));
    });

    return requestTimer;
}

bool Machinery::removeRequest(DeviceName deviceName, RequestName requestName)
{
    if (_requests[deviceName].contains(requestName)) {
        // request was registered
        _requestTimers[deviceName][requestName]->stop();
        _requestTimers[deviceName].remove(requestName);
        _requests[deviceName].remove(requestName);

        return true;
    }

    // request was not registered
    return false;
}


// base device operation callbacks
void Machinery::onDeviceStart(QSharedPointer<Status> status)
{
    if (removeRequest(status->getDeviceName(), START_DEVICE)) {
        emit deviceHasStarted(status);
    }
}


void Machinery::onDeviceFinish(QSharedPointer<Status> status)
{
    if (removeRequest(status->getDeviceName(), FINISH_DEVICE)) {
        emit deviceHasFinished(status);
    }
}

void Machinery::onDeviceRestart(QSharedPointer<Status> status)
{
    if (removeRequest(status->getDeviceName(), RESTART_DEVICE)) {
        emit deviceHasRestarted(status);
    }
}

void Machinery::onDeviceCheckStatus(QSharedPointer<Status> status)
{
    if (removeRequest(status->getDeviceName(), CHECK_STATUS)) {
        emit receivedDeviceStatus(status);
    }
}

void Machinery::onDeviceCheckConnection(QSharedPointer<Status> status)
{
    if (removeRequest(status->getDeviceName(), CHECK_CONNECTION)) {
        emit receivedDeviceConnectionStatus(status);
    }
}

// camera device opeation callbacks
void Machinery::onGetImage(QSharedPointer<QImage> image, QSharedPointer<Status> status)
{
    if (removeRequest(status->getDeviceName(), GET_IMAGE)) {
        // register request for next frame from the camera
        QSharedPointer<QTimer> requestTimer = registerRequest(CAMERA, GET_IMAGE);
        requestTimer->start();
        emit receivedNextFrame(image, status);
    }
}

void Machinery::onTakeImage(QSharedPointer<QImage> image, QSharedPointer<Status> status)
{
    if (removeRequest(status->getDeviceName(), TAKE_IMAGE)) {
        emit receivedCapturedImage(image, status);
    }
}

void Machinery::onStopGetImage(QSharedPointer<Status> status)
{
    removeRequest(status->getDeviceName(), STOP_GET_IMAGE);
}

// alcotester device operation callbacks
void Machinery::onWarmingUpAlcotester(QSharedPointer<Status> status)
{
    if (removeRequest(status->getDeviceName(), WARMING_UP_ALCOTESTER)) {
        emit alcotesterWarmedUp(status);
    }
}

void Machinery::onCoolingDownAlcotester(QSharedPointer<Status> status)
{
    if (removeRequest(status->getDeviceName(), COOLING_DOWN_ALCOTESTER)) {
        emit alcotesterCooledDown(status);
    }
}

void Machinery::onAlcotesterDataReadSuccess(QSharedPointer<Status> status, double value)
{
    if (removeRequest(status->getDeviceName(), ACTIVATE_ALCOTESTER)) {
        emit receivedAlcotesterData(status, value);
    }
}

void Machinery::onAlcotesterDataReadFail(QSharedPointer<Status> status)
{
    if (removeRequest(status->getDeviceName(), ACTIVATE_ALCOTESTER)) {
        emit failedToReceiveDataFromAcotester(status);
    }
}

// POS device operation callbacks
void Machinery::onActivatePOS(QSharedPointer<Status> status)
{
    if (removeRequest(status->getDeviceName(), ACTIVATE_POS)) {
        emit posActivated(status);
    }
}

void Machinery::onDeactivatePOS(QSharedPointer<Status> status)
{
    if (removeRequest(status->getDeviceName(), DEACTIVATE_POS)) {
        emit posDeactivated(status);
    }
}

void Machinery::onTransactionSucceded(double money, QSharedPointer<Status> status)
{
    if (removeRequest(status->getDeviceName(), TAKE_MONEY)) {
        emit transactionSucceded(money, status);
    }
}

void Machinery::onTransactionFailed(QSharedPointer<Status> status)
{
    if (removeRequest(status->getDeviceName(), TAKE_MONEY)) {
        emit transactionFailed(status);
    }
}

// printer device operation callback
void Machinery::onWarmingUpPrinter(QSharedPointer<Status> status)
{
    if (removeRequest(status->getDeviceName(), WARMING_UP_PRINTER)) {
        emit printerWarmedUp(status);
    }
}

void Machinery::onCoolingDownPrinter(QSharedPointer<Status> status)
{
    if (removeRequest(status->getDeviceName(), COOLING_DOWN_PRINTER)) {
        emit printerCooledDown(status);
    }
}

void Machinery::onPrintImage(QSharedPointer<Status> status)
{
    if (removeRequest(status->getDeviceName(), PRINT_IMAGE)) {
        emit imagePrinted(status);
    }
}

void Machinery::onError(QSharedPointer<Status> status)
{
    if (removeRequest(status->getDeviceName(), status->getRequestName())) {
        emit error(status);
    }
}
