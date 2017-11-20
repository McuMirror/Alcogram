#pragma once

#include <QFont>
#include <QImage>
#include <opencv2/core.hpp>

namespace Utils {
    QFont getFont(const QString& family, int pixelSize, qreal spacing, int weight);
    cv::Mat qImageToCvMat(const QImage& inImage, bool inCloneImageData = true);
}
