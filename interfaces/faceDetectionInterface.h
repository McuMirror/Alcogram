#pragma once

#include <QImage>
#include <QRect>
#include <QList>

// interface for face detector
class FaceDetectionInterface
{
public:
    virtual void detect(const QImage& image) = 0;

    // number of detected faces
    virtual int facesNumber() const = 0;

    // rects of detected faces
    virtual QList<QRect> faceRects() const = 0;
};
