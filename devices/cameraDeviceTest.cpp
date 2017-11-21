#include <QCameraInfo>
#include <QSharedPointer>

#include "cameraDeviceTest.h"


CameraDeviceTest::CameraDeviceTest(QObject *parent)
    : QObject(parent)
{

    _capturedImage.reset(new QImage());
    _camera = new QCamera(QCameraInfo::defaultCamera());
    _cameraFrameGrabber = new CameraFrameGrabber();
    _camera->setViewfinder(_cameraFrameGrabber);

    QObject::connect(_cameraFrameGrabber, &CameraFrameGrabber::frameAvailable, [this] (QImage frame) {
        if (_callback != nullptr) {
            if (_captureMode == CAMERA_IMAGE_CAPTURE) {
                _capturedImage = QSharedPointer<QImage>(new QImage(frame.mirrored()));
                _callback(_captureMode, _capturedImage);
                _callback = nullptr;
                _captureMode = CAMERA_STREAM;
            } else {
                _callback(_captureMode, QSharedPointer<QImage>(new QImage(frame.mirrored())));
            }
        }
    });
}

CameraDeviceTest::~CameraDeviceTest()
{
    delete _camera;
    delete _cameraFrameGrabber;
}


void CameraDeviceTest::turnOn(DeviceCallback callback)
{
    _camera->start();
    callback(OK);
}

void CameraDeviceTest::turnOff(DeviceCallback callback)
{
    _camera->stop();
    callback(OK);
}

void CameraDeviceTest::setImageCaptureCallback(ImageCaptureCallback callback)
{
    _callback = callback;
}

void CameraDeviceTest::captureImage()
{
    _captureMode = CAMERA_IMAGE_CAPTURE;
}

const QImage& CameraDeviceTest::getCapturedImage()
{
    return *_capturedImage;
}

void CameraDeviceTest::reset()
{
    _capturedImage.reset(new QImage());
}
