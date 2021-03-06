#include "sessionData.h"

SessionData::SessionData(QObject *parent) : QObject(parent)
{

}

void SessionData::setImage(QSharedPointer<QImage> image)
{
    _image = image;
}

void SessionData::addAlcoValue(double value)
{
    _alcoValues.append(value);
}

void SessionData::cameraGetImageTimeout()
{
    _cameraGetImageTimeoutNumber++;
}

int SessionData::getCameraGetImageTimeoutNumber() const
{
    return _cameraGetImageTimeoutNumber;
}

QSharedPointer<QImage> SessionData::getImage() const
{
    return _image;
}

void SessionData::removeImage()
{
    _image.clear();
}

const QList<double>& SessionData::getAlcoValues() const
{
    return _alcoValues;
}


void SessionData::reset()
{
    _alcoValues.clear();
    _image.clear();
    _cameraGetImageTimeoutNumber = 0;
}
