#pragma once

#include <QObject>

#include "interfaces/faceDetectionInterface.h"

class FaceDetector : public QObject
                    , public FaceDetectionInterface
{
    Q_OBJECT
public:
    explicit FaceDetector(QObject *parent = 0);

    void detect(const QImage &image) override;
    int facesNumber() const override;
    QList<QRect> faceRects() const override;
};
