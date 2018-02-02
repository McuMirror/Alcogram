#pragma once

#include <QObject>
#include <QMap>
#include <QSet>
#include <QTimer>

#include "deviceInterfaces/alcotesterinterface.h"
#include "deviceInterfaces/cameraInterface.h"
#include "deviceInterfaces/posinterface.h"
#include "deviceInterfaces/printerinterface.h"
#include "deviceInterfaces/billacceptorinterface.h"

#include "configManager.h"

// class for interaction with devices via device interfaces
class Machinery : public QObject
{
    Q_OBJECT
public:
    explicit Machinery(QObject *parent = 0);

    // get device names
    QList<DeviceName> getDeviceNames() const;

    // returns the number of devices
    int devicesCount() const;

    //###########
    // base device operations
    // correspond to the same methods in device interfaces
    //###########

    // start device
    // @param deviceName - name of device
    void start(DeviceName deviceName);

    // finish the device
    // @param deviceName - name of device
    void finish(DeviceName deviceName);

    // restart the device
    // @param deviceName - name of device
    void restart(DeviceName deviceName);

    // check device status
    // @param deviceName - name of device
    void checkStatus(DeviceName deviceName);

    // check device connection
    // @param deviceName - name of device
    void checkConnectionStatus(DeviceName deviceName);

    //###########
    // camera device operations
    //###########

    // sets callback receivedNextFrame for receiving frames from camera
    void getImage();

    // sets callback receivedCapturedImage for receiving captured frame form camera
    void takeImage();

    // reset callback for receiving frames from camera
    void stopGetImage();

    //###########
    // alcotester device opeations
    //###########

    // start warming up alcotester
    void warmingUpAlcotester();

    // activate alcotester for receiving data
    void activateAlcotester();

    // start cooling down alcotester
    void coolingDownAlcotester();

    //###########
    // POS device operations
    //###########

    // activate POS device
    void activatePOS();

    // deactivate POS device
    void deactivatePOS();

    // send amount of money to POS device for payment
    // @param money - amount of money
    void takeMoney(double money);

    //###########
    // bill acceptor device operations
    //###########

    void activateBillAcceptor();

    void deactivateBillAcceptor();

    //###########
    // printer device operations
    //###########

    // start warming up printer
    void warmingUpPrinter();

    // start cooling down printer
    void coolingDownPrinter();

    // send image to printer for printing
    // @param image - image to print
    void printImage(const QImage& image);

signals:
    //###########
    // base device operation signals
    //###########

    // emits after device has started
    void deviceHasStarted(QSharedPointer<Status> status);

    // emits after device has finished
    void deviceHasFinished(QSharedPointer<Status> status);

    // emits after device has restarted
    void deviceHasRestarted(QSharedPointer<Status> status);

    // emits when a device status was received
    void receivedDeviceStatus(QSharedPointer<Status> status);

    // emits when a device connection status was received
    void receivedDeviceConnectionStatus(QSharedPointer<Status> status);

    //###########
    // camera device operation signals
    //###########

    // emits when frame was obtained from the camera on GET_IMAGE request
    void receivedNextFrame(QSharedPointer<QImage> image, QSharedPointer<Status> status);

    // emits when frame was obtained from the camera on TAKE_IMAGE request
    void receivedCapturedImage(QSharedPointer<QImage> image, QSharedPointer<Status> status);

    //###########
    // alcotester device operation signals
    //###########

    // emits when alcotester has warmed up
    void alcotesterWarmedUp(QSharedPointer<Status> status);

    // emits when alcotester has cooled down
    void alcotesterCooledDown(QSharedPointer<Status> status);

    // emits when data were obtained from the alcotester
    void receivedAlcotesterData(QSharedPointer<Status> status, double value);

    // emits when it was not possible to obtain data from the alkotester
    void failedToReceiveDataFromAcotester(QSharedPointer<Status> status);

    //###########
    // POS device operation signals
    //###########

    // emits after activation of the POS device
    void posActivated(QSharedPointer<Status> status);

    // emits after deactivation of the POS device
    void posDeactivated(QSharedPointer<Status> status);

