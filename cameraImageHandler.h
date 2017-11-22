#pragma once

#include <QObject>
#include <QSharedPointer>
#include <QImage>
#include <QPixmap>

#include "interfaces/faceDetectionInterface.h"

class CameraImageHandler : public QObject
{
    Q_OBJECT
public:
    CameraImageHandler();
    CameraImageHandler(FaceDetectionInterface* faceDetector);

    void setFaceDetector(FaceDetectionInterface* faceDetector);
    void setImageToProcess(QSharedPointer<QImage> imageToProcess);
    bool isRunning() const;

signals:
    void processed(QPixmap processedImage);
public slots:
    void process();

private:
    QSharedPointer<QImage> _imageToProcess;
    FaceDetectionInterface* _faceDetector;
};
