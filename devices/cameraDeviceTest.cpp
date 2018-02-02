#include <QCameraInfo>
#include <QSharedPointer>

#include "cameraDeviceTest.h"


CameraDeviceTest::CameraDeviceTest(QObject *parent)
    : QObject(parent)
{
    _mode = STREAM;
    _camera = new QCamera(QCameraInfo::defaultCamera());
    _cameraFrameGrabber = new CameraFrameGrabber();
    _camera->setViewfinder(_cameraFrameGrabber);

    QObject::connect(_cameraFrameGrabber, &CameraFrameGrabber::frameAvailable, [this] (QImage frame) {
        if (_callback != nullptr) {
            _callback(QSharedPointer<QImage>(new QImage(frame.mirrored()))
                      , QSharedPointer<Status>(new Status(0, CAMERA, _mode == STREAM ? GET_IMAGE : TAKE_IMAGE)));

            //if (_mode == TAKE_IMAGE) {
            _callback = nullptr;
            //}
        }
    });

    _camera->start();
}

CameraDeviceTest::~CameraDeviceTest()
{
    delete _camera;
    delete _cameraFrameGrabber;
}

// BaseDeviceInterface interface
void CameraDeviceTest::start(DeviceCallback onStart)
{
    onStart(QSharedPointer<Status>(new Status(0, CAMERA, START_DEVICE)));
}

void CameraDeviceTest::finish(DeviceCallback onFinish)
{
    onFinish(QSharedPointer<Status>(new Status(0, CAMERA, FINISH_DEVICE)));
}

void CameraDeviceTest::restart(DeviceCallback onRestart)
{
    onRestart(QSharedPointer<Status>(new Status(0, CAMERA, RESTART_DEVICE)));
}

void CameraDeviceTest::checkStatus(DeviceCallback onCheckStatus)
{
    onCheckStatus(QSharedPointer<Status>(new Status(STATUS_OK, CAMERA, CHECK_STATUS)));
}

void CameraDeviceTest::connectionStatus(DeviceCallback onConnection)
{
    onConnection(QSharedPointer<Status>(new Status(DEVICE_CONNECTED, CAMERA, CHECK_CONNECTION)));
}

void CameraDeviceTest::isConnected(OnIsConnectedCallback onIsConnected)
{

}

// CameraInterface interface
void CameraDeviceTest::getImage(ImageCaptureCallback onGetImage)
{
    _mode = STREAM;
    _callback = onGetImage;
}

void CameraDeviceTest::takeImage(ImageCaptureCallback onTookImage)
{
    _mode = CAPTURE;
    _callback = onTookImage;
}

void CameraDeviceTest::stopGetImage(DeviceCallback onStopGetImage)
{
    _callback = nullptr;
    onStopGetImage(QSharedPointer<Status>(new Status(0, CAMERA, STOP_GET_IMAGE)));
}
