#pragma once

#include <QObject>
#include <QMap>
#include <QSet>
#include <QTimer>

#include "deviceInterfaces/alcotesterinterface.h"
#include "deviceInterfaces/cameraInterface.h"
#include "deviceInterfaces/posinterface.h"
#include "deviceInterfaces/printerinterface.h"

class Machinery : public QObject
{
    Q_OBJECT
public:
    explicit Machinery(QObject *parent = 0);

    // base device operations
    void start(DeviceName deviceName);
    void finish(DeviceName deviceName);
    void restart(DeviceName deviceName);
    void checkStatus(DeviceName deviceName);
    void checkConnectionStatus(DeviceName deviceName);

    // camera device operations
    void getImage();
    void takeImage();
    void stopGetImage();

    // alcotester device opeations
    void warmingUpAlcotester();
    void activateAlcotester();
    void coolingDownAlcotester();

    // pos device operations
    void activatePOS();
    void deactivatePOS();
    void takeMoney(double money);

    // printer device operations
    void warmingUpPrinter();
    void coolingDownPrinter();
    void printImage(const QImage& image);

signals:
    // base device operation signals
    void deviceStarted(QSharedPointer<Status> status);
    void deviceFinished(QSharedPointer<Status> status);
    void deviceRestarted(QSharedPointer<Status> status);
    void receivedDeviceStatus(QSharedPointer<Status> status);
    void receivedDeviceConnectionStatus(QSharedPointer<Status> status);

    // camera device operation signals
    void receivedNextFrame(QSharedPointer<QImage> image, QSharedPointer<Status> status);
    void receivedCapturedImage(QSharedPointer<QImage> image, QSharedPointer<Status> status);

    // alcotester device operation signals
    void alcotesterWarmedUp(QSharedPointer<Status> status);
    void alcotesterCooledDown(QSharedPointer<Status> status);
    void receivedAlcotesterData(QSharedPointer<Status> status, double value);
    void failedToReceiveDataFromAcotester(QSharedPointer<Status> status);

    // POS device operation signals
    void posActivated(QSharedPointer<Status> status);
    void posDeactivated(QSharedPointer<Status> status);
    void transactionSucceded(double money, QSharedPointer<Status> status);
    void transactionFailed(QSharedPointer<Status> status);

    // printer device operation signals
    void printerWarmedUp(QSharedPointer<Status> status);
    void printerCooledDown(QSharedPointer<Status> status);
    void imagePrinted(QSharedPointer<Status> status);

    void requestTimeout(QSharedPointer<Status> status);

private:
    QSharedPointer<QTimer> registerRequest(DeviceName deviceName, RequestName requestName);
    bool removeRequest(DeviceName deviceName, RequestName requestName);

    // base device operation callbacks
    void onDeviceStart(QSharedPointer<Status> status);
    void onDeviceFinish(QSharedPointer<Status> status);
    void onDeviceRestart(QSharedPointer<Status> status);
    void onDeviceCheckStatus(QSharedPointer<Status> status);
    void onDeviceCheckConnection(QSharedPointer<Status> status);

    // camera device operation callbacks
    void onGetImage(QSharedPointer<QImage> image, QSharedPointer<Status> status);
    void onTakeImage(QSharedPointer<QImage> image, QSharedPointer<Status> status);
    void onStopGetImage(QSharedPointer<Status> status);

    // alcotester device operation callbacks
    void onWarmingUpAlcotester(QSharedPointer<Status> status);
    void onCoolingDownAlcotester(QSharedPointer<Status> status);
    void onAlcotesterDataReadSuccess(QSharedPointer<Status> status, double value);
    void onAlcotesterDataReadFail(QSharedPointer<Status> status);

    // POS device operation callbacks
    void onActivatePOS(QSharedPointer<Status> status);
    void onDeactivatePOS(QSharedPointer<Status> status);
    void onTransactionSucceded(double money, QSharedPointer<Status> status);
    void onTransactionFailed(QSharedPointer<Status> status);

    // printer device operation callbacks
    void onWarmingUpPrinter(QSharedPointer<Status> status);
    void onCoolingDownPrinter(QSharedPointer<Status> status);
    void onPrintImage(QSharedPointer<Status> status);

    QMap<DeviceName, QSet<RequestName>> _requests;
    QMap<DeviceName, QMap<RequestName, QSharedPointer<QTimer>>> _requestTimers;
    QMap<DeviceName, BaseDeviceInterface*> _devices;

    CameraInterface* _camera;
    AlcotesterInterface* _alcotester;
    POSInterface* _pos;
    PrinterInterface* _printer;
};
