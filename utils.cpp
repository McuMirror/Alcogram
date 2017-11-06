#include "utils.h"

namespace Utils {
    QFont getFont(const QString& family, int pixelSize, qreal spacing, int weight)
    {
        QFont font(family);
        font.setPixelSize(pixelSize);
        font.setLetterSpacing(QFont::AbsoluteSpacing, spacing);
        font.setWeight(weight);

        return font;
    }
}
