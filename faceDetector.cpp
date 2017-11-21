#include "faceDetector.h"

FaceDetector::FaceDetector(QObject *parent) : QObject(parent)
{

}

void FaceDetector::detect(const QImage& image)
{

}

int FaceDetector::facesNumber() const
{
    return 2;
}

QList<QRect> FaceDetector::faceRects() const
{
    return QList<QRect>({QRect(280, 130, 50, 80), QRect(100, 200, 80, 120)});
}
