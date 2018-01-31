#include <QMutexLocker>

#include <dlib/image_processing.h>
//#include <dlib/opencv.h>
//#include <opencv2/imgproc.hpp>
#include <ctime>

#include "faceDetector.h"
#include "utils.h"

FaceDetector::FaceDetector(QObject *parent)
    : QObject(parent)
    , _mutex()
{
    deserialize("E:\\mmod_human_face_detector.dat") >> _net;
    //_rects = QList<QRect>();
    //_rects.append(QRect(200, 100, 150, 150));
    //_rects.append(QRect(370, 100, 150, 150));
}

void FaceDetector::detect(const QImage& image)
{
    //cv::Mat mat = Utils::qImageToCvMat(image);
    //matrix<rgb_pixel> dlibImage;
    QMutexLocker ml(&_mutex);

    if (_dlibImage.size() == 0) {
        _dlibImage.set_size(image.height(), image.width());
    }

    for (int j = 0; j < image.height(); j++) {
        const QRgb* l = (QRgb*) image.scanLine(j);

        for (int i = 0; i < image.width(); i++) {
            QRgb c = l[i];
            _dlibImage(j, i).red = qRed(c);
            _dlibImage(j, i).green = qGreen(c);
            _dlibImage(j, i).blue = qBlue(c);
        }
    }

    //assign_image(dlibImage, cv_image<bgr_pixel>(mat));

    //int start = clock();

    std::vector<mmod_rect> dets = _net(_dlibImage);

    //int stop = clock();

    //int time = (stop-start)/double(CLOCKS_PER_SEC) * 1000;

    _rects = QList<QRect>();

    for (mmod_rect d : dets) {
        rectangle r = d.rect;
        _rects.append(QRect(r.left(), r.top(), r.width(), r.height()));
    }

    //_rects.append(QRect(200, 100, 150, 150));
}

int FaceDetector::facesCount() const
{
    QMutexLocker ml(&_mutex);
    return _rects.size();
}

QList<QRect> FaceDetector::faceRects() const
{
    QMutexLocker ml(&_mutex);
    return _rects;
}
