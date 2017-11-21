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

    void turnOn(DeviceCallback callback) override;
    void turnOff(DeviceCallback callback) override;
    void setImageCaptureCallback(ImageCaptureCallback callback) override;
    void captureImage() override;
    const QImage& getCapturedImage() override;
    void reset() override;

private:
    QCamera* _camera = nullptr;
    CameraFrameGrabber* _cameraFrameGrabber = nullptr;

    ImageCaptureCallback _callback = nullptr;
    int _captureMode = CAMERA_STREAM;
    QSharedPointer<QImage> _capturedImage;
};
