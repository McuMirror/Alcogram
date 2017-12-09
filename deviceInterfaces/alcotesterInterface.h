#pragma once

#include <QList>

#include "baseDeviceInterface.h"

const int NOT_RECOGNIZED = 4; // TODO: get rid of it

typedef std::function<void(int, double)> AlcoTestCallback; // _1 - status code, _2 - permille

class AlcotesterInterface : public BaseDeviceInterface
{
public:
    virtual void test(AlcoTestCallback callback) = 0; // TODO: logging
    virtual void abort(DeviceCallback callback) = 0; // TODO: logging
    virtual QList<double> getPersonsValues() const = 0;
};
