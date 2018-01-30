#pragma once

#include <QObject>
#include <QSharedPointer>
#include <QImage>
#include <QPixmap>

#include "interfaces/faceDetectionInterface.h"

// finds faces in image and marks them
class CameraImageHandler : public QObject
{
    Q_OBJECT
public:
    CameraImageHandler();
    CameraImageHandler(FaceDetectionInterface* faceDetector);

    void setFaceDetector(FaceDetectionInterface* faceDetector);

    // sets image to process
    void setImageToProcess(QSharedPointer<QImage> imageToProcess);

    // face detecting in progress
    bool isRunning() const;

signals:
    // emits when image has processed
    void processed(QPixmap processedImage);

public slots:
    // called when next frame appeared
    void process();

private:
    QSharedPointer<QImage> _imageToProcess;
    FaceDetectionInterface* _faceDetector;
};
