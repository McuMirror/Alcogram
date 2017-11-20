#include "faceDetector.h"

FaceDetector::FaceDetector(QObject *parent) : QObject(parent)
{

}

void FaceDetector::detect(const QImage& image)
{

}

int FaceDetector::facesNumber() const
{
    return 1;
}

QList<QRect> FaceDetector::faceRects() const
{
    return QList<QRect>({QRect(280, 130, 80, 100)});
}
