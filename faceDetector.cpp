#include <dlib/image_processing.h>
#include <dlib/opencv.h>
#include <opencv2/imgproc.hpp>
#include <ctime>

#include "faceDetector.h"
#include "utils.h"

FaceDetector::FaceDetector(QObject *parent)
    : QObject(parent)
{
    deserialize("E:\\mmod_human_face_detector.dat") >> _net;
}

void FaceDetector::detect(const QImage& image)
{
    cv::Mat mat = Utils::qImageToCvMat(image);
    matrix<rgb_pixel> dlibImage;

    /*if (_dlibImage.size() == 0) {
        _dlibImage.set_size(image.height(), image.width());
    }

    for (int j = 0; j < image.height(); j++) {
        const uchar* l = image.scanLine(j);

        int k = 0;

        for (int i = 0; i < image.width(); i++) {
            _dlibImage(j, i).red = l[k + 2];
            _dlibImage(j, i).green = l[k + 1];
            _dlibImage(j, i).blue = l[k];
            k += 3;
        }
    }*/

    assign_image(dlibImage, cv_image<bgr_pixel>(mat));

    int start = clock();

    std::vector<mmod_rect> dets = _net(dlibImage);

    int stop = clock();

    int time = (stop-start)/double(CLOCKS_PER_SEC) * 1000;

    _rects = QList<QRect>();

    for (mmod_rect d : dets) {
        rectangle r = d.rect;
        _rects.append(QRect(r.left(), r.top(), r.width(), r.height()));
    }
}

int FaceDetector::facesNumber() const
{
    return _rects.size();
}

QList<QRect> FaceDetector::faceRects() const
{
    return _rects;
}
