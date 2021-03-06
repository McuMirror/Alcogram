#pragma once

#include <QObject>
#include <QMutex>

#include <dlib/dnn.h>
#include <dlib/image_processing/frontal_face_detector.h>

#include "interfaces/faceDetectionInterface.h"

using namespace dlib;

template <long num_filters, typename SUBNET> using con5d = con<num_filters,5,5,2,2,SUBNET>;
template <long num_filters, typename SUBNET> using con5  = con<num_filters,5,5,1,1,SUBNET>;

template <typename SUBNET> using downsampler  = relu<affine<con5d<32, relu<affine<con5d<32, relu<affine<con5d<16,SUBNET>>>>>>>>>;
template <typename SUBNET> using rcon5  = relu<affine<con5<45,SUBNET>>>;

using net_type = loss_mmod<con<1,9,9,1,1,rcon5<rcon5<rcon5<downsampler<input_rgb_image_pyramid<pyramid_down<6>>>>>>>>;


class FaceDetector : public QObject
                    , public FaceDetectionInterface
{
    Q_OBJECT
public:
    explicit FaceDetector(QObject *parent = 0);

    // finds faces in the image
    void detect(const QImage &image) override;

    // number of found faces
    int facesCount() const override;

    // rects of found faces
    QList<QRect> faceRects() const override;

private:
    mutable QMutex _mutex;
    net_type _net;
    QList<QRect> _rects; // detected face rects
    matrix<rgb_pixel> _dlibImage; // image for dlib face dectector
};
