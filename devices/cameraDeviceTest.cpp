#include <QCameraInfo>

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
           _callback(_captureMode, frame);

           if (_captureMode == CAMERA_IMAGE_CAPTURE) {
               _capturedImage.reset(new QImage(frame.mirrored()));
               _captureMode = CAMERA_STREAM;
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
    return *_capturedImage.data();
}

void CameraDeviceTest::reset()
{
    _capturedImage.reset(new QImage());
}
