#pragma once

#include <QImage>
#include <QSharedPointer>

#include "baseDeviceInterface.h"

const int CAMERA_STREAM = 2;        // TODO: get rid of it
const int CAMERA_IMAGE_CAPTURE = 3; //

typedef std::function<void(int, QSharedPointer<QImage>)> ImageCaptureCallback;

class CameraInterface : public BaseDeviceInterface
{
public:
    virtual void setImageCaptureCallback(ImageCaptureCallback callback) = 0;
    virtual void captureImage() = 0;
    virtual const QImage& getCapturedImage() = 0;
};
