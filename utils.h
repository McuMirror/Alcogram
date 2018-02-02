#pragma once

#include <QFont>
#include <QImage>
//#include <opencv2/core.hpp>
#include "stateMachine.h"

namespace Utils {
    // returns QFont
    // @param family - font family
    // @param pixelSize - font size in px
    // @param weight - font weight
    QFont getFont(const QString& family, int pixelSize, qreal spacing, int weight);

    // eg SPLASH_SCREEN -> 0.1 ect
    QString getStateNameNumber(StateName stateName);

    // rotate point around center
    // @param cx - center x coordinate
    // @param cy - center y coordinate
    // @param angle - rotate angle
    // @param p - point to rotate
    QPoint rotatePoint(float cx, float cy, float angle, const QPoint& p);

    // finds the coordinates of the point around the center so that it is inside the rectangle
    QPoint movePointInRect(const QRect& rect, const QPoint& point, const QPoint& center, float radius);
}
