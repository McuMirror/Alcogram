#pragma once

#include <QImage>
#include <QRect>
#include <QList>

class FaceDetectionInterface
{
public:
    virtual void detect(const QImage& image) = 0;
    virtual int facesNumber() const = 0;
    virtual QList<QRect> faceRects() const = 0;
};
