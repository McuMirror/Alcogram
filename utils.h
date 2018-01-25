#pragma once

#include <QFont>
#include <QImage>
//#include <opencv2/core.hpp>
#include "stateMachine.h"

namespace Utils {
    QFont getFont(const QString& family, int pixelSize, qreal spacing, int weight);
    //cv::Mat qImageToCvMat(const QImage& inImage, bool inCloneImageData = true);
    QString getStateNameNumber(StateName stateName);
    QPoint rotatePoint(float cx, float cy, float angle, const QPoint& p);
    QPoint movePointInRect(const QRect& rect, const QPoint& point, const QPoint& center, float radius);
}
