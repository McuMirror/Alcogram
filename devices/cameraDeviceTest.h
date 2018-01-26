#pragma once

#include <QObject>
#include <QCamera>
#include <QMediaRecorder>
#include <QSharedPointer>

#include "deviceInterfaces/cameraInterface.h"
#include "cameraFrameGrabber.h"

class CameraDeviceTest : public QObject
                       , public CameraInterface
{
    Q_OBJECT
public:
    explicit CameraDeviceTest(QObject *parent = 0);
    ~CameraDeviceTest();

    // BaseDeviceInterface interface
    void start(DeviceCallback onStart);
    void finish(DeviceCallback onFinish);
    void restart(DeviceCallback onRestart);
    void checkStatus(DeviceCallback onCheckStatus);
    void connectionStatus(DeviceCallback onConnection);
    void isConnected(OnIsConnectedCallback onIsConnected);

    // CameraInterface interface
    void getImage(ImageCaptureCallback onGetImage);
    void takeImage(ImageCaptureCallback onTookImage);
    void stopGetImage(DeviceCallback onStopGetImage);

private:
    enum Mode {
        STREAM, CAPTURE
    };

    QCamera* _camera = nullptr;
    CameraFrameGrabber* _cameraFrameGrabber = nullptr;

    ImageCaptureCallback _callback = nullptr;
    Mode _mode;
};
