#pragma once

#include <QObject>
#include <QSharedPointer>
#include <QMap>

#include "deviceInterfaces/status.h"

class SessionData : public QObject
{
    Q_OBJECT
public:
    explicit SessionData(QObject *parent = 0);

    void setImage(QSharedPointer<QImage> image);
    void addAlcoValue(double value);

    void cameraGetImageTimeout();
    int getCameraGetImageTimeoutNumber() const;

    const QList<double>& getAlcoValues() const;
    QSharedPointer<QImage> getImage() const;

    void reset();

private:
    int _cameraGetImageTimeoutNumber = 0;
    QList<double> _alcoValues;
    QSharedPointer<QImage> _image;
};
