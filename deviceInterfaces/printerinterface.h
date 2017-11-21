#pragma once

#include <QImage>

#include "baseDeviceInterface.h"

const int PRINT_COMPLETE = 5;

typedef std::function<void(int, int)> PrinterCallback; // _1 - status code, _2 - number of printed photos

class PrinterInterface : public BaseDeviceInterface
{
public:
    virtual void print(const QImage& image, int amount, PrinterCallback callback) = 0;
};