    // emits after success transaction
    void transactionSucceded(double money, QSharedPointer<Status> status);

    // emits after failed transaction
    void transactionFailed(QSharedPointer<Status> status);

    //###########
    // bill acceptor device operation signals
    //###########

    // emits after activation of the bill acceptor device
    void billAcceptorActivated(QSharedPointer<Status> status);

    // emits after deactivation of the bill acceptor device
    void billAcceptorDeactivated(QSharedPointer<Status> status);

    // emits after the adoption of banknote
    void moneyReceived(double money, QSharedPointer<Status> status);

    // emits after failing banknote
    void moneyRejected(QSharedPointer<Status> status);

    //###########
    // printer device operation signals
    //###########

    // emits when printer has warmed up
    void printerWarmedUp(QSharedPointer<Status> status);

    // emits when printer has cooled down
    void printerCooledDown(QSharedPointer<Status> status);

    // emits when image has printed
    void imagePrinted(QSharedPointer<Status> status);

    // emits when error has occured
    void error(QSharedPointer<Status> status);

private:
    // register request to the device
    // @params deviceName - name of a device
    // @params requestName - name of a request
    QSharedPointer<QTimer> registerRequest(DeviceName deviceName, RequestName requestName);

    // remove registered request for device
    // returns true if request is registered
    // @params deviceName - name of a device
    // @params requestName - name of a request
    bool removeRequest(DeviceName deviceName, RequestName requestName);

    //###########
    // base device operation callbacks
    //###########

    void onDeviceStart(QSharedPointer<Status> status);
    void onDeviceFinish(QSharedPointer<Status> status);
    void onDeviceRestart(QSharedPointer<Status> status);
    void onDeviceCheckStatus(QSharedPointer<Status> status);
    void onDeviceCheckConnection(QSharedPointer<Status> status);

    //###########
    // camera device operation callbacks
    //###########

    void onGetImage(QSharedPointer<QImage> image, QSharedPointer<Status> status);
    void onTakeImage(QSharedPointer<QImage> image, QSharedPointer<Status> status);
    void onStopGetImage(QSharedPointer<Status> status);

    //###########
    // alcotester device operation callbacks
    //###########

    void onWarmingUpAlcotester(QSharedPointer<Status> status);
    void onCoolingDownAlcotester(QSharedPointer<Status> status);
    void onAlcotesterDataReadSuccess(QSharedPointer<Status> status, double value);
    void onAlcotesterDataReadFail(QSharedPointer<Status> status);

    //###########
    // POS device operation callbacks
    //###########

    void onActivatePOS(QSharedPointer<Status> status);
    void onDeactivatePOS(QSharedPointer<Status> status);
    void onTransactionSucceded(double money, QSharedPointer<Status> status);
    void onTransactionFailed(QSharedPointer<Status> status);

    //###########
    // bill acceptor device operation callbacks
    //###########

    void onActivateBillAcceptor(QSharedPointer<Status> status);
    void onDeactivateBillAcceptor(QSharedPointer<Status> status);
    void onMoneyReceived(double money, QSharedPointer<Status> status);
    void onMoneyRejected(QSharedPointer<Status> status);

    //###########
    // printer device operation callbacks
    //###########

    void onWarmingUpPrinter(QSharedPointer<Status> status);
    void onCoolingDownPrinter(QSharedPointer<Status> status);
    void onPrintImage(QSharedPointer<Status> status);

    void onError(QSharedPointer<Status> status);

    QMap<DeviceName, QSet<RequestName>> _requests; // registered requests
    QMap<DeviceName, QMap<RequestName, QSharedPointer<QTimer>>> _requestTimers; // timers for each device request
    QMap<DeviceName, BaseDeviceInterface*> _devices; // device interfaces

    ConfigManager* _configManager;

    // device interfaces
    CameraInterface* _camera; // camera device
    AlcotesterInterface* _alcotester; // alcotester device
    POSInterface* _pos; // POS device
    PrinterInterface* _printer; // printer device
    BillAcceptorInterface* _billAcceptor;
};
