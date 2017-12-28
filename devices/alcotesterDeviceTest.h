#pragma once

#include <QObject>

#include "deviceInterfaces/alcotesterinterface.h"

class AlcotesterDeviceTest : public QObject
                            , public AlcotesterInterface
{
    Q_OBJECT
public:
    explicit AlcotesterDeviceTest(QObject *parent = 0);

    void turnOn(DeviceCallback callback) override;
    void turnOff(DeviceCallback callback) override;
    void reset();

    void test(AlcoTestCallback callback) override;
    void abort(DeviceCallback callback) override;
    QList<double> getPersonsValues() const override;

private:
    QList<double> _personValues;
};
