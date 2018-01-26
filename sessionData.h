#pragma once

#include <QObject>
#include <QSharedPointer>

class SessionData : public QObject
{
    Q_OBJECT
public:
    explicit SessionData(QObject *parent = 0);

    void setImage(QSharedPointer<QImage> image);
    void addAlcoValue(double value);

    const QList<double>& getAlcoValues() const;
    QSharedPointer<QImage> getImage() const;

    void reset();

private:
    QList<double> _alcoValues;
    QSharedPointer<QImage> _image;
};
