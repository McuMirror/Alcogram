#pragma once

#include <QImage>

#include "baseDeviceInterface.h"

const int PRINT_COMPLETE = 5;

//typedef std::function<void(int, int)> OnImagePrintedCallback; // _1 - status code, _2 - number of printed photos

class PrinterInterface : public BaseDeviceInterface
{
public:
    virtual void warmUp(DeviceCallback onWarmUp) = 0;
    virtual void coolDown(DeviceCallback onCoolDown) = 0;
    virtual void printImage(const QImage& image, DeviceCallback onImagePrinted) = 0;
};
