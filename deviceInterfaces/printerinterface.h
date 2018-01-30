#pragma once

#include <QImage>

#include "baseDeviceInterface.h"

//typedef std::function<void(int, int)> OnImagePrintedCallback; // _1 - status code, _2 - number of printed photos

class PrinterInterface : public BaseDeviceInterface
{
public:
    // start warming up a printer
    virtual void warmUp(DeviceCallback onWarmUp) = 0;

    // start cooling down a printer
    virtual void coolDown(DeviceCallback onCoolDown) = 0;

    // send image to print to a printer
    virtual void printImage(const QImage& image, DeviceCallback onImagePrinted) = 0;
};
