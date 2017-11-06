#pragma once

#include <QFont>

namespace Utils {
    QFont getFont(const QString& family, int pixelSize, qreal spacing, int weight);
}
