#include <QPainter>
#include <QTimer>

#include "cameraImageHandler.h"

CameraImageHandler::CameraImageHandler()
{

}

CameraImageHandler::CameraImageHandler(FaceDetectionInterface* faceDetector)
    : _faceDetector(faceDetector)
{
}

void CameraImageHandler::setFaceDetector(FaceDetectionInterface* faceDetector)
{
    _faceDetector = faceDetector;
}

void CameraImageHandler::setImageToProcess(QSharedPointer<QImage> imageToProcess)
{
    _imageToProcess = imageToProcess;
}

void CameraImageHandler::process()
{
    QPixmap processedImage = QPixmap::fromImage(*_imageToProcess);

    // find faces
    _faceDetector->detect(*_imageToProcess);

    // draw rects for found faces
    if (_faceDetector->facesCount() > 0) {
        for (const QRect& faceRect : _faceDetector->faceRects()) {
            QPainter p(&processedImage);
            QPen pen(QColor(255, 0, 0));
            pen.setWidth(2);
            p.setPen(pen);

            p.drawRect(faceRect);
        }
    }

    emit processed(processedImage);
}
