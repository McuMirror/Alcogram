#pragma once

#include <QImage>
#include <QSharedPointer>

#include "baseDeviceInterface.h"

typedef std::function<void(QSharedPointer<QImage>, QSharedPointer<Status>)> ImageCaptureCallback;

class CameraInterface : public BaseDeviceInterface
{
public:
    // set callback for image stream from camera
    virtual void getImage(ImageCaptureCallback onGetImage) = 0;

    // take an image from camera once
    virtual void takeImage(ImageCaptureCallback onTookImage) = 0;

    // stop image stream from camera
    virtual void stopGetImage(DeviceCallback onStopGetImage) = 0;
};
